#include "asda2_control.h"
#include "mcc_generated_files/can1.h"
#include <stdlib.h>
#include <math.h>

ASDA2_Axis_t axes_state[NUM_AXES];

// Internal Planner Buffer
static internal_block_t plan_buffer[PLANNER_BUFFER_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;
static volatile uint8_t block_count = 0;

void ASDA2_Initialize(void)
{
    for(int i = 0; i < NUM_AXES; i++)
    {
        axes_state[i].current_pos_int = 0;
        axes_state[i].current_pos_fp = 0;
        axes_state[i].state = STATE_IDLE;
        axes_state[i].digital_inputs = 0;
    }
    head = 0;
    tail = 0;
    block_count = 0;
}

// --- CANopen Helpers ---
void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse) {
    CAN1_MSG_OBJ msg;
    uint8_t node_id = axis_idx + 1;
    msg.id = 0x200 + node_id;
    msg.msgAttr.dlc = 6;
    msg.msgAttr.idType = 0;
    msg.data[0] = 0x0F;
    msg.data[1] = 0x00;
    msg.data[2] = (uint8_t)(target_pulse & 0xFF);
    msg.data[3] = (uint8_t)((target_pulse >> 8) & 0xFF);
    msg.data[4] = (uint8_t)((target_pulse >> 16) & 0xFF);
    msg.data[5] = (uint8_t)((target_pulse >> 24) & 0xFF);
    CAN1_Transmit(&msg);
}

void Send_SYNC_Message(void) {
    CAN1_MSG_OBJ sync_msg;
    sync_msg.id = 0x80;
    sync_msg.msgAttr.dlc = 0;
    sync_msg.msgAttr.idType = 0;
    CAN1_Transmit(&sync_msg);
}

void CANopen_Start_Nodes(void) {
    CAN1_MSG_OBJ nmt;
    nmt.id = 0x000;
    nmt.msgAttr.dlc = 2;
    nmt.msgAttr.idType = 0;
    nmt.data[0] = 0x01;
    nmt.data[1] = 0x00;
    CAN1_Transmit(&nmt);
}

void Process_Incoming_CAN_Messages(void) {
    CAN1_MSG_OBJ rx_msg;
    while (CAN1_Receive(&rx_msg)) {
        if (rx_msg.id >= 0x181 && rx_msg.id <= 0x184) {
            uint8_t axis_idx = rx_msg.id - 0x181;
            axes_state[axis_idx].digital_inputs = (uint32_t)rx_msg.data[2] |
                                                 ((uint32_t)rx_msg.data[3] << 8) |
                                                 ((uint32_t)rx_msg.data[4] << 16) |
                                                 ((uint32_t)rx_msg.data[5] << 24);
        }
    }
}

// --- Homing ---
bool Is_Home_Switch_Active(uint8_t axis_idx) {
    return (axes_state[axis_idx].digital_inputs & (1 << 2)) != 0;
}

void Start_Axis_Homing(uint8_t axis_mask) {
    for (int i = 0; i < NUM_AXES; i++) {
        if (axis_mask & (1 << i)) {
            axes_state[i].state = STATE_HOMING_SEARCH;
            axes_state[i].direction = 1;
        }
    }
}

// --- Movement & Trajectory ---
bool move_to_absolute_position(float *target_mm, float feed_rate_mm_min)
{
    if (block_count >= PLANNER_BUFFER_SIZE) return false;

    internal_block_t *new_block = &plan_buffer[head];
    uint32_t max_delta_steps = 0;
    float feed_rate_hz = (feed_rate_mm_min / 60.0f) * STEPS_PER_MM;

    for (int i = 0; i < NUM_AXES; i++) {
        ASDA2_Axis_t *ax = &new_block->axis_data[i];
        ax->target_pos = (int32_t)(target_mm[i] * STEPS_PER_MM);

        int32_t start_pos = (block_count == 0) ? axes_state[i].current_pos_int :
            plan_buffer[(head + PLANNER_BUFFER_SIZE - 1) % PLANNER_BUFFER_SIZE].axis_data[i].target_pos;

        uint32_t delta = labs(ax->target_pos - start_pos);
        ax->steps_remaining = delta;
        ax->direction = (ax->target_pos < start_pos) ? 1 : 0;

        if (delta > max_delta_steps) max_delta_steps = delta;
    }

    if (max_delta_steps == 0) return true;

    for (int i = 0; i < NUM_AXES; i++) {
        ASDA2_Axis_t *ax = &new_block->axis_data[i];
        if (ax->steps_remaining > 0) {
            float axis_vel_hz = feed_rate_hz * ((float)ax->steps_remaining / max_delta_steps);
            ax->vel_target_hz = (uint16_t)axis_vel_hz;
            uint16_t accel_cycles = ACCEL_TIME_MS / MASTER_PERIOD_MS;
            uint32_t vel_diff = (ax->vel_target_hz > MIN_PULSE_FREQ_HZ) ? (ax->vel_target_hz - MIN_PULSE_FREQ_HZ) : 0;
            ax->vel_step_fp = (accel_cycles > 0) ? ((uint64_t)vel_diff << FP_SCALE) / accel_cycles : 0;
            ax->accel_steps = (uint32_t)(((uint64_t)(MIN_PULSE_FREQ_HZ + ax->vel_target_hz) * ACCEL_TIME_MS) / 2000);
            if (ax->steps_remaining >= (ax->accel_steps * 2)) ax->decel_steps = ax->accel_steps;
            else { ax->accel_steps = ax->steps_remaining / 2; ax->decel_steps = ax->steps_remaining - ax->accel_steps; }
            ax->current_vel_fp = ((uint32_t)MIN_PULSE_FREQ_HZ << FP_SCALE);
            ax->state = STATE_ACCEL;
        } else ax->state = STATE_IDLE;
    }
    head = (head + 1) % PLANNER_BUFFER_SIZE;
    block_count++;
    return true;
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0;
    Process_Incoming_CAN_Messages();

    // Homing and Trajectory Logic combined in one pass for axes_state
    for (int i = 0; i < NUM_AXES; i++) {
        ASDA2_Axis_t *ax = &axes_state[i];

        // Handle Homing (Higher priority than planner)
        if (ax->state == STATE_HOMING_SEARCH) {
            if (Is_Home_Switch_Active(i)) ax->state = STATE_HOMING_LOCATE;
            else {
                uint32_t dist_fp = ((uint64_t)HOMING_SEEK_RATE_HZ << FP_SCALE) * MASTER_PERIOD_MS / 1000;
                ax->current_pos_fp += (ax->direction == 0) ? dist_fp : -dist_fp;
            }
        } else if (ax->state == STATE_HOMING_LOCATE) {
            if (!Is_Home_Switch_Active(i)) { ax->current_pos_fp &= 0xFFFF0000; ax->state = STATE_IDLE; }
            else {
                uint32_t dist_fp = ((uint64_t)HOMING_FEED_RATE_HZ << FP_SCALE) * MASTER_PERIOD_MS / 1000;
                ax->current_pos_fp += (ax->direction == 0) ? -dist_fp : dist_fp;
            }
        }

        // Handle Trajectory if block active
        if (block_count > 0 && ax->state != STATE_HOMING_SEARCH && ax->state != STATE_HOMING_LOCATE) {
            ASDA2_Axis_t *plan_ax = &plan_buffer[tail].axis_data[i];
            if (plan_ax->state != STATE_IDLE) {
                if (plan_ax->state == STATE_ACCEL) {
                    plan_ax->current_vel_fp += plan_ax->vel_step_fp;
                    if (plan_ax->current_vel_fp >= ((uint32_t)plan_ax->vel_target_hz << FP_SCALE)) {
                        plan_ax->current_vel_fp = ((uint32_t)plan_ax->vel_target_hz << FP_SCALE);
                        plan_ax->state = STATE_CONST;
                    }
                } else if (plan_ax->state == STATE_DECEL) {
                    if (plan_ax->current_vel_fp > (((uint32_t)MIN_PULSE_FREQ_HZ << FP_SCALE) + plan_ax->vel_step_fp))
                        plan_ax->current_vel_fp -= plan_ax->vel_step_fp;
                    else plan_ax->current_vel_fp = ((uint32_t)MIN_PULSE_FREQ_HZ << FP_SCALE);
                }
                uint32_t displacement_fp = (plan_ax->current_vel_fp * MASTER_PERIOD_MS) / 1000;
                if ((displacement_fp >> FP_SCALE) >= plan_ax->steps_remaining) {
                    ax->current_pos_fp = (int32_t)plan_ax->target_pos << FP_SCALE;
                    plan_ax->state = STATE_IDLE;
                } else {
                    if (plan_ax->direction == 0) ax->current_pos_fp += displacement_fp;
                    else ax->current_pos_fp -= displacement_fp;
                    plan_ax->steps_remaining -= (displacement_fp >> FP_SCALE);
                    if (plan_ax->state == STATE_CONST && plan_ax->steps_remaining <= plan_ax->decel_steps) plan_ax->state = STATE_DECEL;
                }
            }
        }
        ax->current_pos_int = ax->current_pos_fp >> FP_SCALE;
        Send_Axis_PDO(i, ax->current_pos_int);
    }

    Send_SYNC_Message();
    if (block_count > 0) {
        bool block_done = true;
        for(int i=0; i<NUM_AXES; i++) if(plan_buffer[tail].axis_data[i].state != STATE_IDLE) block_done = false;
        if(block_done) { tail = (tail + 1) % PLANNER_BUFFER_SIZE; block_count--; }
    }
}

#include "asda2_control.h"
#include "mcc_generated_files/can1.h"
#include <stdlib.h>

ASDA2_Axis_t axes_state[NUM_AXES];

void ASDA2_Initialize(void)
{
    for(int i = 0; i < NUM_AXES; i++)
    {
        axes_state[i].current_pos_int = 0;
        axes_state[i].current_pos_fp = 0;
        axes_state[i].state = STATE_IDLE;
        axes_state[i].digital_inputs = 0;
    }
}

/**
 * move_single_axis_abs - Tek eksenli bağımsız hareket
 * YÜKSEK ÇÖZÜNÜRLÜK NOTU: 10.000 step/mm için 64-bit hız değişkeni kullanılarak
 * taşma (overflow) engellenmiştir.
 */
bool move_single_axis_abs(uint8_t axis_idx, float target_mm, float feed_rate_mm_min)
{
    if (axis_idx >= NUM_AXES) return false;
    ASDA2_Axis_t *ax = &axes_state[axis_idx];
    if (ax->state != STATE_IDLE) return false;

    ax->target_pos = (int32_t)(target_mm * STEPS_PER_MM);
    ax->steps_remaining = labs(ax->target_pos - ax->current_pos_int);
    ax->direction = (ax->target_pos < ax->current_pos_int) ? 1 : 0;
    if (ax->steps_remaining == 0) return true;

    ax->vel_target_hz = (uint32_t)((feed_rate_mm_min / 60.0f) * STEPS_PER_MM);
    uint16_t accel_cycles = ACCEL_TIME_MS / MASTER_PERIOD_MS;
    uint32_t vel_diff = (ax->vel_target_hz > MIN_PULSE_FREQ_HZ) ? (ax->vel_target_hz - MIN_PULSE_FREQ_HZ) : 0;

    // 64-bit ara hesaplama ile hassas ivme artışı
    ax->vel_step_fp = (accel_cycles > 0) ? (((uint64_t)vel_diff << FP_SCALE) / accel_cycles) : 0;

    // Yavaşlama mesafesi hesabı
    uint32_t accel_dist = (uint32_t)(((uint64_t)(MIN_PULSE_FREQ_HZ + ax->vel_target_hz) * ACCEL_TIME_MS) / 2000);
    if (ax->steps_remaining >= (accel_dist * 2)) ax->decel_start_steps = accel_dist;
    else ax->decel_start_steps = ax->steps_remaining / 2;

    ax->current_vel_fp = ((uint64_t)MIN_PULSE_FREQ_HZ << FP_SCALE);
    ax->state = STATE_ACCEL;
    return true;
}

void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse) {
    CAN_MSG_OBJ msg;
    uint8_t data[8];
    msg.msgId = 0x200 + (axis_idx + 1);
    msg.field.dlc = 6;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.data = data;

    data[0] = 0x0F;
    data[1] = 0x00;
    data[2] = (uint8_t)(target_pulse & 0xFF);
    data[3] = (uint8_t)((target_pulse >> 8) & 0xFF);
    data[4] = (uint8_t)((target_pulse >> 16) & 0xFF);
    data[5] = (uint8_t)((target_pulse >> 24) & 0xFF);

    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

void Send_SYNC_Message(void) {
    CAN_MSG_OBJ sync_msg;
    sync_msg.msgId = 0x80;
    sync_msg.field.dlc = 0;
    sync_msg.field.idType = CAN_FRAME_STD;
    sync_msg.field.frameType = CAN_FRAME_DATA;
    sync_msg.data = (void*)0;

    CAN1_Transmit(CAN_PRIORITY_HIGH, &sync_msg);
}

void CANopen_Start_Nodes(void) {
    CAN_MSG_OBJ nmt;
    uint8_t data[2];
    nmt.msgId = 0x000;
    nmt.field.dlc = 2;
    nmt.field.idType = CAN_FRAME_STD;
    nmt.field.frameType = CAN_FRAME_DATA;
    nmt.data = data;
    data[0] = 0x01;
    data[1] = 0x00;

    CAN1_Transmit(CAN_PRIORITY_HIGH, &nmt);
}

void Process_Incoming_CAN_Messages(void) {
    CAN_MSG_OBJ rx_msg;
    uint8_t data[8];
    rx_msg.data = data;
    while (CAN1_Receive(&rx_msg)) {
        if (rx_msg.msgId >= 0x181 && rx_msg.msgId <= 0x184) {
            uint8_t idx = rx_msg.msgId - 0x181;
            axes_state[idx].digital_inputs = (uint32_t)data[2] |
                                            ((uint32_t)data[3] << 8) |
                                            ((uint32_t)data[4] << 16) |
                                            ((uint32_t)data[5] << 24);
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0;
    Process_Incoming_CAN_Messages();
    for (int i = 0; i < NUM_AXES; i++) {
        ASDA2_Axis_t *ax = &axes_state[i];
        if (ax->state == STATE_IDLE) { Send_Axis_PDO(i, ax->current_pos_int); continue; }

        if (ax->state == STATE_ACCEL) {
            ax->current_vel_fp += ax->vel_step_fp;
            if (ax->current_vel_fp >= ((uint64_t)ax->vel_target_hz << FP_SCALE)) {
                ax->current_vel_fp = ((uint64_t)ax->vel_target_hz << FP_SCALE);
                ax->state = STATE_CONST;
            }
        }
        else if (ax->state == STATE_DECEL) {
            if (ax->current_vel_fp > (((uint64_t)MIN_PULSE_FREQ_HZ << FP_SCALE) + ax->vel_step_fp))
                ax->current_vel_fp -= ax->vel_step_fp;
            else ax->current_vel_fp = ((uint64_t)MIN_PULSE_FREQ_HZ << FP_SCALE);
        }

        // Yer değiştirme hesabı: (Vel_FP * 4ms) / 1000
        uint32_t displacement_fp = (uint32_t)((ax->current_vel_fp * MASTER_PERIOD_MS) / 1000);

        if ((displacement_fp >> FP_SCALE) >= ax->steps_remaining) {
            ax->current_pos_int = ax->target_pos;
            ax->current_pos_fp = (int64_t)ax->target_pos << FP_SCALE;
            ax->state = STATE_IDLE;
        } else {
            if (ax->direction == 0) ax->current_pos_fp += displacement_fp;
            else ax->current_pos_fp -= displacement_fp;
            ax->steps_remaining -= (displacement_fp >> FP_SCALE);
            ax->current_pos_int = (int32_t)(ax->current_pos_fp >> FP_SCALE);
            if (ax->state == STATE_CONST && ax->steps_remaining <= ax->decel_start_steps) ax->state = STATE_DECEL;
        }
        Send_Axis_PDO(i, ax->current_pos_int);
    }
    Send_SYNC_Message();
}

#ifndef ASDA2_CONTROL_H
#define	ASDA2_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// --- Configuration Constants ---
#define NUM_AXES            4
#define STEPS_PER_MM        10000.0f
#define MASTER_PERIOD_MS    4
#define FP_SCALE            16
#define PLANNER_BUFFER_SIZE 10
#define MIN_PULSE_FREQ_HZ   300
#define ACCEL_TIME_MS       200

#define HOMING_SEEK_RATE_HZ  2000
#define HOMING_FEED_RATE_HZ  300

// Axis indices
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_A 3

// --- Motion Control Enums ---
typedef enum {
    STATE_IDLE,
    STATE_ACCEL,
    STATE_CONST,
    STATE_DECEL,
    STATE_HOMING_SEARCH,
    STATE_HOMING_LOCATE
} motion_state_t;

// --- Axis Structure ---
typedef struct {
    int32_t current_pos_int;   // Current integer pulse position
    int32_t current_pos_fp;    // Current 16.16 Fixed-Point position

    int32_t target_pos;        // Target position for the current block
    uint32_t steps_remaining;  // Steps left in the current move

    uint32_t current_vel_fp;   // Current velocity (16.16 Fixed-Point)
    uint32_t vel_step_fp;      // Velocity increment per 4ms cycle
    uint16_t vel_target_hz;    // Target velocity in Hz

    uint32_t accel_steps;      // Theoretical steps in accel phase
    uint32_t decel_steps;      // Theoretical steps in decel phase
    uint32_t accel_count;      // Internal counter

    motion_state_t state;
    bool direction;            // 0: Positive, 1: Negative
    uint32_t digital_inputs;   // Object 0x60FD
} ASDA2_Axis_t;

// --- Planner Block structure ---
typedef struct {
    ASDA2_Axis_t axis_data[NUM_AXES];
    bool is_valid;
} internal_block_t;

// --- Public Function Prototypes ---
void ASDA2_Initialize(void);
void move_to_absolute_position(float *target_mm, float feed_rate_mm_min);
void Process_Incoming_CAN_Messages(void);
void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse);
void Send_SYNC_Message(void);
void CANopen_Start_Nodes(void);
void Start_Axis_Homing(uint8_t axis_mask);

// Global axis array access
extern ASDA2_Axis_t axes_state[NUM_AXES];

#endif

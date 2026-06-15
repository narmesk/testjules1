#ifndef ASDA2_CONTROL_H
#define	ASDA2_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// --- Configuration Constants ---
#define NUM_AXES            4
#define STEPS_PER_MM        10000.0f
#define MASTER_PERIOD_MS    4
#define FP_SCALE            16
#define MIN_PULSE_FREQ_HZ   300
#define ACCEL_TIME_MS       200

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
    STATE_DECEL
} motion_state_t;

// --- Axis Structure (Independent) ---
typedef struct {
    int32_t current_pos_int;   // Current integer pulse position
    int64_t current_pos_fp;    // 64-bit 16.16 Fixed-Point precise position

    int32_t target_pos;        // Target pulse for the current move
    uint32_t steps_remaining;  // Pulses left to travel

    uint64_t current_vel_fp;   // Current velocity (64-bit 16.16 Fixed-Point)
    uint64_t vel_step_fp;      // Velocity increment per 4ms cycle
    uint32_t vel_target_hz;    // Target velocity in Hz

    uint32_t decel_start_steps; // Distance threshold to start deceleration

    motion_state_t state;
    bool direction;            // 0: Positive, 1: Negative
    uint32_t digital_inputs;   // Feedback from Drive (Object 0x60FD)
} ASDA2_Axis_t;

// --- Public Function Prototypes ---
void ASDA2_Initialize(void);

/**
 * move_single_axis_abs - Moves a specific axis independently.
 * No interpolation overhead.
 */
bool move_single_axis_abs(uint8_t axis_idx, float target_mm, float feed_rate_mm_min);

void Process_Incoming_CAN_Messages(void);
void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse);
void Send_SYNC_Message(void);
void CANopen_Start_Nodes(void);

// Global axis array access
extern ASDA2_Axis_t axes_state[NUM_AXES];

#endif

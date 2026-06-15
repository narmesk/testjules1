#ifndef ASDA2_CONTROL_H
#define	ASDA2_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// --- Konfigürasyon Sabitleri ---
#define NUM_AXES            4
#define STEPS_PER_MM        10000.0f
#define MASTER_PERIOD_MS    4
#define FP_SCALE            16
#define MIN_PULSE_FREQ_HZ   300
#define ACCEL_TIME_MS       200

#define HOMING_SEEK_RATE_HZ  2000
#define HOMING_FEED_RATE_HZ  300

// --- Hareket Durumları ---
typedef enum {
    STATE_IDLE,
    STATE_ACCEL,
    STATE_CONST,
    STATE_DECEL,
    STATE_HOMING_SEARCH,
    STATE_HOMING_LOCATE
} motion_state_t;

// --- Optimize Edilmiş Eksen Yapısı ---
typedef struct {
    int32_t current_pos_int;   // Tam sayı pulse pozisyonu (PDO için)
    int64_t current_pos_fp;    // 64-bit 16.16 Fixed-Point hassas pozisyon (Overflow engelleme)

    int32_t target_pos;        // Hedef pulse
    uint32_t steps_remaining;  // Kalan yol

    uint64_t current_vel_fp;   // Mevcut hız (64-bit 16.16 Fixed-Point)
    uint64_t vel_step_fp;      // İvme artışı (64-bit)
    uint32_t vel_target_hz;    // Hedef hız (Hz)

    uint32_t decel_start_steps; // Yavaşlama eşiği

    motion_state_t state;
    bool direction;
    uint32_t digital_inputs;
} ASDA2_Axis_t;

// --- Fonksiyon Prototipleri ---
void ASDA2_Initialize(void);
bool move_single_axis_abs(uint8_t axis_idx, float target_mm, float feed_rate_mm_min);
void Process_Incoming_CAN_Messages(void);
void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse);
void Send_SYNC_Message(void);
void CANopen_Start_Nodes(void);

extern ASDA2_Axis_t axes_state[NUM_AXES];

#endif

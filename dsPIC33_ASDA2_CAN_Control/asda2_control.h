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
// İnterpolasyon kalktığı için karmaşık bağımlılıklar temizlendi
typedef struct {
    int32_t current_pos_int;   // Tam sayı pulse pozisyonu (PDO için)
    int32_t current_pos_fp;    // 16.16 Fixed-Point hassas pozisyon

    int32_t target_pos;        // Hedef pulse
    uint32_t steps_remaining;  // Kalan yol

    uint32_t current_vel_fp;   // Mevcut hız (Fixed-Point)
    uint32_t vel_step_fp;      // Her periyottaki hız artışı (İvme)
    uint16_t vel_target_hz;    // İstenen hedef hız

    uint32_t decel_start_steps; // Yavaşlamaya başlanacak mesafe eşiği

    motion_state_t state;
    bool direction;            // 0: +, 1: -
    uint32_t digital_inputs;   // Sürücüden gelen 0x60FD verisi
} ASDA2_Axis_t;

// --- Fonksiyon Prototipleri ---
void ASDA2_Initialize(void);

// OPTİMİZE: Tek eksenli bağımsız hareket fonksiyonu
// İnterpolasyon matematiği içermez, doğrudan hedefe odaklanır
bool move_single_axis_abs(uint8_t axis_idx, float target_mm, float feed_rate_mm_min);

void Process_Incoming_CAN_Messages(void);
void Send_Axis_PDO(uint8_t axis_idx, int32_t target_pulse);
void Send_SYNC_Message(void);
void CANopen_Start_Nodes(void);
void Start_Axis_Homing(uint8_t axis_mask);

extern ASDA2_Axis_t axes_state[NUM_AXES];

#endif

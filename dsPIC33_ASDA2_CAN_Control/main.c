#include "mcc_generated_files/system.h"
#include "mcc_generated_files/timer5.h"
#include "asda2_control.h"

/**
 * ASDA-A2 CNC Kontrol - Bağımsız Eksen Sürümü
 * 64 MIPS / 1Mbps CAN
 */

int main(void)
{
    // Donanım ve Sürücüleri Başlat
    SYSTEM_Initialize();

    // ASDA2 Yapılarını Sıfırla
    ASDA2_Initialize();

    // Timer5 (4ms Master Task) Başlat
    Timer5_Initialize();

    // Küresel Kesmeleri Aç
    __builtin_enable_interrupts();

    // Tüm CAN Node'larını Başlat
    CANopen_Start_Nodes();

    while (1)
    {
        // LED Heartbeat (CPU çalışıyor göstergesi)
        static uint32_t counter = 0;
        if(counter++ > 200000) {
            LATBbits.LATB9 = !LATBbits.LATB9;
            counter = 0;
        }

        // ÖRNEK: X eksenini hareket ettir (Test için yorumu kaldırabilirsin)
        /*
        static bool move_triggered = false;
        if(!move_triggered) {
            move_single_axis_abs(AXIS_X, 100.0f, 1200.0f);
            move_triggered = true;
        }
        */
    }

    return 0;
}

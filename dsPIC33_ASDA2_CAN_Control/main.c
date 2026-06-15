#include "mcc_generated_files/system.h"
#include "mcc_generated_files/timer5.h"
#include "asda2_control.h"

int main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // ASDA-A2 kontrol yapılarını sıfırla
    ASDA2_Initialize();

    // Tüm sürücülere "Start" komutu gönder (Global NMT)
    CANopen_Start_Nodes();

    // Master Timer (4ms) başlat
    Timer5_Initialize();

    // Küresel kesmeleri (Global Interrupts) aktif et - KRİTİK!
    __builtin_enable_interrupts();

    while (1)
    {
        // G01/G00 Komut işleme ve Seri Haberleşme buraya gelecek

        // ÖRNEK: X eksenini 100mm'ye 1200mm/dk hızla gönder
        /*
        static bool started = false;
        if(!started) {
            move_single_axis_abs(AXIS_X, 100.0f, 1200.0f);
            started = true;
        }
        */
    }

    return 0;
}

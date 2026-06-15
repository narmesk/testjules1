#include "mcc_generated_files/system.h"
#include <xc.h>

/**
 * BASİT LED TESTİ (RB9)
 * Bu kodun tek amacı RB9 pinindeki LED'in yanıp söndüğünü görmektir.
 * Eğer LED yanmıyorsa donanım (kristal, besleme, mclr) sorunludur.
 */

int main(void)
{
    // Sistem saatini ve Pinleri başlat
    SYSTEM_Initialize();

    while (1)
    {
        // RB9 PININI TOGGLE ET
        LATBbits.LATB9 = !LATBbits.LATB9;

        // Uzun bir gecikme (64 MIPS'te yaklaşık 0.5 saniye)
        for(volatile uint32_t i=0; i<4000000; i++) {
            Nop();
        }
    }

    return 0;
}

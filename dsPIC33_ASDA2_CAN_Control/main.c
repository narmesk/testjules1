#include "mcc_generated_files/system.h"
#include "asda2_control.h"
#include <xc.h>

/**
 * CAN + LED DOĞRULAMA TESTİ
 * Hedef: LED (RB9) yanıp sönmeye devam ederken, her LED ON olduğunda
 * CAN hattından bir SYNC mesajı göndermek.
 */

int main(void)
{
    // Donanımı başlat (Clock, PPS, DMA, CAN)
    SYSTEM_Initialize();

    // ASDA2 yapılarını sıfırla (NMT vb. için)
    ASDA2_Initialize();

    while (1)
    {
        // LED DURUMUNU EVİR
        LATBbits.LATB9 = !LATBbits.LATB9;

        // Eğer LED ON ise (Lojik 1), CAN mesajı gönder
        if(LATBbits.LATB9) {
            // SYNC mesajı gönder (ID: 0x80)
            Send_SYNC_Message();
        }

        // Yaklaşık 1 saniye gecikme
        for(volatile uint32_t i=0; i<8000000; i++) {
            Nop();
        }
    }

    return 0;
}

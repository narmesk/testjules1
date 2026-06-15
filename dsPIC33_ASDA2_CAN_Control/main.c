#include "mcc_generated_files/system.h"
#include "asda2_control.h"
#include <xc.h>

/**
 * CAN DONANIM DOĞRULAMA (RB10 ve RB9)
 */

int main(void)
{
    // 1. Saat ve Port Başlatma
    OSCILLATOR_Initialize();

    // Analogları Kapat
    ANSELA = 0; ANSELB = 0; ANSELC = 0;

    // Pin Yönleri
    TRISBbits.TRISB9 = 0;  // LED
    TRISBbits.TRISB10 = 0; // CAN TX (RB10)

    // PPS Eşleme (Hard-coded Safe)
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));
    RPINR26bits.C1RXR = 44;
    _RP42R = 0x0E;
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    // --- MANUEL TEST: 5 KEZ BLINK ---
    for(int i=0; i<5; i++) {
        LATBbits.LATB9 = 1; LATBbits.LATB10 = 1;
        for(volatile uint32_t j=0; j<2000000; j++);
        LATBbits.LATB9 = 0; LATBbits.LATB10 = 0;
        for(volatile uint32_t j=0; j<2000000; j++);
    }

    // --- CAN BAŞLATMA ---
    DMA_Initialize();
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();

    ASDA2_Initialize();

    while (1)
    {
        // LED DURUMUNU EVİR
        LATBbits.LATB9 = !LATBbits.LATB9;

        // CAN MESAJI GÖNDER (Sürekli)
        // Eğer CAN modülü düzgün yapılandırıldıysa RB10'da trafik görmelisin.
        Send_SYNC_Message();

        // Hızlı Delay (Gözle görülür hızlı yanıp sönme)
        for(volatile uint32_t i=0; i<2000000; i++);
    }

    return 0;
}

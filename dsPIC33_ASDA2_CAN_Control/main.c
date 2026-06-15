#include "mcc_generated_files/system.h"
#include "asda2_control.h"
#include <xc.h>

/**
 * TEŞHİS MODU (DIAGNOSTIC MODE)
 * Bu kod, donanımın nerede takıldığını LED patternleri ile anlatır.
 */

int main(void)
{
    // 1. Temel Saat
    OSCILLATOR_Initialize();

    // 2. Pinler ve Güç
    PIN_MANAGER_Initialize();

    // --- MANUEL TEST: 5 KEZ YAVAŞ BLINK ---
    // Eğer bunu görüyorsan: Kristal ve Temel Boot Tamam.
    for(int i=0; i<5; i++) {
        LATBbits.LATB9 = 1; for(volatile uint32_t j=0; j<2000000; j++);
        LATBbits.LATB9 = 0; for(volatile uint32_t j=0; j<2000000; j++);
    }

    // 3. DMA Başlatma
    // LED 1 saniye sabit yanacak: DMA Init Başlıyor
    LATBbits.LATB9 = 1; for(volatile uint32_t j=0; j<4000000; j++);
    DMA_Initialize();
    LATBbits.LATB9 = 0; for(volatile uint32_t j=0; j<2000000; j++);

    // 4. CAN Başlatma
    // LED 2 saniye sabit yanacak: CAN Init Başlıyor (Mode switch bekleniyor)
    LATBbits.LATB9 = 1; for(volatile uint32_t j=0; j<8000000; j++);
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
    LATBbits.LATB9 = 0; for(volatile uint32_t j=0; j<2000000; j++);

    ASDA2_Initialize();

    while (1)
    {
        // LED HIZLI TOGGLE: HER ŞEY YOLUNDA, VERİ GÖNDERİLİYOR
        LATBbits.LATB9 = !LATBbits.LATB9;

        // CAN MESAJI GÖNDER
        Send_SYNC_Message();

        // Çok hızlı delay (Logic analizörde veri görmen için)
        for(volatile uint32_t i=0; i<500000; i++);
    }

    return 0;
}

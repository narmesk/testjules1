#include "mcc_generated_files/system.h"
#include <xc.h>

/**
 * PİN VE DONANIM DOĞRULAMA (RB10 ve RB9)
 * Bu kodun amacı:
 * 1. RB10 (CAN TX) pininin fiziksel olarak çıkış verip vermediğini test etmek.
 * 2. CAN modülü kilitlenirse LED üzerinden haber almak.
 */

int main(void)
{
    // 1. Sadece Temel Donanım (Saat ve Portlar)
    OSCILLATOR_Initialize();

    // Analogları Kapat
    ANSELA = 0; ANSELB = 0; ANSELC = 0;

    // Pinleri Çıkış Yap
    TRISBbits.TRISB9 = 0;  // LED
    TRISBbits.TRISB10 = 0; // CAN TX (RB10)

    // --- TEST 1: MANUEL PIN TOGGLE ---
    // Lojik analizörde RB10 ve RB9'un aynı anda yanıp söndüğünü görmelisin.
    // Eğer RB10 hala 0 ise donanım (pin kısa devresi vb.) kontrol edilmelidir.
    for(int i=0; i<5; i++) {
        LATBbits.LATB9 = 1;
        LATBbits.LATB10 = 1;
        for(volatile uint32_t j=0; j<2000000; j++);
        LATBbits.LATB9 = 0;
        LATBbits.LATB10 = 0;
        for(volatile uint32_t j=0; j<2000000; j++);
    }

    // --- TEST 2: CAN BAŞLATMA ---
    // Eğer kod burayı geçerse LED hızlı yanıp sönmeye başlayacak.
    DMA_Initialize();
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();

    while (1)
    {
        // CAN Modülü Loopback'te çalışıyorsa RB10'u artık CAN devralmış olmalı.
        // Bu yüzden manuel toggle burada etkisiz kalacaktır, sadece LED'i izle.
        LATBbits.LATB9 = !LATBbits.LATB9;

        // Hızlı Göz kırpma: CAN Aktif
        for(volatile uint32_t i=0; i<1000000; i++);
    }

    return 0;
}

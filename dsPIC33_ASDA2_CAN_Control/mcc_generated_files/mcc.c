#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 - Brute Force PPS Eşleme (RP42 / RB10)

    // 1. ANALOG ÖZELLİKLERİ DEVRE DIŞI BIRAK (TÜM PORTLAR)
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // 2. PIN YÖNLERİ
    TRISBbits.TRISB9 = 0;   // LED
    TRISBbits.TRISB10 = 0;  // CAN TX
    TRISBbits.TRISB12 = 1;  // CAN RX

    // 3. PPS KİLİDİNİ AÇ
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 RX: RP44 (RB12) -> 44
    RPINR26bits.C1RXR = 44;

    // ECAN1 TX: RP42 (RB10) -> 0x0E (C1TX Fonksiyonu)
    // dsPIC33EP'de RP42'nin hangi RPOR register'ında olduğu bazen belirsizdir.
    // Tüm şüpheli adreslere doğrudan yazarak garantiye alıyoruz.
    _RP42R = 0x0E;          // Standart Makro
    *((uint16_t*)0x068A) = 0x000E; // RPOR5 (RP42 Low)
    *((uint16_t*)0x0688) = 0x0E00; // RPOR4 (RP42 High - Bazı versiyonlar için)

    // 4. PPS KİLİTLE
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    // Başlangıçta pini lojik 1 (Recessive) yap (CAN modülü devralana kadar)
    LATBbits.LATB10 = 1;
}

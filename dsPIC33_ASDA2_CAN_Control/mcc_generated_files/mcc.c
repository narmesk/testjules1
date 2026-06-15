#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 Güvenli Pin ve Güç Başlatma

    // 1. TÜM MODÜLLERİ UYANDIR (PMD Registerları)
    // Eğer bir modül PMD ile kapatılmışsa registerlarına yazamazsın.
    PMD1 = 0x0000;
    PMD2 = 0x0000;
    PMD3 = 0x0000;
    PMD4 = 0x0000;
    PMD6 = 0x0000;
    PMD7 = 0x0000;

    // 2. ANALOGLARI KAPAT
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // 3. PIN YÖNLERİ
    TRISBbits.TRISB9 = 0;   // LED
    TRISBbits.TRISB10 = 0;  // CAN TX (RB10)
    TRISBbits.TRISB12 = 1;  // CAN RX (RB12)

    // 4. PPS (Peripheral Pin Select) - EN KRİTİK NOKTA
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); // Unlock

    // ECAN1 RX: RP44 (RB12)
    RPINR26bits.C1RXR = 44;

    // ECAN1 TX: RP42 (RB10)
    // dsPIC33EP64MC504'te RP42, RPOR5 register'ındadır. (RPOR4 değil!)
    // Hem makro hem doğrudan adres ile garantiye alıyoruz.
    _RP42R = 0x0E;
    *((uint16_t*)0x068A) = 0x000E; // RPOR5 adresi (Eğer makro hatalıysa)

    __builtin_write_OSCCONL(OSCCON | (1<<6)); // Lock
}

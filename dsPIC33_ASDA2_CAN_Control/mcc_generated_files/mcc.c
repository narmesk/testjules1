#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 - Brute Force PPS Eşleme

    // 1. DİJİTAL MODA ZORLA
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // 2. PIN YÖNLERİ
    TRISBbits.TRISB9 = 0;   // LED
    TRISBbits.TRISB10 = 0;  // CAN TX
    TRISBbits.TRISB12 = 1;  // CAN RX

    // 3. PPS UNLOCK
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 RX: RP44 (RB12)
    RPINR26bits.C1RXR = 44;

    // ECAN1 TX: RP42 (RB10)
    // dsPIC33EP'de RP42'nin bulunduğu register belirsizliği için
    // hem RPOR4 hem RPOR5 hem de makroyu aynı anda dolduruyoruz.
    _RP42R = 0x0E;
    *((uint16_t*)0x0688) = 0x0E00; // RPOR4 (High word - RP42)
    *((uint16_t*)0x068A) = 0x000E; // RPOR5 (Low word - RP42)

    // 4. PPS LOCK
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    // Donanım devralana kadar pini 1 yap
    LATBbits.LATB10 = 1;
}

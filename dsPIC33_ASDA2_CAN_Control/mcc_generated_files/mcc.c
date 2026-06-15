#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 için En Güvenli Pin Başlatma

    // Tüm Analogları Devre Dışı Bırak
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // Pin Yönleri
    TRISBbits.TRISB9 = 0;   // LED
    TRISBbits.TRISB10 = 0;  // CAN TX (Output)
    TRISBbits.TRISB12 = 1;  // CAN RX (Input)

    // PPS Kilit Açma
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 RX: RP44 (RB12) -> 44
    RPINR26bits.C1RXR = 44;

    // ECAN1 TX: RP42 (RB10) -> Function 0x0E (C1TX)
    // dsPIC33EP'de RP42, RPOR5 içerisindedir (RPOR4 değil!)
    _RP42R = 0x0E;

    // PPS Kilitleme
    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 Final Hardware Config

    // 1. Digital Mode Force
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // 2. Pin Directions
    TRISBbits.TRISB9 = 0;   // LED (Heartbeat)
    TRISBbits.TRISB10 = 0;  // ECAN1 TX
    TRISBbits.TRISB12 = 1;  // ECAN1 RX

    // 3. PPS Mapping (Verified macros)
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    RPINR26bits.C1RXR = 44; // RB12
    _RP42R = 0x0E;          // RB10

    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

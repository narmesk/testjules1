#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 PPS Configuration
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 Mapping (RP54 RX, RP55 TX as typical example)
    RPINR26bits.C1RXR = 54;
    RPOR6bits.RP55R = 0x0E; // C1TX

    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

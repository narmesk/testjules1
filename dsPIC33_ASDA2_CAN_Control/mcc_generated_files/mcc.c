#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 Peripheral Pin Select (PPS) Configuration

    // Unlock Registers
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 RX: Map to RP54 (Example)
    // RPINR26bits.C1RXR = 54;

    // ECAN1 TX: Map to RP55 (Example)
    // RPOR6bits.RP55R = 0x0E; // 0x0E is C1TX function

    // Lock Registers
    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

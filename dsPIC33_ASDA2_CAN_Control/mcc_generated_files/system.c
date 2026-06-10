#include "system.h"
#include "mcc.h"
#include "can1.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();
    CAN1_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // dsPIC33EP64MC504 Clock initialization for 70 MIPS
    // Fosc = Fin * (M / (N1 * N2))
    // Example with 7.37MHz Internal FRC:
    // CLKDIVbits.PLLPRE = 0; // N1 = 2
    // PLLFBDbits.PLLDIV = 74; // M = 76
    // CLKDIVbits.PLLPOST = 0; // N2 = 2
    // Fosc = 7.37 * (76 / (2 * 2)) = 140.03 MHz
    // Fcy = Fosc / 2 = 70 MIPS

    /*
    OSCCONbits.NOSC = 0b001; // New Oscillator selection: FRC with PLL
    OSCCONbits.OSWEN = 1;    // Request Oscillator Switch
    while (OSCCONbits.COSC != 0b001); // Wait for Switch
    while (OSCCONbits.LOCK != 1);     // Wait for PLL Lock
    */
}

#include "system.h"
#include "mcc.h"
#include "can1.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();

    // CAN and DMA must be enabled for dsPIC33EP ECAN
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
}

void OSCILLATOR_Initialize(void)
{
    // 70 MIPS Configuration (7.37MHz FRC -> 140MHz Fosc)
    CLKDIVbits.PLLPRE = 0;  // N1=2
    PLLFBDbits.PLLDIV = 74; // M=76
    CLKDIVbits.PLLPOST = 0; // N2=2

    // Initiate Clock Switch
    __builtin_write_OSCCONH(0x01); // New: FRC w/ PLL
    __builtin_write_OSCCONL(OSCCON | 0x01);
    while (OSCCONbits.COSC != 0x01);
    while (OSCCONbits.LOCK != 1);
}

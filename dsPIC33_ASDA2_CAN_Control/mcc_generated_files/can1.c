#include "can1.h"

// dsPIC33EP ECAN requires DMA Memory.
// Define a buffer in a specific DMA-accessible RAM section.
/*
__eds__ uint16_t ECAN1_MsgBuf[4][8] __attribute__((aligned(32*2), section("space(xmemory)")));
*/

void CAN1_Initialize(void)
{
    C1CTRL1bits.REQOP = 4; // Configuration Mode
    while(C1CTRL1bits.OPMODE != 4);

    // 1Mbps @ 70MIPS Example
    // Fbak = 70MHz. Tq = 2 * (1+BRP) / Fbak.
    // 1Mbps -> 1us. 14 Tq total.
    C1CFG1bits.BRP = 2; // BRP = 2 -> Tq = 2 * 3 / 70M = 85.7ns
    C1CFG2bits.SEG1PH = 4;
    C1CFG2bits.SEG2PH = 4;
    C1CFG2bits.PRSEG = 2;
    C1CFG2bits.SAM = 1;

    // DMA and Buffer Setup Placeholder
    // C1FMSKSEL1 = 0x0000; // Use Filter Mask 0
    // C1BUFPNT1 = 0x0000;  // Filter 0 uses Buffer 0

    C1CTRL1bits.REQOP = 0; // Normal Mode
    while(C1CTRL1bits.OPMODE != 0);
}

bool CAN1_Transmit(CAN1_MSG_OBJ *msg)
{
    // 1. Check if Buffer 0 is free (C1TR01CONbits.TXREQ0 == 0)
    // 2. Format msg into ECAN1_MsgBuf[0]
    // 3. Set TXREQ0 = 1

    return false;
}

bool CAN1_Receive(CAN1_MSG_OBJ *msg)
{
    // Check C1RXFUL1 register for new messages
    return false;
}

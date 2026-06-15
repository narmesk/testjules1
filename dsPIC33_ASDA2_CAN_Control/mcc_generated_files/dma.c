#include "dma.h"

void DMA_Initialize(void)
{
    // dsPIC33EP64MC504 DMA Configuration for ECAN1

    // DMA0: CAN1 RX
    DMA0CONbits.CHEN = 0;
    DMA0CONbits.SIZE = 0;   // Word
    DMA0CONbits.DIR = 0;    // Peripheral to RAM
    DMA0CONbits.HALF = 0;
    DMA0CONbits.NULLW = 0;
    DMA0CONbits.AMODE = 0b10; // Peripheral Indirect Addressing
    DMA0CONbits.MODE = 0;     // Continuous

    DMA0PAD = (uint16_t)&C1RXD;
    DMA0CNT = 7;            // 8 words per message
    DMA0REQ = 0x22;         // CAN1 RX IRQ

    // DMA1: CAN1 TX
    DMA1CONbits.CHEN = 0;
    DMA1CONbits.SIZE = 0;   // Word
    DMA1CONbits.DIR = 1;    // RAM to Peripheral
    DMA1CONbits.HALF = 0;
    DMA1CONbits.NULLW = 0;
    DMA1CONbits.AMODE = 0b10; // Peripheral Indirect Addressing
    DMA1CONbits.MODE = 0;     // Continuous

    DMA1PAD = (uint16_t)&C1TXD;
    DMA1CNT = 7;            // 8 words per message
    DMA1REQ = 0x46;         // CAN1 TX IRQ
}

void DMA_PeripheralAddressSet(uint8_t channel, uint16_t address)
{
    if(channel == 0) DMA0PAD = address;
    else if(channel == 1) DMA1PAD = address;
}

void DMA_StartAddressASet(uint8_t channel, uint16_t address)
{
    if(channel == 0) DMA0STAL = address;
    else if(channel == 1) DMA1STAL = address;
}

void DMA_ChannelEnable(uint8_t channel)
{
    if(channel == 0) DMA0CONbits.CHEN = 1;
    else if(channel == 1) DMA1CONbits.CHEN = 1;
}

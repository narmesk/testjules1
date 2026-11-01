#include "dma.h"

void DMA_Initialize(void)
{
    // dsPIC33EP64MC504 DMA Configuration for CAN1 (ECAN)

    // DMA0: CAN1 RX
    DMA0CONbits.CHEN = 0;   // Disable channel
    DMA0CONbits.SIZE = 0;   // Word
    DMA0CONbits.DIR = 0;    // Peripheral to RAM
    DMA0CONbits.AMODE = 0b10; // Peripheral Indirect Addressing (KRİTİK: ECAN için bu mod zorunlu)
    DMA0CONbits.MODE = 0;     // Continuous

    DMA0PAD = (uint16_t)&C1RXD; // CAN1 Receive Register
    DMA0CNT = 7;            // 8 words per message
    DMA0REQ = 0x22;         // CAN1 RX IRQ (KRİTİK: DMA0 CAN1 RX'e bağlanmalı)

    // DMA1: CAN1 TX
    DMA1CONbits.CHEN = 0;   // Disable channel
    DMA1CONbits.SIZE = 0;   // Word
    DMA1CONbits.DIR = 1;    // RAM to Peripheral
    DMA1CONbits.AMODE = 0b10; // Peripheral Indirect Addressing (KRİTİK: ECAN için bu mod zorunlu)
    DMA1CONbits.MODE = 0;     // Continuous

    DMA1PAD = (uint16_t)&C1TXD; // CAN1 Transmit Register
    DMA1CNT = 7;            // 8 words per message
    DMA1REQ = 0x46;         // CAN1 TX IRQ (KRİTİK: DMA1 CAN1 TX'e bağlanmalı)

    // STAL adresleri can1.c içindeki CAN1_TransmitEnable/ReceiveEnable tarafından set edilecektir.

    DMA0CONbits.CHEN = 1; // Enable Channel 0
    DMA1CONbits.CHEN = 1; // Enable Channel 1

    // UART1 için kullandığın DMA2/3 ayarlarını koruyoruz (varsa)
    DMA2CONbits.CHEN = 0;
    DMA2REQ = 0x0B; // UART1 RX
    // ...
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

#ifndef DMA_H
#define	DMA_H

#include <xc.h>
#include <stdint.h>

void DMA_Initialize(void);
void DMA_PeripheralAddressSet(uint8_t channel, uint16_t address);
void DMA_StartAddressASet(uint8_t channel, uint16_t address);
void DMA_ChannelEnable(uint8_t channel);

#endif

#ifndef CAN1_H
#define	CAN1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// dsPIC33EP64MC504 ECAN Mesaj Nesnesi Yapısı
typedef struct {
    uint32_t id;
    struct {
        uint8_t idType; // 0: Standard, 1: Extended
        uint8_t dlc;
    } msgAttr;
    uint8_t data[8];
} CAN1_MSG_OBJ;

/**
 * dsPIC33EP ECAN Modülü için DMA Bellek Yapısı
 * Her mesaj 8 word (16 byte) yer kaplar.
 * Bu yapı DMA RAM (EDS) alanında tanımlanmalıdır.
 */
typedef struct {
    uint16_t word0;
    uint16_t word1;
    uint16_t word2;
    uint16_t word3;
    uint16_t data[4]; // 8 byte veri
} ECAN1_MSG_BUF;

void CAN1_Initialize(void);
bool CAN1_Transmit(CAN1_MSG_OBJ *msg);
bool CAN1_Receive(CAN1_MSG_OBJ *msg);

#endif

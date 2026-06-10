#ifndef CAN1_H
#define	CAN1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t id;
    struct {
        uint8_t idType;
        uint8_t msgType;
        uint8_t dlc;
    } msgAttr;
    uint8_t data[8];
} CAN1_MSG_OBJ;

void CAN1_Initialize(void);
bool CAN1_Transmit(CAN1_MSG_OBJ *msg);
bool CAN1_Receive(CAN1_MSG_OBJ *msg);

#endif

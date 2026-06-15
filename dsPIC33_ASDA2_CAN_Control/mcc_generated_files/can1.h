#ifndef CAN1_H
#define	CAN1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "can_types.h"

void CAN1_Initialize(void);
void CAN1_TransmitEnable(void);
void CAN1_ReceiveEnable(void);
CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode);
CAN_OP_MODES CAN1_OperationModeGet(void);
CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg);
bool CAN1_Receive(CAN_MSG_OBJ *recCanMsg);
bool CAN1_IsBusOff(void);
uint8_t CAN1_ReceivedMessageCountGet(void);
void CAN1_Sleep(void);

// Deprecated APIs
bool CAN1_transmit(CAN_TX_PRIOIRTY priority, uCAN_MSG *sendCanMsg);
bool CAN1_receive(uCAN_MSG *recCanMsg);

#endif

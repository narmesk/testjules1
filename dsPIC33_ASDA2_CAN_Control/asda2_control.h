#ifndef ASDA2_CONTROL_H
#define	ASDA2_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SERVOS 4

// CiA402 State Machine Definitions
typedef enum {
    STATE_NOT_READY_TO_SWITCH_ON,
    STATE_SWITCH_ON_DISABLED,
    STATE_READY_TO_SWITCH_ON,
    STATE_SWITCHED_ON,
    STATE_OPERATION_ENABLED,
    STATE_QUICK_STOP_ACTIVE,
    STATE_FAULT_REACTION_ACTIVE,
    STATE_FAULT
} DS402_STATE;

typedef struct {
    uint8_t nodeId;
    DS402_STATE currentState;
    uint16_t statusWord;
    uint16_t controlWord;
    int32_t targetPosition;
    int32_t actualPosition;
    bool isEnabled;
} ASDA2_Servo_t;

// NMT Commands
#define NMT_START_REMOTE_NODE        0x01
#define NMT_STOP_REMOTE_NODE         0x02
#define NMT_ENTER_PRE_OPERATIONAL    0x80
#define NMT_RESET_NODE               0x81
#define NMT_RESET_COMMUNICATION      0x82

extern ASDA2_Servo_t servos[MAX_SERVOS];

void ASDA2_Initialize(void);
void ASDA2_SetNMTState(uint8_t nodeId, uint8_t command);
void ASDA2_DriveStateMachine(void);

#endif

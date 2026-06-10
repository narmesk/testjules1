#include "asda2_control.h"
#include "mcc_generated_files/can1.h"

ASDA2_Servo_t servos[MAX_SERVOS];

void ASDA2_Initialize(void)
{
    for(int i = 0; i < MAX_SERVOS; i++)
    {
        servos[i].nodeId = i + 1; // Default node IDs 1, 2, 3, 4
        servos[i].currentState = STATE_NOT_READY_TO_SWITCH_ON;
        servos[i].isEnabled = false;
        servos[i].targetPosition = 0;
        servos[i].actualPosition = 0;
    }
}

void ASDA2_SetNMTState(uint8_t nodeId, uint8_t command)
{
    CAN1_MSG_OBJ msg;
    msg.id = 0x000; // NMT Protocol ID
    msg.msgAttr.dlc = 2;
    msg.msgAttr.idType = 0; // Standard ID
    msg.data[0] = command;
    msg.data[1] = nodeId;
    CAN1_Transmit(&msg);
}

void ASDA2_DriveStateMachine(void)
{
    for(int i = 0; i < MAX_SERVOS; i++)
    {
        // Implementation of DS402 State Machine logic for each servo
        // This will be expanded in the next phase
    }
}

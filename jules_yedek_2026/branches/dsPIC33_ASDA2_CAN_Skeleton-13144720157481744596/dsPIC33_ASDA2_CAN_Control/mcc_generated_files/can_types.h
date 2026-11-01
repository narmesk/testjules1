#ifndef _CAN_TYPES_H
#define _CAN_TYPES_H

#include <stdint.h>

typedef union {
    uint8_t msgfields;
    struct{
        uint8_t idType:1;       // 1 bit (Standard Frame or Extended Frame)
        uint8_t frameType:1;    // 1 bit (Data Frame or RTR Frame)
        uint8_t dlc:4;          // 4 bit (No of data bytes a message frame contains)
        uint8_t reserved:2;     // 2 bit (Reserved bit)
    };
} CAN_MSG_FIELD;

typedef struct
{
    uint32_t msgId;          // 29 bit (SID: 11bit, EID:18bit)
    CAN_MSG_FIELD field;     // CAN TX/RX Message Object Control
    uint8_t *data;           // Pointer to message data
} CAN_MSG_OBJ;

typedef enum
{
    CAN_TX_MSG_REQUEST_SUCCESS = 0,             // Transmit message object successfully placed into Transmit Buffer
    CAN_TX_MSG_REQUEST_DLC_ERROR = 1,           // Transmit message object DLC size is more than Transmit Buffer configured DLC size
    CAN_TX_MSG_REQUEST_BUFFER_FULL = 3,         // Transmit Buffer is Full
} CAN_TX_MSG_REQUEST_STATUS;

typedef enum
{
    CAN_NORMAL_OPERATION_MODE = 0,
    CAN_NORMAL_2_0_MODE = 0,
    CAN_DISABLE_MODE = 1,
    CAN_LOOPBACK_MODE = 2,
    CAN_INTERNAL_LOOPBACK_MODE = 2,
    CAN_LISTEN_ONLY_MODE = 3,
    CAN_CONFIGURATION_MODE = 4,
    CAN_LISTEN_ALL_MESSAGES_MODE = 7
} CAN_OP_MODES;

typedef enum{
    CAN_PRIORITY_HIGH = 0b11,
    CAN_PRIORITY_MEDIUM = 0b10,
    CAN_PRIORITY_LOW = 0b01,
    CAN_PRIORITY_NONE = 0b00
} CAN_TX_PRIOIRTY;

typedef enum
{
    CAN_FRAME_STD       = 0,
    CAN_FRAME_EXT       = 1,
} CAN_MSG_OBJ_ID_TYPE;

typedef enum
{
    CAN_FRAME_DATA      = 0,
    CAN_FRAME_RTR       = 1,
} CAN_MSG_OBJ_FRAME_TYPE;

typedef enum
{
    CAN_OP_MODE_REQUEST_SUCCESS,     // Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL,        // Requested Operation mode set failure.
}  CAN_OP_MODE_STATUS;

typedef enum
{
    CAN_DLC_0,
    CAN_DLC_1,
    CAN_DLC_2,
    CAN_DLC_3,
    CAN_DLC_4,
    CAN_DLC_5,
    CAN_DLC_6,
    CAN_DLC_7,
    CAN_DLC_8
} CAN_DLC;

#define CAN_MSG_DATA    0x01
#define CAN_MSG_RTR     0x02
#define CAN_BUF_FULL	0x05
#define CAN_BUF_EMPTY	0x06

typedef union {
    struct {
        uint32_t id;
        uint8_t idType;
        uint8_t msgtype;
        uint8_t dlc;
        uint8_t data0;
        uint8_t data1;
        uint8_t data2;
        uint8_t data3;
        uint8_t data4;
        uint8_t data5;
        uint8_t data6;
        uint8_t data7;
    } frame;
    uint8_t array[16];
} uCAN_MSG;

#endif

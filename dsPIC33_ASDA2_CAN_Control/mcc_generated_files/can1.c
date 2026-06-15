#include "can1.h"
#include "dma.h"

#define CAN1_TX_DMA_CHANNEL 1
#define CAN1_RX_DMA_CHANNEL 0
#define CAN1_MESSAGE_BUFFERS 8

typedef struct __attribute__((packed))
{
    unsigned priority                   :2;
    unsigned remote_transmit_enable     :1;
    unsigned send_request               :1;
    unsigned error                      :1;
    unsigned lost_arbitration           :1;
    unsigned message_aborted            :1;
    unsigned transmit_enabled           :1;
} CAN1_TX_CONTROLS;

/* dsPIC33EP DMA RAM Alignment */
static unsigned int can1msgBuf [CAN1_MESSAGE_BUFFERS][8] __attribute__((aligned(128)));

static void CAN1_DMACopy(uint8_t buffer_number, CAN_MSG_OBJ *message)
{
    uint16_t ide=can1msgBuf[buffer_number][0] & 0x0001U;
    if(ide==0U) {
        message->msgId =(can1msgBuf[buffer_number][0] & 0x1FFCU) >> 2U;
        message->field.idType = CAN_FRAME_STD;
    } else {
        uint32_t id=can1msgBuf[buffer_number][0] & 0x1FFCU;
        message->msgId = (id << 16U) + ((uint32_t)can1msgBuf[buffer_number][1] << 6U) + (can1msgBuf[buffer_number][2] >> 10U);
        message->field.idType = CAN_FRAME_EXT;
    }
    message->field.frameType = (can1msgBuf[buffer_number][0] & 0x0002U) ? CAN_FRAME_RTR : CAN_FRAME_DATA;
    message->data[0] =(uint8_t) can1msgBuf[buffer_number][3];
    message->data[1] =(uint8_t) (can1msgBuf[buffer_number][3] >> 8);
    message->data[2] =(uint8_t) can1msgBuf[buffer_number][4];
    message->data[3] =(uint8_t) (can1msgBuf[buffer_number][4] >> 8);
    message->data[4] =(uint8_t) can1msgBuf[buffer_number][5];
    message->data[5] =(uint8_t) (can1msgBuf[buffer_number][5] >> 8);
    message->data[6] =(uint8_t) can1msgBuf[buffer_number][6];
    message->data[7] =(uint8_t) (can1msgBuf[buffer_number][6] >> 8);
    message->field.dlc =(uint8_t) (can1msgBuf[buffer_number][2] & 0x000FU);
}

static void CAN1_MessageToBuffer(uint16_t* buffer, CAN_MSG_OBJ* message)
{
    if(message->field.idType == CAN_FRAME_STD) {
        buffer[0]= ((message->msgId & 0x07FF) << 2) + ((uint16_t)(message->field.frameType << 1) & 0x0002);
        buffer[1]= 0;
        buffer[2]= message->field.dlc & 0x0F;
    } else {
        buffer[0]= ( ( (uint16_t)(message->msgId >> 16 ) & 0x1FFC ) ) | 0x3;
        buffer[1]= (uint16_t)(message->msgId >> 6) & 0x0FFF;
        buffer[2]= (message->field.dlc & 0x0F) + ( (uint16_t)(message->msgId << 10) & 0xFC00) + ((uint16_t)(message->field.frameType << 9) & 0x0200);
    }
    if(message->data != (void*)0) {
        buffer[3]= ((uint16_t)message->data[1]<<8) + message->data[0];
        buffer[4]= ((uint16_t)message->data[3]<<8) + message->data[2];
        buffer[5]= ((uint16_t)message->data[5]<<8) + message->data[4];
        buffer[6]= ((uint16_t)message->data[7]<<8) + message->data[6];
    }
}

void CAN1_Initialize(void)
{
    // Mode Switch Timeout ekleyerek asılı kalmayı önlüyoruz
    uint16_t timeout = 0;

    C1CTRL1bits.REQOP = 4;
    while(C1CTRL1bits.OPMODE != 4 && timeout++ < 1000);

    C1CTRL1bits.CANCKS = 0x1;
    C1CFG1 = 0x0003;
    C1CFG2 = 0x0190;
    C1FCTRL = 0x0002;

    C1TR01CONbits.TXEN0 = 1;
    C1TR01CONbits.TXEN1 = 0;

    // Loopback Modu
    timeout = 0;
    C1CTRL1bits.REQOP = 2;
    while(C1CTRL1bits.OPMODE != 2 && timeout++ < 1000);
}

void CAN1_TransmitEnable()
{
    DMA_PeripheralAddressSet(CAN1_TX_DMA_CHANNEL, (uint16_t) &C1TXD);
    DMA_StartAddressASet(CAN1_TX_DMA_CHANNEL, __builtin_dmaoffset(&can1msgBuf));
    DMA_ChannelEnable(CAN1_TX_DMA_CHANNEL);
}

void CAN1_ReceiveEnable()
{
    DMA_PeripheralAddressSet(CAN1_RX_DMA_CHANNEL, (uint16_t) &C1RXD);
    DMA_StartAddressASet(CAN1_RX_DMA_CHANNEL, __builtin_dmaoffset(&can1msgBuf) );
    DMA_ChannelEnable(CAN1_RX_DMA_CHANNEL);
}

CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg)
{
    CAN1_TX_CONTROLS * pTxControls = (CAN1_TX_CONTROLS*)&C1TR01CON;
    if (pTxControls->send_request == 0) {
        CAN1_MessageToBuffer(&can1msgBuf[0][0], sendCanMsg);
        pTxControls->priority = priority;
        pTxControls->send_request = 1;
        return CAN_TX_MSG_REQUEST_SUCCESS;
    }
    return CAN_TX_MSG_REQUEST_BUFFER_FULL;
}

bool CAN1_Receive(CAN_MSG_OBJ *recCanMsg)
{
    if (C1RXFUL1 != 0) {
        for (int i=1 ; i < 8; i++) {
            if ((C1RXFUL1 >> i) & 0x1) {
               CAN1_DMACopy(i, recCanMsg);
               C1RXFUL1 &= ~(1 << i);
               return true;
            }
        }
    }
    return false;
}

void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void) { IFS2bits.C1IF = 0; }

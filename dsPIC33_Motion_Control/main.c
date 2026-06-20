#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi - Modüler 4-Eksen Yapısı (V6)
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 */

// ---- Uygulama Değişkenleri ----
unsigned int Aux_Value_In;
unsigned int Aux_Value_Out;
unsigned char OUTPUTSH_VAL;
unsigned char OUTPUTSL_VAL;
unsigned char INPUTSH_VAL;
unsigned char INPUTSL_VAL;

APP_CONFIG AppConfig;
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};

extern UdpServerStep UdpServerStep1;
extern unsigned char uart_cnt;
unsigned char other_cnt;

// Eksen Verileri (Örnek takip için)
int32_t axis_actual_pos[5] = {0, 0, 0, 0, 0}; // 1-4 arası node'lar için
uint16_t axis_status[5] = {0, 0, 0, 0, 0};

// ---- CANopen Yardımcı Fonksiyonları ----

/**
 * Belirli bir node'a SDO yazma komutu gönderir.
 * len: 1, 2 veya 4 byte
 */
void CAN_WriteSDO(uint8_t nodeId, uint16_t index, uint8_t subindex, uint32_t data, uint8_t len)
{
    CAN_MSG_OBJ msg;
    uint8_t sdo_data[8] = {0};

    // Command specifier belirleme
    if (len == 1) sdo_data[0] = 0x2F;
    else if (len == 2) sdo_data[0] = 0x2B;
    else sdo_data[0] = 0x23;

    sdo_data[1] = (uint8_t)(index & 0xFF);
    sdo_data[2] = (uint8_t)((index >> 8) & 0xFF);
    sdo_data[3] = subindex;
    sdo_data[4] = (uint8_t)(data & 0xFF);
    sdo_data[5] = (uint8_t)((data >> 8) & 0xFF);
    sdo_data[6] = (uint8_t)((data >> 16) & 0xFF);
    sdo_data[7] = (uint8_t)((data >> 24) & 0xFF);

    msg.msgId = 0x600 + nodeId;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.dlc = 8;
    msg.data = sdo_data;

    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
    DelayMs(10); // Sürücünün işleme süresi
}

/**
 * SDO üzerinden veri okuma isteği gönderir.
 */
void CAN_ReadSDO_Request(uint8_t nodeId, uint16_t index, uint8_t subindex)
{
    CAN_MSG_OBJ msg;
    uint8_t sdo_data[8] = {0x40, 0, 0, 0, 0, 0, 0, 0};

    sdo_data[1] = (uint8_t)(index & 0xFF);
    sdo_data[2] = (uint8_t)((index >> 8) & 0xFF);
    sdo_data[3] = subindex;

    msg.msgId = 0x600 + nodeId;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.dlc = 8;
    msg.data = sdo_data;

    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

/**
 * PDO kullanarak hızlı Controlword gönderimi.
 */
void Drive_SendControlword(uint8_t nodeId, uint16_t cw)
{
    CAN_MSG_OBJ msg;
    uint8_t data[2];
    data[0] = (uint8_t)(cw & 0xFF);
    data[1] = (uint8_t)((cw >> 8) & 0xFF);

    msg.msgId = 0x200 + nodeId;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.dlc = 2;
    msg.data = data;

    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

// ---- Hareket Kontrol Fonksiyonları ----

/**
 * Belirtilen motoru ilklendirir ve Servo-On yapar.
 */
void Drive_Init(uint8_t nodeId)
{
    // 1. Mod Ayarı: Profile Position
    CAN_WriteSDO(nodeId, 0x6060, 0x00, 0x01, 1);

    // 2. Maksimum Tork Sınırı (%100)
    CAN_WriteSDO(nodeId, 0x6072, 0x00, 1000, 2);

    // 3. NMT Operational Moduna Geçir
    CAN_MSG_OBJ nmt;
    uint8_t nmt_data[2] = {0x01, nodeId};
    nmt.msgId = 0x000;
    nmt.field.dlc = 2;
    nmt.data = nmt_data;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &nmt);
    DelayMs(100);

    // 4. DS402 Servo-On Sıralaması
    Drive_SendControlword(nodeId, 0x0080); DelayMs(100); // Fault Reset
    Drive_SendControlword(nodeId, 0x0006); DelayMs(100); // Shutdown
    Drive_SendControlword(nodeId, 0x0007); DelayMs(100); // Switch On
    Drive_SendControlword(nodeId, 0x000F); DelayMs(100); // Enable Operation
}

/**
 * Belirtilen motoru hareket ettirir.
 * position: hedef (pulse)
 * velocity: hız (pulse/sn)
 * accel: ivme (pulse/sn^2)
 * relative: true ise bağıl, false ise mutlak hareket
 */
void Drive_Move(uint8_t nodeId, int32_t position, uint32_t velocity, uint32_t accel, bool relative)
{
    // 1. Profil Parametrelerini Ayarla
    CAN_WriteSDO(nodeId, 0x6081, 0x00, velocity, 4);
    CAN_WriteSDO(nodeId, 0x6083, 0x00, accel, 4);
    CAN_WriteSDO(nodeId, 0x6084, 0x00, accel, 4);

    // 2. Hedef Pozisyonu Yaz
    CAN_WriteSDO(nodeId, 0x607A, 0x00, (uint32_t)position, 4);

    // 3. Hareketi Tetikle (Bit 4: New Set-point)
    uint16_t cw = relative ? 0x005F : 0x001F;
    Drive_SendControlword(nodeId, cw);
    DelayMs(50);

    // 4. Handshake: Tetiklemeyi temizle (Bit 4: 0)
    Drive_SendControlword(nodeId, 0x000F);
}

/**
 * Motor durumlarını sorgular.
 */
void Drive_QueryStatus(uint8_t nodeId)
{
    CAN_ReadSDO_Request(nodeId, 0x6064, 0x00); // Actual Position
    DelayMs(5);
    CAN_ReadSDO_Request(nodeId, 0x6041, 0x00); // Statusword
}

// ---- Sistem Fonksiyonları ----

void PLC_VaribleClear(void);
void Timer3ISR(void);
void read_input(void);
void set_outpus(void);

int main(void)
{
    SYSTEM_Initialize();
    IEC0bits.U1TXIE = 0; IEC0bits.U1RXIE = 0; IEC4bits.U1EIE = 0;

    TMR3_SetInterruptHandler(Timer3ISR);
    IEC0bits.T3IE = false;
    TMR3_SoftwareCounterClear();

    INTERRUPT_GlobalEnable();
    LEDLIVE_SetHigh();
    PLC_VaribleClear();

    // Ethernet
    memset((void*) &AppConfig, 0x00, sizeof (AppConfig));
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
    AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
    AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
    StackInit();

    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();

    DelayMs(3000); // Sürücülerin boot süresi

    CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
    DelayMs(100);
    CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
    DelayMs(100);

    // ----- ADIM 1: Sürücüleri İlklendir (Örn: Node 1) -----
    Drive_Init(1);
    // Drive_Init(2); // Varsa diğer eksenler
    // Drive_Init(3);
    // Drive_Init(4);

    // ----- ADIM 2: Test Hareketi (Node 1, 1 tur, hız 1M pulse/sn) -----
    Drive_Move(1, 1280000, 1000000, 5000000, true);

    while (1)
    {
        StackTask();
        UdpServerTask();
        read_input();

        // IO Atamaları
        DoutPort.bitField.Bit0 = Aux0;
        DoutPort.bitField.Bit1 = Aux1;
        DoutPort.bitField.Bit2 = Aux2;
        DoutPort.bitField.Bit3 = Aux3;
        DoutPort.bitField.Bit4 = Aux4;
        DoutPort.bitField.Bit5 = Aux5;
        DoutPort.bitField.Bit6 = Aux6;
        DoutPort.bitField.Bit7 = Aux7;
        DoutPort.bitField.Bit8 = Aux8;

        OUTPUTSL_VAL = (unsigned char)(DoutPort.allvalue & 0xFF);
        OUTPUTSH_VAL = (unsigned char)((DoutPort.allvalue >> 8) & 0xFF);

        set_outpus();

        uart_cnt++;
        if (uart_cnt >= 40)
        {
            UdpServerStep1 = UDP_SERVER_REQUEST_RECEIVED;
            uart_cnt = 0;
            other_cnt++;
            if (other_cnt >= 10)
            {
                other_cnt = 0;
                LEDLIVE_Toggle();

                // Durum sorgulama (SDO üzerinden periyodik)
                Drive_QueryStatus(1);
            }
        }

        // CAN Okuma ve Ayrıştırma (Dispatcher)
        if (CAN1_ReceiveCountGet() > 0)
        {
            CAN_MSG_OBJ rxMsg;
            if (CAN1_Receive(&rxMsg))
            {
                // SDO Cevabı mı? (0x580 + nodeId)
                if (rxMsg.msgId >= 0x581 && rxMsg.msgId <= 0x584)
                {
                    uint8_t nodeId = rxMsg.msgId - 0x580;
                    uint16_t index = rxMsg.data[1] | (rxMsg.data[2] << 8);

                    if (index == 0x6064) // Actual Position
                    {
                        axis_actual_pos[nodeId] = (int32_t)rxMsg.data[4] |
                                                  ((int32_t)rxMsg.data[5] << 8) |
                                                  ((int32_t)rxMsg.data[6] << 16) |
                                                  ((int32_t)rxMsg.data[7] << 24);
                    }
                    else if (index == 0x6041) // Statusword
                    {
                        axis_status[nodeId] = rxMsg.data[4] | (rxMsg.data[5] << 8);
                    }
                }
            }
        }
    }

    return 1;
}

void PLC_VaribleClear(void)
{
    OUTPUTSH_VAL = 0; OUTPUTSL_VAL = 0;
    INPUTSH_VAL = 0; INPUTSL_VAL = 0;
    DinPort.allvalue = 0; DoutPort.allvalue = 0;
    Aux0 = 0; Aux1 = 0; Aux2 = 0; Aux3 = 0;
    Aux4 = 0; Aux5 = 0; Aux6 = 0; Aux7 = 0;
    Aux8 = 0; Aux9 = 0; Aux10 = 0; Aux11 = 0;
}

void Timer3ISR(void)
{
    IEC0bits.U1RXIE = 0; IFS0bits.U1RXIF = 0;
    uart_cnt = 0; UdpServerStep1 = UDP_SERVER_LISTEN;
    IEC0bits.T3IE = false; IFS0bits.T3IF = false;
}

void read_input(void)
{
    if (IN1_GetValue())  CLEAR_BIT(INPUTSL_VAL, 0); else SET_BIT(INPUTSL_VAL, 0);
    if (IN2_GetValue())  CLEAR_BIT(INPUTSL_VAL, 1); else SET_BIT(INPUTSL_VAL, 1);
    if (IN3_GetValue())  CLEAR_BIT(INPUTSL_VAL, 2); else SET_BIT(INPUTSL_VAL, 2);
    if (IN4_GetValue())  CLEAR_BIT(INPUTSL_VAL, 3); else SET_BIT(INPUTSL_VAL, 3);
    if (IN5_GetValue())  CLEAR_BIT(INPUTSL_VAL, 4); else SET_BIT(INPUTSL_VAL, 4);
    if (IN6_GetValue())  CLEAR_BIT(INPUTSL_VAL, 5); else SET_BIT(INPUTSL_VAL, 5);
    if (IN7_GetValue())  CLEAR_BIT(INPUTSL_VAL, 6); else SET_BIT(INPUTSL_VAL, 6);
    if (IN8_GetValue())  CLEAR_BIT(INPUTSL_VAL, 7); else SET_BIT(INPUTSL_VAL, 7);
    if (IN9_GetValue())  CLEAR_BIT(INPUTSH_VAL, 0); else SET_BIT(INPUTSH_VAL, 0);
    if (IN10_GetValue()) CLEAR_BIT(INPUTSH_VAL, 1); else SET_BIT(INPUTSH_VAL, 1);
    if (IN11_GetValue()) CLEAR_BIT(INPUTSH_VAL, 2); else SET_BIT(INPUTSH_VAL, 2);
    if (IN12_GetValue()) CLEAR_BIT(INPUTSH_VAL, 3); else SET_BIT(INPUTSH_VAL, 3);
    if (IN13_GetValue()) CLEAR_BIT(INPUTSH_VAL, 4); else SET_BIT(INPUTSH_VAL, 4);
    if (IN14_GetValue()) CLEAR_BIT(INPUTSH_VAL, 5); else SET_BIT(INPUTSH_VAL, 5);
    if (IN15_GetValue()) CLEAR_BIT(INPUTSH_VAL, 6); else SET_BIT(INPUTSH_VAL, 6);
    if (IN16_GetValue()) CLEAR_BIT(INPUTSH_VAL, 7); else SET_BIT(INPUTSH_VAL, 7);
    DinPort.allvalue = (uint16_t)((INPUTSH_VAL << 8) | INPUTSL_VAL);
}

void set_outpus(void)
{
    if (CHECK_BIT(OUTPUTSL_VAL, 0)) DQ1_SetHigh(); else DQ1_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 1)) DQ2_SetHigh(); else DQ2_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 2)) DQ3_SetHigh(); else DQ3_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 3)) DQ4_SetHigh(); else DQ4_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 4)) DQ5_SetHigh(); else DQ5_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 5)) DQ6_SetHigh(); else DQ6_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 6)) DQ7_SetHigh(); else DQ7_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 7)) DQ8_SetHigh(); else DQ8_SetLow();
}

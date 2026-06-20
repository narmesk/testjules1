#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi - Hareket İyileştirmesi (V3)
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 * Pozisyonlama El Sıkışması ve SDO ile Hedef Gönderimi
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

// ---- CANopen Sabitleri ----
#define NODE_ID         0x601
#define RXPDO1_CW       0x201
#define RXPDO2_TPOS     0x301

// SDO Yazma Bufferları
uint8_t sdo6060[8] = {0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00}; // Profile Position
uint8_t sdo6081[8] = {0x23, 0x81, 0x60, 0x00, 0x20, 0x4E, 0x00, 0x00}; // 20000 unit/s (Hız artırıldı)
uint8_t sdo6083[8] = {0x23, 0x83, 0x60, 0x00, 0x20, 0x4E, 0x00, 0x00}; // Accel
uint8_t sdo6084[8] = {0x23, 0x84, 0x60, 0x00, 0x20, 0x4E, 0x00, 0x00}; // Decel

// SDO ile Hedef Pozisyon Gönderimi (0x607A) - Daha güvenilir
uint8_t sdo607A[8] = {0x23, 0x7A, 0x60, 0x00, 0x10, 0x27, 0x00, 0x00}; // 10000 pulse

// NMT
uint8_t nmtStart[2] = {0x01, 0x00};

// Controlword (0x6040) Komutları
uint8_t cw_fault_reset[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t cw_shutdown[8]    = {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t cw_ready[8]       = {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t cw_enable[8]      = {0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t cw_start[8]       = {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // New Set-Point + Absolute

// Okuma Komutları
uint8_t read_6064[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}; // Actual Position
uint8_t read_6041[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}; // Statusword

// ---- Fonksiyonlar ----
void CAN_SendMessage(uint32_t msgId, uint8_t dlc, uint8_t *data)
{
    CAN_MSG_OBJ msg;
    msg.msgId = msgId;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.dlc = dlc;
    msg.data = data;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

void PLC_VaribleClear(void);
void Timer3ISR(void);
void read_input(void);
void set_outpus(void);

int main(void)
{
    SYSTEM_Initialize();

    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;
    IEC4bits.U1EIE = 0;

    TMR3_SetInterruptHandler(Timer3ISR);
    IEC0bits.T3IE = false;
    TMR3_SoftwareCounterClear();

    INTERRUPT_GlobalEnable();
    LEDLIVE_SetHigh();
    PLC_VaribleClear();

    // Ethernet Yapılandırması
    memset((void*) &AppConfig, 0x00, sizeof (AppConfig));
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    AppConfig.Flags.bInConfigMode = TRUE;
    memcpypgm2ram((void*) &AppConfig.MyMACAddr, (ROM void*) SerializedMACAddress, sizeof (AppConfig.MyMACAddr));
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
    AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
    AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
    StackInit();

    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();

    // Sürücü boot bekleme
    DelayMs(3000);

    CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
    DelayMs(100);
    CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
    DelayMs(100);

    // ----- ADIM 1: Temel SDO Yapılandırması -----
    CAN_SendMessage(NODE_ID, 8, sdo6060); DelayMs(150);
    CAN_SendMessage(NODE_ID, 8, sdo6081); DelayMs(150);
    CAN_SendMessage(NODE_ID, 8, sdo6083); DelayMs(150);
    CAN_SendMessage(NODE_ID, 8, sdo6084); DelayMs(150);

    // ----- ADIM 2: NMT Start -----
    CAN_SendMessage(0x000, 2, nmtStart);
    DelayMs(500);

    // ----- ADIM 3: DS402 Servo-On -----
    CAN_SendMessage(RXPDO1_CW, 8, cw_fault_reset); DelayMs(300);
    CAN_SendMessage(RXPDO1_CW, 8, cw_shutdown);    DelayMs(200);
    CAN_SendMessage(RXPDO1_CW, 8, cw_ready);       DelayMs(200);
    CAN_SendMessage(RXPDO1_CW, 8, cw_enable);      DelayMs(500);

    // ----- ADIM 4: HAREKET TETİKLEME (El Sıkışması İle) -----
    // 1. Hedef pozisyonu SDO üzerinden gönder (Kesin çözüm)
    CAN_SendMessage(NODE_ID, 8, sdo607A); DelayMs(200);

    // 2. New Set-Point'i tetikle (Bit 4)
    CAN_SendMessage(RXPDO1_CW, 8, cw_start); DelayMs(200);

    // 3. Statusword oku (Set-point acknowledge kontrolü için)
    CAN_SendMessage(NODE_ID, 8, read_6041); DelayMs(100);

    // 4. New Set-Point'i temizle (Toggle işlemi)
    CAN_SendMessage(RXPDO1_CW, 8, cw_enable); DelayMs(100);

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
        DoutPort.bitField.Bit9 = Aux9;
        DoutPort.bitField.Bit10 = Aux10;
        DoutPort.bitField.Bit11 = Aux11;

        OUTPUTSL_VAL = (unsigned char)(DoutPort.allvalue & 0xFF);
        OUTPUTSH_VAL = (unsigned char)((DoutPort.allvalue >> 8) & 0xFF);

        set_outpus();

        uart_cnt++;
        if (uart_cnt >= 40)
        {
            UdpServerStep1 = UDP_SERVER_REQUEST_RECEIVED;
            uart_cnt = 0;
            other_cnt++;
            if (other_cnt >= 6)
            {
                other_cnt = 0;
                LEDLIVE_Toggle();

                // Pozisyon sorgula
                CAN_SendMessage(NODE_ID, 8, read_6064);
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
    LastBit.allvalue = 0; AuxBit.allvalue = 0;
    TmrEnBits.allvalue = 0; TmrBits.allvalue = 0;
    Aux0 = 0; Aux1 = 0; Aux2 = 0; Aux3 = 0;
    Aux4 = 0; Aux5 = 0; Aux6 = 0; Aux7 = 0;
    Aux8 = 0; Aux9 = 0; Aux10 = 0; Aux11 = 0;
    Aux_Value_In = 0;
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

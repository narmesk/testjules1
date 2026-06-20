#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 * 1 Mbit/s CAN hızı ile 0x0C (CANopen) modunda çalışma
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

// ---- CANopen Sabitleri ve Mesaj Tanımları ----
#define NODE_ID         0x601   // SDO Haberleşmesi için (0x600 + NodeID)
#define RXPDO1_CW       0x201   // Controlword (0x6040) için
#define RXPDO2_TPOS     0x301   // Target Position (0x607A) için

// SDO Yazma Bufferları (Object Dictionary Yapılandırması)
uint8_t sdo6060[8] = {0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00}; // Profile Position Modu
uint8_t sdo6081[8] = {0x23, 0x81, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00}; // Profil Hızı: 1000 unit/s
uint8_t sdo6083[8] = {0x23, 0x83, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00}; // Profil İvmesi: 1000 unit/s^2
uint8_t sdo6084[8] = {0x23, 0x84, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00}; // Profil Yavaşlaması: 1000 unit/s^2
uint8_t sdo60E0[8] = {0x23, 0xE0, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00}; // Pozitif Tork Sınırı (%100)
uint8_t sdo60E1[8] = {0x23, 0xE1, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00}; // Negatif Tork Sınırı (%100)

// NMT (Network Management) Komutları
uint8_t nmtStart[2] = {0x01, 0x00}; // Tüm düğümleri OPERATIONAL moda geçir

// Controlword (0x6040) Durum Geçişleri
uint8_t cw_shutdown[8] = {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Ready to Switch On
uint8_t cw_ready[8]    = {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Switched On
uint8_t cw_enable[8]   = {0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Operation Enabled
uint8_t cw_start[8]    = {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // New Set-Point (Hareketi Başlat)

// Hareket Verileri
uint8_t target_pos[8]  = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Örn: 10000 pulse

// Okuma Komutları (SDO Upload)
uint8_t read_6064[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}; // Actual Position
uint8_t read_607A[8] = {0x40, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}; // Target Position
uint8_t read_6061[8] = {0x40, 0x61, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}; // Mode of Operation Display

// ---- Fonksiyon Prototipleri ----
void PLC_VaribleClear(void);
void Timer3ISR(void);
void read_input(void);
void set_outpus(void);
void CAN_SendMessage(uint32_t msgId, uint8_t dlc, uint8_t *data);

// CAN Mesaj Gönderim Fonksiyonu
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

int main(void)
{
    // 1. Donanım ve Sistem İlklendirme
    SYSTEM_Initialize();

    // UART Interruptlarını temizle
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;
    IEC4bits.U1EIE = 0;

    // Timer3 Yapılandırması (Haberleşme zamanlaması için)
    TMR3_SetInterruptHandler(Timer3ISR);
    IEC0bits.T3IE = false;
    TMR3_SoftwareCounterClear();

    INTERRUPT_GlobalEnable();
    LEDLIVE_SetHigh();
    PLC_VaribleClear();

    // Ethernet Yığını (Stack) Yapılandırması
    memset((void*) &AppConfig, 0x00, sizeof (AppConfig));
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    AppConfig.Flags.bInConfigMode = TRUE;
    memcpypgm2ram((void*) &AppConfig.MyMACAddr, (ROM void*) SerializedMACAddress, sizeof (AppConfig.MyMACAddr));
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
    StackInit();

    // 2. CAN Modülünü Hazırla
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();

    // Konfigürasyon Moduna Geç ve Normal Modu Başlat
    CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
    DelayMs(100);
    CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
    DelayMs(100);

    // ----- ADIM 1: SDO Üzerinden Sürücü Parametrelerini Ayarla -----
    // PP Modunu Ayarla, Hız ve İvme Değerlerini Yaz
    CAN_SendMessage(NODE_ID, 8, sdo6060); DelayMs(50); // PP Mode
    CAN_SendMessage(NODE_ID, 8, sdo6081); DelayMs(50); // Velocity
    CAN_SendMessage(NODE_ID, 8, sdo6083); DelayMs(50); // Accel
    CAN_SendMessage(NODE_ID, 8, sdo6084); DelayMs(50); // Decel
    CAN_SendMessage(NODE_ID, 8, sdo60E0); DelayMs(50); // Torque Lim
    CAN_SendMessage(NODE_ID, 8, sdo60E1); DelayMs(50);

    // ----- ADIM 2: NMT Start -----
    // Sürücüyü Pre-Operational moddan Operational moda geçir
    CAN_SendMessage(0x000, 2, nmtStart);
    DelayMs(100);

    // ----- ADIM 3: DS402 Durum Makinesini Tetikle (Servo On) -----
    // Sürücüye tork uygulatmak için gerekli sıra
    CAN_SendMessage(RXPDO1_CW, 8, cw_shutdown); DelayMs(50); // Durum: Ready to Switch On
    CAN_SendMessage(RXPDO1_CW, 8, cw_ready);    DelayMs(50); // Durum: Switched On
    CAN_SendMessage(RXPDO1_CW, 8, cw_enable);   DelayMs(50); // Durum: Operation Enabled (Servo On)

    // ----- ADIM 4: Başlangıç Bilgilerini Kontrol Et -----
    CAN_SendMessage(NODE_ID, 8, read_6064); DelayMs(50); // Pozisyon Oku

    // ----- ADIM 5: Hedef Pozisyona Hareket Başlat -----
    // Önce hedefi gönder, sonra New Set-Point (bit 4) ile tetikle
    CAN_SendMessage(RXPDO2_TPOS, 8, target_pos); DelayMs(50);
    CAN_SendMessage(RXPDO1_CW, 8, cw_start);     DelayMs(50); // Hareket Başlar

    // ----- ADIM 6: Durum İzleme -----
    CAN_SendMessage(NODE_ID, 8, read_6064); DelayMs(50);

    while (1)
    {
        StackTask();
        UdpServerTask();
        read_input();

        // Dijital Çıkış Atamaları
        DoutPort.bitField.Bit0 = Aux0;
        DoutPort.bitField.Bit1 = Aux1;
        DoutPort.bitField.Bit2 = Aux2;
        DoutPort.bitField.Bit3 = Aux3;
        DoutPort.bitField.Bit4 = Aux4;
        DoutPort.bitField.Bit5 = Aux5;
        DoutPort.bitField.Bit6 = Aux6;
        DoutPort.bitField.Bit7 = Aux7;
        DoutPort.bitField.Bit8 = Aux8;

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
            }
        }
    }

    return 1;
}

void PLC_VaribleClear(void)
{
    OUTPUTSH_VAL = 0;
    OUTPUTSL_VAL = 0;
    INPUTSH_VAL = 0;
    INPUTSL_VAL = 0;
    DinPort.allvalue = 0;
    DoutPort.allvalue = 0;
    LastBit.allvalue = 0;
    AuxBit.allvalue = 0;
    TmrEnBits.allvalue = 0;
    TmrBits.allvalue = 0;
    Aux0 = 0; Aux1 = 0; Aux2 = 0; Aux3 = 0;
    Aux4 = 0; Aux5 = 0; Aux6 = 0; Aux7 = 0;
    Aux8 = 0; Aux9 = 0; Aux10 = 0; Aux11 = 0;
    Aux_Value_In = 0;
}

void Timer3ISR(void)
{
    IEC0bits.U1RXIE = 0;
    IFS0bits.U1RXIF = 0;
    uart_cnt = 0;
    UdpServerStep1 = UDP_SERVER_LISTEN;
    IEC0bits.T3IE = false;
    IFS0bits.T3IF = false;
}

void read_input(void)
{
    // Girişleri oku ve INPUTS registerlarına işle (Logic Inverted)
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

    DinPort.allvalue = (INPUTSH_VAL * 256) + INPUTSL_VAL;
}

void set_outpus(void)
{
    // OUTPUTSL_VAL bitlerine göre fiziksel çıkışları güncelle
    if (CHECK_BIT(OUTPUTSL_VAL, 0)) DQ1_SetHigh(); else DQ1_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 1)) DQ2_SetHigh(); else DQ2_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 2)) DQ3_SetHigh(); else DQ3_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 3)) DQ4_SetHigh(); else DQ4_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 4)) DQ5_SetHigh(); else DQ5_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 5)) DQ6_SetHigh(); else DQ6_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 6)) DQ7_SetHigh(); else DQ7_SetLow();
    if (CHECK_BIT(OUTPUTSL_VAL, 7)) DQ8_SetHigh(); else DQ8_SetLow();
}

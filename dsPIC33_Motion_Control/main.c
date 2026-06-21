#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi - V23 (CSP Kararlılık Sürümü)
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 * Fix: Ternary ifadeler kaldırıldı, 12-bit IO ve ISR korundu, CSP geçişi eklendi.
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
static unsigned short wOriginalAppConfigChecksum;

extern UdpServerStep UdpServerStep1;
extern unsigned char uart_cnt;
unsigned char other_cnt;

// Hareket Durum Makinesi
typedef enum {
    M_INIT_HARDWARE,
    M_START_PP_TURN,
    M_WAIT_PP_DONE,
    M_PREPARE_CSP_SYNC,
    M_CSP_RUNNING
} motion_state_t;

motion_state_t m_state = M_INIT_HARDWARE;
uint32_t m_timer = 0;
uint32_t m_loop_ticks = 0;

// Eksen Veri Havuzu (1-4)
int32_t axis_target_pos[5] = {0, 0, 0, 0, 0};
int32_t axis_actual_pos[5] = {0, 0, 0, 0, 0};
uint16_t axis_status[5] = {0, 0, 0, 0, 0};

// ---- CANopen Altyapı Fonksiyonları ----

void CAN_WriteSDO(uint8_t nodeId, uint16_t index, uint8_t subindex, uint32_t data, uint8_t len)
{
    CAN_MSG_OBJ msg = {0};
    uint8_t sdo_data[8] = {0};

    if (len == 1) {
        sdo_data[0] = 0x2F;
    } else if (len == 2) {
        sdo_data[0] = 0x2B;
    } else {
        sdo_data[0] = 0x23;
    }

    sdo_data[1] = (uint8_t)(index & 0xFF);
    sdo_data[2] = (uint8_t)((index >> 8) & 0xFF);
    sdo_data[3] = subindex;
    sdo_data[4] = (uint8_t)(data & 0xFF);
    sdo_data[5] = (uint8_t)((data >> 8) & 0xFF);
    sdo_data[6] = (uint8_t)((data >> 16) & 0xFF);
    sdo_data[7] = (uint8_t)((data >> 24) & 0xFF);

    msg.msgId = 0x600 + nodeId;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.dlc = 8;
    msg.data = sdo_data;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
    DelayMs(20);
}

void Drive_SendPDO(uint8_t nodeId, uint16_t cw, int32_t pos)
{
    CAN_MSG_OBJ msg = {0};
    uint8_t pdo_data[8] = {0};

    pdo_data[0] = (uint8_t)(cw & 0xFF);
    pdo_data[1] = (uint8_t)((cw >> 8) & 0xFF);
    pdo_data[2] = (uint8_t)(pos & 0xFF);
    pdo_data[3] = (uint8_t)((pos >> 8) & 0xFF);
    pdo_data[4] = (uint8_t)((pos >> 16) & 0xFF);
    pdo_data[5] = (uint8_t)((pos >> 24) & 0xFF);

    msg.msgId = 0x200 + nodeId;
    msg.field.idType = CAN_FRAME_STD;
    msg.field.frameType = CAN_FRAME_DATA;
    msg.field.dlc = 6;
    msg.data = pdo_data;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

/**
 * Başlangıç SDO ve PDO Ayarları
 */
void Drive_PowerUp(uint8_t nodeId)
{
    // 1. PDO Haritalama
    CAN_WriteSDO(nodeId, 0x1600, 0x00, 0x00, 1);
    CAN_WriteSDO(nodeId, 0x1A00, 0x00, 0x00, 1);
    CAN_WriteSDO(nodeId, 0x1600, 0x01, 0x60400010, 4);
    CAN_WriteSDO(nodeId, 0x1600, 0x02, 0x607A0020, 4);
    CAN_WriteSDO(nodeId, 0x1600, 0x00, 0x02, 1);
    CAN_WriteSDO(nodeId, 0x1A00, 0x01, 0x60410010, 4);
    CAN_WriteSDO(nodeId, 0x1A00, 0x02, 0x60640020, 4);
    CAN_WriteSDO(nodeId, 0x1A00, 0x00, 0x02, 1);

    // 2. Profil Ayarları
    CAN_WriteSDO(nodeId, 0x6060, 0x00, 0x01, 1); // Başlangıç: PP
    CAN_WriteSDO(nodeId, 0x6081, 0x00, 1280000, 4);
    CAN_WriteSDO(nodeId, 0x6083, 0x00, 6400000, 4);

    // 3. NMT Start
    CAN_MSG_OBJ nmt = {0};
    uint8_t nmt_data[2] = {0x01, nodeId};
    nmt.msgId = 0x000; nmt.field.dlc = 2; nmt.data = nmt_data;
    nmt.field.idType = CAN_FRAME_STD; nmt.field.frameType = CAN_FRAME_DATA;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &nmt);
    DelayMs(100);

    // 4. DS402 Servo-On
    Drive_SendPDO(nodeId, 0x0080, 0); DelayMs(100);
    Drive_SendPDO(nodeId, 0x0006, 0); DelayMs(100);
    Drive_SendPDO(nodeId, 0x0007, 0); DelayMs(100);
    Drive_SendPDO(nodeId, 0x000F, 0); DelayMs(200);
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
    TMR3_SoftwareCounterClear();
    IEC0bits.T3IE = false;

    INTERRUPT_GlobalEnable();
    LEDLIVE_SetHigh();
    PLC_VaribleClear();

    // Ethernet (Orijinal Blok)
    memset((void*) &AppConfig, 0x00, sizeof (AppConfig));
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    AppConfig.Flags.bInConfigMode = TRUE;
    memcpypgm2ram((void*) &AppConfig.MyMACAddr, (ROM void*) SerializedMACAddress, sizeof (AppConfig.MyMACAddr));
    wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*) & AppConfig, sizeof (AppConfig));
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
    AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
    AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
    AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2 << 8ul | MY_DEFAULT_GATE_BYTE3 << 16ul | MY_DEFAULT_GATE_BYTE4 << 24ul;
    AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2 << 8ul | MY_DEFAULT_PRIMARY_DNS_BYTE3 << 16ul | MY_DEFAULT_PRIMARY_DNS_BYTE4 << 24ul;
    AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2 << 8ul | MY_DEFAULT_SECONDARY_DNS_BYTE3 << 16ul | MY_DEFAULT_SECONDARY_DNS_BYTE4 << 24ul;
    StackInit();

    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
    DelayMs(3000);

    CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
    DelayMs(10);
    CAN1_OperationModeSet(CAN_NORMAL_OPERATION_MODE);
    DelayMs(100);

    m_state = M_INIT_HARDWARE;

    while (1)
    {
        StackTask();
        UdpServerTask();
        read_input();

        // --- TAM IO EŞLEŞMESİ (Aux0-Aux11) ---
        DoutPort.bitField.Bit0 = Aux0;   DoutPort.bitField.Bit1 = Aux1;
        DoutPort.bitField.Bit2 = Aux2;   DoutPort.bitField.Bit3 = Aux3;
        DoutPort.bitField.Bit4 = Aux4;   DoutPort.bitField.Bit5 = Aux5;
        DoutPort.bitField.Bit6 = Aux6;   DoutPort.bitField.Bit7 = Aux7;
        DoutPort.bitField.Bit8 = Aux8;   DoutPort.bitField.Bit9 = Aux9;
        DoutPort.bitField.Bit10 = Aux10; DoutPort.bitField.Bit11 = Aux11;

        OUTPUTSL_VAL = (unsigned char)(DoutPort.allvalue & 0xFF);
        OUTPUTSH_VAL = (unsigned char)((DoutPort.allvalue >> 8) & 0xFF);
        set_outpus();

        // --- HAREKET AKIŞI (4ms Senkron Kontrolü) ---
        m_loop_ticks++;
        if (m_loop_ticks >= 3000) // Donanım hızına göre kalibre (4-6ms arası)
        {
            m_loop_ticks = 0;

            switch(m_state) {
                case M_INIT_HARDWARE:
                    Drive_PowerUp(1); // Node 1 İlklendir
                    m_state = M_START_PP_TURN;
                    break;

                case M_START_PP_TURN:
                    // V5'te başarılı olan 1 tam tur bağıl hareket
                    Drive_SendPDO(1, 0x005F, 1280000);
                    m_state = M_WAIT_PP_DONE;
                    m_timer = 0;
                    break;

                case M_WAIT_PP_DONE:
                    // Hareket bitince 2sn bekle
                    if (++m_timer >= 500) {
                        m_state = M_PREPARE_CSP_SYNC;
                    }
                    break;

                case M_PREPARE_CSP_SYNC:
                    // Akıcı hareket için CSP moduna geç
                    CAN_WriteSDO(1, 0x6060, 0x00, 0x0C, 1);
                    // Yazılımsal hedefi gerçek pozisyonla senkronla
                    axis_target_pos[1] = axis_actual_pos[1];
                    m_state = M_CSP_RUNNING;
                    break;

                case M_CSP_RUNNING:
                    // +18 Pulse Sürekli Akış
                    axis_target_pos[1] += 18;
                    Drive_SendPDO(1, 0x000F, axis_target_pos[1]);
                    break;

                default: break;
            }

            // SYNC
            CAN_MSG_OBJ sync = {0};
            sync.msgId = 0x080; sync.field.dlc = 0;
            sync.field.idType = CAN_FRAME_STD; sync.field.frameType = CAN_FRAME_DATA;
            CAN1_Transmit(CAN_PRIORITY_HIGH, &sync);
        }

        // --- YAŞAM SİNYALİ VE ETHERNET TETİKLEME ---
        uart_cnt++;
        if (uart_cnt >= 40) {
            UdpServerStep1 = UDP_SERVER_REQUEST_RECEIVED;
            uart_cnt = 0;
            other_cnt++;
            if (other_cnt >= 20) { other_cnt = 0; LEDLIVE_Toggle(); }
        }

        // --- CAN FEEDBACK (Asenkron Dispatcher) ---
        while (CAN1_ReceivedMessageCountGet() > 0) {
            CAN_MSG_OBJ rx; uint8_t rxData[8]; rx.data = rxData;
            if (CAN1_Receive(&rx)) {
                if (rx.msgId >= 0x181 && rx.msgId <= 0x184) {
                    uint8_t nid = (uint8_t)(rx.msgId - 0x180);
                    axis_status[nid] = (uint16_t)rx.data[0] | ((uint16_t)rx.data[1] << 8);
                    axis_actual_pos[nid] = (int32_t)rx.data[2] | ((int32_t)rx.data[3] << 8) | ((int32_t)rx.data[4] << 16) | ((int32_t)rx.data[5] << 24);
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

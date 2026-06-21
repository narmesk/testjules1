#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi - V34 (Nokta Atışı & Tam Mimari)
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 * Hedef: 4ms çevrim, Kararlı Ethernet, 4-Eksen Manuel/Otomatik, Özel Home.
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

// Eksen Durum Tanımları
typedef enum { A_IDLE, A_WAIT_ACK, A_MOVING } axis_move_state_t;
typedef enum { H_IDLE, H_SEARCH, H_BACKOFF, H_TOUCH, H_DONE } axis_home_state_t;

// Eksen Veri Yapısı (4 Eksen)
typedef struct {
    int32_t target_pos;      // Yazılımsal hedef
    int32_t actual_pos_raw;  // Sürücüden gelen ham değer
    int32_t home_offset;     // Yazılımsal 0 noktası kayması
    uint16_t status;         // Statusword
    uint32_t inputs;         // Digital Inputs (0x60FD)
    axis_move_state_t m_state;
    axis_home_state_t h_state;
    bool req_move;
    bool req_home;
    bool is_auto;            // true: PC, false: Manuel (Sürücü IO)
    uint8_t home_node;       // Sensörün bağlı olduğu sürücü
    uint8_t home_bit;        // Sensörün bit numarası
    bool pp_toggle;          // Handshake için bit 4 durumu
} axis_t;

axis_t axes[5]; // Index 1-4

// ---- CANopen Altyapı Fonksiyonları ----

/**
 * SDO Yazma (Sadece SETUP aşamasında kullanılmalıdır - 15ms bloklar)
 */
void CAN_WriteSDO_Blocking(uint8_t nodeId, uint16_t index, uint8_t subindex, uint32_t data, uint8_t len)
{
    CAN_MSG_OBJ msg = {0};
    uint8_t s_data[8] = {0};
    if (len == 1) s_data[0] = 0x2F;
    else if (len == 2) s_data[0] = 0x2B;
    else s_data[0] = 0x23;

    s_data[1] = (uint8_t)(index & 0xFF); s_data[2] = (uint8_t)((index >> 8) & 0xFF);
    s_data[3] = subindex;
    s_data[4] = (uint8_t)(data & 0xFF); s_data[5] = (uint8_t)((data >> 8) & 0xFF);
    s_data[6] = (uint8_t)((data >> 16) & 0xFF); s_data[7] = (uint8_t)((data >> 24) & 0xFF);

    msg.msgId = 0x600 + nodeId;
    msg.field.idType = CAN_FRAME_STD; msg.field.frameType = CAN_FRAME_DATA; msg.field.dlc = 8;
    msg.data = s_data;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
    DelayMs(15);
}

/**
 * PDO1 Gönderimi (Hızlı - Non-blocking)
 */
void Drive_SendPDO1(uint8_t nodeId, uint16_t cw, int32_t pos)
{
    CAN_MSG_OBJ msg = {0};
    uint8_t d[8] = {0};
    d[0] = (uint8_t)(cw & 0xFF); d[1] = (uint8_t)((cw >> 8) & 0xFF);
    d[2] = (uint8_t)(pos & 0xFF); d[3] = (uint8_t)((pos >> 8) & 0xFF);
    d[4] = (uint8_t)((pos >> 16) & 0xFF); d[5] = (uint8_t)((pos >> 24) & 0xFF);

    msg.msgId = 0x200 + nodeId;
    msg.field.idType = CAN_FRAME_STD; msg.field.frameType = CAN_FRAME_DATA; msg.field.dlc = 6;
    msg.data = d;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

// ---- Hareket ve Home API ----

void Drive_InitializeAxis(uint8_t nodeId)
{
    // PDO Mapping Konfigürasyonu
    CAN_WriteSDO_Blocking(nodeId, 0x1600, 0x00, 0x00, 1);
    CAN_WriteSDO_Blocking(nodeId, 0x1A00, 0x00, 0x00, 1);
    CAN_WriteSDO_Blocking(nodeId, 0x1A01, 0x00, 0x00, 1);

    // RxPDO1: 6040h + 607Ah
    CAN_WriteSDO_Blocking(nodeId, 0x1600, 0x01, 0x60400010, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x1600, 0x02, 0x607A0020, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x1600, 0x00, 0x02, 1);

    // TxPDO1: 6041h + 6064h
    CAN_WriteSDO_Blocking(nodeId, 0x1A00, 0x01, 0x60410010, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x1A00, 0x02, 0x60640020, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x1A00, 0x00, 0x02, 1);

    // TxPDO2: 60FDh (Girişler)
    CAN_WriteSDO_Blocking(nodeId, 0x1A01, 0x01, 0x60FD0020, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x1A01, 0x00, 0x01, 1);

    CAN_WriteSDO_Blocking(nodeId, 0x6060, 0x00, 0x01, 1); // PP Mode

    // NMT Start
    CAN_MSG_OBJ nmt = {0}; uint8_t nmt_d[2] = {0x01, nodeId};
    nmt.msgId = 0x000; nmt.field.dlc = 2; nmt.data = nmt_d;
    nmt.field.idType = CAN_FRAME_STD; nmt.field.frameType = CAN_FRAME_DATA;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &nmt); DelayMs(100);

    // DS402 Ready
    Drive_SendPDO1(nodeId, 0x000F, 0); DelayMs(200);
}

void Drive_SetDynamics(uint8_t nodeId, uint32_t vel, uint32_t acc)
{
    CAN_WriteSDO_Blocking(nodeId, 0x6081, 0x00, vel, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x6083, 0x00, acc, 4);
    CAN_WriteSDO_Blocking(nodeId, 0x6084, 0x00, acc, 4);
}

/**
 * Eksen Mantıksal İşlemesi (4ms çevriminde çağrılır)
 */
void Drive_UpdateLogic(uint8_t n)
{
    // 1. Home Prosedürü
    if (axes[n].req_home) {
        if (n == 1) { // Sensörsüz Drive 1
            axes[n].home_offset = axes[n].actual_pos_raw;
            axes[n].req_home = false;
        } else {
            bool s_active = (axes[axes[n].home_node].inputs & (1UL << axes[n].home_bit));
            switch(axes[n].h_state) {
                case H_IDLE:
                    axes[n].target_pos = axes[n].actual_pos_raw; // Sıçramayı önle
                    axes[n].h_state = H_SEARCH;
                    break;
                case H_SEARCH:
                    if (s_active) {
                        axes[n].h_state = H_BACKOFF;
                        axes[n].target_pos = axes[n].actual_pos_raw - 128000;
                        axes[n].pp_toggle = true; // Tetikle
                    } else {
                        axes[n].target_pos += 1000;
                        axes[n].pp_toggle = !axes[n].pp_toggle;
                    }
                    break;
                case H_BACKOFF:
                    if (axes[n].status & 0x0400) axes[n].h_state = H_TOUCH;
                    break;
                case H_TOUCH:
                    if (s_active) axes[n].h_state = H_DONE;
                    else {
                        axes[n].target_pos += 50;
                        axes[n].pp_toggle = !axes[n].pp_toggle;
                    }
                    break;
                case H_DONE:
                    axes[n].home_offset = axes[n].actual_pos_raw;
                    axes[n].req_home = false; axes[n].h_state = H_IDLE;
                    break;
            }
            // Homing PDO Gönderimi
            uint16_t cw = axes[n].pp_toggle ? 0x001F : 0x000F;
            Drive_SendPDO1(n, cw, axes[n].target_pos);
        }
        return;
    }

    // 2. Manuel/Otomatik Seçimi
    if (!axes[n].is_auto) {
        // Manuel Mod: dsPIC sadece sürücüyü Operation Enabled tutar, hareket sürücü IO'sundadır.
        Drive_SendPDO1(n, 0x000F, axes[n].actual_pos_raw);
    } else if (axes[n].req_move) {
        // Otomatik Mod: PP Handshake (Bit 4 Toggle)
        switch(axes[n].m_state) {
            case A_IDLE:
                Drive_SendPDO1(n, 0x001F, axes[n].target_pos);
                axes[n].m_state = A_WAIT_ACK;
                break;
            case A_WAIT_ACK:
                if (axes[n].status & 0x1000) { // Bit 12: Acknowledge
                    Drive_SendPDO1(n, 0x000F, axes[n].target_pos);
                    axes[n].m_state = A_MOVING;
                }
                break;
            case A_MOVING:
                if (axes[n].status & 0x0400) { // Bit 10: Target Reached
                    axes[n].req_move = false; axes[n].m_state = A_IDLE;
                }
                break;
        }
    }
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
    TMR3_SetInterruptHandler(Timer3ISR); TMR3_SoftwareCounterClear(); IEC0bits.T3IE = false;

    INTERRUPT_GlobalEnable(); LEDLIVE_SetHigh(); PLC_VaribleClear();

    // Ethernet (Orijinal Eksiksiz Yapı)
    memset((void*) &AppConfig, 0x00, sizeof (AppConfig));
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    memcpypgm2ram((void*) &AppConfig.MyMACAddr, (ROM void*) SerializedMACAddress, sizeof (AppConfig.MyMACAddr));
    wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*) & AppConfig, sizeof (AppConfig));
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
    AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
    AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2 << 8ul | MY_DEFAULT_GATE_BYTE3 << 16ul | MY_DEFAULT_GATE_BYTE4 << 24ul;
    StackInit();

    CAN1_TransmitEnable(); CAN1_ReceiveEnable(); DelayMs(3000);
    CAN1_OperationModeSet(CAN_CONFIGURATION_MODE); DelayMs(10);
    CAN1_OperationModeSet(CAN_NORMAL_OPERATION_MODE); DelayMs(100);

    // Sürücüleri İlklendir
    for(uint8_t i=1; i<=4; i++) {
        Drive_InitializeAxis(i);
        Drive_SetDynamics(i, 1280000, 3200000); // Varsayılan: 60 RPM
        axes[i].home_node = i; axes[i].home_bit = 16;
        axes[i].is_auto = true;
    }

    while (1)
    {
        StackTask(); UdpServerTask(); read_input();

        // --- TAM IO EŞLEŞMESİ (12-Bit) ---
        DoutPort.bitField.Bit0 = Aux0;   DoutPort.bitField.Bit1 = Aux1;
        DoutPort.bitField.Bit2 = Aux2;   DoutPort.bitField.Bit3 = Aux3;
        DoutPort.bitField.Bit4 = Aux4;   DoutPort.bitField.Bit5 = Aux5;
        DoutPort.bitField.Bit6 = Aux6;   DoutPort.bitField.Bit7 = Aux7;
        DoutPort.bitField.Bit8 = Aux8;   DoutPort.bitField.Bit9 = Aux9;
        DoutPort.bitField.Bit10 = Aux10; DoutPort.bitField.Bit11 = Aux11;

        OUTPUTSL_VAL = (unsigned char)(DoutPort.allvalue & 0xFF);
        OUTPUTSH_VAL = (unsigned char)((DoutPort.allvalue >> 8) & 0xFF);
        set_outpus();

        // 4ms Senkron Kontrol
        uart_cnt++;
        if (uart_cnt >= 40)
        {
            UdpServerStep1 = UDP_SERVER_REQUEST_RECEIVED; uart_cnt = 0;
            for(uint8_t n=1; n<=4; n++) Drive_UpdateLogic(n);

            // Global SYNC
            CAN_MSG_OBJ s = {0}; s.msgId = 0x080; s.field.dlc = 0;
            s.field.idType = CAN_FRAME_STD; s.field.frameType = CAN_FRAME_DATA;
            CAN1_Transmit(CAN_PRIORITY_HIGH, &s);

            other_cnt++; if (other_cnt >= 20) { other_cnt = 0; LEDLIVE_Toggle(); }
        }

        // CAN Feedback Dispatcher (Non-blocking)
        while (CAN1_ReceivedMessageCountGet() > 0) {
            CAN_MSG_OBJ rx; uint8_t rd[8]; rx.data = rd;
            if (CAN1_Receive(&rx)) {
                uint8_t nid = (uint8_t)(rx.msgId & 0x0F);
                if (nid >= 1 && nid <= 4) {
                    if((rx.msgId & 0xFF0) == 0x180) { // TPDO1
                        axes[nid].status = (uint16_t)rx.data[0] | ((uint16_t)rx.data[1] << 8);
                        axes[nid].actual_pos_raw = (int32_t)rx.data[2] | ((int32_t)rx.data[3] << 8) | ((int32_t)rx.data[4] << 16) | ((int32_t)rx.data[5] << 24);
                    } else if((rx.msgId & 0xFF0) == 0x280) { // TPDO2
                        axes[nid].inputs = (uint32_t)rx.data[0] | ((uint32_t)rx.data[1] << 8) | ((uint32_t)rx.data[2] << 16) | ((uint32_t)rx.data[3] << 24);
                    }
                }
            }
        }
    }
    return 1;
}

void PLC_VaribleClear(void)
{
    OUTPUTSH_VAL = 0; OUTPUTSL_VAL = 0; INPUTSH_VAL = 0; INPUTSL_VAL = 0;
    DinPort.allvalue = 0; DoutPort.allvalue = 0;
    Aux0 = 0; Aux1 = 0; Aux2 = 0; Aux3 = 0;
    Aux4 = 0; Aux5 = 0; Aux6 = 0; Aux7 = 0;
    Aux8 = 0; Aux9 = 0; Aux10 = 0; Aux11 = 0;
}

void Timer3ISR(void)
{
    IEC0bits.U1RXIE = 0; IFS0bits.U1RXIF = 0; uart_cnt = 0;
    UdpServerStep1 = UDP_SERVER_LISTEN; IEC0bits.T3IE = false; IFS0bits.T3IF = false;
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

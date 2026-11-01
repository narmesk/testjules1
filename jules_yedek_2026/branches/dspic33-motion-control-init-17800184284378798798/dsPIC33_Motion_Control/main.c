#include "main.h"
#include "mcc_generated_files/dma.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

/*
 * dsPIC33EP Motion Control Projesi - V38 (Nihai Nokta Atışı)
 * Delta ASDA-A2 Sürücü Kontrolü (CANopen DS402)
 * Senaryo: 1 Tur (PP) -> 2sn Bekle -> Sürekli +18 Pulse Akış (PP Handshake)
 */

// ---- Yapılandırma ----
#define ACTIVE_AXIS_COUNT 1  // Test için 1, saha için 4 yapınız.

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
typedef enum { M_IDLE, M_START_MOVE, M_WAIT_ACK, M_MOVING, M_STREAMING } axis_move_state_t;
typedef enum { H_IDLE, H_SEARCH, H_BACKOFF, H_TOUCH, H_DONE } axis_home_state_t;

// Test Senaryo Durumları
typedef enum { T_INIT, T_TURN, T_WAIT, T_STREAM } test_state_t;
test_state_t global_test_state = T_INIT;
uint32_t global_test_timer = 0;

// Eksen Veri Yapısı (4 Eksen)
typedef struct {
    int32_t target_pos;
    int32_t actual_pos;
    uint16_t status;
    uint32_t inputs;
    axis_move_state_t m_state;
    axis_home_state_t h_state;
    bool req_move;
    bool req_home;
    bool is_auto;
    uint8_t home_sensor_node;
    uint8_t home_sensor_bit;
    bool pp_toggle;
} axis_t;

axis_t axes[5]; // Index 1-4

// ---- CANopen Çekirdek Fonksiyonlar ----

void CAN_WriteSDO(uint8_t nodeId, uint16_t index, uint8_t subindex, uint32_t data, uint8_t len)
{
    CAN_MSG_OBJ msg = {0};
    uint8_t s_data[8] = {0};

    if (len == 1) {
        s_data[0] = 0x2F;
    } else if (len == 2) {
        s_data[0] = 0x2B;
    } else {
        s_data[0] = 0x23;
    }

    s_data[1] = (uint8_t)(index & 0xFF);
    s_data[2] = (uint8_t)((index >> 8) & 0xFF);
    s_data[3] = subindex;
    s_data[4] = (uint8_t)(data & 0xFF);
    s_data[5] = (uint8_t)((data >> 8) & 0xFF);
    s_data[6] = (uint8_t)((data >> 16) & 0xFF);
    s_data[7] = (uint8_t)((data >> 24) & 0xFF);

    msg.msgId = 0x600 + nodeId;
    msg.field.idType = CAN_FRAME_STD; msg.field.frameType = CAN_FRAME_DATA; msg.field.dlc = 8;
    msg.data = s_data;

    // TX Buffer'ın boşalmasını bekle
    uint16_t timeout = 5000;
    while(C1TR01CONbits.TXREQ0 == 1 && --timeout);
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
    DelayMs(15);
}

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

    uint16_t timeout = 1000;
    while(C1TR01CONbits.TXREQ0 == 1 && --timeout);
    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
}

// ---- Hareket & Home API ----

void Drive_Initialize(uint8_t nodeId)
{
    // PDO Mapping
    CAN_WriteSDO(nodeId, 0x1600, 0x00, 0x00, 1);
    CAN_WriteSDO(nodeId, 0x1A00, 0x00, 0x00, 1);
    CAN_WriteSDO(nodeId, 0x1A01, 0x00, 0x00, 1);
    CAN_WriteSDO(nodeId, 0x1600, 0x01, 0x60400010, 4); // CW
    CAN_WriteSDO(nodeId, 0x1600, 0x02, 0x607A0020, 4); // Target
    CAN_WriteSDO(nodeId, 0x1600, 0x00, 0x02, 1);
    CAN_WriteSDO(nodeId, 0x1A00, 0x01, 0x60410010, 4); // Status
    CAN_WriteSDO(nodeId, 0x1A00, 0x02, 0x60640020, 4); // Actual
    CAN_WriteSDO(nodeId, 0x1A00, 0x00, 0x02, 1);
    CAN_WriteSDO(nodeId, 0x1A01, 0x01, 0x60FD0020, 4); // Inputs
    CAN_WriteSDO(nodeId, 0x1A01, 0x00, 0x01, 1);

    CAN_WriteSDO(nodeId, 0x6060, 0x00, 0x01, 1); // PP Mode
    CAN_WriteSDO(nodeId, 0x6081, 0x00, 1280000, 4); // 60 RPM
    CAN_WriteSDO(nodeId, 0x6083, 0x00, 3200000, 4); // Accel

    CAN_MSG_OBJ nmt = {0}; uint8_t n_d[2] = {0x01, nodeId};
    nmt.msgId = 0x000; nmt.field.dlc = 2; nmt.data = n_d;
    nmt.field.idType = CAN_FRAME_STD; nmt.field.frameType = CAN_FRAME_DATA;
    CAN1_Transmit(CAN_PRIORITY_HIGH, &nmt); DelayMs(100);

    Drive_SendPDO1(nodeId, 0x000F, 0); DelayMs(200);
}

void Drive_UpdateLogic(uint8_t n)
{
    // 1. Home Prosedürü
    if (axes[n].req_home == true) {
        if (n == 1) {
            CAN_WriteSDO(n, 0x607C, 0x00, 0, 4); axes[n].req_home = false;
        } else {
            bool s_active = (axes[axes[n].home_sensor_node].inputs & (1UL << axes[n].home_sensor_bit));
            switch(axes[n].h_state) {
                case H_IDLE: axes[n].target_pos = axes[n].actual_pos; axes[n].h_state = H_SEARCH; break;
                case H_SEARCH:
                    if (s_active == true) {
                        axes[n].h_state = H_BACKOFF;
                        axes[n].target_pos = axes[n].actual_pos - 64000;
                    } else {
                        axes[n].target_pos += 1000;
                    }
                    break;
                case H_BACKOFF:
                    if (axes[n].status & 0x0400) { axes[n].h_state = H_TOUCH; }
                    break;
                case H_TOUCH:
                    if (s_active == true) { axes[n].h_state = H_DONE; }
                    else { axes[n].target_pos += 50; }
                    break;
                case H_DONE:
                    CAN_WriteSDO(n, 0x607C, 0x00, 0, 4);
                    axes[n].req_home = false; axes[n].h_state = H_IDLE;
                    break;
            }
            uint16_t cw_h;
            if (axes[n].pp_toggle == true) { cw_h = 0x001F; } else { cw_h = 0x000F; }
            Drive_SendPDO1(n, cw_h, axes[n].target_pos);
            axes[n].pp_toggle = !axes[n].pp_toggle;
        }
        return;
    }

    // 2. Hareket Mantığı (Manual/Auto/Streaming)
    if (axes[n].is_auto == false) {
        // Manuel Mod: Sadece enabled tutulur
        Drive_SendPDO1(n, 0x000F, axes[n].actual_pos);
    } else {
        // Otomatik veya Streaming: Handshake Toggle
        uint16_t cw_a;
        if (axes[n].pp_toggle == true) {
            cw_a = 0x003F; // Start + Immediate
        } else {
            cw_a = 0x002F; // Clear bit 4
        }
        Drive_SendPDO1(n, cw_a, axes[n].target_pos);
        axes[n].pp_toggle = !axes[n].pp_toggle;
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

    // Ethernet (Orijinal Blok)
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

    // Eksenleri İlklendir
    for (uint8_t i = 1; i <= ACTIVE_AXIS_COUNT; i++) {
        Drive_Initialize(i);
        axes[i].home_sensor_node = i; axes[i].home_sensor_bit = 16;
        axes[i].is_auto = true;
    }

    while (1)
    {
        StackTask(); UdpServerTask(); read_input();

        // IO Mapping
        DoutPort.bitField.Bit0 = Aux0;   DoutPort.bitField.Bit1 = Aux1;
        DoutPort.bitField.Bit2 = Aux2;   DoutPort.bitField.Bit3 = Aux3;
        DoutPort.bitField.Bit4 = Aux4;   DoutPort.bitField.Bit5 = Aux5;
        DoutPort.bitField.Bit6 = Aux6;   DoutPort.bitField.Bit7 = Aux7;
        DoutPort.bitField.Bit8 = Aux8;   DoutPort.bitField.Bit9 = Aux9;
        DoutPort.bitField.Bit10 = Aux10; DoutPort.bitField.Bit11 = Aux11;
        OUTPUTSL_VAL = (unsigned char)(DoutPort.allvalue & 0xFF);
        OUTPUTSH_VAL = (unsigned char)((DoutPort.allvalue >> 8) & 0xFF);
        set_outpus();

        // 4ms Senkron Çevrim (uart_cnt tabanlı)
        uart_cnt++;
        if (uart_cnt >= 40)
        {
            UdpServerStep1 = UDP_SERVER_REQUEST_RECEIVED;
            uart_cnt = 0;

            // --- TEST SENARYOSU ---
            switch(global_test_state) {
                case T_INIT:
                    axes[1].target_pos = 1280000; // 1 Tur
                    global_test_state = T_TURN;
                    break;
                case T_TURN:
                    if (axes[1].status & 0x0400) { // Tur Bitti
                        global_test_state = T_WAIT;
                        global_test_timer = 0;
                    }
                    break;
                case T_WAIT:
                    if (++global_test_timer >= 500) { // 2 saniye
                        global_test_state = T_STREAM;
                    }
                    break;
                case T_STREAM:
                    axes[1].target_pos += 18; // +18 Akış
                    break;
            }

            for (uint8_t n = 1; n <= ACTIVE_AXIS_COUNT; n++) {
                Drive_UpdateLogic(n);
            }

            // SYNC
            CAN_MSG_OBJ s = {0}; s.msgId = 0x080; s.field.dlc = 0;
            s.field.idType = CAN_FRAME_STD; s.field.frameType = CAN_FRAME_DATA;
            while(C1TR01CONbits.TXREQ0 == 1); CAN1_Transmit(CAN_PRIORITY_HIGH, &s);

            other_cnt++; if (other_cnt >= 20) { other_cnt = 0; LEDLIVE_Toggle(); }
        }

        // CAN Feedback Dispatcher
        while (CAN1_ReceivedMessageCountGet() > 0) {
            CAN_MSG_OBJ rx; uint8_t rd[8]; rx.data = rd;
            if (CAN1_Receive(&rx)) {
                uint8_t nid = (uint8_t)(rx.msgId & 0x0F);
                if (nid >= 1 && nid <= 4) {
                    if ((rx.msgId & 0xFF0) == 0x180) {
                        axes[nid].status = (uint16_t)rx.data[0] | ((uint16_t)rx.data[1] << 8);
                        axes[nid].actual_pos = (int32_t)rx.data[2] | ((int32_t)rx.data[3] << 8) | ((int32_t)rx.data[4] << 16) | ((int32_t)rx.data[5] << 24);
                    } else if ((rx.msgId & 0xFF0) == 0x280) {
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

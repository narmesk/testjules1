#include "ModbusHandler.h"
#include "Logger.h"
#include "ConfigStorage.h"

extern String price_arr[];
extern String parameters[];
extern String transmitpacket;
extern char modbustask;
extern String pt_quantity;
extern int pt_slave_ids[24];
extern int pt_target_count;
extern char modbusPTres[];
extern int flag1;
extern String StringDeleteChar(String str, char ch);

ScreenCommand Screen_Vals[8];
uint8_t uart1_tx_buffer[MODBUS_REQUEST_BYTES];
uint8_t uart1_rx_buffer[MODBUS_RESPONSE_BYTES];
uint8_t IDsStatus[25];
uint8_t IDsPointer = 0;

enum ModbusState {
    STATE_IDLE,
    STATE_SEND_PACKET,
    STATE_WAIT_RESPONSE
};
ModbusState current_communication_state = STATE_IDLE;
uint32_t response_timeout_timer = 0;
uint8_t rx_index = 0;
uint8_t header_ok = 0;
uint8_t current_target_slave_id = 0;

uint16_t Calculate_Modbus_CRC(const uint8_t *buf, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// "43.75" gibi ondalikli fiyat metnini kurus/cent cinsinden tam sayiya (4375) cevirir.
// toInt() dogrudan kullanilirsa noktadan sonrasi (kurus kismi) sessizce kayboluyordu.
uint32_t PriceStringToUnits(const String &priceStr) {
    return (uint32_t)round(priceStr.toFloat() * 100.0f);
}

void ParsePriceArrayToStruct(void) {
    for (uint8_t i = 0; i < 8; i++) {
        Screen_Vals[i].Val_Pos1 = PriceStringToUnits(price_arr[i * 3 + 0]);
        Screen_Vals[i].Val_Pos2 = PriceStringToUnits(price_arr[i * 3 + 1]);
        Screen_Vals[i].Val_Pos3 = PriceStringToUnits(price_arr[i * 3 + 2]);
    }
}

void ModbusTaskStart() {
    if (modbustask == 's') {
        for (int i = 0; i < 24; i++) {
            modbusPTres[i] = '0';
            IDsStatus[i] = 0;
        }
        IDsPointer = 0;
        current_communication_state = STATE_SEND_PACKET;
        modbustask = 'f';
    }

    if (modbustask == 'f') {
        ModbusTaskSend();
    }
}

void ModbusTaskSend() {
    // 24 slotun tamami taranir; stock.json'da tanimli olmayan (pt_slave_ids[i] == 0)
    // slotlar atlanir. pt_target_count sadece bilgi amacli (ör. /modbusstatus) tutulur.
    const int total_slots = 24;

    if (IDsPointer >= total_slots) {
        modbustask = 'x';
        current_communication_state = STATE_IDLE;
        return;
    }

    if (pt_slave_ids[IDsPointer] == 0) {
        IDsPointer++;
        current_communication_state = STATE_SEND_PACKET;
        return;
    }

    if (modbusPTres[IDsPointer] == '1') {
        IDsPointer++;
        current_communication_state = STATE_SEND_PACKET;
        return;
    }

    switch (current_communication_state) {

        case STATE_SEND_PACKET: {
            ParsePriceArrayToStruct();

            current_target_slave_id = pt_slave_ids[IDsPointer];
            IDsStatus[IDsPointer] = 1;

            uart1_tx_buffer[0] = current_target_slave_id;
            uart1_tx_buffer[1] = 0x10;
            uart1_tx_buffer[2] = 0x00;
            uart1_tx_buffer[3] = 0x00;
            uart1_tx_buffer[4] = 0x00;
            uart1_tx_buffer[5] = 0x18;
            uart1_tx_buffer[6] = 0x30;

            uint8_t *dPtr = &uart1_tx_buffer[7];
            for (uint8_t temp_cnt = 0; temp_cnt < 8; temp_cnt++) {
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos1 >> 8);
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos1 & 0xFF);
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos2 >> 8);
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos2 & 0xFF);
            }
            for (uint8_t temp_cnt = 0; temp_cnt < 8; temp_cnt++) {
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos3 >> 8);
                *dPtr++ = (uint8_t)(Screen_Vals[temp_cnt].Val_Pos3 & 0xFF);
            }

            uint16_t tx_crc = Calculate_Modbus_CRC(uart1_tx_buffer, 55);
            uart1_tx_buffer[55] = (uint8_t)(tx_crc & 0xFF);
            uart1_tx_buffer[56] = (uint8_t)((tx_crc >> 8) & 0xFF);

            if (Serial1.available()) {
                Serial1.read();
            }
            Serial1.write(uart1_tx_buffer, MODBUS_REQUEST_BYTES);
            Serial1.flush();

            rx_index = 0;
            header_ok = 0;
            response_timeout_timer = millis();
            current_communication_state = STATE_WAIT_RESPONSE;
            break;
        }

        case STATE_WAIT_RESPONSE: {
            if (Serial1.available() && rx_index < MODBUS_RESPONSE_BYTES) {
                uint8_t incoming_byte = Serial1.read();

                if (!header_ok) {
                    if (rx_index == 0 && incoming_byte == current_target_slave_id) {
                        uart1_rx_buffer[rx_index++] = incoming_byte;
                        header_ok = 1;
                    }
                } else {
                    uart1_rx_buffer[rx_index++] = incoming_byte;
                }
                response_timeout_timer = millis();
            }

            if (rx_index >= MODBUS_RESPONSE_BYTES) {
                uint16_t rx_crc = Calculate_Modbus_CRC(uart1_rx_buffer, 6);
                if ((uart1_rx_buffer[6] == (uint8_t)(rx_crc & 0xFF)) && (uart1_rx_buffer[7] == (uint8_t)((rx_crc >> 8) & 0xFF))) {
                    modbusPTres[IDsPointer] = '1';
                    IDsStatus[IDsPointer] = 3;
                } else {
                    IDsStatus[IDsPointer] = 2;
                }
                IDsPointer++;
                current_communication_state = STATE_SEND_PACKET;
            }
            else if ((millis() - response_timeout_timer) > 40) {
                IDsStatus[IDsPointer] = 2;
                IDsPointer++;
                current_communication_state = STATE_SEND_PACKET;
            }
            break;
        }

        default:
            current_communication_state = STATE_IDLE;
            break;
    }
}

String SendSerialProcStart() {
    transmitpacket = "";
    String pricedelm = "";
    String paramdelm = "";
    int priceleftc = 0;
    int priceleftctot = 36;

    String prices = "";
    for (int i = 0; i < 12; i++) {
        prices += price_arr[i];
    }

    pricedelm = StringDeleteChar(prices, '.');
    pricedelm = StringDeleteChar(prices, ':');

    priceleftc = priceleftctot - pricedelm.length();
    for (int i = 0; i < priceleftc; i++)
        pricedelm += "0";

    if (parameters[1].length() == 1)
        paramdelm = "00" + parameters[1];
    else if (parameters[1].length() == 2)
        paramdelm = "0" + parameters[1];
    transmitpacket += paramdelm;

    if (parameters[2].length() == 1)
        paramdelm = "0" + parameters[2];
    else
        paramdelm = parameters[2];
    transmitpacket += paramdelm;

    if (parameters[5].length() == 1)
        paramdelm = "0" + parameters[5];
    else
        paramdelm = parameters[5];
    transmitpacket += paramdelm;

    transmitpacket += pricedelm;
    transmitpacket += "170003";
    serverlog(transmitpacket, "ln");
    flag1 = 1;

    return "";
}

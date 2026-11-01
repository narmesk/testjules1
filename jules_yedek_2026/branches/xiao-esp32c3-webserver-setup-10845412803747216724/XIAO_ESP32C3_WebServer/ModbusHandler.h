#ifndef MODBUS_HANDLER_H
#define MODBUS_HANDLER_H

#include <Arduino.h>

typedef struct {
    uint32_t Val_Pos1;
    uint32_t Val_Pos2;
    uint32_t Val_Pos3;
} ScreenCommand;

#define MODBUS_REQUEST_BYTES  57
#define MODBUS_RESPONSE_BYTES  8

String SendSerialProcStart();
void ModbusTaskStart();
void ModbusTaskSend();
uint16_t Calculate_Modbus_CRC(const uint8_t *buf, uint8_t len);
void ParsePriceArrayToStruct(void);
uint32_t PriceStringToUnits(const String &priceStr);

#endif

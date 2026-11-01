#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

struct KeyValuePair {
    String key;
    String value;
};

// Global Değişken Beyanları (extern)
extern AsyncWebServer server;
extern KeyValuePair espsettings[12];
extern String price_arr[24];
extern String parameters[48];

extern String sid;
extern String pid;
extern String ptype;
extern String pmaster;
extern String devprod;
extern String mode_apsta;
extern String espsettings_sid;

extern String ap_ssid_buff_def;
extern String ap_password_buff_def;
extern int ap_maxconnections;

extern int wifiretries;
extern int wifimaxretries;
extern int wifi_fail_try;

extern String serverName_process;
extern String serverName_pool;
extern String serverName_poolp;
extern String cfjres;

extern String receivepacket;
extern String receivepacket_before;
extern String transmitpacket;

extern char is_modbus_hmi;
extern char modbustask;
extern char change_source;
extern char modbusPTres[24];

extern unsigned long currentTime;
extern unsigned long cTime_wificonnect;
extern const long intv_wificonnect;

extern int serialbufferarr[60];
extern int flag1;
extern String pt_quantity;

// stock.json'dan yuklenen Modbus hedef PT listesi (ptid degerleri = gercek slave ID)
extern int pt_slave_ids[24];
extern int pt_target_count;

extern String msg_str;

extern const char* PARAM_params[48];
extern const char* PARAM_leds[18];

// Yardımcı Fonksiyonlar
String StringDeleteChar(String str, char ch);

#endif

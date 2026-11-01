#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "driver/uart.h"
#include <WiFiClientSecure.h>

#define RXD2 3
#define TXD2 2

unsigned long currentTime = millis();
unsigned long cTime_wificonnect = millis();
unsigned long cTime_serverpool = millis();
unsigned long cTime_restartnormal = millis();
unsigned long cTime_modbuschecksend = millis();
unsigned long intv_wificonnect = 600000;
unsigned long intv_serverpool = 60000;
unsigned long intv_restartnormal = 3600000;
unsigned long intv_modbuschecksend = 60000;

String prices = "";
String price_arr[24] = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };
int priceupdatefinish = 0;
String transmitpacket = "";
String receivepacket = "";
String receivepacket_before = "000000000000000000000000N";
String receivepacket2 = "";
String pmaster = "";
String pid = "";
String ptype = "";
String parameters[48];
String sid = "";
String devprod = "p";
String logs = "";
String mode_apsta = "apsta";
String ap_ssid_buff_def = "Ledovate-Price-Change";
String ap_password_buff_def = "123456789";
int wifi_fail_try = 0;
String msg_str;

uint8_t wifiretries = 0;
uint8_t wifimaxretries = 3;
uint8_t ap_maxconnections = 2;
int ap_currentstations = 0, ap_newstations = 0;

typedef struct espsettings_kv_ {
  String key;
  String value;
} espsettings_kv_t;

espsettings_kv_t espsettings[] = {
  { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }
};
String espsettings_sid = "";

const char *PARAM_leds[] = { "led1", "led2", "led3", "led4", "led5", "led6", "led7", "led8", "led9", "led10", "led11", "led12", "led13", "led14", "led15", "led16", "brightness", "speed" };
const char *PARAM_params[] = { "ledparam1", "ledparam2", "ledparam3", "ledparam4", "ledparam5", "ledparam6", "ledparam7", "ledparam8", "ledparam9", "ledparam10", "ledparam11", "ledparam12", "ledparam13", "ledparam14", "ledparam15", "ledparam16", "ledparam17", "ledparam18", "ledparam19", "ledparam20", "ledparam21", "ledparam22", "ledparam23", "ledparam24", "ledparam25", "ledparam26", "ledparam27", "ledparam28", "ledparam29", "ledparam30", "ledparam31", "ledparam32", "ledparam33", "ledparam34", "ledparam35", "ledparam36", "ledparam37", "ledparam38", "ledparam39", "ledparam40", "ledparam41", "ledparam42", "ledparam43", "ledparam44", "ledparam45", "ledparam46", "ledparam47", "ledparam48" };

String serverName = "https://ledobe.com";
String serverName_process = serverName + "/app/process.php";
String serverName_pool = serverName + "/app/cfg/ppool/";
String serverName_ledcfg = serverName + "/app/cfg/ledcfg/";
String serverName_pres = serverName + "/app/cfg/pres/";
String serverName_poolp = "";
String poolres = "";
String cfjres = "";

unsigned int flag1 = 0;
int incomingByte;

String modbus_function = "16";
String modbus_startingaddr = "0";
String modbus_startingnum = "0";
String modbus_quantity = "24";
String modbus_bytecount = "48";
String pt_quantity = "0";
String pt_startingaddr = "0";
String waitForReply = "";
int serialtrynum = 3;
int serialtrynum_cur = 0;
char modbusPTres[24];
char modbustask = 'x';
byte serialbufferarr[57];
char is_modbus_hmi = 'm';
char change_source = 'a';

// Forward Declarations (Fonksiyonların alt tarafta tanınması için ön bildirimler)
void serverlog(String msg, String ln);
String getvfrom_espsettings(String key1);
String sendhttp(String httpRequestData, String httpType, String serverPath);
void ModbusTaskSend_Self();
void ModbusTaskSend_RP();
void ModbusTaskSend_RPPT();
String ModbusTaskSend();
void ReadSerialBuffer();
String WriteLedConfig(char *proct);
String LedPrepareResult(String type = "res");
String StringDeleteChar(String words, char no);
String SendSerialProcStart();
void esprestart();

AsyncWebServer server(80);

#define highByte(x) ((x) >> (8))
#define lowByte(x) ((x) & (0xff))

String processor(const String &var) {
  return String();
}

void OnWiFiEvent(WiFiEvent_t event) {
  serverlog("WiFi-event - ", "");
  serverlog((String)event, "");
  serverlog(": ", "");

  switch (event) {
    case 0: serverlog("WIFI_READY,0,ESP32 wifi ready", "ln"); break;
    case 10: serverlog("AP_START,10,AP Started", "ln"); break;
    case 12: serverlog("AP_STACONNECTED,12,AP Station Connected", "ln"); break;
    case 14: serverlog("AP_STAIPASSIGNED,14,AP Station IP Assigned", "ln"); break;
    case 13: serverlog("AP_STADISCONNECTED,13,AP Station Disconnected", "ln"); break;
    default: serverlog("otherevent", "ln"); break;
  }
}

String StringDeleteChar(String words, char no) {
  char c;
  for (int i = 0; i < words.length(); ++i) {
    c = words.charAt(i);
    if (c == no) {
      words.remove(i, 1);
      i--;
    }
  }
  return words;
}

String LedPrepareResult(String type) {
  if (is_modbus_hmi == 'm') {
    String receivepacket_wanted = "";
    receivepacket = "";
    for (int i = 0; i < 24; i++) {
      receivepacket += modbusPTres[i];
      receivepacket_wanted += '1';
    }
    if (receivepacket.substring(0, parameters[2].toInt()) == receivepacket_wanted.substring(0, parameters[2].toInt()))
      receivepacket += 'O';
    else
      receivepacket += 'N';
  }
  String receivepacketfilter = receivepacket;
  receivepacketfilter.trim();
  if (receivepacketfilter.length() >= 25) {
    receivepacketfilter = receivepacketfilter.substring(receivepacketfilter.length() - 25);
  }
  serverlog("filtered:", "ln");
  serverlog(receivepacketfilter, "");

  String msg = "{\"res\":\"" + receivepacketfilter + "\"}";
  if (type == "res") return msg;
  else if (type == "trim") return receivepacketfilter;
  return "";
}

int FileWriteF(String txt, String path) {
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    serverlog("Failed to open file for writing", "ln");
    return 0;
  }
  int bytesWritten = file.print(txt);
  if (bytesWritten > 0) {
    serverlog("ledconfig.json written success:", "");
    serverlog((String)bytesWritten, "ln");
  } else {
    serverlog("ledconfig.json write failed", "ln");
  }
  file.close();
  return bytesWritten;
}

String WriteLedConfig(char *proct) {
  File file = LittleFS.open("/ledconfig.json", FILE_READ);
  if (!file) {
    serverlog("Failed to open ledconfig.json for reading", "ln");
    return "";
  }
  String ledconfigstr;
  serverlog("ledconfig.json Read Success", "ln");
  while (file.available()) {
    ledconfigstr += String((char)file.read());
  }
  file.close();

  JsonDocument doc;  // ArduinoJson v7 formatı
  DeserializationError error = deserializeJson(doc, ledconfigstr);
  if (error) {
    serverlog("deserializeJson() failed: ", "");
    serverlog(error.f_str(), "ln");
    return "";
  }

  const char *gstationidc = doc["sid"];
  sid = gstationidc ? gstationidc : "";
  serverName_poolp = serverName_pool + sid + ".json";

  const char *pidc = doc["pid"];
  pid = pidc ? pidc : "";
  const char *ptypec = doc["ptype"];
  ptype = ptypec ? ptypec : "";
  const char *pmasterc = doc["pmaster"];
  pmaster = pmasterc ? pmasterc : "";

  if (String(proct) == "p") {
    doc["ptype"] = "pr";
    for (int i = 0; i < 24; i++) {
      doc["leds"][i]["v"] = price_arr[i];
    }
  } else if (String(proct) == "m") {
    doc["ptype"] = "pm";
    for (int z = 0; z < 48; z++) {
      doc["parameters"][z]["v"] = parameters[z];
    }
  } else if (String(proct) == "s") {
    doc["ptype"] = "st";
    doc["sid"] = espsettings_sid;
    for (int x = 0; x <= 11; x++) {
      if (espsettings[x].key != "")
        doc["esp32sett"][espsettings[x].key] = espsettings[x].value;
    }
  }

  if (String(proct) == "p" || String(proct) == "m" || String(proct) == "s") {
    doc["pdate"] = "";
    doc["pmaster"] = "ln";
    doc["pid"] = random(11111111, 99999999);
  }

  ledconfigstr = "";
  serializeJson(doc, ledconfigstr);

  const char *vtmp = "";
  for (int i = 0; i < 48; i++) {
    vtmp = doc["parameters"][i]["v"];
    parameters[i] = vtmp ? String(vtmp) : "";
  }

  int x = 0;
  JsonObject root = doc["esp32sett"];
  for (JsonPair kv : root) {
    espsettings[x].key = String(kv.key().c_str());
    espsettings[x].value = kv.value().as<String>();
    x += 1;
    if (x >= 12) break;
  }
  devprod = getvfrom_espsettings("devprod");

  if (String(proct) == "x") {
    const char *vtmp1 = "";
    for (int i = 0; i < 24; i++) {
      vtmp1 = doc["leds"][i]["v"];
      price_arr[i] = vtmp1 ? String(vtmp1) : "0";
    }
  }

  if (String(proct) != "x") {
    FileWriteF(ledconfigstr, "/ledconfig.json");
    String serverName_process2 = serverName_process + "?savecfj=t&sid=" + sid;
    sendhttp(ledconfigstr, "httpjson", serverName_process2);
  }
  return "";
}

String SendSerialProcStart() {
  transmitpacket = "";
  String lastpart = "170003";
  String pricedelm = "";
  String paramdelm = "";
  int priceleftc = 0;
  int priceleftctot = 36;

  String prices = "";
  for (int i = 0; i < 12; i++) {
    prices += price_arr[i];
  }

  pricedelm = StringDeleteChar(prices, '.');
  pricedelm = StringDeleteChar(pricedelm, ':');

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
  transmitpacket += lastpart;

  serverlog(transmitpacket, "ln");
  flag1 = 1;

  return "";
}

void serverlog(String msg, String ln) {
  if (devprod == "d" || devprod == "D") {
    if (ln == "ln")
      Serial.println(msg);
    else if (ln == "")
      Serial.print(msg);
  }
}

String getvfrom_espsettings(String key1) {
  for (int x = 0; x < 12; x++) {
    if (key1 == espsettings[x].key) {
      return espsettings[x].value;
    }
  }
  return "";
}

void esprestart() {
  serverlog("Restarting in 2 seconds", "ln");
  delay(2000);
  ESP.restart();
}

void initap() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);

    if (WiFi.status() != WL_CONNECTED) {
      String ap_ssid_buff = "";
      String ap_password_buff = "";

      if (getvfrom_espsettings("apid") == "") {
        ap_ssid_buff = ap_ssid_buff_def;
        ap_password_buff = ap_password_buff_def;
      } else {
        ap_ssid_buff = getvfrom_espsettings("apid");
        ap_password_buff = getvfrom_espsettings("appw");
      }

      char ap_ssid[ap_ssid_buff.length() + 1];
      ap_ssid_buff.toCharArray(ap_ssid, ap_ssid_buff.length() + 1);

      char ap_password[ap_password_buff.length() + 1];
      ap_password_buff.toCharArray(ap_password, ap_password_buff.length() + 1);

      serverlog("Setting AP ", "ln");

      if (WiFi.softAP(ap_ssid, ap_password, 1, false, ap_maxconnections) == true) {
        delay(100);
        IPAddress Ip(7, 3, 2, 1);
        IPAddress NMask(255, 255, 255, 0);
        WiFi.softAPConfig(Ip, Ip, NMask);

        serverlog("AP: ", "");
        serverlog(ap_ssid, "");
        serverlog(" : ", "");
        serverlog((String)WiFi.softAPIP().toString().c_str(), "ln");
      } else {
        delay(100);
        serverlog("Unable to Create Access Point", "ln");
      }
    }
  }
}

void initwifi() {
  if (mode_apsta == "apsta") {
    WiFi.mode(WIFI_STA);

    String wifi_ssid_buff = "";
    String wifi_password_buff = "";
    wifi_ssid_buff = getvfrom_espsettings("wifiid");
    wifi_password_buff = getvfrom_espsettings("wifipw");

    if (wifi_ssid_buff != "" && wifi_password_buff != "") {
      char wifi_ssid[wifi_ssid_buff.length() + 1];
      wifi_ssid_buff.toCharArray(wifi_ssid, wifi_ssid_buff.length() + 1);

      char wifi_password[wifi_password_buff.length() + 1];
      wifi_password_buff.toCharArray(wifi_password, wifi_password_buff.length() + 1);

      WiFi.begin(wifi_ssid, wifi_password);

      serverlog("Connecting to ", "");
      serverlog(wifi_ssid, "ln");

      while (WiFi.status() != WL_CONNECTED && wifiretries < wifimaxretries) {
        serverlog(".", "");
        wifiretries++;
        delay(1000);
      }

      if (WiFi.status() == WL_CONNECTED) {
        serverlog("", "ln");
        serverlog("Connected!", "ln");
        serverlog("IP address for localnetwork ", "");
        serverlog(wifi_ssid, "");
        serverlog(" : ", "");
        serverlog(WiFi.localIP().toString().c_str(), "ln");
        serverlog("wifi rssi:", "");
        serverlog((String)WiFi.RSSI(), "ln");
        wifi_fail_try = 0;
        WiFi.softAPdisconnect();
      } else {
        serverlog("Unable to Connect to ", "");
        serverlog(wifi_ssid, "ln");
      }
      wifiretries = 0;
    }
  }
}

String sendhttp(String httpRequestData, String httpType, String serverPath) {
  if (WiFi.status() == WL_CONNECTED && mode_apsta == "apsta") {
    WiFiClientSecure client;
    //client.setInsecure();  // Sunucu SSL sertifikasını doğrulamadan geçmesini sağlar.
    HTTPClient http;
    http.begin(serverPath.c_str());
    int httpResponseCode = 0;
    String payload = "{}";

    if (httpType == "httpget")
      httpResponseCode = http.GET();
    else if (httpType == "httppost") {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpResponseCode = http.POST(httpRequestData);
    } else if (httpType == "httpjson") {
      http.addHeader("Content-Type", "application/json");
      httpResponseCode = http.POST(httpRequestData);
    }

    if (httpResponseCode > 0) {
      serverlog("HTTP Response code: ", "");
      serverlog((String)httpResponseCode, "ln");
      payload = http.getString();
      serverlog(payload, "ln");
    } else {
      serverlog("Error code: ", "");
      serverlog((String)httpResponseCode, "ln");
    }
    http.end();
    return payload;
  } else {
    return "";
  }
}

String checkncfj() {
  if (cfjres.substring(0, 8) == "{\"pdate\"") {
    serverlog("cfj download success", "ln");
    return "1";
  }
  return "0";
}

void receivepacketsend() {
  String receivepacket_trim = LedPrepareResult("trim");
  String receivepacket_res = LedPrepareResult("res");

  serverlog("receivepacket:" + receivepacket_trim + "  receivepacket_before:" + receivepacket_before, "ln");
  if (pmaster == "sv" && receivepacket_before != receivepacket_trim) {
    cfjres = "";
    String serverName_process1 = serverName_process + "?procres=" + receivepacket_trim + "&pid=" + pid;
    cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process1);
    receivepacket_before = receivepacket_trim;
    serverlog("receivepacketsend success", "ln");
  }
}
void initserver() {
  // SPIFFS yerine LittleFS tabanlı handler yönlendirmeleri sağlandı
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /", "ln");
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/parameters", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /parameters", "ln");
    request->send(LittleFS, "/parameters.html", String(), false, processor);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /settings", "ln");
    request->send(LittleFS, "/settings.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /style.css", "ln");
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /js.js", "ln");
    request->send(LittleFS, "/js.js", "text/javascript");
  });

  server.on("/getledconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /getledconfig", "ln");
    request->send(LittleFS, "/ledconfig.json", "text/plain");
  });

  server.on("/sendparam", HTTP_POST, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_POST /sendparam", "ln");
    if (request->hasParam(PARAM_params[0], true)) {
      for (int i = 0; i < 48; i++) {
        parameters[i] = request->getParam(PARAM_params[i], true)->value();
        serverlog(parameters[i], "");
      }
      serverlog("", "ln");
      change_source = 'a';
      WriteLedConfig("m");
    }
    request->send(200, "text/plain", "{\"res\":\"paramok\"}");
  });

  server.on("/sendsett", HTTP_POST, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_POST /sendsett", "ln");
    int paramsNr = request->params();

    if (paramsNr > 0) {
      int z = 0;
      for (int i = 0; i < paramsNr; i++) {
        // getParam dönüş tipi const AsyncWebParameter* formatına çekildi
        const AsyncWebParameter *p = request->getParam(i);
        serverlog(p->name(), "");
        serverlog(":", "");
        serverlog(p->value(), "");
        serverlog("  ", "");

        if (p->name() == "gstationid") {
        } else if (p->name() == "sid") {
          espsettings_sid = p->value();
        } else {
          if (z < 12) {
            espsettings[z].key = p->name();
            espsettings[z].value = p->value();
            z += 1;
          }
        }
      }
      serverlog("", "ln");
      change_source = 'a';
      WriteLedConfig("s");
      request->send(200, "text/plain", "{\"res\":\"paramok\"}");
      esprestart();
    } else {
      request->send(200, "text/plain", "{\"res\":\"0\"}");
    }
  });

  server.on("/sendserial", HTTP_POST, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_POST /sendserial", "ln");
    if (request->hasParam(PARAM_leds[0], true)) {
      for (int i = 0; i < 18; i++) {
        if (request->hasParam(PARAM_leds[i], true))
          price_arr[i] = request->getParam(PARAM_leds[i], true)->value();
      }
      WriteLedConfig("p");

      if (is_modbus_hmi == 'h')
        SendSerialProcStart();
      else if (is_modbus_hmi == 'm')
        modbustask = 's';

      change_source = 'a';
    }
    request->send(200, "text/plain", "{\"res\":\"ok\"}");
  });

  server.on("/checkserial", HTTP_GET, [](AsyncWebServerRequest *request) {
    serverlog("HTTP_GET /checkserial", "");
    String ledmessage = LedPrepareResult();
    serverlog(ledmessage, "ln");
    request->send(200, "text/plain", ledmessage);
  });

  server.begin();
}
static uint16_t MODBUS_CRC16_v1(const unsigned char *buf, unsigned int len) {
  uint16_t crc = 0xFFFF;
  unsigned int i = 0;
  char bit = 0;

  for (i = 0; i < len; i++) {
    crc ^= buf[i];

    for (bit = 0; bit < 8; bit++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return (crc >> 8) | (crc << 8);
}

void ModbusTaskStart() {
  if (modbustask == 's') {
    for (int i = 0; i < 24; i++) {
      modbusPTres[i] = '0';
    }
    modbustask = 'f';
  }
  if (modbustask == 'f')
    ModbusTaskSend_Self();
  if (modbustask == 'r')
    ModbusTaskSend_RP();
  if (modbustask == 't')
    ModbusTaskSend_RPPT();
}

void ModbusTaskSend_Self() {
  ModbusTaskSend();
  if (change_source == 's')
    receivepacketsend();
}

void ModbusTaskSend_RP() {
}

void ModbusTaskSend_RPPT() {
}

char CheckSerialRes() {
  if (serialbufferarr[0] != 0) {
    if (serialbufferarr[8] == 0) {
      uint16_t crcr = MODBUS_CRC16_v1(serialbufferarr, sizeof(serialbufferarr) - 51);
      if (highByte(crcr) == serialbufferarr[6] && lowByte(crcr) == serialbufferarr[7])
        return '1';
    }
  }
  return 'n';
}

char CheckModbusTask() {
  for (int k = 0; k < parameters[2].toInt(); k++) {
    if (modbusPTres[k] != '1') {
      return '0';
    }
  }
  modbustask = 'x';
  return '1';
}

String ModbusTaskSend() {
  pt_startingaddr = parameters[1];
  pt_quantity = parameters[2];
  unsigned char valsbyte[58];
  int rowb = 0;

  String price_arr2[24];
  for (int i = 0; i < 24; i++) {
    price_arr2[i] = StringDeleteChar(price_arr[i], '.');
  }

  for (int i = 0; i < 58; i++) {
    valsbyte[i] = 0;
  }

  for (int tryn = 0; tryn < serialtrynum; tryn++) {
    for (int m = 0; m < pt_quantity.toInt(); m++) {
      if (m < 24 && modbusPTres[m] == '0') {
        rowb = 0;
        msg_str = "SerialTry: " + String(tryn + 1) + "-" + String(m + 1);
        serverlog(msg_str, "ln");

        valsbyte[rowb++] = (pt_startingaddr.toInt()) + m;
        valsbyte[rowb++] = modbus_function.toInt();
        valsbyte[rowb++] = 0;
        valsbyte[rowb++] = modbus_startingaddr.toInt();
        valsbyte[rowb++] = 0;
        valsbyte[rowb++] = modbus_quantity.toInt();
        valsbyte[rowb++] = modbus_bytecount.toInt();

        valsbyte[rowb++] = highByte(price_arr2[6].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[6].toInt());
        valsbyte[rowb++] = highByte(price_arr2[7].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[7].toInt());
        valsbyte[rowb++] = highByte(price_arr2[0].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[0].toInt());
        valsbyte[rowb++] = highByte(price_arr2[1].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[1].toInt());
        valsbyte[rowb++] = highByte(price_arr2[2].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[2].toInt());
        valsbyte[rowb++] = highByte(price_arr2[3].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[3].toInt());
        valsbyte[rowb++] = highByte(price_arr2[4].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[4].toInt());
        valsbyte[rowb++] = highByte(price_arr2[5].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[5].toInt());
        valsbyte[rowb++] = highByte(price_arr2[8].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[8].toInt());
        valsbyte[rowb++] = highByte(price_arr2[9].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[9].toInt());
        valsbyte[rowb++] = highByte(price_arr2[10].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[10].toInt());
        valsbyte[rowb++] = highByte(price_arr2[11].toInt());
        valsbyte[rowb++] = lowByte(price_arr2[11].toInt());

        valsbyte[rowb++] = 0;
        valsbyte[rowb++] = 0;
        valsbyte[rowb++] = 0;
        valsbyte[rowb++] = 0;

        valsbyte[rowb++] = highByte((((price_arr[17].toInt()) - 1) * 200) + 700);
        valsbyte[rowb++] = lowByte((((price_arr[17].toInt()) - 1) * 200) + 700);
        valsbyte[rowb++] = highByte((price_arr[16].toInt()) - 1);
        valsbyte[rowb++] = lowByte((price_arr[16].toInt()) - 1);

        uint16_t crcr = MODBUS_CRC16_v1(valsbyte, 55);

        valsbyte[55] = highByte(crcr);
        valsbyte[56] = lowByte(crcr);
        valsbyte[57] = 0xff;

        msg_str = "";
        for (int k = 0; k < 58; k++) {
          msg_str += String(valsbyte[k], HEX) + "-";
        }
        serverlog(msg_str, "ln");

        Serial1.write(valsbyte, 58);
        delay(170);

        ReadSerialBuffer();

        msg_str = "";
        for (int k = 0; k < 57; k++)
          msg_str += String(serialbufferarr[k], HEX) + "-";
        serverlog(msg_str, "ln");

        if (CheckSerialRes() == '1') {
          modbusPTres[m] = '1';
        }

        msg_str = "";
        for (int k = 0; k < 24; k++)
          msg_str += String(modbusPTres[k]) + "-";
        serverlog(msg_str, "ln");
      }
    }
  }
  CheckModbusTask();
  return "";
}

void ReadSerialBuffer() {
  for (int k = 0; k < 57; k++) {
    serialbufferarr[k] = 0;
  }

  int i = 0;
  if (Serial1.available()) {
    while (Serial1.available() > 0) {
      int inChar = (int)Serial1.read();
      serialbufferarr[i] = inChar;
      i++;
      if (i == 56) break;
    }
  }
}

String modbustest() {
  serverlog("sendingserialtest:", "ln");
  byte CardNumber[58] = { 0x24, 0x10, 0x00, 0x00, 0x00, 0x18, 0x30, 0x01, 0xb9, 0x01, 0xba, 0x00, 0x6f, 0x00, 0x70, 0x00, 0xdd, 0x00, 0xde, 0x01, 0x4b, 0x01, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x34, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x94, 0xff };
  for (int i = 0; i < 58; i++) {
    Serial.print(CardNumber[i], HEX);
  }
  Serial.println("");
  Serial.write(CardNumber, 58);
  Serial1.write(CardNumber, 58);

  return "";
}

void setup() {
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // SPIFFS yerine tamamen stabil LittleFS sistem başlatıcısı entegre edildi
  if (!LittleFS.begin(true)) {
    serverlog("An Error has occurred while mounting LittleFS", "ln");
    return;
  }

  WriteLedConfig("x");

  if (devprod == "d" || devprod == "D")
    Serial.begin(115200);
  delay(100);

  initwifi();
  initap();

  initserver();
}

void loop() {
  if (mode_apsta == "apsta") {
    if ((millis() - cTime_wificonnect >= intv_wificonnect)) {
      cTime_wificonnect = millis();
      serverlog("Checking Wifi status.................", "ln");

      if (WiFi.status() != WL_CONNECTED) {
        serverlog("Restart wifi before Pooling Server...", "ln");
        initwifi();
        if (WiFi.status() != WL_CONNECTED)
          initap();
      } else {
        serverlog("wifi is already connected.................", "ln");
      }
    }

    if ((millis() - cTime_serverpool >= intv_serverpool)) {
      serverlog((String)millis(), "");
      cTime_serverpool = millis();

      if (WiFi.status() == WL_CONNECTED) {
        serverlog("WİFİ is up, Pooling Server...", "ln");
        serverlog(serverName_poolp, "ln");
        poolres = "";
        poolres = sendhttp("", "httpget", serverName_poolp);
        if (poolres == "1") {
          cfjres = "";
          String serverName_process1 = serverName_process + "?getcfj=t&sid=" + sid + "&pstart=t";
          cfjres = sendhttp("", "httpget", serverName_process1);
          serverlog("cfj:", "ln");
          serverlog(cfjres, "ln");
          if (cfjres != "" && checkncfj() == "1") {
            FileWriteF(cfjres, "/ledconfig.json");
            change_source = 's';
            WriteLedConfig("x");
            if (ptype == "pr") {
              receivepacket_before = "000000000000000000000000N";
              modbustask = 's';
            } else {
              String serverName_process1 = serverName_process + "?procres=" + "t" + "&pid=" + pid;
              String receivepacket_res = "{\"res\":\"paramok\"}";
              cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process1);
            }
          }
        }
      } else {
        serverlog(" Server Pooling try but WiFi was not connected", "ln");
      }
    }
  }

  if ((millis() - cTime_restartnormal >= intv_restartnormal)) {
    cTime_restartnormal = millis();
    serverlog("Restart Normally..........", "ln");
    esprestart();
  }

  if ((millis() - cTime_modbuschecksend >= intv_modbuschecksend) || modbustask == 's') {
    cTime_modbuschecksend = millis();
    if (modbustask != 'x') {
      serverlog("Sendingmodbusserials...:", "");
      ModbusTaskStart();
    }
  }
}

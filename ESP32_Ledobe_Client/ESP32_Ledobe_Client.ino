/*
  ================================================================================
  ESP32_Ledobe_Client.ino - Sadeleştirilmiş Ledobe Bulut İstemcisi
  ================================================================================
  Bu sürüm, tüm Modbus RTU, RS232/RS485, seri paket protokolleri ve donanım
  CRC işlemlerinden tamamen arındırılmıştır.

  Temel Görevleri:
  1. Wi-Fi İstemci (STA) ve Yerel Erişim Noktası (SoftAP) Yönetimi.
  2. LittleFS üzerinden yerel web sunucusu (Ayar, Parametre ve Fiyat yönetimi).
  3. ledobe.com uzak sunucusu ile HTTPS tabanlı çift yönlü senkronizasyon:
     - Periyodik Polling (ppool/{sid}.json)
     - Yeni konfigürasyon indirme (process.php?getcfj=t)
     - Yerel LittleFS /ledconfig.json güncelleme
     - Sunucuya işlem onay bildirimi (process.php?procres=t)
     - Yerel web arayüzünden yapılan değişiklikleri sunucuya kaydetme (savecfj=t)
  ================================================================================
*/

#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// --- Zamanlayıcılar ve Aralıklar ---
unsigned long currentTime = millis();
unsigned long cTime_wificonnect = millis();
unsigned long cTime_serverpool = millis();
unsigned long cTime_restartnormal = millis();

unsigned long intv_wificonnect = 600000;    // 10 dakika
unsigned long intv_serverpool = 60000;       // 1 dakika (60 sn)
unsigned long intv_restartnormal = 3600000; // 1 saat

// --- Sistem ve Konfigürasyon Değişkenleri ---
String price_arr[24] = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };
String parameters[48];
String pmaster = "sv";
String pid = "";
String ptype = "";
String sid = "";
String devprod = "d"; // 'd' = Debug log aktif, 'p' = Prod
String mode_apsta = "apsta";
String ap_ssid_buff_def = "Ledovate-Price-Change";
String ap_password_buff_def = "123456789";

uint8_t wifiretries = 0;
uint8_t wifimaxretries = 3;
uint8_t ap_maxconnections = 2;

typedef struct espsettings_kv_ {
  String key;
  String value;
} espsettings_kv_t;

espsettings_kv_t espsettings[] = {
  { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" },
  { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }, { "", "" }
};
String espsettings_sid = "";

const char *PARAM_leds[] = {
  "led1", "led2", "led3", "led4", "led5", "led6", "led7", "led8",
  "led9", "led10", "led11", "led12", "led13", "led14", "led15", "led16",
  "brightness", "speed"
};

const char *PARAM_params[] = {
  "ledparam1", "ledparam2", "ledparam3", "ledparam4", "ledparam5", "ledparam6", "ledparam7", "ledparam8",
  "ledparam9", "ledparam10", "ledparam11", "ledparam12", "ledparam13", "ledparam14", "ledparam15", "ledparam16",
  "ledparam17", "ledparam18", "ledparam19", "ledparam20", "ledparam21", "ledparam22", "ledparam23", "ledparam24",
  "ledparam25", "ledparam26", "ledparam27", "ledparam28", "ledparam29", "ledparam30", "ledparam31", "ledparam32",
  "ledparam33", "ledparam34", "ledparam35", "ledparam36", "ledparam37", "ledparam38", "ledparam39", "ledparam40",
  "ledparam41", "ledparam42", "ledparam43", "ledparam44", "ledparam45", "ledparam46", "ledparam47", "ledparam48"
};

// --- Uzak Sunucu (ledobe.com) Adresleri ---
String serverName         = "https://ledobe.com";
String serverName_process = serverName + "/app/process.php";
String serverName_pool    = serverName + "/app/cfg/ppool/";
String serverName_ledcfg  = serverName + "/app/cfg/ledcfg/";
String serverName_pres    = serverName + "/app/cfg/pres/";
String serverName_poolp   = "";
String poolres            = "";
String cfjres             = "";

char change_source = 'a'; // 'a' = Yerel Web AP, 's' = Uzak Sunucu

// --- Web Sunucusu Nesnesi ---
AsyncWebServer server(80);

// --- Fonksiyon Bildirimleri ---
void serverlog(String msg, String ln = "");
String getvfrom_espsettings(String key1);
String sendhttp(String httpRequestData, String httpType, String serverPath);
String checkncfj();
int FileWriteF(String txt, String path);
String WriteLedConfig(const char *proct);
void initwifi();
void initap();
void initserver();
void esprestart();
void pollServer();

String processor(const String &var) {
  return String();
}

void serverlog(String msg, String ln) {
  if (devprod == "d" || devprod == "D") {
    if (ln == "ln")
      Serial.println(msg);
    else
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

int FileWriteF(String txt, String path) {
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    serverlog("Failed to open file for writing: " + path, "ln");
    return 0;
  }
  int bytesWritten = file.print(txt);
  if (bytesWritten > 0) {
    serverlog(path + " written success: " + String(bytesWritten), "ln");
  } else {
    serverlog(path + " write failed", "ln");
  }
  file.close();
  return bytesWritten;
}

String WriteLedConfig(const char *proct) {
  File file = LittleFS.open("/ledconfig.json", FILE_READ);
  if (!file) {
    serverlog("Failed to open ledconfig.json for reading", "ln");
    return "";
  }
  String ledconfigstr = "";
  while (file.available()) {
    ledconfigstr += (char)file.read();
  }
  file.close();

  JsonDocument doc;
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

  for (int i = 0; i < 48; i++) {
    const char *vtmp = doc["parameters"][i]["v"];
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
    for (int i = 0; i < 24; i++) {
      const char *vtmp1 = doc["leds"][i]["v"];
      price_arr[i] = vtmp1 ? String(vtmp1) : "0";
    }
  }

  // Yerel değişiklik yapıldıysa sunucuya da gönder
  if (String(proct) != "x") {
    FileWriteF(ledconfigstr, "/ledconfig.json");
    String serverName_process2 = serverName_process + "?savecfj=t&sid=" + sid;
    sendhttp(ledconfigstr, "httpjson", serverName_process2);
  }
  return "";
}

String sendhttp(String httpRequestData, String httpType, String serverPath) {
  if (WiFi.status() == WL_CONNECTED && mode_apsta == "apsta") {
    WiFiClientSecure client;
    // client.setInsecure(); // Gerekirse aktif edilebilir
    HTTPClient http;
    http.begin(serverPath.c_str());
    int httpResponseCode = 0;
    String payload = "{}";

    if (httpType == "httpget") {
      httpResponseCode = http.GET();
    } else if (httpType == "httppost") {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpResponseCode = http.POST(httpRequestData);
    } else if (httpType == "httpjson") {
      http.addHeader("Content-Type", "application/json");
      httpResponseCode = http.POST(httpRequestData);
    }

    if (httpResponseCode > 0) {
      serverlog("HTTP Response code: " + String(httpResponseCode), "ln");
      payload = http.getString();
      serverlog("Payload: " + payload, "ln");
    } else {
      serverlog("HTTP Error code: " + String(httpResponseCode), "ln");
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
  serverlog("cfj download fail", "ln");
  return "0";
}

void pollServer() {
  if ((millis() - cTime_serverpool >= intv_serverpool)) {
    cTime_serverpool = millis();

    if (WiFi.status() == WL_CONNECTED) {
      serverlog("WiFi is up, Pooling Server...", "ln");
      serverlog("Pool URL: " + serverName_poolp, "ln");

      poolres = sendhttp("", "httpget", serverName_poolp);
      if (poolres == "1") {
        cfjres = "";
        String serverName_process1 = serverName_process + "?getcfj=t&sid=" + sid + "&pstart=t";
        cfjres = sendhttp("", "httpget", serverName_process1);
        serverlog("cfj received:", "ln");
        serverlog(cfjres, "ln");

        if (cfjres != "" && checkncfj() == "1") {
          FileWriteF(cfjres, "/ledconfig.json");
          change_source = 's';
          WriteLedConfig("x");

          // Sunucuya işlemin başarıyla alındığını bildir
          String serverName_procres = serverName_process + "?procres=t&pid=" + pid;
          String receivepacket_res = "{\"res\":\"paramok\"}";
          cfjres = sendhttp(receivepacket_res, "httpjson", serverName_procres);
          serverlog("Server updated successfully, response sent.", "ln");
        }
      }
    } else {
      serverlog("Server Pooling skipped: WiFi is not connected", "ln");
    }
  }
}

void initap() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);

    String ap_ssid = getvfrom_espsettings("apid");
    String ap_pass = getvfrom_espsettings("appw");
    if (ap_ssid == "") ap_ssid = ap_ssid_buff_def;
    if (ap_pass == "") ap_pass = ap_password_buff_def;

    serverlog("Starting AP: " + ap_ssid, "ln");
    if (WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str(), 1, false, ap_maxconnections)) {
      IPAddress Ip(7, 3, 2, 1);
      IPAddress NMask(255, 255, 255, 0);
      WiFi.softAPConfig(Ip, Ip, NMask);
      serverlog("AP IP: " + WiFi.softAPIP().toString(), "ln");
    } else {
      serverlog("Unable to Create Access Point", "ln");
    }
  }
}

void initwifi() {
  if (mode_apsta == "apsta") {
    WiFi.mode(WIFI_STA);

    String wifi_ssid = getvfrom_espsettings("wifiid");
    String wifi_pass = getvfrom_espsettings("wifipw");

    if (wifi_ssid != "" && wifi_pass != "") {
      WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
      serverlog("Connecting to " + wifi_ssid, "ln");

      while (WiFi.status() != WL_CONNECTED && wifiretries < wifimaxretries) {
        serverlog(".");
        wifiretries++;
        delay(1000);
      }

      if (WiFi.status() == WL_CONNECTED) {
        serverlog("", "ln");
        serverlog("Connected! Local IP: " + WiFi.localIP().toString(), "ln");
        serverlog("RSSI: " + String(WiFi.RSSI()), "ln");
        WiFi.softAPdisconnect();
      } else {
        serverlog("Unable to Connect to " + wifi_ssid, "ln");
      }
      wifiretries = 0;
    }
  }
}

void initserver() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/parameters", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/parameters.html", String(), false, processor);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/settings.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/js.js", "text/javascript");
  });

  server.on("/getledconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/ledconfig.json", "text/plain");
  });

  server.on("/sendparam", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_params[0], true)) {
      for (int i = 0; i < 48; i++) {
        parameters[i] = request->getParam(PARAM_params[i], true)->value();
      }
      change_source = 'a';
      WriteLedConfig("m");
    }
    request->send(200, "text/plain", "{\"res\":\"paramok\"}");
  });

  server.on("/sendsett", HTTP_POST, [](AsyncWebServerRequest *request) {
    int paramsNr = request->params();
    if (paramsNr > 0) {
      int z = 0;
      for (int i = 0; i < paramsNr; i++) {
        const AsyncWebParameter *p = request->getParam(i);
        if (p->name() == "sid") {
          espsettings_sid = p->value();
        } else if (p->name() != "gstationid") {
          if (z < 12) {
            espsettings[z].key = p->name();
            espsettings[z].value = p->value();
            z += 1;
          }
        }
      }
      change_source = 'a';
      WriteLedConfig("s");
      request->send(200, "text/plain", "{\"res\":\"paramok\"}");
      esprestart();
    } else {
      request->send(200, "text/plain", "{\"res\":\"0\"}");
    }
  });

  server.on("/sendserial", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_leds[0], true)) {
      for (int i = 0; i < 18; i++) {
        if (request->hasParam(PARAM_leds[i], true))
          price_arr[i] = request->getParam(PARAM_leds[i], true)->value();
      }
      change_source = 'a';
      WriteLedConfig("p");
    }
    request->send(200, "text/plain", "{\"res\":\"ok\"}");
  });

  server.on("/checkserial", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "{\"res\":\"111111111111111111111111O\"}");
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  if (!LittleFS.begin(true)) {
    serverlog("An Error has occurred while mounting LittleFS", "ln");
    return;
  }

  WriteLedConfig("x");

  initwifi();
  initap();

  initserver();
  serverlog("ESP32 Ledobe Client ready.", "ln");
}

void loop() {
  if (mode_apsta == "apsta") {
    // Wi-Fi bağlantısını periyodik denetle
    if ((millis() - cTime_wificonnect >= intv_wificonnect)) {
      cTime_wificonnect = millis();
      if (WiFi.status() != WL_CONNECTED) {
        initwifi();
        if (WiFi.status() != WL_CONNECTED) initap();
      }
    }

    // Uzak bulut sunucusunu sorgula (Polling)
    pollServer();
  }

  // Periyodik yeniden başlatma
  if ((millis() - cTime_restartnormal >= intv_restartnormal)) {
    cTime_restartnormal = millis();
    serverlog("Periodic Restart...", "ln");
    esprestart();
  }
}

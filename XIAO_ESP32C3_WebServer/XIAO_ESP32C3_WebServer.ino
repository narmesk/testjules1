#include "Config.h"
#include "Logger.h"
#include "WiFiManagerMod.h"
#include "ConfigStorage.h"
#include "WebServerMod.h"
#include "ModbusHandler.h"

void setup() {
    Serial.begin(115200);
    // XIAO ESP32C3 1. Donanımsal UART Hattı Kurulumu (D6:TX, D7:RX)
    Serial1.begin(9600, SERIAL_8N1, D7, D6);

    // LittleFS Başlatma
    if (!LittleFS.begin(true)) {
        serverlog("LittleFS Mount Failed", "ln");
    } else {
        serverlog("LittleFS Mounted Successfully", "ln");
    }

    // Konfigürasyon Okuma
    WriteLedConfig("x");

    // WiFi Event Bağlama
    WiFi.onEvent(OnWiFiEvent);

    // WiFi ve WebServer Kurulumu
    initwifi();
    initap();
    initserver();
}

void loop() {
    // Zamanlayıcı Kontrolleri ve Modbus Görev İşleyicisi
    currentTime = millis();

    // Periyodik WiFi Bağlantı Kontrolü
    if (currentTime - cTime_wificonnect >= intv_wificonnect) {
        cTime_wificonnect = currentTime;
        if (WiFi.status() != WL_CONNECTED) {
            initwifi();
        }
    }

    // Modbus Görevi Varsa Çalıştır
    ModbusTaskStart();

    // İşlemcinin watchdog korumasını rahatlatmak için minik bekleme payı
    delay(1);
}

#include "Config.h"
#include "Logger.h"
#include "WiFiManagerMod.h"
#include "ConfigStorage.h"
#include "WebServerMod.h"
#include "ModbusHandler.h"

// Periyodik yeniden baslatma zamanlayicisi (1 saat)
unsigned long cTime_restartnormal = 0;
const unsigned long intv_restartnormal = 3600000;

void setup()
{
    Serial.begin(115200);
    // XIAO ESP32C3 1. Donanimsal UART Hatti Kurulumu (D6:TX, D7:RX)
    Serial1.begin(9600, SERIAL_8N1, D7, D6);

    // LittleFS Baslatma
    if (!LittleFS.begin(true))
    {
        serverlog("LittleFS Mount Failed", "ln");
    }
    else
    {
        serverlog("LittleFS Mounted Successfully", "ln");
    }

    // Konfigurasyon Okuma
    WriteLedConfig("x");

    // WiFi Event Baglama
    WiFi.onEvent(OnWiFiEvent);

    // WiFi STA (Local Network) ve AP Kurulumu
    initwifi();
    initap();

    // Web Sunucusunu Baslatma
    initserver();

    // Local Network Bilgilendirme Logu
    if (WiFi.status() == WL_CONNECTED)
    {
        serverlog("Local Network Connected!", "ln");
        serverlog("Local IP: http://" + WiFi.localIP().toString() + "/", "ln");
    }
    else
    {
        serverlog("Local Network not connected, Access Point active: http://" + WiFi.softAPIP().toString() + "/", "ln");
    }

    cTime_restartnormal = millis();
}

void loop()
{
    // Zamanlayici Kontrolleri ve Modbus Gorev Isleyicisi
    currentTime = millis();

    // Periyodik WiFi / Local Network Baglanti Kontrolu
    if (currentTime - cTime_wificonnect >= intv_wificonnect)
    {
        cTime_wificonnect = currentTime;
        if (WiFi.status() != WL_CONNECTED)
        {
            serverlog("Reconnecting to Local Network...", "ln");
            initwifi();
            // Eger Local Network'e baglanamazsa yerel AP acilmasini sagla
            if (WiFi.status() != WL_CONNECTED)
            {
                initap();
            }
        }
    }

    // Periyodik Sistem Yeniden Baslatma (1 saatlik koruma dongusu)
    if (currentTime - cTime_restartnormal >= intv_restartnormal)
    {
        cTime_restartnormal = currentTime;
        serverlog("Periodic Routine Restart...", "ln");
        esprestart();
    }

    // Modbus Gorevi Varsa Calistir
    ModbusTaskStart();

    // Islemcinin watchdog korumasini rahatlatmak icin minik bekleme payi
    delay(1);
}

#include "WiFiManagerMod.h"
#include "Logger.h"
#include "ConfigStorage.h"

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
void OnWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
#else
void OnWiFiEvent(WiFiEvent_t event) {
#endif
    serverlog("WiFi-event - ", "");
    serverlog((String)event, "");
    serverlog(": ", "");

    switch (event) {
        case 0:  serverlog("WIFI_READY,0,ESP32 wifi ready", "ln"); break;
        case 10: serverlog("AP_START,10,AP Started", "ln"); break;
        case 12: serverlog("AP_STACONNECTED,12,AP Station Connected", "ln"); break;
        case 14: serverlog("AP_STAIPASSIGNED,14,AP Station IP Assigned", "ln"); break;
        case 13: serverlog("AP_STADISCONNECTED,13,AP Station Disconnected", "ln"); break;
        default: serverlog("otherevent", "ln"); break;
    }
}

void initap() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_AP_STA);
        if (WiFi.status() != WL_CONNECTED) {
            String ap_ssid_buff = (getvfrom_espsettings("apid") == "") ? ap_ssid_buff_def : getvfrom_espsettings("apid");
            String ap_password_buff = (getvfrom_espsettings("apid") == "") ? ap_password_buff_def : getvfrom_espsettings("appw");

            char ap_ssid[ap_ssid_buff.length() + 1];
            ap_ssid_buff.toCharArray(ap_ssid, ap_ssid_buff.length() + 1);

            char ap_password[ap_password_buff.length() + 1];
            ap_password_buff.toCharArray(ap_password, ap_password_buff.length() + 1);

            serverlog("Setting AP ", "ln");
            if (WiFi.softAP(ap_ssid, ap_password, 1, false, ap_maxconnections)) {
                delay(100);
                IPAddress Ip(192, 168, 4, 1);
                IPAddress NMask(255, 255, 255, 0);
                WiFi.softAPConfig(Ip, Ip, NMask);

                serverlog("AP: ", "");
                serverlog(ap_ssid, "");
                serverlog(" : ", "");
                serverlog("http://" + WiFi.softAPIP().toString() + "/", "ln");
            } else {
                delay(100);
                serverlog("Unable to Create Access Point", "ln");
            }
        }
    }
}

void initwifi() {
    if (mode_apsta == "apsta") {
        WiFi.mode(WIFI_AP_STA);
        String wifi_ssid_buff = getvfrom_espsettings("wifiid");
        String wifi_password_buff = getvfrom_espsettings("wifipw");

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
                serverlog("http://" + WiFi.localIP().toString() + "/", "ln");
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

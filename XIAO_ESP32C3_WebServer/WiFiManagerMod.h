#ifndef WIFI_MANAGER_MOD_H
#define WIFI_MANAGER_MOD_H

#include "Config.h"

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
void OnWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
#else
void OnWiFiEvent(WiFiEvent_t event);
#endif

void initap();
void initwifi();

#endif

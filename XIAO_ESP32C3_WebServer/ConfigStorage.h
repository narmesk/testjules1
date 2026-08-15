#ifndef CONFIG_STORAGE_H
#define CONFIG_STORAGE_H

#include "Config.h"

String getvfrom_espsettings(String key1);
int FileWriteF(String txt, String path);
String WriteLedConfig(const String proct);
String LedPrepareResult(String type = "res");
String sendhttp(String httpRequestData, String httpType, String serverPath);
String checkncfj();
void receivepacketsend();
void esprestart();
bool LoadStockForHardware();

#endif

#include "Logger.h"
#include "ConfigStorage.h"  // <-- Bu satırı ekledik

void serverlog(String str, String mode) {
    if (getvfrom_espsettings("log") == "C" || getvfrom_espsettings("log") == "") {
        if (mode == "ln")
            Serial.println(str);
        else
            Serial.print(str);
    }
}

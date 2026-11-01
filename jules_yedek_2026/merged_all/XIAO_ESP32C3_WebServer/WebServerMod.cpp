#include "WebServerMod.h"
#include "Logger.h"
#include "ConfigStorage.h"
#include "ModbusHandler.h"

void initserver() {
    // 🌐 Web Sayfası Ana Yolları (LittleFS)
    // NOT: HTML dosyalari template processor OLMADAN gonderilir. Processor kullanilirsa
    // ESPAsyncWebServer dosyadaki "%...%" araliklarini placeholder sanip siler ve
    // CSS'teki "width: 100%; height: 100%" gibi ifadeler bozulur.
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /", "ln");
        if (LittleFS.exists("/index.html"))
            request->send(LittleFS, "/index.html", "text/html");
        else
            request->send(404, "text/plain", "Hata: /index.html LittleFS icinde bulunamadi! Lutfen Data Upload yaptiginizdan emin olun.");
    });

    server.on("/parameters", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /parameters", "ln");
        if (LittleFS.exists("/parameters.html"))
            request->send(LittleFS, "/parameters.html", "text/html");
        else
            request->send(404, "text/plain", "Hata: /parameters.html bulunamadi.");
    });

    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /settings", "ln");
        if (LittleFS.exists("/settings.html"))
            request->send(LittleFS, "/settings.html", "text/html");
        else
            request->send(404, "text/plain", "Hata: /settings.html bulunamadi.");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /style.css", "ln");
        if (LittleFS.exists("/style.css"))
            request->send(LittleFS, "/style.css", "text/css");
        else
            request->send(404, "text/plain", "Hata: /style.css bulunamadi.");
    });

    server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /js.js", "ln");
        if (LittleFS.exists("/js.js"))
            request->send(LittleFS, "/js.js", "text/javascript");
        else
            request->send(404, "text/plain", "Hata: /js.js bulunamadi.");
    });

    server.on("/getledconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /getledconfig", "ln");
        if (LittleFS.exists("/ledconfig.json"))
            request->send(LittleFS, "/ledconfig.json", "application/json");
        else
            request->send(404, "text/plain", "Hata: /ledconfig.json bulunamadi.");
    });

    server.on("/getstock", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /getstock", "ln");
        if (LittleFS.exists("/stock.json"))
            request->send(LittleFS, "/stock.json", "application/json");
        else
            request->send(404, "text/plain", "Hata: /stock.json bulunamadi.");
    });

    // 📩 API & Form Endpoints
    server.on("/sendparam", HTTP_POST, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_POST /sendparam", "ln");
        if (request->hasParam(PARAM_params[0], true)) {
            for (int i = 0; i < 48; i++) {
                parameters[i] = request->getParam(PARAM_params[i], true)->value();
                serverlog(parameters[i], "");
            }
            serverlog("", "ln");
            change_source = 'a';
            WriteLedConfig(String("m"));
        }
        request->send(200, "text/plain", "{\"res\":\"paramok\"}");
    });

    server.on("/sendsett", HTTP_POST, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_POST /sendsett", "ln");
        int paramsNr = request->params();
        if (paramsNr > 0) {
            int z = 0;
            for (int i = 0; i < paramsNr; i++) {
                const AsyncWebParameter* p = request->getParam(i);
                serverlog(p->name(), "");
                serverlog(":", "");
                serverlog(p->value(), "");
                serverlog("  ", "");

                if (p->name() == "sid") {
                    espsettings_sid = p->value();
                } else if (p->name() != "gstationid") {
                    espsettings[z].key = p->name();
                    espsettings[z].value = p->value();
                    z += 1;
                }
            }
            serverlog("", "ln");
            change_source = 'a';
            WriteLedConfig(String("s"));
            request->send(200, "text/plain", "{\"res\":\"paramok\"}");
            esprestart();
        } else {
            request->send(200, "text/plain", "{\"res\":\"0\"}");
        }
    });

    server.on("/sendstock", HTTP_POST, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_POST /sendstock", "ln");
        File file = LittleFS.open("/stock.json", "r");
        if (!file) {
            request->send(500, "text/plain", "{\"res\":\"err\"}");
            return;
        }

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        if (error) {
            serverlog("stock.json deserializeJson() failed: ", "");
            serverlog(error.f_str(), "ln");
            request->send(500, "text/plain", "{\"res\":\"err\"}");
            return;
        }

        int paramsNr = request->params();
        for (int i = 0; i < paramsNr; i++) {
            const AsyncWebParameter* p = request->getParam(i);
            doc[p->name()] = p->value();
        }

        String stockjsonstr = "";
        serializeJson(doc, stockjsonstr);

        File wfile = LittleFS.open("/stock.json", "w");
        if (wfile) {
            wfile.print(stockjsonstr);
            wfile.close();
        }

        request->send(200, "text/plain", "{\"res\":\"paramok\"}");
    });

    // NOT: Fiyatlar ve PT hedef listesi artik POST body'den degil, dogrudan stock.json'dan
    // okunuyor (LoadStockForHardware). Boylece donanima gonderilen veri her zaman
    // Settings/Price sayfalarinda en son kaydedilmis stock.json ile birebir eslesir.
    server.on("/sendserial", HTTP_POST, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_POST /sendserial", "ln");
        if (LoadStockForHardware()) {
            WriteLedConfig(String("p"));
            if (is_modbus_hmi == 'h')
                SendSerialProcStart();
            else if (is_modbus_hmi == 'm')
                modbustask = 's'; // Burası asenkron durum makinesini tetikler

            change_source = 'a';
            request->send(200, "text/plain", "{\"res\":\"ok\"}");
        } else {
            request->send(500, "text/plain", "{\"res\":\"err\"}");
        }
    });

    server.on("/checkserial", HTTP_GET, [](AsyncWebServerRequest *request) {
        serverlog("HTTP_GET /checkserial", "");
        String ledmessage = LedPrepareResult();
        serverlog(ledmessage, "ln");
        request->send(200, "text/plain", ledmessage);
    });

    // 📊 DURUM ÇUBUĞU İÇİN YENİ EKLENEN API ENDPOINT (GET)
    // Web arayüzündeki durum çubuğu burayı sorgulayarak canlı ilerlemeyi çeker.
    server.on("/modbusstatus", HTTP_GET, [](AsyncWebServerRequest *request) {
        extern char modbustask;
        extern uint8_t IDsPointer;
        extern String pt_quantity;
        extern char modbusPTres[];

        // Eğer henüz parametre atanmadıysa veya miktar boşsa güvenli değer belirle
        String total_dev = (pt_quantity.length() > 0) ? pt_quantity : "0";

        // Son sorgulanan cihazın durumunu dizi sınırları dahilinde yakalıyoruz
        char current_dev_res = '0';
        if (IDsPointer > 0 && IDsPointer <= 24) {
            current_dev_res = modbusPTres[IDsPointer - 1];
        }

        // Web sayfasına basılacak anlık JSON durum metni
        String statusJson = "{";
        statusJson += "\"task\":\"" + String(modbustask) + "\",";          // 's': Başlıyor, 'f': Çalışıyor, 'x': Bitti
        statusJson += "\"current\":" + String(IDsPointer) + ",";          // O anki/son sorgulanan cihaz indeksi
        statusJson += "\"total\":" + total_dev + ",";                     // Toplam sorgulanacak cihaz sayısı
        statusJson += "\"last_res\":\"" + String(current_dev_res) + "\""; // Son cihazın sonucu ('1': Başarılı, '0': Yanıt yok)
        statusJson += "}";

        request->send(200, "application/json", statusJson);
    });

    // 404 Handler
    server.onNotFound([](AsyncWebServerRequest *request) {
        String message = "Hata: Sayfa Bulunamadi (404)\n\n";
        message += "URL: " + request->url() + "\n";
        message += "Method: " + String((request->method() == HTTP_GET) ? "GET" : "POST") + "\n";
        request->send(404, "text/plain", message);
    });

    server.begin();
}

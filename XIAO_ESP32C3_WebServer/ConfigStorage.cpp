#include "ConfigStorage.h"
#include "Logger.h"

void esprestart() {
    serverlog("Restarting in 2 seconds", "ln");
    delay(2000);
    ESP.restart();
}

String getvfrom_espsettings(String key1) {
    for (int x = 0; x < sizeof(espsettings) / sizeof(espsettings[0]); x++) {
        if (key1 == espsettings[x].key) {
            return espsettings[x].value;
        }
    }
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

String LedPrepareResult(String type) {
    if (is_modbus_hmi == 'm') {
        // "Tamamlandi" (O) durumu artik sabit PUMP PIECE uzunluguna gore degil,
        // stock.json'da tanimli (pt_slave_ids[i] != 0) her PT'nin gercekten '1'
        // (basarili) olup olmadigina bakilarak belirlenir. Tanimsiz slotlar
        // (henuz denenmedigi icin '0' kalanlar) kontrole dahil edilmez.
        receivepacket = "";
        bool all_active_done = true;
        for (int i = 0; i < 24; i++) {
            receivepacket += modbusPTres[i];
            if (pt_slave_ids[i] != 0 && modbusPTres[i] != '1') all_active_done = false;
        }

        receivepacket += all_active_done ? 'O' : 'N';
    }

    String receivepacketfilter = receivepacket;
    receivepacketfilter.trim();
    receivepacketfilter = receivepacketfilter.substring(receivepacketfilter.length() - 25);
    serverlog("filtered:", "ln");
    serverlog(receivepacketfilter, "");

    String msg = "{\"res\":\"" + receivepacketfilter + "\"}";

    if (type == "res")
        return msg;
    else if (type == "trim")
        return receivepacketfilter;

    return msg;
}

String WriteLedConfig(const String proct) {
    File file = LittleFS.open("/ledconfig.json", "r");
    if (!file) {
        serverlog("Failed to open ledconfig.json for reading", "ln");
        return "";
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        serverlog("deserializeJson() failed: ", "");
        serverlog(error.f_str(), "ln");
        return "";
    }

    const char* gstationidc = doc["sid"] | "";
    sid = gstationidc;
    serverName_poolp = serverName_pool + sid + ".json";

    const char* pidc = doc["pid"] | "";
    pid = pidc;
    const char* ptypec = doc["ptype"] | "";
    ptype = ptypec;
    const char* pmasterc = doc["pmaster"] | "";
    pmaster = pmasterc;

    if (proct == "p") {
        doc["ptype"] = "pr";
        for (int i = 0; i < 24; i++) {
            doc["leds"][i]["v"] = price_arr[i];
        }
    } else if (proct == "m") {
        doc["ptype"] = "pm";
        for (int z = 0; z < 48; z++) {
            doc["parameters"][z]["v"] = parameters[z];
        }
    } else if (proct == "s") {
        doc["ptype"] = "st";
        doc["sid"] = espsettings_sid;
        for (int x = 0; x <= 11; x++) {
            if (espsettings[x].key != "")
                doc["esp32sett"][espsettings[x].key] = espsettings[x].value;
        }
    }

    if (proct == "p" || proct == "m" || proct == "s") {
        doc["pdate"] = "";
        doc["pmaster"] = "ln";
        doc["pid"] = random(11111111, 99999999);
    }

    String ledconfigstr = "";
    serializeJson(doc, ledconfigstr);

    const char* vtmp = "";
    for (int i = 0; i < 48; i++) {
        vtmp = doc["parameters"][i]["v"];
        parameters[i] = vtmp;
    }

    int x = 0;
    JsonObject root = doc["esp32sett"].as<JsonObject>();
    for (auto kv : root) {
        espsettings[x].key = kv.key().c_str();
        espsettings[x].value = kv.value().as<String>();
        x += 1;
        if (x >= 12) break;
    }
    devprod = getvfrom_espsettings("devprod");

    if (proct == "x") {
        const char* vtmp1 = "";
        for (int i = 0; i < 24; i++) {
            vtmp1 = doc["leds"][i]["v"] | "0";
            price_arr[i] = String(vtmp1);
        }
    }

    if (proct != "x") {
        FileWriteF(ledconfigstr, "/ledconfig.json");
        String serverName_process2 = serverName_process + "?savecfj=t&sid=" + sid;
        sendhttp(ledconfigstr, "httpjson", serverName_process2);
    }

    return "";
}

// stock.json'u kaynak alarak donanima gonderilecek fiyatlari (Full Service, ilk 8 urun,
// CASH/CREDIT/DEBIT) ve hedef PT listesini (pttype/ptid dolu olan slotlar) hazirlar.
// ptid degeri artik gercek Modbus slave ID olarak kullanilir; eski FIRST PUMP ID/PUMP PIECE
// parametrelerinin yerini alir.
bool LoadStockForHardware() {
    File file = LittleFS.open("/stock.json", "r");
    if (!file) {
        serverlog("LoadStockForHardware: stock.json acilamadi", "ln");
        return false;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        serverlog("LoadStockForHardware: deserializeJson() failed: ", "");
        serverlog(error.f_str(), "ln");
        return false;
    }

    for (int i = 0; i < 8; i++) {
        String cash = doc["stockcashprice" + String(i + 1)] | "0.00";
        String credit = doc["stockcreditprice" + String(i + 1)] | "0.00";
        String debit = doc["stockdebitprice" + String(i + 1)] | "0.00";
        price_arr[i * 3 + 0] = cash;
        price_arr[i * 3 + 1] = credit;
        price_arr[i * 3 + 2] = debit;
    }

    // pt_slave_ids[i], stock.json'daki PT(i+1) slotuna karsilik gelir (sikistirilmiyor);
    // boylece checkserial/badge indeksleri (ledres{n}) ile birebir hizali kalir.
    // 0 = bu slotta tanimli PT yok, Modbus dongusunde atlanir.
    pt_target_count = 0;
    for (int i = 0; i < 24; i++) {
        String ptId = doc["ptid" + String(i + 1)] | "";
        if (ptId == "") {
            pt_slave_ids[i] = 0;
        } else {
            pt_slave_ids[i] = ptId.toInt();
            pt_target_count++;
        }
    }
    pt_quantity = String(pt_target_count);

    serverlog("LoadStockForHardware: " + String(pt_target_count) + " PT hedeflendi", "ln");
    return true;
}

String sendhttp(String httpRequestData, String httpType, String serverPath) {
    if (serverPath.indexOf("example.com") != -1 || serverPath == "") {
        serverlog("sendhttp skipped (example.com / empty URL)", "ln");
        return "";
    }

    if (WiFi.status() == WL_CONNECTED && mode_apsta == "apsta") {
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
    }
    return "";
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

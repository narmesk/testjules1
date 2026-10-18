/*
  ================================================================================
  RemoteServer_Module.ino - Uzak Sunucu (Ledobe Cloud) İletişim Modülü
  ================================================================================
  Bu modül, ESP32 web sunucusu ile uzak bulut sunucu (ledobe.com) arasındaki
  tüm HTTP iletişimini ve veri senkronizasyonunu yönetir.

  Temel İşlevler:
  1. sendhttp(): HTTP GET/POST (JSON ve Form) isteklerini gönderir.
  2. pollServer(): Sunucu havuzunu (ppool) periyodik olarak sorgular (Poll).
  3. checkncfj(): İndirilen konfigürasyon JSON paketinin geçerliliğini doğrular.
  4. receivepacketsend(): Cihaz çalışma / Modbus yanıt verilerini sunucuya iletir.
  5. uploadConfigToServer(): Yerel web arayüzünden yapılan değişiklikleri sunucuya gönderir.
  ================================================================================
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// --- Sunucu URL Tanımlamaları ---
String serverName         = "https://www.ledobe.com/app/";
String serverName_process = serverName + "process.php";
String serverName_pool    = serverName + "cfg/ppool/";
String serverName_ledcfg  = serverName + "cfg/ledcfg/";
String serverName_pres    = serverName + "cfg/pres/";
String serverName_poolp   = ""; // Örn: https://www.ledobe.com/app/cfg/ppool/{sid}.json

// --- Sistem Durum Değişkenleri ---
extern String sid;                   // Station ID / Cihaz Benzersiz Kimliği
extern String pid;                   // Price ID / Tabela ID
extern String pmaster;               // Master Modu ("sv" = Server Master)
extern String mode_apsta;            // Wi-Fi Modu ("apsta" veya "ap")
extern String receivepacket_before;  // Son gönderilen yanıt paketi
extern String cfjres;                // Sunucudan indirilen CFJ metni
extern String poolres;               // Pool sorgu yanıtı ("1" veya "0")
extern String ptype;                 // İletişim Tipi ("pr" = Fiyat değişikliği)
extern char change_source;           // Değişiklik kaynağı ('s' = server)
extern char modbustask;              // Modbus görev bayrağı ('s' = server start)

extern unsigned long cTime_serverpool;
extern unsigned long intv_serverpool; // Sorgulama aralığı (örn. 5000ms)

// Yardımcı Fonksiyon Deklarasyonları (Main projeden çağrılan)
extern void serverlog(String msg, String opt);
extern void FileWriteF(String content, String filepath);
extern void WriteLedConfig(String opt);
extern String LedPrepareResult(String mode);

/**
 * @brief Genel HTTP İstemci Fonksiyonu
 * @param httpRequestData POST yapılcak gövde verisi (JSON veya Form)
 * @param httpType "httpget", "httppost" veya "httpjson"
 * @param serverPath İstek atılacak tam URL adresi
 * @return String Sunucudan dönen yanıt metni (payload)
 */
String sendhttp(String httpRequestData, String httpType, String serverPath) {
  if (WiFi.status() == WL_CONNECTED && mode_apsta == "apsta") {
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
      serverlog("HTTP Response code: ", "");
      serverlog(String(httpResponseCode), "ln");
      payload = http.getString();
      serverlog(payload, "ln");
    } else {
      serverlog("Error code: ", "");
      serverlog(String(httpResponseCode), "ln");
    }

    http.end();
    return payload;
  } else {
    return "";
  }
}

/**
 * @brief Sunucudan indirilen CFJ konfigürasyon verisinin geçerliliğini kontrol eder.
 * @return String "1" ise geçerli CFJ, "0" ise geçersiz.
 */
String checkncfj() {
  if (cfjres.substring(0, 8) == "{\"pdate\"") {
    serverlog("cfj download success", "ln");
    return "1";
  } else {
    serverlog("cfj download fail", "ln");
    return "0";
  }
}

/**
 * @brief Yerel panoda yapılan Modbus/Çalışma sonucunu sunucuya raporlar.
 * URL: process.php?procres={paket_ozeti}&pid={pid}
 */
void receivepacketsend() {
  String receivepacket_trim = LedPrepareResult("trim");
  String receivepacket_res  = LedPrepareResult("res");

  serverlog("receivepacket:" + receivepacket_trim + "  receivepacket_before:" + receivepacket_before, "ln");

  if (pmaster == "sv" && receivepacket_before != receivepacket_trim) {
    cfjres = "";
    String serverName_process1 = serverName_process + "?procres=" + receivepacket_trim + "&pid=" + pid;
    cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process1);
    receivepacket_before = receivepacket_trim;
    serverlog("receivepacketsend success", "ln");
  }
}

/**
 * @brief Yerel Arayüzden Ayar/Fiyat Değişince Sunucuya Konfigürasyonu Yükler
 * URL: process.php?savecfj=t&sid={sid}
 * @param ledconfigstr Yerel /ledconfig.json dosya içeriği
 */
void uploadConfigToServer(String ledconfigstr) {
  String serverName_process2 = serverName_process + "?savecfj=t&sid=" + sid;
  sendhttp(ledconfigstr, "httpjson", serverName_process2);
}

/**
 * @brief Uzak Sunucu Sorgulama Döngüsü (Polling Engine)
 * Main loop içerisinden zamanlayıcı (intv_serverpool) ile çağrılır.
 */
void pollServer() {
  if ((millis() - cTime_serverpool >= intv_serverpool)) {
    cTime_serverpool = millis();

    if (WiFi.status() == WL_CONNECTED) {
      serverlog("WiFi is up, Pooling Server...", "ln");

      // 1. Adım: Cihaza ait havuz durumunu kontrol et (Örn: ppool/{sid}.json)
      serverName_poolp = serverName_pool + sid + ".json";
      poolres = sendhttp("", "httpget", serverName_poolp);

      // 2. Adım: Sunucuda yeni güncellenmiş konfigürasyon/fiyat var mı? ("1")
      if (poolres == "1") {
        cfjres = "";
        String serverName_process1 = serverName_process + "?getcfj=t&sid=" + sid + "&pstart=t";
        cfjres = sendhttp("", "httpget", serverName_process1);

        serverlog("cfj:", "ln");
        serverlog(cfjres, "ln");

        // 3. Adım: İndirilen JSON geçerli mi?
        if (cfjres != "" && checkncfj() == "1") {
          // CFJ dosyasını yerel LittleFS belleğe kaydet
          FileWriteF(cfjres, "/ledconfig.json");

          change_source = 's'; // Değişiklik kaynağı: Server
          WriteLedConfig("x");

          if (ptype == "pr") { // Fiyat güncelleme süreci
            receivepacket_before = "000000000000000000000000N";
            modbustask = 's'; // Modbus güncellemeyi başlat
          } else { // Parametre güncelleme onay süreci
            String serverName_process_ok = serverName_process + "?procres=t&pid=" + pid;
            String receivepacket_res = "{\"res\":\"paramok\"}";
            cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process_ok);
          }
        }
      }
    } else {
      serverlog("WiFi Disconnected while pooling", "ln");
    }
  }
}

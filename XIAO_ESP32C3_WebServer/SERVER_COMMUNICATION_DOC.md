# Uzak Sunucu (ledobe.com Cloud) İletişim ve Senkronizasyon Mimarisi

Bu doküman, ESP32 (XIAO ESP32-C3) gömülü web sunucusu yazılımının uzak bulut sunucusu (`https://ledobe.com`) ile gerçekleştirdiği iletişim mimarisini, veri akışını, HTTP/HTTPS protokollerini, API uç noktalarını (endpoints) ve cihaz-sunucu senkronizasyonunu adım adım detaylandırmaktadır.

---

## 1. Sunucu Bağlantı Yapılandırması ve Parametreler

Yazılım içerisinde tanımlı temel bulut sunucu adresleri şu şekildedir:

```cpp
String serverName         = "https://ledobe.com";
String serverName_process = serverName + "/app/process.php"; // Ana işlem, kayıt ve onay adresi
String serverName_pool    = serverName + "/app/cfg/ppool/";   // Cihaz sorgu ve bayrak (pool) klasörü
String serverName_ledcfg  = serverName + "/app/cfg/ledcfg/";  // Konfigürasyon JSON klasörü
String serverName_pres    = serverName + "/app/cfg/pres/";    // İşlem sonuç klasörü
String serverName_poolp   = "";                              // Örn: https://ledobe.com/app/cfg/ppool/{sid}.json
```

### Kritik Sistem Değişkenleri:
- **`sid` (Station ID):** İstasyon / Cihaza ait benzersiz kimlik dizisi (Örn: `"0zUgGe9oLvYsT3Hv"`).
- **`pid` (Price/Process ID):** Her bir fiyat/parametre güncelleme işlemine özel rastgele üretilen benzersiz işlem numarası.
- **`pmaster` (Master Modu):** `"sv"` (Server Master) ise cihaz öncelikli olarak `ledobe.com` sunucusundan gelen komut ve güncellemeleri dinler.
- **`ptype` (Paket Tipi):**
  - `"pr"`: Fiyat güncelleme paketi (Price change).
  - `"pm"`: Sistem parametresi güncelleme paketi (Parameter change).
  - `"st"`: Cihaz Wi-Fi / Sistem ayarları güncelleme paketi (Settings change).

---

## 2. HTTPS / HTTP İletişim Katmanı (`sendhttp`)

ESP32, sunucu ile olan tüm iletişimini `WiFiClientSecure` ve `HTTPClient` kütüphanelerini kullanarak SSL/TLS destekli HTTPS üzerinden gerçekleştirir.

```cpp
String sendhttp(String httpRequestData, String httpType, String serverPath)
```

### Desteklenen HTTP Metodları ve Formatlar:
1. **`httpget`:** Sunucudan havuz durumunu veya konfigürasyon JSON dosyasını çekmek için kullanılır (`HTTP GET`).
2. **`httppost`:** Form verisi göndermek için `application/x-www-form-urlencoded` başlığı ile istek iletir.
3. **`httpjson`:** `application/json` başlığı (`Content-Type: application/json`) ekleyerek JSON formatındaki veriyi (`httpRequestData`) sunucuya `HTTP POST` metodu ile iletir.

---

## 3. Uzak Sunucu Senkronizasyon Akışı (Adım Adım Detaylı İşleyiş)

ESP32 ile `ledobe.com` arasındaki veri iletişimi 4 ana adımdan oluşan bir döngü şeklinde çalışır:

```
+------------------+         (1) HTTP GET Pool Sorgusu          +-------------------+
|                  | -----------------------------------------> |                   |
|                  | <----------------------------------------- |                   |
|                  |            "1" (Güncelleme Var)            |                   |
|                  |                                            |                   |
|                  |         (2) HTTP GET Configuration         |                   |
|   ESP32 Cihazı   | -----------------------------------------> |    ledobe.com     |
|   (XIAO ESP32C3) | <----------------------------------------- |   Cloud Server    |
|                  |            CFJ (JSON Verisi)               |                   |
|                  |                                            |                   |
|                  |      (3) HTTP POST Result (procres)        |                   |
|                  | -----------------------------------------> |                   |
|                  | <----------------------------------------- |                   |
+------------------+                    OK                      +-------------------+
```

---

### Adım 1: Periyodik Sunucu Sorgulama (Polling - `intv_serverpool`)
- `loop()` fonksiyonu içerisinde her **60 saniyede bir** (`intv_serverpool = 60000 ms`) sunucu kontrol edilir.
- Cihaza özel sorgu adresi dinamik olarak oluşturulur:
  `serverName_poolp = https://ledobe.com/app/cfg/ppool/{sid}.json`
- `sendhttp("", "httpget", serverName_poolp)` çağrısı yapılarak sunucuya `HTTP GET` isteği gönderilir.
- **Yanıt Değerlendirmesi:**
  - Sunucudan dönen yanıt **`"1"`** ise: Bulut panelde bu cihaz için yeni bir fiyat veya ayar değişikliği yapılmıştır.
  - Yanıt **`"0"`** veya boş ise: Yeni bir güncelleme yoktur, polling döngüsü bir sonraki periyoda kadar beklemeye geçer.

---

### Adım 2: Güncel Konfigürasyonun İndirilmesi (`getcfj`)
- Sunucudan `"1"` yanıtı alındığında ESP32, güncel konfigürasyon paketi olan CFJ dosyasını indirmek için istek atar:
  ```http
  GET /app/process.php?getcfj=t&sid={sid}&pstart=t HTTP/1.1
  Host: ledobe.com
  ```
- Sunucu, cihaza ait tüm fiyat, led parlaklık/hız ve sistem parametrelerini içeren JSON verisini döndürür.

---

### Adım 3: JSON Doğrulama, Kaydetme ve Yerel Güncelleme (`checkncfj` & `LittleFS`)
- İndirilen `cfjres` yanıtı `checkncfj()` fonksiyonu ile doğrulanır:
  - Yanıtın ilk 8 karakteri `{"pdate"` ile başlıyorsa JSON paketinin geçerli olduğu onaylanır.
- Geçerli JSON metni `LittleFS` dosya sistemine `/ledconfig.json` adıyla yazılır (`FileWriteF`).
- `change_source = 's'` (Değişiklik kaynağı: Sunucu) bayrağı set edilir.
- `WriteLedConfig("x")` fonksiyonu çağrılarak hafızadaki `price_arr` ve `parameters` dizileri güncellenir.

---

### Adım 4: Donanım/Ekran Güncellemesi ve Sonuç Raporlama (`procres`)

#### A) Fiyat Güncellemesi Durumu (`ptype == "pr"`)
1. Cihaz, `receivepacket_before = "000000000000000000000000N"` sıfırlamasını yapar ve Modbus haberleşmesini başlatmak için `modbustask = 's'` komutunu verir.
2. Cihaz RS232/RS485 hattı üzerinden bağlı tabela kartlarına yeni fiyatları iletir.
3. Tabela kartlarından yanıtlar toplanıp başarı paketi (`receivepacket_trim`, örn: `111111111111111111111111O`) oluşturulduğunda `receivepacketsend()` tetiklenir.
4. Sunucuya işlemin başarı sonucu `HTTP POST` ile iletilir:
   ```http
   POST /app/process.php?procres={receivepacket_trim}&pid={pid} HTTP/1.1
   Host: ledobe.com
   Content-Type: application/json

   {"res": "111111111111111111111111O"}
   ```

#### B) Sistem Parametre Güncellemesi Durumu (`ptype != "pr"`)
1. Eğer indirilen veri sadece parametre veya ayar değişikliği ise, ekrana fiyat yazma süreci atlanır.
2. Sunucuya doğrudan parametre onay yanıtı gönderilir:
   ```http
   POST /app/process.php?procres=t&pid={pid} HTTP/1.1
   Host: ledobe.com
   Content-Type: application/json

   {"res": "paramok"}
   ```

---

## 4. Yerel Web Arayüzünden Sunucuya Veri Yükleme (`savecfj`)

Kullanıcı `ledobe.com` paneli yerine, ESP32'nin kendi ürettiği Wi-Fi Access Point (Örn: `Ledovate-Price-Change`) web arayüzünden (`/sendserial`, `/sendparam`, `/sendsett`) bir değişiklik yaparsa:

1. Yerel `/ledconfig.json` dosyası yeni değerlerle güncellenir.
2. `WriteLedConfig()` fonksiyonu içerisinden `savecfj` isteği tetiklenir:
   ```http
   POST /app/process.php?savecfj=t&sid={sid} HTTP/1.1
   Host: ledobe.com
   Content-Type: application/json

   { ... guncel_ledconfig_json_icerigi ... }
   ```
3. Böylece yerel web arayüzünden yapılan tüm fiyat/ayar değişiklikleri anında `ledobe.com` bulut veritabanı ile senkronize edilir.

---

## 5. Donanım Haberleşmesi (Modbus RS232 / RS485 - Yüzeysel Özet)

ESP32, `ledobe.com` sunucusundan aldığı fiyat bilgilerini panolara iletmek için **Modbus RTU (RS232/RS485)** donanım hattını (`Serial1`, `RXD2: GPIO3`, `TXD2: GPIO2`) kullanır.

- **Paket Yapısı:** 58 Baytlık özel Modbus RTU paketleri oluşturulur (`ModbusTaskSend`).
- **Fonksiyon Kodu:** `16` (0x10 - Preset Multiple Registers).
- **Hata Kontrolü:** `MODBUS_CRC16_v1` algoritması ile CRC hesaplanır.
- **İşlem Tamamlama:** Bağlı tüm panolardan (`pt_quantity`) doğru yanıt alındığında `modbusPTres` dizisi `'1'` ile doldurulur ve sonuç `ledobe.com` sunucusuna raporlanır.

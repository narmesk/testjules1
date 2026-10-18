# Uzak Sunucu (Ledobe Cloud) İletişim ve Senkronizasyon Mimarisi

Bu doküman, ESP32 web sunucusu yazılımının uzak bulut sunucusu (`https://www.ledobe.com/app/`) ile gerçekleştirdiği iletişim mimarisini, veri akışını, endpoint yapılarını ve adım adım işleyişini detaylandırmaktadır.

---

## 1. Temel Sunucu Bağlantı Noktaları (Endpoints) ve Değişkenler

Sistemde tanımlı uzak sunucu adresleri şu şekildedir:

```cpp
String serverName         = "https://www.ledobe.com/app/";
String serverName_process = serverName + "process.php"; // İşlem ve onay adresi
String serverName_pool    = serverName + "cfg/ppool/";   // Cihaz bayrak/sorgu havuzu
String serverName_ledcfg  = serverName + "cfg/ledcfg/";  // Konfigürasyon klasörü
String serverName_pres    = serverName + "cfg/pres/";    // Sonuç/Yanıt klasörü
String serverName_poolp   = "";                          // Cihaza özel pool JSON adresi
```

### Parametre Açıklamaları:
- **`sid` (Station ID / Cihaz ID):** İstasyonun/cihazın benzersiz kimliği (Örn: `"1001"`).
- **`pid` (Price/Panel ID):** Tabela / Ekran kontrol kartı numarası.
- **`pmaster` (Master Modu):** `"sv"` ise cihaz sunucudan gelen komutlara öncelik verir (Server Master).

---

## 2. HTTP İletişim Altyapısı (`sendhttp`)

Tüm HTTP istekleri, ESP32 `HTTPClient` kütüphanesi kullanılarak `sendhttp()` yardımcı fonksiyonu üzerinden iletilir.

```cpp
String sendhttp(String httpRequestData, String httpType, String serverPath)
```

### İstek Tipleri:
1. **`httpget`:** Sunucudan durum/sorgu verisi çekmek için kullanılır.
2. **`httppost`:** `application/x-www-form-urlencoded` formatında veri gönderir.
3. **`httpjson`:** `application/json` başlığı (`Content-Type: application/json`) ile JSON formatındaki payload verisini sunucuya iletir.

---

## 3. Uzak Sunucu İletişim Akışı (Adım Adım İşleyiş)

Sistemdeki uzak sunucu senkronizasyonu 4 ana aşamadan oluşur:

### Adım 1: Periyodik Sorgulama (Polling - `pollServer`)
- ESP32, `cTime_serverpool` ve `intv_serverpool` (örn. 5 saniyede bir) zamanlayıcısını kullanarak sunucu sorgusu başlatır.
- `serverName_poolp` adresi oluşturulur: `https://www.ledobe.com/app/cfg/ppool/{sid}.json`
- `sendhttp("", "httpget", serverName_poolp)` fonksiyonu ile HTTP GET isteği atılır.
- **Yanıt Kontrolü:**
  - Dönen yanıt **`"1"`** ise: Sunucuda bu cihaz için **yeni bir fiyat veya parametre değişikliği** beklemektedir.
  - Dönen yanıt **`"0"`** veya boş ise: Güncelleme yoktur, döngü sonlanır.

---

### Adım 2: Yeni Konfigürasyonun İndirilmesi (`getcfj`)
- Sunucudan `"1"` yanıtı alındığında, cihaz güncel konfigürasyon dosyasını ister:
  ```
  URL: https://www.ledobe.com/app/process.php?getcfj=t&sid={sid}&pstart=t
  Method: HTTP GET
  ```
- Sunucu, cihaza ait güncel JSON konfigürasyon metnini (CFJ) döndürür.

---

### Adım 3: JSON Doğrulama ve Kaydetme (`checkncfj` & `FileWriteF`)
- İndirilen `cfjres` yanıtı `checkncfj()` fonksiyonu ile kontrol edilir:
  - Metin `{"pdate"` başlığı ile başlıyorsa veri **geçerlidir**.
- Geçerli JSON verisi, ESP32'nin yerel dosya sistemine (LittleFS) `/ledconfig.json` olarak kaydedilir.
- `change_source = 's'` (sunucudan değişiklik yapıldı) bayrağı set edilir ve yerel hafıza güncellenir.

---

### Adım 4: Donanım/Modbus Güncellemesi ve Sonuç Raporlama (`procres`)

#### A) Fiyat Güncellemesi (`ptype == "pr"`)
1. Cihaz Modbus hattı üzerinden bağlı LED tabela ekranlarına yeni fiyatları yazmak için `modbustask = 's'` komutunu devreye sokar.
2. Tabela güncellemeyi tamamlayıp yeni durum paketini hazırladığında `receivepacketsend()` çağrılır.
3. Sunucuya işlemin başarıyla tamamlandığı ve panonun son durumu gönderilir:
   ```
   URL: https://www.ledobe.com/app/process.php?procres={receivepacket_trim}&pid={pid}
   Method: HTTP POST (JSON Payload: receivepacket_res)
   ```

#### B) Sistem Parametre Güncellemesi (`ptype != "pr"`)
1. Sadece parametre/ayar değiştiyse, donanıma doğrudan bildirim yapılarak sunucuya parametre onay mesajı gönderilir:
   ```
   URL: https://www.ledobe.com/app/process.php?procres=t&pid={pid}
   Payload: {"res": "paramok"}
   ```

---

## 4. Cihaz Tarafından Sunucuya Konfigürasyon Yükleme (`savecfj`)

Eğer kullanıcı ayarları veya fiyatları sunucu üzerinden değil de **ESP32'nin kendi yerel Access Point web arayüzünden** değiştirirse:

1. Yerel `/ledconfig.json` dosyası güncellenir.
2. `uploadConfigToServer()` fonksiyonu tetiklenir:
   ```
   URL: https://www.ledobe.com/app/process.php?savecfj=t&sid={sid}
   Method: HTTP POST (JSON Payload: ledconfigstr)
   ```
3. Böylece yerel web arayüzünden yapılan tüm değişiklikler anında bulut sunucudaki merkezi veritabanına da senkronize edilmiş olur.

---

## 5. Özet Veri Akış Şeması

```
[ ESP32 Cihazı ]                                     [ Uzak Cloud Sunucu ]
       |                                                      |
       |--- HTTP GET: cfg/ppool/{sid}.json ------------------>|  (1. Poll Sorgusu)
       |<-- Response: "1" (Yeni Değişiklik Var) --------------|
       |                                                      |
       |--- HTTP GET: process.php?getcfj=t&sid={sid} -------->|  (2. Konfigürasyon İsteyi)
       |<-- Response: {"pdate":..., "prices":...} ------------|  (3. JSON Konfigürasyon)
       |                                                      |
       |[ Kaydet: /ledconfig.json & Modbus Güncelle ]          |
       |                                                      |
       |--- HTTP POST: process.php?procres={status}&pid={pid}->|  (4. Sonuç Onayı)
       |<-- Response: OK -------------------------------------|
```

# Jules - Konuşma, Hafıza ve Proje Kararları Tam Yedek Dokümanı

Bu belge, bu oturuma ve geçmiş geliştirme oturumlarına ait tüm kaydedilmiş teknik hafıza maddelerini, mimari kararları, konuşmalarda alınan kritik notları ve donanım/protokol kurallarını eksiksiz olarak içermektedir.

---

## 1. Genel İletişim ve Çalışma Tercihleri
- **Dil Politikası:** Tüm genel açıklamalar, mantık tartışmaları ve mimari raporlar Türkçe olarak yürütülür. Kod seviyesindeki tanımlayıcılar (değişkenler, fonksiyon adları vb.) ve Delphi UI arayüz dizgileri İngilizce tutulur.
- **Kod İletim Tercihi:** Kullanıcı kaynak dosyalarını parça parça, teker teker (önce dosya adını sonra içeriğini) vermeyi tercih eder; derleme işlemine acele edilmeden analiz yapılıp kaydedilmelidir.
- **Detaylı Raporlama:** Yapılan tüm kod değişikliklerinin ne olduğu ve neden yapıldığı ayrıntılı teknik raporlar halinde kullanıcıya sunulmalıdır.

---

## 2. dsPIC33EP Motion Control ve ASDA-A2 CANopen Kontrolü
- **ASDA-A2 CANopen Çözünürlüğü:** Delta ASDA-A2 servo sürücünün CANopen modundaki (0x0C) varsayılan çözünürlüğü tur başına **1,280,000 pals**'tir.
- **Profil Pozisyon (PP) Modu:**
  - Hareketi tetiklemek için `Controlword` bit 4 (**New Set-point**) yükselen kenar gerektirir.
  - El sıkışma (handshake) doğrulaması `Statusword` bit 12 (**Set-point acknowledge**) üzerinden zorunludur.
  - ASDA-A2 için sürekli pozisyon akışı (döngü başına +18 pals), toggling 'New Set-point' (Bit 4) ve 'Change set immediately' (Bit 5) kullanılarak sağlanır.
- **CSP (Cyclic Synchronous Position) Modu:**
  - Motor sıçramalarını ve arızaları (jump/fault) önlemek için, başlatmada sürücünün `axis_actual_pos` değeri ile yazılımın `axis_target_pos` değeri bloklayıcı SDO okuması (0x6064) ile mutlaka senkronize edilmelidir.
- **CANopen SDO Komut Baytları:**
  - `0x2F`: 1-bayt yazma
  - `0x2B`: 2-bayt yazma
  - `0x23`: 4-bayt yazma
  - `0x40`: Okuma isteği
  - `0x43`, `0x4B`, `0x4F`: Okuma yanıtları
- **CAN Bus ve DMA Mimarisi:**
  - dsPIC33EP ECAN uygulaması EDS-space DMA bellek tamponları (`can1msgBuf`) ve Peripheral Pin Select (PPS) eşlemesi gerektirir.
  - DMA modülünün başlatılması (`DMA_Initialize`) ve DMA Kanal 1 (TX) ile Kanal 0 (RX) olarak özel yapılandırılması şarttır.
  - CAN veriyolu taşmasını (bus flooding) ve analizör desenkronizasyonunu engellemek için `CAN1_Transmit` çağrılmadan önce `C1TR01CONbits.TXREQ0 == 0` kontrolü yapılmalıdır.
  - MCC tarafından üretilen CAN1 API'sinde tampon sorgulaması için `CAN1_ReceivedMessageCountGet()` kullanılmalı ve `CAN_MSG_OBJ` yapıları 8-bayt veri dizileriyle sıfırlanmalıdır.
- **dsPIC33 Kodlama Standartları ve Zamanlama:**
  - dsPIC33 C kodlarında okunabilirlik için ternary operatörü (`? :`) yerine **kesinlikle açık if/else blokları** tercih edilir.
  - dsPIC33 hareket motoru, Ethernet (UDP) yığınının kararlılığını sağlamak için kesinlikle bloklamayan **4 ms döngü süresine** uymalıdır. Ana döngüde `DelayMs` gibi bloklayıcı çağrılar yasaktır.
  - `Timer3ISR`, UART/UDP zaman aşımı işleyicisidir ve tek atımlı (one-shot) çalışarak kendini devre dışı bırakır. Bu kesme kesinlikle periyodik sistem nabzı (heartbeat) olarak kullanılamaz veya değiştirilemez.
  - Tek eksenli optimizasyon (`ACTIVE_AXIS_COUNT` veya bağımsız hareket), çok eksenli senkronizasyon ve hız ölçekleme yükünü kaldırarak CPU yükünü düşürür.
  - 12-bit sanal IO eşlemesi, `Aux0-Aux11` değişkenlerini PLC benzeri mantık için `DoutPort` bit alanı yapısına bağlar.
  - Hareket kontrol mimarisi Node ID 1-4 olan 4 ekseni durum makinesi ve PDO tabanlı 4 ms gerçek zamanlı döngüyle yönetir; alt pals hassasiyeti için 16.16 sabit nokta aritmetiği ve trapezoidal hız profili kullanır.

---

## 3. PIC18F46K42 Remote Controller (PIC18FREMOTE)
- **Saat ve Zamanlayıcı Yapılandırması:**
  - 44.2368 MHz FOSC (11.0592 MHz harici kristal + 4x PLL).
  - Timer0: 0.5 ms kesme.
  - Timer1: 45 ms zamanlama.
  - Pin Eşleşmeleri: RA6/RA7 harici kristal osilatör (ANSELA = 0xC0), UART1 RC6(TX)/RC7(RX), UART2 RB3(TX)/RB2(RX), I2C1 RC3(SCL)/RC4(SDA).
- **Ana Döngü Mimarisi:**
  - 500 µs zamanlayıcı kesme bayrağı (`update`) ve çift fazlı görev serpiştirme (`update_frame` 0/1).
  - Faz 1: Tuş takımı durum makinesi, şifre hesabı (`ActPassVal`) ve ekran bilgi görevleri (~15.625 FPS / 32 ms).
  - Faz 0: Bloklamayı önlemek için 8 sayfa ve 3 çip boyunca kesikli GLCD render işlemi (`GLCD_Picture_Discrete`).
- **GLCD ve Yazı Tipi (Font) Yönetimi:**
  - LMC19264A-01 Grafik LCD (192x64 çözünürlük, AIP31108 / KS0108 denetleyicileri). PORTD veri yolu, RC0-RC2 EN/RW/RS kontrolleri, RE0-RE2 çip seçiciler (CS3/CS2/CS1).
  - 1536 bayt MCU RAM gölge tamponu (`glcd_buffer[1536]`).
  - Yazı tipi tablolarında (`Arial_bold_14`, `Calibri36`) `FONT_WIDTH_TABLE`, 6 baytlık meta veri başlığıdır (`[size_h, size_l, default_width, default_height, first_char, char_count]`). Karakter genişlikleri ve başlangıç ofsetleri indekslenirken `FONT_WIDTH_TABLE` (+6) ofseti eklenmelidir.
  - Kısmi sayfa yazı tipi bit eşlem satırları tablolarda üst bitlere kaydırılmış olarak saklanır; eksik piksel boşluklarını önlemek için `GLCDWriteData` öncesinde yazı tipine özgü sağa kaydırma (`data >>= 4`, `data >>= 2`, `data >>= 1`) uygulanmalıdır.
  - Menü sistemi tek geçişli doğrudan render (`CreateMenu*`, `RefreshMenu*`) ile `glcd_buffer` üzerine çizilir ve ardından `GLCD_Render()` çağrılır (eski 64 adımlı sliced yapı kaldırılmıştır).
  - `remotecontroller.h` içerisinde `Menu_State_t` (`Menu_Creating = 0`, `Menu_Created = 1`, `Menu_Wait = 2`) tanımlıdır.
- **EEPROM ve POS / Modbus İletişimi:**
  - `DATAEE_ReadByte` ve `DATAEE_WriteByte` ile fiyat verileri (11-58), yapılandırma (101-148), slave ID sayıları (149-152), sistem parametreleri (201-242) ve POS/ürün takas tabloları saklanır.
  - UART2 (`pos.c`): Commander (15 bayt, STX 0x02, XOR sağlama) ve Gilbarco (9 bayt, 0x82/0x02) protokollerini ayrıştırır ve `PosCSwapTable` ile ekran tamponuna eşler.
  - `REMOTE_CONTROLLER_comApplication()`: POS terminal senkronizasyon bayraklarını (ACK/NACK/SYNC) yönetir ve Modbus fiyat/yapılandırma iletimi için durum makinesini (`SYS_STATE` 0-4) sürer.

---

## 4. Boru Bükme Makinesi (Borubukme)
- **Haberleşme:** Donanımla özel UDP protokolü üzerinden iletişim kurulur (128-bayt TX / 64-bayt RX, CRC16 doğrulamalı).
- **Sabitler:** Hareket kontrol komut sabitleri (`HOME_POS_GO`, `RESET_GO` vb.) `Glob.pas` biriminden alınmalıdır. `Borubukme.pas` içinde yerel yeniden tanımlama yapılmamalıdır.
- **Eksen Eşleşmeleri:**
  - Eksen A: Eksen Değiştirme (Dikey)
  - Eksen B: Boru Bükme
  - Eksen C: Boru Döndürme
  - Eksen D: Boru Sürme (Feeding)
  - JSON 'APozisyon' verisi pratikte donanımdaki 'DPozisyon'a karşılık gelir.
- **Donanım Çıkışları (`OutVal`):**
  - Bit 1: Dayama
  - Bit 2: Sıkma Sabit
  - Bit 3: Eksen Değiştirme / CValf
  - Bit 4: Sürme / AValf
  - Bit 5: Start Pulse
  - Bit 6: Stop Pulse
  - Bit 7: Initial Stop
- **Reçete Yapısı ve Güvenlik:**
  - Reçeteler JSON dizileri halinde tutulur, hassasiyet için x100 ölçekleme katsayısı kullanılır.
  - Donanım durum beklemelerinde `repeat...until Application.ProcessMessages` döngüleri re-entrancy ve UI kararsızlığı riski taşır, dikkatle yönetilmelidir.

---

## 5. Seeed Studio XIAO ESP32C3 Web Server & Ledobe Client
- **Dizin ve Kütüphane Yapısı:** Arduino IDE kuralları gereği proje `XIAO_ESP32C3_WebServer/XIAO_ESP32C3_WebServer.ino` altındadır.
- **Depolama:** SPIFFS yerine `LittleFS` kullanılır (`LittleFS.begin(true)`). Web dosyaları (HTML, CSS, JS) ve `ledconfig.json` `/data` dizinindedir.
- **Şablon İşleme Kuralı:** CSS içindeki `%` işaretlerinin kaybolmaması için HTML dosyaları `ESPAsyncWebServer` üzerinden şablon işlemcisi (template processor) olmadan düz metin olarak sunulmalıdır.
- **Donanım Seri Port (UART):** Modbus için `Serial1`, GPIO 20 (RX) ve GPIO 21 (TX) (D7 ve D6 pinleri) üzerine eşlenmiştir.
- **Bulut Senkronizasyonu (`ledobe.com`):**
  - `https://ledobe.com/app/` üzerinden `process.php` ve `cfg/ppool/` uç noktalarına HTTPS GET/POST istekleri (`WiFiClientSecure`).
  - Hatalı HTTP 405 yanıtlarını engellemek için `serverPath` boşsa veya `example.com` içeriyorsa giden HTTP istekleri atlanır (`ConfigStorage.cpp`).
- **ESP32_Ledobe_Client:** Modbus RTU, RS232/RS485 ve seri rutinleri temizlenmiş; yalnızca LittleFS web yönetimi ve HTTPS bulut eşitlemesi içeren hafif/bağımsız versiyondur.

---

## 6. Delphi Projeleri (Price Tuning Tool & PTsimulator)
- **Fiyat Ayar Aracı (Price Tuning Tool):**
  - 6 gridli VCL arayüzü (Base Prices, Tuning, Calculated Result - Full ve Self).
  - 1 saatlik deneme süresi güvenliği (`SecurityLogic.pas`): Uygulamanın ilk çalıştırılma zamanını `ico/license.ico` dosyasına ikili (binary) timestamp olarak yazar ve doğrular.
- **PTsimulator (Panel Simülatörü):**
  - dsPIC33EP LED panel seri haberleşmesini simüle eder.
  - 57 baytlık Modbus RTU özel çerçeve protokolü ile fiyat gösterge güncellemelerini test eder (`ModbusUtils.pas`, `PT_simulator.pas`).

---

## 7. dsPIC30F2010 Kayan Kapı Kontrolü
- `kayankapi.c` / `kayankapi.h`: PWM hız kontrolü, ADC akım algılama (sıkışma güvenliği), QEI enkoder konum takibi ve UART Modbus RTU haberleşmesini içeren kapı otomasyon mimarisi.

# Main Loop (Ana Döngü) Mimari ve Çalışma Analizi

Bu doküman, PIC18F46K42 mikrodenetleyicisi üzerinde çalışan uzaktan kumanda/kontrolör yazılımının ana döngü (`while(1)`) yapısını, zamanlama mekanizmalarını, görev paylaşımını ve ekran/tuş takımı yönetimini detaylandırmaktadır.

---

## 1. Zamanlama Mimari ve Frekans (500 µs Timer Interrupt)

Ana döngü, `update == 1` bayrağı (flag) üzerinden senkronize edilmektedir.
- **Timer Kesmesi:** Her 500 µs'de (0.5 ms / 2 kHz) çalışan Timer1 kesmesi `update` bayrağını `1` yapar.
- **Çevrim Süresi Senkronizasyonu:**
  - Döngü başında `READTIMER1()` ile zaman ölçümü başlatılır (`TestNum1`).
  - `CLRWDT()` ile Watchdog Timer (Bekçi Köpeği) beslenir, böylece sistemin kilitlenmesi önlenir.
  - İşlemler tamamlandıktan sonra `update = 0` yapılarak bir sonraki 500 µs kesmesi beklenir.
  - Döngü sonunda tekrar `READTIMER1()` ile geçen süre hesaplanır (`TestNum2`).

---

## 2. Görev Bölüştürme Mekanizması (`update_frame`)

500 µs periyotlu zamanlayıcı, iş yükünü hafifletmek için iki paralel zaman dilimine (`update_frame = 0` ve `update_frame = 1`) bölünmüştür:
- **Toplam Çerçeve Süresi:** $2 \times 500\text{ }\mu\text{s} = 1000\text{ }\mu\text{s} = 1\text{ ms}$.
- **Görsel Kare Hızı (Frame Rate):**
  - Ekran güncellemesi 64 döngüde bir tam tur atar ($64 \times 1\text{ ms} = 64\text{ ms}$).
  - $1 / 0.064\text{ s} \approx 15.625\text{ FPS}$ (Frame Per Second) tazeleme oranına ulaşılır.

---

## 3. `update_frame == 1` Diliminde Çalışan Görevler

### A. Genel Uygulama Taskı
- `REMOTE_CONTROLLER_runApplicaiton()`: Her 1 ms'de bir çalışarak sistemin temel mantık ve iletişim süreçlerini yürütür.

### B. Tuş Takımı İşleme ve Şifre / Değer Giriş Mantığı (`ActPassVal`)
1. **Tuş Basım Algılaması:**
   - `ButtonKeyPress == 0` durumunda yeni tuş basımı beklenir.
   - `ButtonKeyNum != 20` (20 = Tuş yok / No key) ise tuş basıldı kabul edilir (`ButtonKeyPress = 1`).
   - Tuş bırakıldığında (`ButtonKeyNum == 20`), `ButtonKeyPress = 0` yapılarak yeni basımlara izin verilir.

2. **Sayısal Tuşlar (0-9):**
   - Rakam girildiğinde imleç konumuna (`cursor_pos`) göre basamak değeri hesaplanır:
     - `cursor_pos == 3` -> `ActPassVal4 = ButtonKeyNum * 1000` (Binler basamağı) -> İmleç 2'ye geçer.
     - `cursor_pos == 2` -> `ActPassVal3 = ButtonKeyNum * 100` (Yüzler basamağı) -> İmleç 1'e geçer.
     - `cursor_pos == 1` -> `ActPassVal2 = ButtonKeyNum * 10` (Onlar basamağı) -> İmleç 0'a geçer.
     - `cursor_pos == 0` -> `ActPassVal1 = ButtonKeyNum` (Birler basamağı) -> İmleç 3'e sıfırlanır.
   - `ActPassVal = ActPassVal1 + ActPassVal2 + ActPassVal3 + ActPassVal4;` formülü ile 4 haneli toplam değer elde edilir.

3. **Özel Fonksiyon Tuşları:**
   - **`ButtonKeyNum == 15` (Temizle / Clear):** Tüm basamak değerlerini ve imleci sıfırlar (`cursor_pos = 3`, `ActPassVal = 0`).
   - **`ButtonKeyNum == 10` (Onayla / Enter):**
     - Girilen `ActPassVal == 9876` ise konfigürasyon menü mesaj indeksi yetkili moda ayarlanır (`config_menu_messageidx = 2`).
     - Yanlış şifrede ise normal mesaj moduna ayarlanır (`config_menu_messageidx = 1`).

### C. Ekran Bilgileri Güncelleme & İmleç Yanıp Sönme (Blink)
- `REMOTE_CONTROLLER_runInformation()`: Her 32 ms'de bir çalışarak ekran metin ve durum bilgilerini günceller (~17 µs sürer).
- **İmleç Yanıp Sönme Kontrolü:**
  - `update_cnt` 63'e ulaştığında (her 64 ms'de bir) sıfırlanır.
  - Veri giriş modunda (`entering_mode == 1`) `cursor_blink_time` sayacı artırılır (7 döngüde bir `cursor_blink` bayrağı `0` ve `1` arasında durum değiştirir).

---

## 4. `update_frame == 0` Diliminde Çalışan Görevler (Parçalı GLCD Donanım Çizimi)

192x64 GLCD ekranının tamamını tek seferde basmak mikrodenetleyiciyi uzun süre meşgul edip 500 µs zamanlayıcı töleransını bozacağı için **parçalı (discrete) donanım yazma** yöntemi kullanılmıştır.

- `GLCD_Picture_Discrete(glcd_rdcache, glcd_state, glcd_chip)`: Yaklaşık 172 µs sürer.
- **Tarama Adımları:**
  - `glcd_state` (0 ile 7 arası sayfa / page adresi) her karede 1 artırılır.
  - 8 sayfa tamamlandığında (`glcd_state > 7`), bir sonraki entegre çipe geçilir (`glcd_chip++`).
  - 3 çip (Sol: CS1, Orta: CS2, Sağ: CS3) sırayla taranarak tüm 192x64 piksel ekran yenilenmiş olur.

---

## 5. Özet Zaman Tasarrufu ve Donanım Koruma

- **Buzzer Kontrolü:** `TestNum > 690` durumunda buzzer kapatılır (`BUZZ_LAT = 0`).
- **Mikrodenetleyici Yük Dağılımı:**
  - `update_frame == 1` dilimi: Tuş algılama + Uygulama mantığı + Ekran metin hazırlığı (~17 µs).
  - `update_frame == 0` dilimi: Donanıma 1 sayfalık GLCD veri aktarımı (~172 µs).
  - Her iki dilim de 500 µs'lik Timer kesme periyodunun çok altında kalarak işlemcinin aşırı yüklenmesini önler.

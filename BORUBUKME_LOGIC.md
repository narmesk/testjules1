# Boru Bükme Makinesi Kontrol Mantığı (Pipe Bending Machine Logic)

Bu belge, Boru Bükme makinesinin çalışma prensiplerini, I/O eşleşmelerini ve iletişim protokolünü özetler.

## 1. Donanım ve İletişim (Hardware & Communication)

- **Protokol:** UDP üzerinden özel paket yapısı.
- **Paket Boyutu:** TX (Gönderilen) 128 Byte, RX (Alınan) 64 Byte.
- **CRC Kontrolü:** CRC16 kullanılır.
- **Bağlantı:** `IdUDPClient1` bileşeni üzerinden sağlanır.

### TX Paket Yapısı (Önemli Alanlar)
| Offset | Tanım | Açıklama |
| :--- | :--- | :--- |
| 0-3 | Header | `$12345678` |
| 4-7 | Protocol Index | Paket sıra numarası |
| 15 | Output Value | Dijital çıkışlar (OutVal) |
| 32-103 | Motion Data | Eksen hareket verileri (Hız, Pozisyon, İvme) |
| 126-127 | CRC16 | Paket bütünlük kontrolü |

## 2. I/O Eşleşmeleri (I/O Mapping)

### Dijital Çıkışlar (Outputs - OutVal Bits)
- **Bit 0:** Pause/Stop Durumu (Sarı LED/Durum)
- **Bit 1:** Dayama (Referans Dayaması)
- **Bit 2:** Sıkma Sabit (Valve Control)
- **Bit 3:** Eksen Değiştirme (Motor Aşağı/Yukarı)
- **Bit 4:** Boru Sürme (AValf Kontrolü)
- **Bit 5:** Otomatik Start Pulse
- **Bit 6:** Otomatik Stop Pulse
- **Bit 7:** İlk Başlangıç Stop Pulse

### Dijital Girişler (Inputs - InVal Bits)
- **Bit 0:** Auto/Manuel Seçici Anahtar
- **Bit 1:** Start Butonu
- **Bit 2:** Stop Butonu
- **Bit 7:** Sistem Kontaktör Geri Beslemesi
- **Bit 8-9:** Sıkma Sabit Sensörleri
- **Bit 10-11:** Eksen Sensörleri
- **Bit 12-13:** Sürme Sensörleri
- **Bit 14-15:** Dayama Sensörleri

## 3. Eksen Tanımları (Axis Mapping)

| Eksen | Görev | Birim | Notlar |
| :--- | :--- | :--- | :--- |
| **Axis A** | Eksen Değiştirme | mm | Bazı yerlerde `APozisyon` olarak geçer |
| **Axis B** | Boru Bükme | Derece (°) | `BPozisyon` |
| **Axis C** | Boru Döndürme | Derece (°) | `CPozisyon` |
| **Axis D** | Boru Sürme | mm | `DPozisyon`. **Kümülatif** çalışır (`AccumulateDPozisyon`). |

## 4. Çalışma Modları (Operating Modes)

1. **Manuel Mod:** Eksenlerin tekil kontrolü ve valf testleri için kullanılır.
2. **Otomatik Mod:** Reçetedeki adımların sırasıyla işletildiği moddur.
   - `IslenenAdim` değişkeni mevcut adımı takip eder.
   - Her adımda valf durumları kontrol edilir ve ardından eksen hareketleri başlatılır.
   - Boru sürme ekseni (Axis D) önceki adımların toplamı üzerine ekleme yaparak gider.

## 5. Hata Kodları ve Uyarılar

- **CRC Hatası:** "Paket Bütünlüğü (CRC) Hatası!" mesajı `JvMemo1`'e yazılır.
- **Kontaktör Hatası:** Sistem kontaktörü çekili değilse veya sürücüler hazır değilse (`ReadyStat`), sistem otomatik olarak `RESET_GO` komutu gönderir ve işlemi durdurur.
- **Açılış Kontrolü:** Program açılırken anahtar "Manuel" konumda değilse uyarı verir.

## 6. Geliştirme Notları (Developer Notes)

- **Re-entrancy:** `repeat-until` döngüleri içindeki `Application.ProcessMessages` kullanımına dikkat edin. Döngü bitmeden başka bir işlemin girmesi (örn. buton tıklama) değişken durumlarını bozabilir.
- **Hız Katsayıları:**
  - Axis A: `Hız * 3`
  - Axis B: `Hız * 21`
  - Axis C: `Hız * 200`
  - Axis D: `Hız * 50`

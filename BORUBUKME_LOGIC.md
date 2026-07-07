# Boru Bükme Makinesi Kontrol Mantığı (Pipe Bending Machine Logic)

Bu belge, Boru Bükme makinesinin çalışma prensiplerini, I/O eşleşmelerini ve iletişim protokolünü özetler.

## 1. Donanım ve İletişim (Hardware & Communication)

- **Protokol:** UDP üzerinden özel paket yapısı.
- **CRC Kontrolü:** CRC16 kullanılır.
- **Hız Katsayıları (Multiplier):**
  - Axis A: `Hız * 3`
  - Axis B: `Hız * 21`
  - Axis C: `Hız * 200`
  - Axis D: `Hız * 50`

## 2. I/O Eşleşmeleri (I/O Mapping)

### Dijital Çıkışlar (Outputs - OutVal Bits)
- **Bit 1:** Dayama Pistonu
- **Bit 2:** Sıkma Sabit (Valve Control)
- **Bit 3:** Eksen Değiştirme (Motor Aşağı/Yukarı)
- **Bit 4:** Sürme Pistonu
- **Bit 5:** Otomatik Start Pulse
- **Bit 6:** Otomatik Stop Pulse
- **Bit 7:** Referans/Start Durdurma Pulse

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

| Eksen (HW) | Görev | JSON Karşılığı | Çarpan (Hız) | Birim |
| :--- | :--- | :--- | :--- | :--- |
| **Axis A** | Eksen Değiştirme | `CPozisyon` | x3 | mm |
| **Axis B** | Boru Bükme | `DPozisyon` | x21 | Derece (°) |
| **Axis C** | Boru Döndürme | `BPozisyon` | x200 | Derece (°) |
| **Axis D** | Boru Sürme | `APozisyon` | x50 | mm |

> *Not: Axis D (Sürme) **kümülatif** çalışır (`AccumulateDPozisyon`).*

## 4. Geliştirici Notları (Developer Notes)

- **Bekleme Döngüleri:** `NoneWaitSleep` fonksiyonu `Application.ProcessMessages` kullanarak UI'ın donmasını engeller.
- **Karakter Kodlaması:** Kaynak kodlar **Windows-1254** kodlamasındadır. Yeni ekleme yaparken Türkçe karakterleri doğrudan kullanabilirsiniz.

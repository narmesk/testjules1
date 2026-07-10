# Boru Bükme Makinesi Kontrol Mantığı (Pipe Bending Machine Logic)

Bu belge, Boru Bükme makinesinin çalışma prensiplerini, I/O eşleşmelerini ve iletişim protokolünü özetler.

## 1. Donanım ve İletişim (Hardware & Communication)

- **Protokol:** UDP üzerinden özel paket yapısı.
- **CRC Kontrolü:** CRC16 kullanılır.
- **Hız Katsayıları (Multiplier):**
  - Axis A (HW): `Recipe.CHiz * 3`
  - Axis B (HW): `Recipe.DHiz * 21`
  - Axis C (HW): `Recipe.BHiz * 200`
  - Axis D (HW): `Recipe.AHiz * 50`

## 2. I/O Eşleşmeleri (I/O Mapping)

### Dijital Çıkışlar (Outputs - OutVal Bits)
- **Bit 1:** Dayama Pistonu
- **Bit 2:** Sıkma Sabit (Valve Control)
- **Bit 3:** Eksen Değiştirme (Motor Aşağı/Yukarı)
- **Bit 4:** Sürme Pistonu
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

| Eksen (HW) | Görev | Reçete Karşılığı | Çarpan (Hız) | Birim |
| :--- | :--- | :--- | :--- | :--- |
| **Axis A** | Eksen Değiştirme | Recipe C (CPozisyon) | x3 | mm |
| **Axis B** | Boru Bükme | Recipe D (DPozisyon) | x21 | Derece (°) |
| **Axis C** | Boru Döndürme | Recipe B (BPozisyon) | x200 | Derece (°) |
| **Axis D** | Boru Sürme | Recipe A (APozisyon) | x50 | mm |

> *Not: Axis D (Sürme) **kümülatif** çalışır (`AccumulateDPozisyon`).*

## 4. Matematiksel Hesaplamalar ve Hassasiyet

### 4.1. Hız Hesaplamaları
Donanım kontrolcüye gönderilen hız verileri, reçetedeki değerlerin aşağıdaki sabitlerle çarpılmasıyla elde edilir:
- **Sürme (HW D):** `AHiz * 50`
- **Döndürme (HW C):** `BHiz * 200`
- **Eksen Değ. (HW A):** `CHiz * 3`
- **Bükme (HW B):** `DHiz * 21`

### 4.2. Pozisyon Hassasiyeti
- Reçete üzerindeki değerler kullanıcıya 0.1 hassasiyetle (örn: 10.5 mm) gösterilir.
- Arka planda (`DataArray`) bu değerler **100 ile çarpılarak** tam sayı (Integer) olarak saklanır (örn: 1050).
- Karta gönderilirken bu 100 ile çarpılmış tam sayı değerleri kullanılır.
- **Formül:** `Saklanan_Pozisyon = Round(Ekran_Değeri * 100)`

## 5. Home (Referans) Mantığı

### 5.1. Tetikleme
- `HOME_POS_GO` komutu (Değer: 15) gönderilir.
- `AccumulateDPozisyon := 0;` (Sürme birikimi sıfırlanır).

### 5.2. Durum İzleme
Sürücülerin Home durumları Status register'larının **8. biti** üzerinden takip edilir.

## 6. Geliştirici Notları
- **NoneWaitSleep:** `Application.ProcessMessages` kullanarak UI'ın donmasını engeller.
- **Encoding:** Dosyalar **Windows-1254** (Turkish ANSI) formatındadır.

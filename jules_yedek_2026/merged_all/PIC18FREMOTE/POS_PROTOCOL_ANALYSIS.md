# POS Terminal Haberleşme Protokolü ve Sürücü Analizi (`pos.c`)

Bu doküman, `PIC18FREMOTE` uzaktan kumanda sisteminin POS (Ödeme/Sipariş) terminalleri (Commander ve Gilbarco protokolleri) ile UART2 üzerinden yaptığı haberleşme protokolünü, paket yapısını, XOR checksum doğrulamasını ve veri işleme mantığını detaylandırmaktadır.

---

## 1. Genel Mimarisi ve UART2 Yapılandırması

Sistem, POS cihazları ile haberleşmek için `UART2` portunu kullanır:
- **Gelen Veri Kesmesi / Polling:** `UART2_DataReady` bayrağı ile gelen baytlar dinlenir.
- **Protokol Farklılaştırıcı (`posidentifier`):**
  - `posidentifier == 2`: Standart Gilbarco Protokolü (STX = `0x02`).
  - `posidentifier == 3`: Modifiye/MSB-Setli Gilbarco veya Commander Protokolü (STX = `0x82` veya ACK/NACK + `0x80`).

---

## 2. Onay (Handshake) Paket Yapısı

POS terminaline yanıt verirken aşağıdaki baytlar gönderilir:

### A. ACK (Onay / Acknowledge - `Pos_Send_Ack`):
- `posidentifier == 3` ise: `0x06 + 0x80 = 0x86`
- Diğer durumlarda: `0x06` (ASCII ACK)

### B. NACK (Hata / Negative Acknowledge - `Pos_Send_Nack`):
- `posidentifier == 3` ise: `0x15 + 0x80 = 0x95`
- Diğer durumlarda: `0x15` (ASCII NAK)

### C. SYNC (Senkronizasyon Paket Dizisi - `Pos_Check_Send_Sync_Req`):
SYNC isteği tetiklendiğinde `pos_send_sync_desp` adımlı durum makinesi sırayla şu 9 baytlık paketi gönderir:
`[0x02, 0x31, 0x30, 0x30, 0x30, 0x30, 0x31, 0x03, 0x03]` (`STX, '1', '0', '0', '0', '0', '1', ETX, ETX`)

---

## 3. Commander Protokolü Çalışma Mantığı (`Pos_Check_Receive_Req`)

Commander tipi POS terminallerinden gelen veri paketleri **15 Bayt** uzunluğundadır.

### A. Başlık (Header) ve Paket Yakalama
- İlk bayt `0x02` (STX - Start of Text) geldiğinde paket yakalama başlar (`uart2_header_ok_flag = 1`).
- Paket dizini `uart2_receive_idx` artırılarak baytlar `uart2_receive_buffer` içerisine toplanır.
- Zaman aşımı için `uart2_receive_timeout_en = 1` aktif edilir.

### B. XOR Checksum Doğrulaması
15. bayt (`uart2_receive_buffer[14]`), paket içerisindeki 1. ile 13. baytlar arasındaki verilerin XOR (`^`) sonucuna eşit olmalıdır:
$$\text{Buffer}[14] = \text{Buffer}[1] \oplus \text{Buffer}[2] \oplus \dots \oplus \text{Buffer}[13]$$
- **Checksum Doğru İse:** `pos_send_ack_flg = 1` yapılır (ACK yanıtı).
- **Checksum Hatalı İse:** `pos_send_nack_flg = 1` yapılır (NACK yanıtı) ve paket sıfırlanır.

### C. Paket İçeriği ve Fiyat Parseleme
1. **Satır / Tabela Numarası (`receive_line_number`):**
   - Paket içerisindeki ASCII karakterlerden dönüştürülür:
   $$\text{LineNo} = (\text{Buffer}[4] - \text{'0'}) \times 10 + (\text{Buffer}[5] - \text{'0'})$$
2. **Fiyat / Değer Parseleme:**
   - 8, 9, 10 ve 11. baytlardan 4 haneli sayı elde edilir ve 10'a bölünerek hassasiyet ayarlanır (Örn: 3330 / 10 = 333 -> 3.33 TL/Lt):
   $$\text{Val} = \frac{(\text{Buffer}[8]-\text{'0'})\times 1000 + (\text{Buffer}[9]-\text{'0'})\times 100 + (\text{Buffer}[10]-\text{'0'})\times 10 + (\text{Buffer}[11]-\text{'0'})}{10}$$

### D. Ekran Eşleme (`PosCSwapTable`) ve Beeper Tetikleme
- Yeni alınan fiyat değeri, `PosCSwapTable` matrisi kullanılarak ekranın ilgili `Screen_Vals[N].Val_Pos1` veya `Val_Pos2` değişkenine atanır.
- Fiyat değiştiğinde:
  - `receive_beeper_timer = 150` (Kısa bip sesi).
  - `receiveended_beeper_timer = 10000` (~5 saniyelik sayaç).
  - `RECEIVEVALFLG1 = 1` ve `RECEIVEVALFLG2 = 1` bayrakları set edilerek Modbus üzerinden RF/Pano güncellemesi tetiklenir.

---

## 4. Gilbarco Protokolü Çalışma Mantığı (`Pos_Check_Gilbar_Receive_Req`)

Gilbarco tipi POS terminallerinden gelen paketler **9 Bayt** uzunluğundadır.

### A. Paket Başlığı ve Bayt Dönüşümü
- `posidentifier == 2`: Başlık `0x02`, veriler doğrudan saklanır.
- `posidentifier != 2`: Başlık `0x82`, veriler `0x80` MSB offset eklenerek saklanır.

### B. XOR Checksum Doğrulaması (9 Bayt)
$$\text{Buffer}[8] = \text{Buffer}[0] \oplus \text{Buffer}[1] \oplus \text{Buffer}[2] \oplus \dots \oplus \text{Buffer}[7]$$

### C. Fiyat Parseleme
- **Satır Numarası:** `Buffer[2] - 0x31`
- **Fiyat Değeri:** `(Buffer[3]-'0')*1000 + (Buffer[4]-'0')*100 + (Buffer[5]-'0')*10 + (Buffer[6]-'0')` değeri 10'a bölünerek hesaplanır.
- `PosCSwapTable` üzerinden HMI ekran değişkenlerine (`Screen_Vals`) aktarılır.

---

## 5. Özet Haberleşme Akış Şeması

```
   [ POS Terminal (UART2) ]
             │
             ├──> Gelen Bayt: 0x02 veya 0x82 (STX Başlık)
             │        │
             │        ├──> Paket Tamamlama (15-Byte Commander / 9-Byte Gilbarco)
             │        │        │
             │        │        ├──> XOR Checksum Kontrolü
             │        │        │        ├─ OK  ──> ACK Gönder (0x06 / 0x86) + Fiyat Parsele
             │        │        │        └─ HATA ─> NACK Gönder (0x15 / 0x95)
             │        │        │
             │        │        └──> Fiyat Değişti mi?
             │        │                 └─ EVET ──> PosCSwapTable ile Screen_Vals'e Yaz
             │        │                             Buzzer Sesini Tetikle (150ms)
             │        │                             RECEIVEVALFLG1 = 1 (Modbus Gönderimi Başlat)
```

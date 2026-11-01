# RF Modem ve Modbus RTU Haberleşme Protokolü Analizi (`modem.c` & `modbus.c`)

Bu doküman, `PIC18FREMOTE` uzaktan kumanda sisteminin RF Modem ve Modbus RTU protokolü üzerinden fiyat tabelalarına / panolara veri aktarımı, konfigürasyon senkronizasyonu ve CRC16 kontrol mekanizmasını detaylandırmaktadır.

---

## 1. Modbus RTU CRC16 Hesaplama Algoritması (`modbus.c`)

Modbus RTU paketlerinin bütünlüğünü doğrulamak için **Look-up Table (Arama Tablosu)** tabanlı hızlı CRC16 algoritması kullanılır.

### A. Arama Tabloları (`auchCRCLo` ve `auchCRCHi`)
- 256 elemanlı sabit sabit tablolar (`const unsigned char`) üzerinden her bayt için XOR işlemi gerçekleştirilir.
- Döngü bazlı bit kaydırma yerine tablo okuması yapıldığı için 8-bit PIC18 mikrodenetleyicide CRC16 hesaplaması mikrosaniyeler mertebesinde tamamlanır.

### B. `ModBus_Get_CRC16_Value(puchMsg, usDataLen)`
1. `uchCRCHi` ve `uchCRCLo` `0xFF` ile başlatılır.
2. Mesajın her baytı için:
   $$uIndex = uchCRCHi \oplus *puchMsg$$
   $$uchCRCHi = uchCRCLo \oplus auchCRCHi[uIndex]$$
   $$uchCRCLo = auchCRCLo[uIndex]$$
3. Sonuç değişkenleri `RXCRC_Low` ve `RXCRC_High` değişkenlerine aktarılır.

---

## 2. RF Modem Paket Çeşitleri ve Gönderim Mekanizmaları (`modem.c`)

UART1 üzerinden RF Modeme gönderilen paketler 8-bayt standart Modbus RTU komut paketleri veya 57-baytlık genişletilmiş fiyat/konfigürasyon veri paketleridir.

### A. Standart 8-Baytlık Komut Paketleri

| Paket Tipi | Komut Baytları | Açıklama |
| :--- | :--- | :--- |
| **`modem_sync_packet`** | `[0x01, 0x06, 0x00, 0x00, syncstat, 0x03, CRC_H, CRC_L]` | Canlılık / Durum bildirimi. `syncstat` ekran durum kodunu içerir. |
| **`modem_config_get_packet`** | `[0x01, 0x06, 0x00, 0x00, 0x00, 0xFF, CRC_H, CRC_L]` | Modeme bağlanan cihazın EEPROM ayarlarını isteme komutu. |
| **`modem_status_packet`** | `[0x02, 0x06, status1, status2, status3, 0x05, CRC_H, CRC_L]` | Hedef slave ID, döngü sayısı ve güncelleme sonucunu (`0x01` Başarılı / `0x00` Başarısız) modeme bildirir. |

### B. Adımlı Gönderim Yapısı (`Modem_Check_*_Req`)
UART1 TX buffer kilitlenmelerini önlemek için gönderimler switch-case tabanlı durum makinesi (`modem_send_sync_desp`, `modem_send_status_desp`, `modem_send_config_get_desp`) ile bayt bayt taranarak yapılır.

---

## 3. Fiyat Paketinin Oluşturulması ve Eşlenmesi (`transfer_price_to_uart` & `modem_send_modbus_request`)

Fiyat güncellemesi sırasında target slave ID'ye **57 Baytlık** özel Modbus Write Multiple Registers (`Function 0x10`) paketi gönderilir:

### A. Paket Yapısı (57 Bayt)
- **`Buffer[0]`:** Hedef Slave ID (`Send_Slave_Id`).
- **`Buffer[1]`:** Fonksiyon Kodu (`0x10` - Preset Multiple Registers).
- **`Buffer[2..3]`:** Başlangıç Adresi (`0x0000` Doğrudan cihaz için, `repeaterloopcnt / Send_SubSlave_Id` Repeater için).
- **`Buffer[4..5]`:** Register Sayısı (`0x0018` = 24 Register).
- **`Buffer[6]`:** Bayt Sayısı (`0x30` = 48 Bayt Veri).
- **`Buffer[7..50]`:** 12 Adet Fiyat/Pano Değeri (`Val_Pos1` ve `Val_Pos2` - Cash/Credit 16-bit High/Low baytları).
- **`Buffer[51..53]`:** 24 adet slave cihazın ayna durum maskesi (`MIRRORSLAVEIDUPDATESTATE` bitmask bitleri).
- **`Buffer[55..56]`:** CRC16 Yüksek ve Düşük baytları.

### B. Ürün Eşleme Matrisi (`ProductCSwapTable`)
Ekran değerleri (`Screen_Vals`) haberleşme hattına verilmeden önce `ProductCSwapTable` indisi üzerinden dönüştürülür:
$$\text{Buffer}[7..8] = \text{Screen\_Vals}[\text{ProductCSwapTable}[0]].\text{Val\_Pos1}$$
Bu eşleme, ekran sıralaması ile tabeladaki LED sıralaması arasındaki ürün kaymalarını (Dizel, Regular, Plus, Supreme) düzeltir.

---

## 4. Modem Gelen Komut Yakalama ve EEPROM Güncelleme

### A. Boşta Dinleme Modu (`Modem_Check_Idle_Receive_Req`)
UART1'den 8 baytlık yanıt geldiğinde CRC16 doğrulanır ve 5. bayttaki (`Buffer[5]`) komut koduna göre işlem yapılır:
- **`0x07` / `0x17` / `0x27`:** Konfigürasyon bayrağı set edilir (`config_flag = 1`).
- **`0x09`:** System Reset emri (`reset_flag = 1`).
- **`0x0A`:** Yeniden gönderim emri (`resend_flag = 1`, `success_flag = 1`).
- **`0x0B` / `0x37`:** Beeper tetiklenir, süreç başlatılır (`start_procedure()`).
- **`0x0C`:** Delphi / PC güncelleme başarısını bildirir (`config_menu_messageidx = 11`).
- **`0x0D`:** Süreç durdurulur (`stop_procedure()`).
- **`0x08`:** Sessizlik / Uyku moduna geçilir (`SYS_STATE = 4`).

### B. Konfigürasyon ve EEPROM Kaydı (`Modem_Check_Config_Receive_Req`)
Modemden 57 baytlık konfigürasyon paketi alındığında:
1. `slaveid_cnt` (Slave cihaz sayısı) ve `loop_cnt` (Tekrar sayısı) okunur.
2. 12 adet konfigürasyon değeri `Config_Vals[0..11]` yapısına ayrıştırılır.
3. Kesmeler geçici olarak kapatılır (`INTERRUPT_GlobalInterruptHighDisable`).
4. Veriler EEPROM'a yazılır (`write_config_paremeter_eeprom()`, `write_slaveidcnt_paremeter_eeprom()`).
5. EEPROM'dan tekrar okunarak doğruluk teyit edilir ve slave ID listesi güncellenir (`eeprom_to_slave_idlist()`).
6. Modeme onay yanıtı verilerek kesmeler tekrar açılır.

---

## 5. Özet Veri Akış Şeması

```
   [ RF Modem / PC (UART1) ]
             │
             ├──> Gelen Veri: 57 Bayt Konfigürasyon Paketi
             │        │
             │        ├──> Modbus CRC16 Doğrulaması (ModBus_Get_CRC16_Value)
             │        │        │
             │        │        ├──> EEPROM'a Yazma (write_config_paremeter_eeprom)
             │        │        └──> Slave ID Listesini Güncelleme (eeprom_to_slave_idlist)
             │        │
             ├──> Gelen Veri: 8 Bayt Yanıt / Komut
                      │
                      ├──> Komut Code 0x0B -> Gönderimi Başlat (start_procedure)
                      ├──> Komut Code 0x09 -> Reset At (reset_flag)
                      └──> Komut Code 0x08 -> Sessizlik Modu (SYS_STATE = 4)
```

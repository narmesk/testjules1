# EEPROM Bellek Haritası ve Parametre Yönetim Analizi (`eeprom_params.c`)

Bu doküman, `PIC18FREMOTE` uzaktan kumanda sisteminde EEPROM (`DATAEE_ReadByte` / `DATAEE_WriteByte`) üzerinde saklanan fiyat kayıtları, konfigürasyon değişkenleri, slave/sub-slave ID listeleri, kanal eşleme tabloları ve sayısal dönüşüm fonksiyonlarının detaylı analizini sunmaktadır.

---

## 1. EEPROM Bellek Haritası (Address Map)

Sistemin dâhili Data EEPROM belleği üzerindeki adres dağılımı aşağıdaki gibidir:

| EEPROM Adres Aralığı | Değişken / Veri Türü | Açıklama |
| :--- | :--- | :--- |
| **`11 - 58`** | `Screen_Vals[0..11]` | 12 adet ekran/ürün fiyatı (`Val_Pos1` Cash, `Val_Pos2` Credit - 16'şar bit High/Low baytları) |
| **`101 - 148`** | `Config_Vals[0..11]` | 12 adet konfigürasyon/ayarlama parametresi (High/Low baytları) |
| **`149 - 150`** | `Config_Vals[12].Val_Pos1` | `slaveid_cnt` (Sistemdeki aktif slave cihaz sayısı) |
| **`151 - 152`** | `Config_Vals[12].Val_Pos2` | `loop_cnt` (Modbus güncelleme tekrar sayısı) |
| **`201`** | `provider_id` | Sağlayıcı / Tedarikçi Kimliği |
| **`202`** | `retrytime` | Haberleşme tekrar deneme süresi (Varsayılan: 10) |
| **`203`** | `repeaterloopcnt` | Tekrarlayıcı (Repeater) döngü sayısı (Varsayılan: 1) |
| **`204`** | `delayedpostime` | Gecikmeli POS sorgu zamanlayıcısı |
| **`205`** | `posidentifier` | POS Protokol Tipi (2: Gilbarco, 3: Commander / MSB set) |
| **`210 - 217`** | `PosCSwapTable[0..15]` | POS kanal / hat sıralama dönüştürme matrisi |
| **`218 - 225`** | `ProductCSwapTable[0..7]`| Ürün (Dizel, Regular, Plus, Supreme) sıralama dönüştürme matrisi |
| **`241`** | `resend_request` | Yeniden gönderme isteği bayrağı |
| **`242`** | `useposflag` | POS terminali kullanım bayrağı (1: Aktif, 0: Pasif) |

---

## 2. Slave ID ve Sub-Slave (Repeater) Bit-Paketleme Mantığı

`SLAVEIDLIST[24]` ve `SLAVESUBIDLIST[24]` dizileri, 24 adet panonun cihaz adreslerini ve bağlı oldukları tekrarlayıcı (repeater) ID'lerini tutar:
- **Paketleme (`slave_idlist_to_eeprom`):** İki adet 8-bitlik Slave ID birleştirilerek 16-bitlik `Config_Vals[N].Val_Pos1` değerine dönüştürülür:
  $$\text{Config\_Vals}[N].\text{Val\_Pos1} = (\text{SLAVEIDLIST}[2k] \ll 8) \mid \text{SLAVEIDLIST}[2k+1]$$
- **Ayrıştırma (`eeprom_to_slave_idlist`):** 16-bitlik `Config_Vals` değişkeninden High bayt ve Low bayt ayrıştırılarak `SLAVEIDLIST` ve `SLAVESUBIDLIST` dizilerine aktarılır.

---

## 3. Sayı - ASCII Metin Dönüşüm Fonksiyonları

### A. `WriteDecimalStringShort(short value)`
16-bitlik işaretli (`short`) tamsayıyı 5 basamaklı ASCII karakter dizisine çevirir:
- Negatif sayılar için `val10 = '-'`, pozitifler için `val10 = '+'` atanır.
- Bölme ve mod alma yöntemi ile basamaklar ayrıştırılır:
  - `val11`: On binler basamağı (`+ 0x30` ASCII offset)
  - `val12`: Binler basamağı
  - `val13`: Yüzler basamağı
  - `val14`: Onlar basamağı
  - `val15`: Birler basamağı

### B. `WriteDecimalStringUChar(unsigned char value)`
8-bitlik imzasız (`unsigned char`) tamsayıyı 3 basamaklı ASCII dizisine çevirir:
- `valc12`: Yüzler basamağı
- `valc11`: Onlar basamağı
- `valc10`: Birler basamağı

---

## 4. Donanım Güvenliği ve Watchdog (WDT) Beslemesi

Tüm EEPROM okuma/yazma fonksiyonlarında (`read_price_eeprom`, `write_price_eeprom`, `write_config_paremeter_eeprom` vb.) her bayt yazım adımının arasına `ClrWdt()` koyulmuştur. EEPROM yazma işlemleri donanımsal olarak birkaç milisaniye sürebildiği için Watchdog Timer'ın işlemciyi sıfırlaması engellenir.

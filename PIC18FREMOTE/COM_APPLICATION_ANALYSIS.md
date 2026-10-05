# REMOTE_CONTROLLER_comApplication Derinlemesine Mimari ve Protokol Analizi

Bu doküman, uzaktan kumanda (RF / RS485 Modem ve POS Terminal) sisteminin haberleşme protokolu, state-machine (durum makinesi) senaryoları ve Modbus tabanlı fiyat/ID güncelleştirme algoritmasını detaylandırmaktadır.

---

## 1. Genel Fonksiyon Amacı ve Çağrı Dinamiği

`REMOTE_CONTROLLER_comApplication()`, ana döngüde her 1 ms'de bir çağrılır (`update_frame == 1`).
Fonksiyonun temel amacı:
1. **POS (Sipariş/Ödeme) Terminali ile Senkronizasyon:** ACK, NACK, SYNC paketlerini işler.
2. **RF / Modem Yönetimi:** Fiyat tabelalarına, panolara veya fiyat göstergelerine Modbus üzerinden fiyat/konfigürasyon güncellemesi gönderir.
3. **Senaryo ve Durum Yönetimi (`SYS_STATE`):** 5 farklı ana senaryo durumunu yöneterek güncelleme süreçlerinin adım adım ilerlemesini sağlar.

---

## 2. Haberleşme Yönetimi ve Temel Ön Kontroller

```c
ClrWdt(); // Watchdog Timer sıfırlanır
if (pos_send_ack_flg == 1)  { pos_send_ack_flg = 0; Pos_Send_Ack(); }
if (pos_send_nack_flg == 1) { pos_send_nack_flg = 0; Pos_Send_Nack(); }
if (pos_send_sync_flg == 1) { pos_send_sync_flg = 0; Pos_Send_Sync(); }

Pos_Check_Send_Sync_Req();  // POS senkronizasyon isteği kontrolü
Pos_Check_Receive_Req();    // POS gelen veri kontrolü
Modem_Check_Send_Sync_Req();// Modem senkronizasyon isteği kontrolü
```
- POS cihazından gelen onay (ACK), red (NACK) ve senkronizasyon (SYNC) istekleri anında yanıtlanır.
- Seri port FIFO / Buffer okumaları tetiklenir.

---

## 3. Tetikleme ve İlk Karılma Mantığı (`RECEIVEVALFLG1` & `TIMERFLG1`)

Eğer modeme sessizlik (silence) sinyali gelmemişse (`modem_receive_silence_signal == 0`):

1. **Gelen Veri İptal/Yeniden Başlatma (`RECEIVEVALFLG1 == 1`):**
   - Yeni bir güncelleme emri geldiğinde aktif çalışan işlemler iptal edilir (`SYS_STATE = 0`).
   - Ekran `SENDING_PAGE4` ("Bekleniyor / Timeout") durumuna çekilir ve kilitlenir (`page_wait_time_lock = 1`).
2. **Timer Tetiklemesi (`TIMERFLG1 == 1`):**
   - Veri doğrulandıktan ve zamanlayıcı süresi dolduktan sonra `SYS_STATE = 2` (Delphi / PC Uyarısı & Hazırlık Modu) durumuna geçilir.
   - Tüm tabela / slave cihaz takip dizisi (`WAITIDUPDATESTATE[24]`) `0x01` (Bekliyor) olarak sıfırlanır.
   - Ekran mesajı `scr_msg_decriptor = 2` ("Gönderim Başladı") durumuna geçer.

---

## 4. `SYS_STATE` Durum Makinesi (Scenario-Based State Machine)

Sistem 5 ana durumda çalışır:

### 🔷 `SYS_STATE = 0`: IDLE (Boşta / Bekleme ve Dinleme Modu)
- **Canlılık Sinyali (Heartbeat):** `process_idle_live_timer` her 4381 döngüde bir (~4.38 saniye) `Modem_Send_Sync()` ile modeme canlılık / durum sinyali gönderir.
- **Rutin Kontroller:**
  - `Modem_Check_Idle_Receive_Req()` ile gelen boşta komutları taranır.
  - `reset_flag == 1`: İşlemci mikrodenetleyicisi yazılımsal olarak resetlenir (`__asm__ volatile ("reset")`).
  - `config_flag == 1`: Konfigürasyon okunur ve `SYS_STATE = 3` durumuna geçilir.
  - `resend_flag == 1`: Yeniden gönderim bayrakları set edilerek süreç başlatılır.

---

### 🔷 `SYS_STATE = 2`: Delphi / PC Uyarısı ve Ön Gönderim
- **Modbus İsteği Hazırlığı:** `modem_send_modbus_request(0x01)` ile master/modem uyarılır.
- **Zamanlayıcılar:**
  - `first_screen_received_timer` (1995 döngü ~ 2 saniye): İlk onay ekranı doğrulama zamanlayıcısı.
  - `process_first_screen_timer` (10503 döngü ~ 10.5 saniye): Bekleme süresi dolduğunda asıl fiyat güncelleme aşamasına (`SYS_STATE = 1`) geçilir.
  - `SLAVEIDUPDATESTATE` dizisi sıfırlanır ve ekran `SENDING_PAGE1` durumuna getirilir.

---

### 🔷 `SYS_STATE = 1`: Fiyat Güncelleme Döngüsü (Price Update Engine)

Bu durum, sistemin en karmaşık ve kritik algoritmasını içerir. 24 adede kadar Tabela / Slave cihaza Modbus üzerinden fiyat aktarır.

#### 🔄 Çift Katmanlı Döngü Mimari:
1. **Dış Döngü (`act_loop_cnt` / `loop_cnt`):** Güncelleme deneme sayısı döngüsü (Örn: 3 tekrar).
2. **İç Döngü (`act_slaveid_cnt` / `slaveid_cnt`):** Hedef tabela / ID listesindeki cihaz döngüsü.

#### 📡 Doğrudan Cihaz vs Repeater (Tekrarlayıcı) Ayrımı:
- **Doğrudan Cihaz (`SLAVESUBIDLIST[i] == 0`):**
  - `modem_send_modbus_request(SLAVEIDLIST[i])` ile doğrudan Modbus paketi gönderilir.
- **Tekrarlayıcı Üzerinden (`SLAVESUBIDLIST[i] != 0`):**
  - `modem_send_modbus_repeater_request(SLAVEID, SLAVESUBID)` ile repeater adresli özel Modbus paketi gönderilir.

#### ⏱ Timout ve Yanıt Kontrolü (1953 ve 1323 Döngü Zamanlayıcıları):
1. **Modbus Gönderim ve Timeout (`process_cnt_uart_send > 1953` ~ 1.95s):**
   - Eğer yanıt alınamamışsa (`modbus_receive_complete == 0`), başarısızlık durumu (`0x00`) modeme raporlanır (`Modem_Send_Status`).
   - Tabela durumu `WAITIDUPDATESTATE[i] = 1` (Hata/Başarısız) yapılır.
2. **Bekleme ve Toptan Onay Kontrolü (`process_cnt_uart_send_status > 1323` ~ 1.32s):**
   - Başarılı yanıt alınmışsa (`modbus_receive_complete == 1`), `SLAVEIDUPDATESTATE[i] = 1` (Güncellendi) ve `WAITIDUPDATESTATE[i] = 2` (Başarılı) yapılır.
   - Sonraki slave cihaza geçilir (`update_state_slaveid_descriptor = 0`).

#### 🏁 Güncelleme Sonucu (Tüm Döngü Bittiğinde):
- Tüm ID'ler kontrol edilir (`SLAVEIDUPDATESTATE` dizisi taranır).
- **Tüm ID'ler Başarılı İse (`succ_update == 1`):**
  - `success_flag = 1`, `syncstat = 99`.
  - `scr_msg_decriptor = 5` ("Tüm Fiyatlar Başarıyla Güncellendi").
  - `Modem_Send_Sync()` ile merkeze başarı raporlanır.
- **Herhangi Bir ID Başarısız İse (`succ_update == 0`):**
  - `success_flag = 0`, `syncstat = 98`.
  - `scr_msg_decriptor = 4` ("Güncelleme Başarısız / Bazı ID'ler Cevap Vermedi").
  - `Modem_Send_Sync()` ile merkeze hata raporlanır.
- Sistem tekrar `SYS_STATE = 0` (IDLE) durumuna döner.

---

### 🔷 `SYS_STATE = 3`: Konfigürasyon Okuma Modu
- `Modem_Check_Config_Get_Req()` ve `Modem_Check_Config_Receive_Req()` ile konfigürasyon paketi beklenir.
- `setting_update_wait_timer > 6932` (~6.9 saniye) dolduğunda otomatik olarak `SYS_STATE = 0` durumuna dönülür.

---

### 🔷 `SYS_STATE = 4`: Sessizlik / Bekleme Modu (Silence Mode)
- `silence_wait_timer > 210` dolduğunda `modem_receive_silence_signal = 1` yapılır.
- Sistem hattı dinlemeye alır, gereksiz haberleşme trafiğini keser.

---

## 5. Özet Performans ve Güvenilirlik Notları

1. **Watchdog Güvencesi:** Fonksiyon girişindeki `ClrWdt()` sayesinde uzun Modbus timeout döngülerinde işlemci resetlenmez.
2. **Yazılımsal Reset Desteği:** Merkezi sistemden gelen `reset_flag` ile kilitlenen veya mod değiştiren cihaz anında `__asm__ volatile ("reset")` ile yeniden başlatılır.
3. **Dinamik HMI Ekran Senkronizasyonu:** Her Modbus durumu değişiminde `screen_sender_running()` çağrılarak kullanıcının GLCD ekranında anlık hangi ID'nin güncellendiği (Kırmızı/Yeşil veya Yanıp Sönen ikonlarla) canlı olarak gösterilir.

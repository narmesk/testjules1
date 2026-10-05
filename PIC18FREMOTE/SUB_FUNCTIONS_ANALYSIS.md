# Alt Fonksiyonlar Mimarisi ve Çalışma Analizi

Bu doküman, uzaktan kumanda sisteminin uygulama ve ekran durum yönetimi görevlerini üstlenen `REMOTE_CONTROLLER_runApplicaiton` ve `REMOTE_CONTROLLER_runInformation` fonksiyonlarının detaylı çalışma mantığını açıklamaktadır.

---

## 1. `REMOTE_CONTROLLER_runApplicaiton(void)` Analizi

Bu fonksiyon, ana döngüde `update_frame == 1` olduğunda (her 1 ms'de bir) çalıştırılan temel arka plan ve iletişim görev kümesidir.

### Görevleri:
1. **İletişim İşlemleri (`REMOTE_CONTROLLER_comApplication()`):**
   - RS485 / Modbus veya özel kablosuz/seri iletişim paketlerinin alım, gönderim ve zaman aşımı (timeout) işlemlerini yönetir.
2. **Tuş Takımı Okuma (`read_keyb()`):**
   - Matris tuş takımını (4x5 tuş matrisi) tarayarak basılı tuşların durumunu günceller ve `ButtonKeyNum` değişkenine aktarır.
3. **Sistem Durum Yönetimi (`main_state` Switch Yapısı):**
   - Şimdilik pasif (commented out) durumda tutulmuş olup, sistem durumuna göre (`IDLE`, `GOSLEEP`, `CONFIGPARAM`, `CONFIGPRICE`, `SENDING`) özel arka plan rutinlerinin tetiklenmesi için tasarlanmıştır.

---

## 2. `REMOTE_CONTROLLER_runInformation(void)` Analizi

Bu fonksiyon, ekran durum makinesini (`main_state`) yönetir ve her 32 ms'de bir taranarak HMI/GLCD ekranında hangi menünün ve sayfanın gösterileceğini belirler.

### A. Ekran Durum Makinesi (`main_state` Yapısı)

#### 1. `IDLE` (Boşta / Bekleme Modu):
- **Otomatik Sayfa Geçişi:**
  - `page_wait_time` sayacı her çağrıda artırılır.
  - Sayaç `> 5000` ulaştığında (yaklaşık 5 saniye) sayfa otomatik değişir (`page_state++`).
  - `IDLE_PAGE1`, `IDLE_PAGE2` ve `IDLE_PAGE3` arasında dairesel döngü sağlanır.
- **Menü Durumu (`menu_state`):**
  - Sayfa değiştiğinde `menu_state = Menu_Creating` yapılır.
  - `Menu_Creating` modunda `CreateMenuIdle(...)` çağrılarak şablon ve sabit görseller çizilir.
  - Sabit çizim bittikten sonra `Menu_Created` moduna geçilir ve dinamik verileri tazelemek için `RefreshMenuIdle(...)` çalıştırılır.

#### 2. `CONFIGID`, `CONFIGPARAM`, `CONFIGPRICE` (Konfigürasyon Modları):
- İlgili konfigürasyon sayfalarında (`CONFIG_ID_PAGE`, `CONFIG_PARAM_PAGE`, `CONFIG_PRICE_PAGE`):
  - `menu_state == Menu_Creating` ise ilgili `CreateMenuConfig*()` fonksiyonu ile menü bileşenleri ilk kez oluşturulur.
  - `menu_state == Menu_Created` ise `RefreshMenuConfig*()` ile değişen ayar değerleri ekranda güncellenir.

#### 3. `ENTERPASSWORD` (Şifre Giriş Modu):
- Şifre ekranı şablonu oluşturulur (`CreateMenuPassword`) ve girilen basamaklar ekranda tazelenir (`RefreshMenuPassWord`).

#### 4. `SENDING` (Veri Gönderim / Fiyat / ID Güncelleme Modu):
- Ekran mesaj indeksi gönderim mesajlarına yönlendirilir (`config_menu_messageidx = scr_msg_decriptor + 6`).
- **Kilitli Değilse (`page_wait_time_lock == 0`):**
  - `page_wait_time > 3000` olduğunda sayfa otomatik ilerletilir (`SENDING_PAGE1` .. `SENDING_PAGE4`).
- Her gönderim sayfasında `CreateMenuSender` ve `RefreshMenuSender` ile gönderim durumu, iletilen cihaz ID'leri ve iletişim durumu gösterilir.

#### 5. `GOSLEEP` (Uyku Modu):
- Test ve zaman ölçüm kodları pasife çekilmiştir (`WriteDecimalStringShort`, `TestNum = TestNum2 - TestNum1`).

---

## 3. Genel Mimari Değerlendirme

- **Oluşturma / Tazeleme Ayrımı (`Menu_Creating` / `Menu_Created`):**
  - Statik çerçeveler ve başlıklar sadece 1 kez `CreateMenu*` ile çizilerek ekran titremesi (flicker) önlenir.
  - Sadece değişen dinamik metin/rakam alanları `RefreshMenu*` ile güncellenerek piksel çizim süresi minimuma indirilir.
- **Aşama Yönetimi:** Ekran donanımı işlemciyi meşgul etmeden yüksek verimle çalışır.

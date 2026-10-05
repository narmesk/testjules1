# HMI Menü Sistemi Haritası ve Tek Geçişli Grafik Çizim Analizi (`menu.c`)

Bu doküman, `PIC18FREMOTE` uzaktan kumanda sisteminin menü mimarisini, eski 64-adımlı (`updatecnt 0..63`) parçalı çizim sisteminden **yeni nesil tek geçişli (Single-pass Direct Render)** çizim yapısına dönüştürülmesini ve menü haritasını detaylandırmaktadır.

---

## 1. HMI Menü Sistemi Mimari Haritası (Menu Map)

Sistemdeki menü ekranları 6 ana kategoride toplanmıştır:

```
                          [ HMI Menü Sistemi Haritası ]
                                        │
     ┌──────────────────┬───────────────┼───────────────┬──────────────────┐
     ▼                  ▼               ▼               ▼                  ▼
[ 1. IDLE ]       [ 2. SENDER ]   [ 3. CONFIG ID ] [ 4. CONFIG PARAM ] [ 5. PASSWORD ]
 - Ana Fiyat       - Tabela Veri   - Sequence ID    - Sub Parameter     - Şifre Giriş
   Tablosu          Aktarım         - Slave ID       - Değer Girme        Görseli
 - 3 Sayfa          Tablosu         - Sub ID
   (P1, P2, P3)   - 4 Sayfa
```

### A. Menü Ekranları Özeti:
1. **`CreateMenuIdle` & `RefreshMenuIdle` (Ana Fiyat Sayfası):**
   - 3 farklı sayfa (`IDLE_PAGE1`, `IDLE_PAGE2`, `IDLE_PAGE3`) arasında 5 saniyede bir otomatik döner.
   - Her sayfada 3 ürünün (Dizel, Regular, Plus, Supreme vb.) **CASH** ve **CREDIT** fiyatlarını dijital gösterge karakterleri ile tazeleyerek gösterir.
2. **`CreateMenuSender` & `RefreshMenuSender` (Veri Aktarım Durum Ekranı):**
   - RF Modem üzerinden 24 adede kadar tabela ve slave cihaza gönderilen fiyat paketlerinin deneme sayılarını (`TryIdx`) ve anlık aktarım durumlarını (`MessageTable`) 4 farklı sayfada gösterir.
3. **`CreateMenuConfigID` & `RefreshMenuConfigID` (Slave / Sub-ID Ayar Ekranı):**
   - Tabela sıra numarası (`Sequence ID`), Slave ID ve Repeater Sub-ID değerlerinin seçildiği ve düzenlendiği menüdür.
4. **`CreateMenuConfigParam` & `RefreshMenuConfigParam` (Alt Parametre Ayar Ekranı):**
   - Sistem alt parametrelerinin (`SUBPARAMS`) görüntülendiği ve değiştirildiği ekrandır.
5. **`CreateMenuConfigPrice` & `RefreshMenuConfigPrice` (Fiyat Ayar Ekranı):**
   - Ürün bazlı Cash ve Credit fiyatlarının elle girildiği veya düzenlendiği ekrandır.
6. **`CreateMenuPassword` & `RefreshMenuPassWord` (Şifre Giriş Ekranı):**
   - Yetkili konfigürasyon menülerine erişim için 4 haneli güvenlik şifresinin (`ActPassVal`) girildiği ekrandır.

---

## 2. Eski Parçalı Yapı (Case 0..63) ile Yeni Tek Geçişli Yapının Karşılaştırılması

### ❌ Eski Parçalı Yapının Çalışma Mantığı:
- Eski GLCD sürücüsünde gölge bellek (RAM Buffer) tam performanslı kullanılmadığı için çizim işlemleri `updatecnt` sayacının 0'dan 63'e kadar saydığı **64 ayrı zaman dilimine** bölünmüştü.
- Örneğin `CreateMenuIdle` içerisinde:
  - `case 0..7`: Ekranı temizleme.
  - `case 9..10`: "PRICE TABLE" başlığını basma.
  - `case 22..45`: Karakter karakter ürün isimlerini basma.
  - `case 46..50`: Çerçeve çizgilerini çekme.
- **Dezavantajları:**
  - Kod uzunluğu binlerce satıra ulaşıyordu.
  - Okunabilirlik son derece düşüktü.
  - Ekran elemanlarını hizalamak ve değiştirmek imkansız hale geliyordu.

---

### 🚀 Yeni Nesil Tek Geçişli (Single-Pass) Yapının Çalışma Mantığı:
Yeni `glcd.c` sürücüsünde 1536-baytlık gölge RAM bellek (`glcd_buffer[1536]`) doğrudan taranarak donanıma aktarıldığı için:
1. **Tek Blokta Çizim:** `GLCD_ClearAll()` ile gölge bellek sıfırlanır, `GLCD_String5x7`, `GLCD_StringArialBold14`, `GLCD_Rectangle` ve `GLCDPutCharDigMin` fonksiyonları ile tüm ekran saniyeler/milisaniyeler içinde gölge belleğe çizilir.
2. **Tek Seferde Render (`GLCD_Render()`):** Tüm grafik ve metinler hazırlandıktan sonra `GLCD_Render()` çağrılarak gölge bellek LCD ekranın 3 çipine tek seferde aktarılır.
3. **Sıfır Titreme (Flicker-Free):** Ekran pikselleri adım adım silinip yazılmadığı için görüntüde hiçbir titreme veya gecikme yaşanmaz.
4. **Geriye Dönük Uyumluluk (Backward Compatibility Bridge):** Ana döngüdeki 64-adımlı tespiti bozmamak için fonksiyonlar `updatecnt == 63` olduğunda doğrudan `_Direct` sürücülerini çağıracak şekilde köprülenmiştir.

---

## 3. Örnek Dönüşüm Kodu (Code Transformation)

### Eski `CreateMenuIdle` (300+ Satır Switch-Case):
```c
switch (updatecnt) {
   case 9:  tx = 3; ty = 0; GLCDPutChar5x7('P'); GLCDPutChar5x7('R'); GLCDPutChar5x7('I'); ... break;
   case 10: tx = 37; GLCDPutChar5x7('T'); GLCDPutChar5x7('A'); ... break;
   // 64 case adımı...
}
```

### Yeni `CreateMenuIdle_Direct` (Sadece 25 Satır Temiz C Kodu):
```c
void CreateMenuIdle_Direct(unsigned char menucount)
{
    GLCD_ClearAll();

    // Başlık ve Kolonlar
    GLCD_String5x7(3, 0, "PRICE TABLE");
    GLCD_String5x7(91, 0, "CASH");
    GLCD_String5x7(147, 0, "CREDIT");

    // Çerçeveler
    GLCD_Rectangle(79, 0, 79, 55, BLACK);
    GLCD_Rectangle(135, 0, 135, 55, BLACK);
    GLCD_Rectangle(0, 8, 191, 8, BLACK);

    // Donanıma Tek Seferde Bas
    GLCD_Render();
}
```

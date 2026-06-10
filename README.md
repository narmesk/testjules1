# Price Tuning Tool (Delphi 12)

Bu araç, JSON formatındaki fiyat listelerini düzenlemek için geliştirilmiş bir VCL Form uygulamasıdır.

## Özellikler
- **JSON Düzenleme:** JSON dosyasındaki `FULLVALxA` ve `SELFVALxA` değerlerini temel alarak `B` ve `C` değerlerini otomatik hesaplar.
- **Dinamik Artış:** Kullanıcı tarafından belirlenen bir artış miktarını `A` değerine ekleyerek `B`'yi, `B` değerine ekleyerek `C`'yi oluşturur.
- **Yüksek DPI Desteği:** Ekran çözünürlüğüne göre form dizaynını otomatik ölçeklendirir.

## Dosyalar
- `PriceTuningTool.dpr`: Ana proje dosyası.
- `MainForm.pas`: Uygulama mantığı ve olay yakalayıcılar.
- `MainForm.dfm`: Form tasarımı.

## Nasıl Kullanılır?
1. Delphi 12 IDE'sini açın.
2. `PriceTuningTool.dpr` dosyasını açın.
3. `F9` tuşuna basarak projeyi derleyip çalıştırın.
4. "JSON Yükle" butonu ile dosyanızı seçin.
5. "Artış Miktarı" alanına istediğiniz değeri girin.
6. "Hesapla & Düzenle" butonuna basarak işlemi gerçekleştirin.
7. "Kaydet" butonu ile güncellenmiş JSON'u kaydedin.

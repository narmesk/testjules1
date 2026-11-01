# Delta ASDA-A2 Sürücü Parametre Ayarları (dsPIC33EP CNC Projesi İçin)

Bu doküman, oluşturulan dsPIC33EP64MC504 CNC kontrol projesinin Delta ASDA-A2 servo sürücüleri ile sorunsuz çalışması için gereken minimum parametre setini içerir.

## 1. Haberleşme (CANopen) Parametreleri
Sürücülerin dsPIC ile 1Mbps hızında konuşabilmesi için bu ayarlar zorunludur.

*   **P3-00 (Station ID):**
    *   X Ekseni: 1
    *   Y Ekseni: 2
    *   Z Ekseni: 3
    *   A Ekseni: 4
*   **P3-01 (CAN Baud Rate):** 0x0403 (1 Mbps)
*   **P3-02 (Protocol):** 1 (CANopen)
*   **P3-09 (SYNC Enable):** 1 (Senkronizasyon aktif)

## 2. Operasyonel Mod Ayarları
Sürücünün kontrol yetkisini CAN hattına devretmesi için:

*   **P1-01 (Control Mode):** 0x000C (CANopen Mode)
*   **P2-10 to P2-17 (Digital Inputs):** Bu parametreleri kullanarak limit switch (L-OT, L-IT) ve Home girişlerini yapılandırın. Eğer switch bağlı değilse "Disabled" (0) yapmanız önerilir.

## 3. Ölçeklendirme (Electronic Gear)
Projede `#define STEPS_PER_MM 10000.0f` kullanılmıştır. Motorun bir turdaki pulse sayısını buna göre ayarlamak için:

*   **P1-44 (Numerator):** 128 (Örnek değer)
*   **P1-45 (Denominator):** 1 (Örnek değer)
*   *Not: ASDA-A2 motor encoderları genellikle 1,280,000 pulse/tur çözünürlüktedir.*

## 4. PDO Yapılandırması
dsPIC kodu **RxPDO1**'i (`0x200 + NodeID`) kullanır.
*   **Data 0-1:** 0x6040 (Control Word) - 2 Byte
*   **Data 2-5:** 0x607A (Target Position) - 4 Byte
*   **DLC:** 6 Byte olmalıdır.

**Önemli:** Parametre değişikliklerinden sonra sürücü enerjisini kesip tekrar vermeyi unutmayın.

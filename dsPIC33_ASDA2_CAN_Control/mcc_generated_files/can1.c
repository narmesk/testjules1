#include "can1.h"

/**
 * dsPIC33EP ECAN DMA RAM Tanımlaması
 * Bu alanın xc16-ld tarafından DMA-uyumlu (aligned) olarak atanması gerekir.
 * Not: Uygulama sırasında linker script veya __attribute__((space(dma))) kullanılır.
 */
/*
__eds__ uint16_t ECAN1_MsgBuf[4][8] __attribute__((space(eds), aligned(128)));
*/

void CAN1_Initialize(void)
{
    // 1. Modülü Yapılandırma Moduna Al
    C1CTRL1bits.REQOP = 4;
    while(C1CTRL1bits.OPMODE != 4);

    // 2. Baud Rate Ayarları (70 MIPS için 1Mbps Örneği)
    C1CFG1 = 0x0003; // BRP = 3
    C1CFG2 = 0x0290; // PH2=3, PH1=3, PR=1

    // 3. Filtre ve Maske Ayarları
    C1FMSKSEL1 = 0x0000; // Mask 0 kullan
    C1BUFPNT1 = 0x0000;  // Filtre 0 -> Buffer 0

    // 4. DMA üzerinden Mesaj Penceresi Ayarları (dsPIC33EP'ye özel)
    // C1FCTRLbits.FSA = 0; // FIFO başlangıcı
    // C1FCTRLbits.DMABS = 0; // 4 mesajlık buffer

    // 5. Normal Modu İste
    C1CTRL1bits.REQOP = 0;
    while(C1CTRL1bits.OPMODE != 0);
}

/**
 * Transmit Fonksiyonu (Taslak)
 * dsPIC33EP'de veri doğrudan C1TX registers yerine DMA RAM'e yazılır.
 */
bool CAN1_Transmit(CAN1_MSG_OBJ *msg)
{
    // Özet İşleyiş:
    // 1. DMA RAM'deki uygun Buffer'a (örn: Buffer 0) veriyi kopyala
    // 2. C1TR01CONbits.TXREQ0 = 1 set et
    // 3. Donanım veriyi DMA üzerinden çekip hatta basacaktır.

    return false; // DMA entegrasyonu sonrası true dönecek
}

bool CAN1_Receive(CAN1_MSG_OBJ *msg)
{
    // Özet İşleyiş:
    // 1. C1RXFUL1 bitlerini kontrol et
    // 2. Mesaj varsa DMA RAM'den oku
    // 3. Bit'i temizle (C1RXFUL1bits.RXFUL0 = 0)

    return false;
}

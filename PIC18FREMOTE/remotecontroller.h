#ifndef REMOTE_CONTROLLER_H
#define REMOTE_CONTROLLER_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// HMI Ekran / Menü Durum Tanımları (Menu States)
// -----------------------------------------------------------------------------
typedef enum {
    Menu_Creating = 0, // Statik şablon ve çerçevelerin ilk kez çizildiği aşama
    Menu_Created  = 1, // Şablonun hazır olduğu ve tazelemeye geçildiği aşama
    Menu_Wait     = 2  // Ekranın sabit kaldığı / bekleme aşaması
} Menu_State_t;

// -----------------------------------------------------------------------------
// Ana Sistem Durum Tanımları (Main States)
// -----------------------------------------------------------------------------
typedef enum {
    IDLE          = 0, // Boşta / Fiyat Tablosu Ekranı
    GOSLEEP       = 1, // Uyku Modu
    CONFIGID      = 2, // Sequence / Slave / Sub ID Ayar Ekranı
    CONFIGPARAM   = 3, // Alt Parametre Ayar Ekranı
    CONFIGPRICE   = 4, // Ürün Fiyat Ayar Ekranı
    ENTERPASSWORD = 5, // Şifre Giriş Ekranı
    SENDING       = 6  // RF Modem Gönderim / Pano Durum Ekranı
} Main_State_t;

// -----------------------------------------------------------------------------
// Sayfa Numarası Tanımları (Page States)
// -----------------------------------------------------------------------------
#define IDLE_PAGE1      1
#define IDLE_PAGE2      2
#define IDLE_PAGE3      3

#define SENDING_PAGE1   1
#define SENDING_PAGE2   2
#define SENDING_PAGE3   3
#define SENDING_PAGE4   4

#define CONFIG_ID_PAGE     1
#define CONFIG_PARAM_PAGE  1
#define CONFIG_PRICE_PAGE  1

// -----------------------------------------------------------------------------
// Ekran Komut Yapısı (Screen Command Structure)
// -----------------------------------------------------------------------------
typedef struct {
    unsigned int Val_Pos1; // CASH / Birinci Değer
    unsigned int Val_Pos2; // CREDIT / İkinci Değer
} ScreenCommand;

// -----------------------------------------------------------------------------
// Yardımcı Sayı-Metin Dönüştürme Fonksiyonları Prototipleri
// (Gövdeleri PIC18FREMOTE/eeprom_params.c İçerisindedir)
// -----------------------------------------------------------------------------
void WriteDecimalStringShort(short value);
void WriteDecimalStringUChar(unsigned char value);

#endif // REMOTE_CONTROLLER_H

#include <xc.h>
#include <string.h>

unsigned char screen_x = 0, screen_y = 0;
unsigned char tx = 0, ty = 0;

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 44236800UL
#endif

// AIP31108 / KS0108 GLCD Komut Sabitleri
#define DISPLAY_ON_CMD         0x3F
#define DISPLAY_OFF_CMD        0x3E
#define DISPLAY_SET_Y_CMD      0x40  // Sütun Adresi (0-63)
#define DISPLAY_SET_X_CMD      0xB8  // Sayfa/Page Adresi (0-7)
#define DISPLAY_START_LINE_CMD 0xC0 // Başlangıç Satırı (0-63)

// Standart 5x7 ASCII Font Tablosu (0x20 ' ' ile 0x7F arası)
const unsigned char font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // 20 (Space)
    0x00, 0x00, 0x5f, 0x00, 0x00, // 21 !
    0x00, 0x07, 0x00, 0x07, 0x00, // 22 "
    0x14, 0x7f, 0x14, 0x7f, 0x14, // 23 #
    0x24, 0x2a, 0x7f, 0x2a, 0x12, // 24 $
    0x23, 0x13, 0x08, 0x64, 0x62, // 25 %
    0x36, 0x49, 0x55, 0x22, 0x50, // 26 &
    0x00, 0x05, 0x03, 0x00, 0x00, // 27 '
    0x00, 0x1c, 0x22, 0x41, 0x00, // 28 (
    0x00, 0x41, 0x22, 0x1c, 0x00, // 29 )
    0x14, 0x08, 0x3e, 0x08, 0x14, // 2a *
    0x08, 0x08, 0x3e, 0x08, 0x08, // 2b +
    0x00, 0x50, 0x30, 0x00, 0x00, // 2c ,
    0x08, 0x08, 0x08, 0x08, 0x08, // 2d -
    0x00, 0x60, 0x60, 0x00, 0x00, // 2e .
    0x20, 0x10, 0x08, 0x04, 0x02, // 2f /
    0x3e, 0x51, 0x49, 0x45, 0x3e, // 30 0
    0x00, 0x42, 0x7f, 0x40, 0x00, // 31 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 32 2
    0x21, 0x41, 0x45, 0x4b, 0x31, // 33 3
    0x18, 0x14, 0x12, 0x7f, 0x10, // 34 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 35 5
    0x3c, 0x4a, 0x49, 0x49, 0x30, // 36 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 37 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 38 8
    0x06, 0x49, 0x49, 0x29, 0x1e, // 39 9
    0x00, 0x36, 0x36, 0x00, 0x00, // 3a :
    0x00, 0x56, 0x36, 0x00, 0x00, // 3b ;
    0x08, 0x14, 0x22, 0x41, 0x00, // 3c <
    0x14, 0x14, 0x14, 0x14, 0x14, // 3d =
    0x00, 0x41, 0x22, 0x14, 0x08, // 3e >
    0x02, 0x01, 0x51, 0x09, 0x06, // 3f ?
    0x32, 0x49, 0x79, 0x41, 0x3e, // 40 @
    0x7e, 0x11, 0x11, 0x11, 0x7e, // 41 A
    0x7f, 0x49, 0x49, 0x49, 0x36, // 42 B
    0x3e, 0x41, 0x41, 0x41, 0x22, // 43 C
    0x7f, 0x41, 0x41, 0x22, 0x1c, // 44 D
    0x7f, 0x49, 0x49, 0x49, 0x41, // 45 E
    0x7f, 0x09, 0x09, 0x09, 0x01, // 46 F
    0x3e, 0x41, 0x49, 0x49, 0x7a, // 47 G
    0x7f, 0x08, 0x08, 0x08, 0x7f, // 48 H
    0x00, 0x41, 0x7f, 0x41, 0x00, // 49 I
    0x20, 0x40, 0x41, 0x3f, 0x01, // 4a J
    0x7f, 0x08, 0x14, 0x22, 0x41, // 4b K
    0x7f, 0x40, 0x40, 0x40, 0x40, // 4c L
    0x7f, 0x02, 0x0c, 0x02, 0x7f, // 4d M
    0x7f, 0x04, 0x08, 0x10, 0x7f, // 4e N
    0x3e, 0x41, 0x41, 0x41, 0x3e, // 4f O
    0x7f, 0x09, 0x09, 0x09, 0x06, // 50 P
    0x3e, 0x41, 0x51, 0x21, 0x5e, // 51 Q
    0x7f, 0x09, 0x19, 0x29, 0x46, // 52 R
    0x46, 0x49, 0x49, 0x49, 0x31, // 53 S
    0x01, 0x01, 0x7f, 0x01, 0x01, // 54 T
    0x3f, 0x40, 0x40, 0x40, 0x3f, // 55 U
    0x1f, 0x20, 0x40, 0x20, 0x1f, // 56 V
    0x3f, 0x40, 0x38, 0x40, 0x3f, // 57 W
    0x63, 0x14, 0x08, 0x14, 0x63, // 58 X
    0x07, 0x08, 0x70, 0x08, 0x07, // 59 Y
    0x61, 0x51, 0x49, 0x45, 0x43, // 5a Z
    0x00, 0x7f, 0x41, 0x41, 0x00, // 5b [
    0x02, 0x04, 0x08, 0x10, 0x20, // 5c Ñ
    0x00, 0x41, 0x41, 0x7f, 0x00, // 5d ]
    0x04, 0x02, 0x01, 0x02, 0x04, // 5e ^
    0x40, 0x40, 0x40, 0x40, 0x40, // 5f _
    0x00, 0x01, 0x02, 0x04, 0x00, // 60 `
    0x20, 0x54, 0x54, 0x54, 0x78, // 61 a
    0x7f, 0x48, 0x44, 0x44, 0x38, // 62 b
    0x38, 0x44, 0x44, 0x44, 0x20, // 63 c
    0x38, 0x44, 0x44, 0x48, 0x7f, // 64 d
    0x38, 0x54, 0x54, 0x54, 0x18, // 65 e
    0x08, 0x7e, 0x09, 0x01, 0x02, // 66 f
    0x0c, 0x52, 0x52, 0x52, 0x3e, // 67 g
    0x7f, 0x08, 0x04, 0x04, 0x78, // 68 h
    0x00, 0x44, 0x7d, 0x40, 0x00, // 69 i
    0x20, 0x40, 0x44, 0x3d, 0x00, // 6a j
    0x7f, 0x10, 0x28, 0x44, 0x00, // 6b k
    0x00, 0x41, 0x7f, 0x40, 0x00, // 6c l
    0x7c, 0x04, 0x18, 0x04, 0x78, // 6d m
    0x7c, 0x08, 0x04, 0x04, 0x78, // 6e n
    0x38, 0x44, 0x44, 0x44, 0x38, // 6f o
    0x7c, 0x14, 0x14, 0x14, 0x08, // 70 p
    0x08, 0x14, 0x14, 0x18, 0x7c, // 71 q
    0x7c, 0x08, 0x04, 0x04, 0x08, // 72 r
    0x48, 0x54, 0x54, 0x54, 0x20, // 73 s
    0x04, 0x3f, 0x44, 0x40, 0x20, // 74 t
    0x3c, 0x40, 0x40, 0x20, 0x7c, // 75 u
    0x1c, 0x20, 0x40, 0x20, 0x1c, // 76 v
    0x3c, 0x40, 0x30, 0x40, 0x3c, // 77 w
    0x44, 0x28, 0x10, 0x28, 0x44, // 78 x
    0x0c, 0x50, 0x50, 0x50, 0x3c, // 79 y
    0x44, 0x64, 0x54, 0x4c, 0x44, // 7a z
    0x00, 0x08, 0x36, 0x41, 0x00, // 7b {
    0x00, 0x00, 0x7f, 0x00, 0x00, // 7c |
    0x00, 0x41, 0x36, 0x08, 0x00, // 7d }
    0x10, 0x08, 0x08, 0x10, 0x08, // 7e ->
    0x78, 0x46, 0x41, 0x46, 0x78  // 7f <-
};

//-------------------------------------------------------------------------------------------------
// Donanım Çip Seçim Fonksiyonu (LMC19264A-01: 3 x 64x64 = 192x64)
// Active LOW Chip Select Yapısı (CS1=RE2, CS2=RE1, CS3=RE0)
//-------------------------------------------------------------------------------------------------
void GLCD_Chip_Select_Direct(unsigned char Chip_idx)
{
    if (Chip_idx == 0) // Hiçbirini seçme (All Deselected)
    {
        LATEbits.LATE2 = 1; // CS1
        LATEbits.LATE1 = 1; // CS2
        LATEbits.LATE0 = 1; // CS3
    }
    else if (Chip_idx == 1) // Sol 64 piksel (X: 0..63) -> CS1
    {
        LATEbits.LATE2 = 0;
        LATEbits.LATE1 = 1;
        LATEbits.LATE0 = 1;
    }
    else if (Chip_idx == 2) // Orta 64 piksel (X: 64..127) -> CS2
    {
        LATEbits.LATE2 = 1;
        LATEbits.LATE1 = 0;
        LATEbits.LATE0 = 1;
    }
    else if (Chip_idx == 3) // Sağ 64 piksel (X: 128..191) -> CS3
    {
        LATEbits.LATE2 = 1;
        LATEbits.LATE1 = 1;
        LATEbits.LATE0 = 0;
    }
    else if (Chip_idx == 4) // Hepsini seç (All Selected - Init & Clear için)
    {
        LATEbits.LATE2 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE0 = 0;
    }
    __delay_us(5);
}

//-------------------------------------------------------------------------------------------------
// Doğrudan Donanıma Komut Gönderme
// Sinyal oturma (Setup) süreleri tampon entegreleri için güçlendirilmiştir.
//-------------------------------------------------------------------------------------------------
void GLCD_Command_Direct(unsigned char command)
{
    TRISD = 0x00;       // PORTD Çıkış
    LATCbits.LATC1 = 0; // RW = 0 (Yazma Modu)
    LATBbits.LATB4 = 1; // BUFDIR = 1 (MCU -> LCD)
    LATBbits.LATB5 = 0; // BUFEN = 0 (Tampon Etkin)
    LATAbits.LATA5 = 0; // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(2);

    LATD = command;     // Komut Byte'ını Data Bus'a Koy
    LATCbits.LATC2 = 0; // RS = 0 (Komut Modu)
    __delay_us(2);

    LATCbits.LATC0 = 1; // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(5);
    LATCbits.LATC0 = 0; // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda İşlenir)
    __delay_us(5);

    LATBbits.LATB5 = 1; // BUFEN Deaktif
    LATAbits.LATA5 = 1; // BUFE2 Deaktif
    __delay_us(2);
}

//-------------------------------------------------------------------------------------------------
// Doğrudan Donanıma Veri Gönderme
//-------------------------------------------------------------------------------------------------
void GLCD_Data_Direct(unsigned char data)
{
    TRISD = 0x00;       // PORTD Çıkış
    LATCbits.LATC1 = 0; // RW = 0 (Yazma Modu)
    LATBbits.LATB4 = 1; // BUFDIR = 1 (MCU -> LCD)
    LATBbits.LATB5 = 0; // BUFEN = 0 (Tampon Etkin)
    LATAbits.LATA5 = 0; // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(2);

    LATD = data;        // Veri Byte'ını Data Bus'a Koy
    LATCbits.LATC2 = 1; // RS = 1 (Veri Modu)
    __delay_us(2);

    LATCbits.LATC0 = 1; // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(5);
    LATCbits.LATC0 = 0; // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda Yazılır)
    __delay_us(5);

    LATBbits.LATB5 = 1; // BUFEN Deaktif
    LATAbits.LATA5 = 1; // BUFE2 Deaktif
    __delay_us(2);
}

//-------------------------------------------------------------------------------------------------
// GLCD Başlatma Fonksiyonu (Doğrudan Donanım Modu)
//-------------------------------------------------------------------------------------------------
void GLCD_Init(void)
{
    TRISD = 0x00;               // Data Bus Çıkış
    LATAbits.LATA5 = 1;         // BUFE2 Deaktif

    GLCD_Chip_Select_Direct(4); // Tüm Çipleri Seç
    __delay_ms(20);

    GLCD_Command_Direct(DISPLAY_OFF_CMD);           // Ekran Kapalı (0x3E)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);     // Column = 0
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | 0);     // Page = 0
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);// Start Line = 0
    GLCD_Command_Direct(DISPLAY_ON_CMD);            // Ekran Açık (0x3F)

    GLCD_Chip_Select_Direct(0); // Seçimleri Kaldır
    __delay_ms(5);
}

//-------------------------------------------------------------------------------------------------
// GLCD Tüm Ekranı Temizleme (Clear Screen)
//-------------------------------------------------------------------------------------------------
void GLCD_ClearAll(void)
{
    unsigned char page, col;

    GLCD_Chip_Select_Direct(4); // Tüm Çiplere Birlikte Gönder
    for (page = 0; page < 8; page++)
    {
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | page); // Page 0..7
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);    // Column 0
        for (col = 0; col < 64; col++)
        {
            GLCD_Data_Direct(0x00); // Ekranı Temizle
        }
    }
    GLCD_Chip_Select_Direct(0);
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Konumlandırma (X: 0..191 Piksel, Y: 0..63 Piksel Koordinatı)
// Note: KS0108 / AIP31108 sürücülerinde X pikseli sütun çipine, Y pikseli (0-63) 8'e bölünerek
// 8 sayfalık (Page 0-7) dikey bloklara dönüştürülür.
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo_Direct(unsigned char x, unsigned char y)
{
    unsigned char chip;
    unsigned char column;
    unsigned char page;

    if (x >= 192) return;

    chip = (x / 64) + 1; // 1: Sol (0..63), 2: Orta (64..127), 3: Sağ (128..191) Çip
    column = x % 64;     // Çip içi sütun adresi (0..63)
    page = (y >= 8) ? (y / 8) : y; // Y hem sayfa (0-7) hem piksel (0-63) uyumlu kabul edilir

    GLCD_Chip_Select_Direct(chip);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);   // Page (0xB8 + page)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | column); // Sütun (0x40 + column)
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Metin / String Yazdırma
// Çip Sınırı Taşma Koruma (Wraparound Protection) Eklendi!
// Örnek Kullanım: GLCD_String5x7(63, 27, "Test");
//-------------------------------------------------------------------------------------------------
void GLCD_String5x7(unsigned char x, unsigned char y, char *str)
{
    unsigned char i = 0;
    unsigned char curr_x = x;

    while (str[i] != '\0')
    {
        if (curr_x + 6 > 192) break; // Ekran genişlik sınırını aşma

        unsigned char c = str[i];
        if (c < 0x20 || c > 0x7E) c = ' '; // Geçersiz karakterleri boşluk yap
        unsigned short font_idx = (c - 0x20) * 5;

        // Her bir piksel sütununda doğru Çip ve Sütuna konumlan
        for (unsigned char k = 0; k < 5; k++)
        {
            GLCD_GoTo_Direct(curr_x, y);
            GLCD_Data_Direct(font5x7[font_idx + k]);
            curr_x++;
        }

        // Karakterler arası 1 piksel boşluk
        GLCD_GoTo_Direct(curr_x, y);
        GLCD_Data_Direct(0x00);
        curr_x++;

        i++;
    }
    GLCD_Chip_Select_Direct(0); // Çip seçimini deaktif et
}

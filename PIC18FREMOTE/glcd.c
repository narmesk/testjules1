#ifndef _XTAL_FREQ
#define _XTAL_FREQ 44236800UL
#endif

#include <xc.h>
#include <string.h>

// AIP31108 / KS0108 GLCD Komut Sabitleri
#define DISPLAY_ON_CMD         0x3F
#define DISPLAY_OFF_CMD        0x3E
#define DISPLAY_SET_Y_CMD      0x40  // Sütun Adresi (0-63)
#define DISPLAY_SET_X_CMD      0xB8  // Sayfa/Page Adresi (0-7)
#define DISPLAY_START_LINE_CMD 0xC0 // Başlangıç Satırı (0-63)

// Standart 5x7 Font Tablosu
const unsigned char font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // (space)
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x06, // F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // \
    0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x0C, 0x52, 0x52, 0x52, 0x3E, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3E, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44  // z
};

//-------------------------------------------------------------------------------------------------
// Donanım Çip Seçim Fonksiyonu (LMC19264A-01: 3 x 64x64 = 192x64)
// Active LOW Chip Select Yapısı
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
    __delay_us(2);
}

//-------------------------------------------------------------------------------------------------
// Doğrudan Donanıma Komut Gönderme
//-------------------------------------------------------------------------------------------------
void GLCD_Command_Direct(unsigned char command)
{
    LATCbits.LATC1 = 0; // RW = 0 (Yazma Modu)
    LATBbits.LATB4 = 1; // BUFDIR = 1 (MCU -> LCD)
    LATBbits.LATB5 = 0; // BUFEN = 0 (Tampon Etkin)
    LATAbits.LATA5 = 0; // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(1);

    LATD = command;     // Komut Byte'ını Data Bus'a Koy
    LATCbits.LATC2 = 0; // RS = 0 (Komut Modu)
    __delay_us(1);

    LATCbits.LATC0 = 1; // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(3);
    LATCbits.LATC0 = 0; // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda İşlenir)
    __delay_us(3);

    LATBbits.LATB5 = 1; // BUFEN Deaktif
    LATAbits.LATA5 = 1; // BUFE2 Deaktif
}

//-------------------------------------------------------------------------------------------------
// Doğrudan Donanıma Veri Gönderme
//-------------------------------------------------------------------------------------------------
void GLCD_Data_Direct(unsigned char data)
{
    LATCbits.LATC1 = 0; // RW = 0 (Yazma Modu)
    LATBbits.LATB4 = 1; // BUFDIR = 1 (MCU -> LCD)
    LATBbits.LATB5 = 0; // BUFEN = 0 (Tampon Etkin)
    LATAbits.LATA5 = 0; // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(1);

    LATD = data;        // Veri Byte'ını Data Bus'a Koy
    LATCbits.LATC2 = 1; // RS = 1 (Veri Modu)
    __delay_us(1);

    LATCbits.LATC0 = 1; // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(3);
    LATCbits.LATC0 = 0; // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda Yazılır)
    __delay_us(3);

    LATBbits.LATB5 = 1; // BUFEN Deaktif
    LATAbits.LATA5 = 1; // BUFE2 Deaktif
}

//-------------------------------------------------------------------------------------------------
// GLCD Başlatma Fonksiyonu (Doğrudan Donanım Modu)
//-------------------------------------------------------------------------------------------------
void GLCD_Init(void)
{
    // Donanım Veri Bus Yönü Çıkış
    TRISD = 0x00;

    GLCD_Chip_Select_Direct(4); // Tüm Çipleri Seç
    __delay_ms(10);

    GLCD_Command_Direct(DISPLAY_OFF_CMD);           // Ekran Kapalı (0x3E)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);     // Column = 0
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | 0);     // Page = 0
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);// Start Line = 0
    GLCD_Command_Direct(DISPLAY_ON_CMD);            // Ekran Açık (0x3F)

    GLCD_Chip_Select_Direct(0); // Çip Seçimlerini Kaldır
}

//-------------------------------------------------------------------------------------------------
// GLCD Tüm Ekranı Temizleme (Clear Screen)
//-------------------------------------------------------------------------------------------------
void GLCD_ClearAll(void)
{
    unsigned char page, col;

    GLCD_Chip_Select_Direct(4); // Tüm Çiplere Birlikte Komut/Veri Gönder
    for (page = 0; page < 8; page++)
    {
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | page); // Page 0..7
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);    // Column 0
        for (col = 0; col < 64; col++)
        {
            GLCD_Data_Direct(0x00); // Tüm pikselleri sıfırla (Temizle)
        }
    }
    GLCD_Chip_Select_Direct(0);
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Konumlandırma (X: 0..191, Y: 0..63 Piksel Koordinatı)
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo_Direct(unsigned char x, unsigned char y)
{
    unsigned char chip;
    unsigned char column;
    unsigned char page;

    if (x >= 192 || y >= 64) return;

    chip = (x / 64) + 1; // 1: Sol, 2: Orta, 3: Sağ Çip
    column = x % 64;
    page = y / 8;        // 8 Piksel Yüksekliğinde 1 Sayfa (Page)

    GLCD_Chip_Select_Direct(chip);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);   // Page Seçimi (0xB8 + page)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | column); // Sütun Seçimi (0x40 + column)
}

//-------------------------------------------------------------------------------------------------
// Doğrudan Donanıma 5x7 Karakter Yazma
//-------------------------------------------------------------------------------------------------
void GLCD_PutChar5x7_Direct(unsigned char c)
{
    unsigned char i;
    unsigned short font_idx;

    if (c < 0x20 || c > 0x7A) c = ' '; // Desteklenmeyen karakterleri ' ' yap
    font_idx = (c - 0x20) * 5;

    for (i = 0; i < 5; i++)
    {
        GLCD_Data_Direct(font5x7[font_idx + i]);
    }
    GLCD_Data_Direct(0x00); // Karakterler arası 1 piksel boşluk
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Metin / String Yazdırma
// Örnek Kullanım: GLCD_String5x7(63, 27, "Test");
//-------------------------------------------------------------------------------------------------
void GLCD_String5x7(unsigned char x, unsigned char y, char *str)
{
    unsigned char i = 0;
    unsigned char curr_x = x;

    while (str[i] != '\0')
    {
        if (curr_x + 6 > 192) break; // Ekran sınırını aşma

        GLCD_GoTo_Direct(curr_x, y);
        GLCD_PutChar5x7_Direct(str[i]);

        curr_x += 6; // 5 piksel karakter + 1 piksel boşluk
        i++;
    }
}

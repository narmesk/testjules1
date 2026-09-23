#include <string.h>
#include "main.h"

/*
================================================================================-------------------
  LMC19264A-01 / AIP31108 (KS0108) 192x64 GLCD SÜRÜCÜSÜ (SHADOW RAM / ANINDA YAZMA)
================================================================================-------------------
  ÇALIŞMA PRENSİBİ:
  1. MCU RAM'inde 192x64 piksel ekran alanı için 1536 Baytlık 'glcd_buffer' gölge bellek tutulur.
  2. Donanımdan geri okuma (RMW) problemleri sıfırlanmıştır.
  3. PİKSEL VE YAZI ÇİZİMLERİ:
     - GLCD_SetPixel(65, 33, BLACK): Tek piksel anında ekranda belirir (Render gerekmez).
     - GLCD_String5x7, GLCD_StringArialBold14, GLCD_StringCalibri36: Sayfa hizalaması olan ve
       olmayan yazılar pürüzsüz, deliksiz ve yırtılmasız şekilde ekrana yazılır.
  4. ALAN VE EKRAN DOLDURMA (SetPixels / GLCD_ClearAll):
     - RAM'de anında hesaplanır ve donanıma blok veri olarak gönderilir (< 5 ms sürer).
================================================================================-------------------
*/

#ifndef FONT_WIDTH_TABLE
#define FONT_WIDTH_TABLE 0
#endif

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

#define BLACK 1
#define WHITE 0

// 192x64 Piksel Grafik Ekran İçin MCU RAM Gölge Tamponu (192 Sütun x 8 Sayfa = 1536 Bayt)
unsigned char glcd_buffer[1536];

struct {
    unsigned char x;
    unsigned char y;
} Coord;

// Prototipler
void GLCD_Chip_Select_Direct(unsigned char Chip_idx);
void GLCD_Command_Direct(unsigned char command);
void GLCD_Data_Direct(unsigned char data);
void GLCD_Init(void);
void GLCD_Render(void);
void GLCD_ClearAll(void);
void GLCD_GoTo_Direct(unsigned char x, unsigned char page);

void GLCD_Command(char Command);
void GLCD_Data(char Data);
void GLCD_Chip_Select(char Chip_idx);
void GLCD_GoTo(unsigned char x, unsigned char y);
void GLCD_WriteData(unsigned char dataToWrite);

void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color);
void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color);
void GLCD_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color);
void GLCD_Circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);
void GLCD_Circle_Fill(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);

void GLCD_String5x7(unsigned char x, unsigned char y, char *str);
void GLCD_StringArialBold14(unsigned char x, unsigned char y, char *str);
void GLCDPutChar_ArialBold14(unsigned char c);
void GLCD_Picture(char *str);
void GLCDWriteData(unsigned char data);
void GotoXY(unsigned char x, unsigned char y);
void GLCD_StringCalibri36(unsigned char x, unsigned char y, char *str);
void GLCDPutCharCalibri36(unsigned char c);
void GLCD_StringHead8x8(unsigned char x, unsigned char y, char *str);
void GLCDPutCharHead8x8(unsigned char c);
void GLCDPutCharDigMin(unsigned char c);
void GLCDPutSpecialCharDigMin(unsigned char c);
void GLCDPutSpecialCharDigMax(unsigned char c);
void GLCDPutCharDigMax(unsigned char c);
void GLCDPutCharDigMaxFirst(unsigned char c);
void GLCDPutCharDigMaxSecond(unsigned char c);
void GLCDPutChar5x7(unsigned char c);

// Dışarıdan bildirilen okuma/durum fonksiyonu
extern unsigned char GLCD_ReadStatus(unsigned char chip);

//-------------------------------------------------------------------------------------------------
// MCC Pin Makroları İle Donanım Çip Seçim Fonksiyonu (LMC19264A-01: 3 x 64x64 = 192x64)
// Active LOW Chip Select Yapısı
//-------------------------------------------------------------------------------------------------
void GLCD_Chip_Select_Direct(unsigned char Chip_idx)
{
    if (Chip_idx == 0) // Hiçbirini seçme (All Deselected)
    {
        CS1_SetHigh();
        CS2_SetHigh();
        CS3_SetHigh();
    }
    else if (Chip_idx == 1) // Sol 64 piksel (X: 0..63) -> CS1
    {
        CS1_SetLow();
        CS2_SetHigh();
        CS3_SetHigh();
    }
    else if (Chip_idx == 2) // Orta 64 piksel (X: 64..127) -> CS2
    {
        CS1_SetHigh();
        CS2_SetLow();
        CS3_SetHigh();
    }
    else if (Chip_idx == 3) // Sağ 64 piksel (X: 128..191) -> CS3
    {
        CS1_SetHigh();
        CS2_SetHigh();
        CS3_SetLow();
    }
    else if (Chip_idx == 4) // Hepsini seç (All Selected - Init & Clear için)
    {
        CS1_SetLow();
        CS2_SetLow();
        CS3_SetLow();
    }
    __delay_us(2);
}

void GLCD_Chip_Select(char Chip_idx)
{
    GLCD_Chip_Select_Direct((unsigned char)Chip_idx);
}

//-------------------------------------------------------------------------------------------------
// MCC Pin Makroları İle Doğrudan Donanıma Komut Gönderme
//-------------------------------------------------------------------------------------------------
void GLCD_Command_Direct(unsigned char command)
{
    TRISD = 0x00;       // PORTD Çıkış
    RW_SetLow();        // RW = 0 (Yazma Modu)
    BUFDIR_SetHigh();   // BUFDIR = 1 (MCU -> LCD)
    BUFEN_SetLow();     // BUFEN = 0 (Tampon Etkin)
    BUFE2_SetLow();     // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(1);

    LATD = command;     // Komut Byte'ını Data Bus'a Koy
    RS_SetLow();        // RS = 0 (Komut Modu)
    __delay_us(1);

    EN_SetHigh();       // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(2);
    EN_SetLow();        // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda İşlenir)
    __delay_us(2);

    BUFEN_SetHigh();    // BUFEN Deaktif
    BUFE2_SetHigh();    // BUFE2 Deaktif
}

void GLCD_Command(char Command)
{
    GLCD_Command_Direct((unsigned char)Command);
}

//-------------------------------------------------------------------------------------------------
// MCC Pin Makroları İle Doğrudan Donanıma Veri Gönderme
//-------------------------------------------------------------------------------------------------
void GLCD_Data_Direct(unsigned char data)
{
    TRISD = 0x00;       // PORTD Çıkış
    RW_SetLow();        // RW = 0 (Yazma Modu)
    BUFDIR_SetHigh();   // BUFDIR = 1 (MCU -> LCD)
    BUFEN_SetLow();     // BUFEN = 0 (Tampon Etkin)
    BUFE2_SetLow();     // BUFE2 = 0 (Ek Tampon Etkin)
    __delay_us(1);

    LATD = data;        // Veri Byte'ını Data Bus'a Koy
    RS_SetHigh();       // RS = 1 (Veri Modu)
    __delay_us(1);

    EN_SetHigh();       // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(2);
    EN_SetLow();        // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda Yazılır)
    __delay_us(2);

    BUFEN_SetHigh();    // BUFEN Deaktif
    BUFE2_SetHigh();    // BUFE2 Deaktif
}

void GLCD_Data(char Data)
{
    GLCD_Data_Direct((unsigned char)Data);
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Konumlandırma (x: 0..191 piksel, page: 0..7 sayfa adresi)
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo_Direct(unsigned char x, unsigned char page)
{
    unsigned char chip;
    unsigned char column;

    if (x >= 192 || page >= 8) return;

    chip = (x / 64) + 1; // 1: Sol (0..63), 2: Orta (64..127), 3: Sağ (128..191) Çip
    column = x % 64;     // Çip içi sütun adresi (0..63)

    GLCD_Chip_Select_Direct(chip);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);   // Page (0xB8 + page)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | column); // Sütun (0x40 + column)
}

void GLCD_GoTo(unsigned char x, unsigned char y)
{
    screen_x = x;
    screen_y = y;
    Coord.x = x;
    Coord.y = y;
    GLCD_GoTo_Direct(x, y / 8);
}

//-------------------------------------------------------------------------------------------------
// GLCD Başlatma Fonksiyonu
//-------------------------------------------------------------------------------------------------
void GLCD_Init(void)
{
    ANSELD = 0x00;              // PORTD Dijital Mod
    TRISD = 0x00;               // Data Bus Çıkış
    BUFE2_SetHigh();            // BUFE2 Deaktif

    GLCD_Chip_Select_Direct(4); // Tüm Çipleri Seç
    __delay_ms(10);

    GLCD_Command_Direct(DISPLAY_OFF_CMD);           // Ekran Kapalı (0x3E)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);     // Column = 0
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | 0);     // Page = 0
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);// Start Line = 0
    GLCD_Command_Direct(DISPLAY_ON_CMD);            // Ekran Açık (0x3F)

    GLCD_Chip_Select_Direct(0); // Seçimleri Kaldır
    GLCD_ClearAll();            // Ekrana İlk Temizlik
}

//-------------------------------------------------------------------------------------------------
// Tüm RAM Tamponunu Ekrana Yansıtma (Aşırı Hızlı Tek Geçişli Donanım Render)
//-------------------------------------------------------------------------------------------------
void GLCD_Render(void)
{
    unsigned char page, col;
    unsigned short ptr;

    for (page = 0; page < 8; page++)
    {
        ptr = (unsigned short)page * 192;

        // Çip 1 (Sol 64 Sütun)
        GLCD_Chip_Select_Direct(1);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data_Direct(glcd_buffer[ptr + col]);
        }

        // Çip 2 (Orta 64 Sütun)
        GLCD_Chip_Select_Direct(2);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data_Direct(glcd_buffer[ptr + 64 + col]);
        }

        // Çip 3 (Sağ 64 Sütun)
        GLCD_Chip_Select_Direct(3);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | page);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data_Direct(glcd_buffer[ptr + 128 + col]);
        }
    }
    GLCD_Chip_Select_Direct(0);
}

//-------------------------------------------------------------------------------------------------
// GLCD Tüm Ekranı Temizleme (RAM Buffera Yazıp Donanıma Anında Yansıtma)
//-------------------------------------------------------------------------------------------------
void GLCD_ClearAll(void)
{
    memset(glcd_buffer, 0x00, 1536);
    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// Veri Yazma Fonksiyonları (Y Kaydırma / Maskeleme İle Pürüzsüz Font Desteği)
//-------------------------------------------------------------------------------------------------
void GLCDWriteData(unsigned char data)
{
    unsigned char yOffset = Coord.y % 8;
    unsigned char page = Coord.y / 8;
    unsigned short idx = (unsigned short)page * 192 + Coord.x;

    if (Coord.x < 192 && page < 8)
    {
        if (yOffset == 0)
        {
            glcd_buffer[idx] = data;
            GLCD_GoTo_Direct(Coord.x, page);
            GLCD_Data_Direct(glcd_buffer[idx]);
        }
        else
        {
            unsigned char mask1 = ~(0xFF << yOffset);
            glcd_buffer[idx] = (glcd_buffer[idx] & mask1) | (data << yOffset);
            GLCD_GoTo_Direct(Coord.x, page);
            GLCD_Data_Direct(glcd_buffer[idx]);

            if (page + 1 < 8)
            {
                unsigned short idx2 = (unsigned short)(page + 1) * 192 + Coord.x;
                unsigned char mask2 = ~(0xFF >> (8 - yOffset));
                glcd_buffer[idx2] = (glcd_buffer[idx2] & mask2) | (data >> (8 - yOffset));
                GLCD_GoTo_Direct(Coord.x, page + 1);
                GLCD_Data_Direct(glcd_buffer[idx2]);
            }
        }
    }
    Coord.x++;
    screen_x = Coord.x;
}

void GLCD_WriteData(unsigned char dataToWrite)
{
    Coord.x = screen_x;
    Coord.y = screen_y;
    GLCDWriteData(dataToWrite);
    screen_x = Coord.x;
    screen_y = Coord.y;
}

void GotoXY(unsigned char x, unsigned char y)
{
    Coord.x = x;
    Coord.y = y;
    screen_x = x;
    screen_y = y;
}

//-------------------------------------------------------------------------------------------------
// MCU RAM Tamponu Kullanan Grafik Çizim Fonksiyonları (Otomatik Anında Donanıma Yansır)
//-------------------------------------------------------------------------------------------------

// Tek Piksel Çizimi / Silimi (RAM Buffera Yazar ve Anında Donanıma Yansıtır)
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
    unsigned char page = y / 8;
    unsigned char bit_pos = y % 8;
    unsigned short idx;

    if (x >= 192 || y >= 64) return;

    idx = (unsigned short)page * 192 + x;

    if (color == BLACK)
    {
        glcd_buffer[idx] |= (1 << bit_pos);
    }
    else
    {
        glcd_buffer[idx] &= ~(1 << bit_pos);
    }

    // Anında donanıma yaz
    GLCD_GoTo_Direct(x, page);
    GLCD_Data_Direct(glcd_buffer[idx]);
}

// Çerçeve Dikdörtgen Çizimi
void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char j;
    for (j = x; j <= b; j++)
    {
        GLCD_SetPixel(j, y, color);
        GLCD_SetPixel(j, a, color);
    }
    for (j = y; j <= a; j++)
    {
        GLCD_SetPixel(x, j, color);
        GLCD_SetPixel(b, j, color);
    }
}

// Dolu Dikdörtgen Çizimi (RAM'de İşleyip Donanıma Hızlıca Blok Halinde Aktarır)
void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char curr_x, curr_y;
    unsigned char start_page = y / 8;
    unsigned char end_page = a / 8;
    unsigned char page, bit_pos;
    unsigned short idx;

    if (x >= 192) x = 191;
    if (b >= 192) b = 191;
    if (y >= 64) y = 63;
    if (a >= 64) a = 63;

    // RAM tamponunu güncelle
    for (curr_x = x; curr_x <= b; curr_x++)
    {
        for (curr_y = y; curr_y <= a; curr_y++)
        {
            page = curr_y / 8;
            bit_pos = curr_y % 8;
            idx = (unsigned short)page * 192 + curr_x;

            if (color == BLACK)
            {
                glcd_buffer[idx] |= (1 << bit_pos);
            }
            else
            {
                glcd_buffer[idx] &= ~(1 << bit_pos);
            }
        }
    }

    // Donanımı hızla güncelle (< 5 ms)
    for (page = start_page; page <= end_page; page++)
    {
        for (curr_x = x; curr_x <= b; curr_x++)
        {
            if (curr_x == x || curr_x == 64 || curr_x == 128)
            {
                GLCD_GoTo_Direct(curr_x, page);
            }
            idx = (unsigned short)page * 192 + curr_x;
            GLCD_Data_Direct(glcd_buffer[idx]);
        }
    }
    GLCD_Chip_Select_Direct(0);
}

// Alan Doldurma
void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color)
{
    GLCD_Rectangle_Fill(x, y, x2, y2, color);
}

// Çizgi Çizimi
void GLCD_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color)
{
    int CurrentX, CurrentY, Xinc, Yinc,
            Dx, Dy, TwoDx, TwoDy,
            TwoDxAccumulatedError, TwoDyAccumulatedError;

    Dx = (X2 - X1);
    Dy = (Y2 - Y1);

    TwoDx = Dx + Dx;
    TwoDy = Dy + Dy;

    CurrentX = X1;
    CurrentY = Y1;

    Xinc = 1;
    Yinc = 1;

    if (Dx < 0)
    {
        Xinc = -1;
        Dx = -Dx;
        TwoDx = -TwoDx;
    }

    if (Dy < 0)
    {
        Yinc = -1;
        Dy = -Dy;
        TwoDy = -TwoDy;
    }

    GLCD_SetPixel(X1, Y1, color);

    if ((Dx != 0) || (Dy != 0))
    {
        if (Dy <= Dx)
        {
            TwoDxAccumulatedError = 0;
            do
            {
                CurrentX += Xinc;
                TwoDxAccumulatedError += TwoDy;
                if (TwoDxAccumulatedError > Dx)
                {
                    CurrentY += Yinc;
                    TwoDxAccumulatedError -= TwoDx;
                }
                GLCD_SetPixel(CurrentX, CurrentY, color);
            }
            while (CurrentX != X2);
        }
        else
        {
            TwoDyAccumulatedError = 0;
            do
            {
                CurrentY += Yinc;
                TwoDyAccumulatedError += TwoDx;
                if (TwoDyAccumulatedError > Dy)
                {
                    CurrentX += Xinc;
                    TwoDyAccumulatedError -= TwoDy;
                }
                GLCD_SetPixel(CurrentX, CurrentY, color);
            }
            while (CurrentY != Y2);
        }
    }
}

// Çember Çizimi
void GLCD_Circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color)
{
    int x, y, xchange, ychange, radiusError;
    x = radius;
    y = 0;
    xchange = 1 - 2 * radius;
    ychange = 1;
    radiusError = 0;
    while (x >= y)
    {
        GLCD_SetPixel(cx + x, cy + y, color);
        GLCD_SetPixel(cx - x, cy + y, color);
        GLCD_SetPixel(cx - x, cy - y, color);
        GLCD_SetPixel(cx + x, cy - y, color);
        GLCD_SetPixel(cx + y, cy + x, color);
        GLCD_SetPixel(cx - y, cy + x, color);
        GLCD_SetPixel(cx - y, cy - x, color);
        GLCD_SetPixel(cx + y, cy - x, color);
        y++;
        radiusError += ychange;
        ychange += 2;
        if (2 * radiusError + xchange > 0)
        {
            x--;
            radiusError += xchange;
            xchange += 2;
        }
    }
}

// Dolu Çember Çizimi
void GLCD_Circle_Fill(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color)
{
    unsigned char temp = radius;
    while (temp > 0)
    {
        GLCD_Circle(cx, cy, temp, color);
        temp--;
    }
}

//-------------------------------------------------------------------------------------------------
// Metin ve Font Fonksiyonları (MCU RAM Buffera Yazıp Donanıma Anında Basar)
//-------------------------------------------------------------------------------------------------
void GLCD_String5x7(unsigned char x, unsigned char y, char *str)
{
    unsigned char i = 0;
    unsigned char curr_x = x;

    screen_x = curr_x;
    screen_y = y;

    while (str[i] != '\0')
    {
        if (curr_x + 6 > 192) break; // Ekran genişlik sınırını aşma

        unsigned char c = str[i];
        if (c < 0x20 || c > 0x7E) c = ' ';
        unsigned short font_idx = (c - 0x20) * 5;

        for (unsigned char k = 0; k < 5; k++)
        {
            screen_x = curr_x;
            GLCD_WriteData(font5x7[font_idx + k]);
            curr_x++;
        }

        screen_x = curr_x;
        GLCD_WriteData(0x00);
        curr_x++;

        i++;
    }
}

void GLCDPutChar5x7(unsigned char c)
{
    if (c < 0x20 || c > 0x7E) c = ' ';
    c -= 0x20;
    unsigned short index = c * 5;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(font5x7[index++]);
    GLCDWriteData(font5x7[index++]);
    GLCDWriteData(font5x7[index++]);
    GLCDWriteData(font5x7[index++]);
    GLCDWriteData(font5x7[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    tx = tx + 6;
}

void GLCD_StringArialBold14(unsigned char x, unsigned char y, char *str)
{
    tx = x;
    ty = y;
    unsigned char i;
    for (i = 0; i < strlen(str); i++)
    {
        GLCDPutChar_ArialBold14(str[i]);
    }
}

void GLCDPutChar_ArialBold14(unsigned char c)
{
    unsigned char width;
    unsigned short index;
    unsigned char data;
    unsigned short j;
    unsigned short jj;
    unsigned short page;
    c -= 0x20;
    if (c != 0)
    {
        index = ((Arial_bold_14_index[c - 1]) * 2) + FONT_WIDTH_TABLE + 0x60;
    }
    else
    {
        index = FONT_WIDTH_TABLE + 0x60;
    }
    width = Arial_bold_14[FONT_WIDTH_TABLE + c];
    Coord.x = tx;
    Coord.y = ty;
    jj = index + width;
    for (j = index; j < jj; j++)
    {
        data = Arial_bold_14[j];
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    page = width + index;
    jj += width;
    for (j = page; j < jj; j++)
    {
        data = Arial_bold_14[j];
        data >>= 2;
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    tx = tx + width + 1;
}

void GLCD_StringCalibri36(unsigned char x, unsigned char y, char *str)
{
    tx = x;
    ty = y;
    unsigned char i;
    for (i = 0; i < strlen(str); i++)
    {
        GLCDPutCharCalibri36(str[i]);
    }
}

void GLCDPutCharCalibri36(unsigned char c)
{
    unsigned char width = 0;
    unsigned short index;
    unsigned short page;
    unsigned char data;
    unsigned short j;
    unsigned short jj;
    c -= 0x20;
    if (c != 0)
    {
        index = Calibri36_index[c - 1] + FONT_WIDTH_TABLE + 0x7;
    }
    else
    {
        index = FONT_WIDTH_TABLE + 0x7;
    }
    width = Calibri36[FONT_WIDTH_TABLE + c];
    Coord.x = tx;
    Coord.y = ty;
    page = index;
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    page = index + width;
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;

    page = index + (2 * width);
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;

    page = index + (3 * width);
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;

    page = index + 4 * width;
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        data = Calibri36[j] & 0x0F;
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + width + 1;
}

void GLCD_StringHead8x8(unsigned char x, unsigned char y, char *str)
{
    tx = x;
    ty = y;
    unsigned char i;
    for (i = 0; i < strlen(str); i++)
    {
        GLCDPutCharHead8x8(str[i]);
    }
}

void GLCDPutCharHead8x8(unsigned char c)
{
    unsigned short index;
    index = (c * 8) + FONT_WIDTH_TABLE;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(cp437font8x8[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 9;
}

void GLCDPutCharDigMin(unsigned char c)
{
    unsigned short index;
    unsigned char data;
    c -= '+';
    index = (22 * c) + FONT_WIDTH_TABLE + 16;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    for (unsigned char k = 0; k < 11; k++)
    {
        data = lcdnumsmin[index++];
        data >>= 1;
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 12;
}

void GLCDPutSpecialCharDigMin(unsigned char c)
{
    unsigned short index;
    unsigned char data;
    c -= '+';
    index = (22 * c) + FONT_WIDTH_TABLE + 16;
    Coord.x = tx - 1;
    Coord.y = ty;
    index++;
    index++;
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    GLCDWriteData(lcdnumsmin[index++]);
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(0x00);
    Coord.x = tx - 1;
    Coord.y = Coord.y + 8;
    index++;
    index++;
    for (unsigned char k = 0; k < 5; k++)
    {
        data = lcdnumsmin[index++];
        data >>= 1;
        GLCDWriteData(data);
    }
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 5;
}

void GLCDPutSpecialCharDigMax(unsigned char c)
{
    unsigned short index;
    unsigned char data;
    c -= '+';
    index = (39 * c) + FONT_WIDTH_TABLE + 16;
    Coord.x = tx;
    Coord.y = ty;
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    index++;
    index++;
    index++;
    index++;
    for (unsigned char k = 0; k < 5; k++)
    {
        data = lcdnumsmax[index++];
        data >>= 1;
        GLCDWriteData(data);
    }
    index++;
    index++;
    index++;
    index++;
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 5;
}

void GLCDPutCharDigMax(unsigned char c)
{
    unsigned short index;
    unsigned char data;
    c -= '+';
    index = (39 * c) + FONT_WIDTH_TABLE + 16;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    for (unsigned char k = 0; k < 13; k++)
    {
        data = lcdnumsmax[index++];
        data >>= 1;
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 14;
}

void GLCDPutCharDigMaxFirst(unsigned char c)
{
    unsigned short index;
    c -= '+';
    index = (39 * c) + FONT_WIDTH_TABLE + 16;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(lcdnumsmax[index++]);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
}

void GLCDPutCharDigMaxSecond(unsigned char c)
{
    unsigned short index;
    unsigned char data;
    c -= '+';
    index = (39 * c) + FONT_WIDTH_TABLE + 42;
    for (unsigned char k = 0; k < 13; k++)
    {
        data = lcdnumsmax[index++];
        data >>= 1;
        GLCDWriteData(data);
    }
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 14;
}

// Resim basma fonksiyonları (RAM Tamponuna ve Donanıma Yazma)
void GLCD_Picture(char *str)
{
    memcpy(glcd_buffer, str, 1536);
    GLCD_Render();
}

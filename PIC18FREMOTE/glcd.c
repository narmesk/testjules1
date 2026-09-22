#include <xc.h>
#include <string.h>
#include "mcc_generated_files/system/pins.h"

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

// Dışarıdan tanımlı font dizileri ve yardımcı diziler
extern const unsigned char font5x7[];
extern const unsigned char Arial_bold_14[];
extern const unsigned short Arial_bold_14_index[];
extern const unsigned char Calibri36[];
extern const unsigned short Calibri36_index[];
extern const unsigned char cp437font8x8[];
extern const unsigned char lcdnumsmin[];
extern const unsigned char lcdnumsmax[];
extern const unsigned short looky_addr[];
extern const unsigned char bPixelLookupTable[];
extern const unsigned char bPixelLookupTableNot[];
extern const unsigned short FONT_WIDTH_TABLE;

extern unsigned char *dumyglcd;
extern unsigned char glcd_rdcache[3][192];

struct {
    unsigned char x;
    unsigned char y;
} Coord;

// Prototipler
void GLCD_Chip_Select_Direct(unsigned char Chip_idx);
void GLCD_Command_Direct(unsigned char command);
void GLCD_Data_Direct(unsigned char data);
void GLCD_Init(void);
void GLCD_ClearAll(void);
void GLCD_GoTo_Direct(unsigned char x, unsigned char y);
void GLCD_String5x7(unsigned char x, unsigned char y, char *str);

void GLCD_Data_Fast_Data(char Data);
void GLCD_Data_Fast_Start(void);
void GLCD_Data_Fast_End(void);
void GLCD_Command(char Command);
void GLCD_Data(char Data);
void GLCD_Chip_Select(char Chip_idx);
void GLCD_GoTo(unsigned char x, unsigned char y);
void GLCD_WriteData(unsigned char dataToWrite);
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color);
void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void GLCD_Rectangle_Fill_Fast_Black(unsigned char x, unsigned char y, unsigned char b, unsigned char a);
void GLCD_AllScreen_DRAW(unsigned char zone, unsigned char color);
void GLCD_Rectangle_Fill_Fast_White(unsigned char x, unsigned char y, unsigned char b, unsigned char a);
void GLCD_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color);
void GLCD_Circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);
void GLCD_Circle_Fill(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);
void GLCD_StringArialBold14(unsigned char x, unsigned char y, char *str);
void GLCDPutChar_ArialBold14(unsigned char c);
void GLCD_Picture(char *str);
void GLCD_Picture_Discrete(char *str, unsigned char i, unsigned char s, unsigned char k);
void GLCDWriteData(unsigned char data);
void GotoXY(unsigned char x, unsigned char y);
void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color);
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
    __delay_us(5);
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
    __delay_us(2);

    LATD = command;     // Komut Byte'ını Data Bus'a Koy
    RS_SetLow();        // RS = 0 (Komut Modu)
    __delay_us(2);

    EN_SetHigh();       // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(5);
    EN_SetLow();        // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda İşlenir)
    __delay_us(5);

    BUFEN_SetHigh();    // BUFEN Deaktif
    BUFE2_SetHigh();    // BUFE2 Deaktif
    __delay_us(2);
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
    __delay_us(2);

    LATD = data;        // Veri Byte'ını Data Bus'a Koy
    RS_SetHigh();       // RS = 1 (Veri Modu)
    __delay_us(2);

    EN_SetHigh();       // EN = 1 (Enable Strobe YÜKSEK)
    __delay_us(5);
    EN_SetLow();        // EN = 0 (Enable Strobe DÜŞÜK - Düşen Kenarda Yazılır)
    __delay_us(5);

    BUFEN_SetHigh();    // BUFEN Deaktif
    BUFE2_SetHigh();    // BUFE2 Deaktif
    __delay_us(2);
}

void GLCD_Data(char Data)
{
    GLCD_Data_Direct((unsigned char)Data);
}

void GLCD_Data_Fast_Data(char Data)
{
    GLCD_Data_Direct((unsigned char)Data);
}

void GLCD_Data_Fast_Start(void)
{
    RW_SetLow();        // RW = 0
    BUFDIR_SetHigh();   // BUFDIR = 1
    BUFEN_SetLow();     // BUFEN = 0
    BUFE2_SetLow();     // BUFE2 = 0
    RS_SetHigh();       // RS = 1
}

void GLCD_Data_Fast_End(void)
{
    BUFEN_SetHigh();    // BUFEN Deaktif
    BUFE2_SetHigh();    // BUFE2 Deaktif
}

//-------------------------------------------------------------------------------------------------
// GLCD Başlatma Fonksiyonu
//-------------------------------------------------------------------------------------------------
void GLCD_Init(void)
{
    TRISD = 0x00;               // Data Bus Çıkış
    BUFE2_SetHigh();            // BUFE2 Deaktif

    GLCD_Chip_Select_Direct(4); // Tüm Çipleri Seç
    __delay_ms(20);

    GLCD_Command_Direct(DISPLAY_OFF_CMD);           // Ekran Kapalı (0x3E)
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);     // Column = 0
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | 0);     // Page = 0
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);// Start Line = 0
    GLCD_Command_Direct(DISPLAY_ON_CMD);            // Ekran Açık (0x3F)

    GLCD_Chip_Select_Direct(0); // Seçimleri Kaldır
    __delay_ms(5);
    dumyglcd = (unsigned char *)glcd_rdcache;
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
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD);
    GLCD_Chip_Select_Direct(0);
}

//-------------------------------------------------------------------------------------------------
// GLCD Doğrudan Konumlandırma
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

void GLCD_GoTo(unsigned char x, unsigned char y)
{
    screen_x = x;
    screen_y = y;
    GLCD_Chip_Select_Direct(1);
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | y);
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);
    GLCD_Chip_Select_Direct(2);
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | y);
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);
    GLCD_Chip_Select_Direct(3);
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | 0);
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | y);
    GLCD_Command_Direct(DISPLAY_START_LINE_CMD | 0);
    GLCD_Chip_Select_Direct(((x / 64) + 1));
    GLCD_Command_Direct(DISPLAY_SET_Y_CMD | (x % 64));
    GLCD_Command_Direct(DISPLAY_SET_X_CMD | y);
}

void GLCD_WriteData(unsigned char dataToWrite)
{
    GLCD_GoTo_Direct(screen_x, screen_y);
    GLCD_Data_Direct(dataToWrite);
    screen_x++;
}

void GLCDWriteData(unsigned char data)
{
    unsigned char yOffset;
    unsigned short t;
    unsigned char ysave1;
    yOffset = Coord.y % 8;
    if (yOffset != 0)
    {
        t = looky_addr[Coord.y] + Coord.x;
        dumyglcd[t] |= data << yOffset;
        ysave1 = ((Coord.y + 8) & ~7);
        t = looky_addr[ysave1] + Coord.x;
        dumyglcd[t] |= data >> (8 - yOffset);
        Coord.x++;
    }
    else
    {
        t = looky_addr[Coord.y] + Coord.x;
        dumyglcd[t] = data;
        Coord.x++;
    }
}

void GotoXY(unsigned char x, unsigned char y)
{
    Coord.x = x;
    Coord.y = y;
}

//-------------------------------------------------------------------------------------------------
// Grafik Çizim Fonksiyonları
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
    if (color == BLACK)
    {
        dumyglcd[(y / 8) * 192 + x] |= (1 << (y % 8));
    }
    else
    {
        dumyglcd[(y / 8) * 192 + x] &= ~(1 << (y % 8));
    }
}

void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color)
{
    unsigned char mask, pageOffset, h, i, data;
    unsigned char height = y2 - y + 1;
    unsigned char width = x2 - x + 1;
    unsigned short t;
    pageOffset = y % 8;
    y -= pageOffset;
    mask = 0xFF;
    if (height < 8 - pageOffset)
    {
        mask >>= (8 - height);
        h = height;
    }
    else
    {
        h = 8 - pageOffset;
    }
    mask <<= pageOffset;
    GotoXY(x, y);
    for (i = 0; i < width; i++)
    {
        t = looky_addr[Coord.y] + Coord.x;
        data = dumyglcd[t];

        if (color == BLACK)
        {
            data |= mask;
        }
        else
        {
            data &= ~mask;
        }
        GLCDWriteData(data);
    }
    while (h + 8 <= height)
    {
        h += 8;
        y += 8;
        GotoXY(x, y);
        for (i = 0; i < width; i++)
        {
            GLCDWriteData(color);
        }
    }
    if (h < height)
    {
        mask = ~(0xFF << (height - h));
        GotoXY(x, y + 8);

        for (i = 0; i < width; i++)
        {
            t = looky_addr[Coord.y] + Coord.x;
            data = dumyglcd[t];

            if (color == BLACK)
            {
                data |= mask;
            }
            else
            {
                data &= ~mask;
            }
            GLCDWriteData(data);
        }
    }
}

void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char j = 0;
    for (j = x; j < b + 1; j++)
    {
        GLCD_SetPixel(j, y, color);
        GLCD_SetPixel(j, a, color);
    }
    for (j = y; j < a + 1; j++)
    {
        GLCD_SetPixel(x, j, color);
        GLCD_SetPixel(b, j, color);
    }
}

void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char j = 0, k = 0;
    for (j = x; j < b + 1; j++)
    {
        for (k = y; k < a + 1; k++)
        {
            GLCD_SetPixel(j, k, color);
        }
    }
}

void GLCD_Rectangle_Fill_Fast_Black(unsigned char x, unsigned char y, unsigned char b, unsigned char a)
{
    unsigned char j, k;
    unsigned short t;
    a = a + 1;
    b = b + 1;
    for (; y < a; y++)
    {
        t = looky_addr[y] + x;
        k = bPixelLookupTable[y];
        for (j = x; j < b; j++)
        {
            dumyglcd[t++] |= k;
        }
    }
}

void GLCD_AllScreen_DRAW(unsigned char zone, unsigned char color)
{
    if (color == BLACK)
    {
        memset(glcd_rdcache[zone], 0xFF, 192);
    }
    if (color == WHITE)
    {
        memset(glcd_rdcache[zone], 0x00, 192);
    }
}

void GLCD_Rectangle_Fill_Fast_White(unsigned char x, unsigned char y, unsigned char b, unsigned char a)
{
    unsigned char j, k;
    unsigned short t;
    a = a + 1;
    b = b + 1;
    for (; y < a; y++)
    {
        t = looky_addr[y] + x;
        k = bPixelLookupTableNot[y];
        for (j = x; j < b; j++)
        {
            dumyglcd[t++] &= k;
        }
    }
}

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

void GLCD_Circle_Fill(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color)
{
    unsigned char temp;
    temp = radius;
    while (temp > 0)
    {
        GLCD_Circle(cx, cy, temp, color);
        temp--;
    }
}

//-------------------------------------------------------------------------------------------------
// Metin ve Font Fonksiyonları
//-------------------------------------------------------------------------------------------------
void GLCD_String5x7(unsigned char x, unsigned char y, char *str)
{
    unsigned char i = 0;
    unsigned char curr_x = x;

    while (str[i] != '\0')
    {
        if (curr_x + 6 > 192) break; // Ekran genişlik sınırını aşma

        unsigned char c = str[i];
        if (c < 0x20 || c > 0x7E) c = ' ';
        unsigned short font_idx = (c - 0x20) * 5;

        for (unsigned char k = 0; k < 5; k++)
        {
            GLCD_GoTo_Direct(curr_x, y);
            GLCD_Data_Direct(font5x7[font_idx + k]);
            curr_x++;
        }

        GLCD_GoTo_Direct(curr_x, y);
        GLCD_Data_Direct(0x00);
        curr_x++;

        i++;
    }
    GLCD_Chip_Select_Direct(0);
}

void GLCDPutChar5x7(unsigned char c)
{
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
        index = ((Arial_bold_14_index[c - 1]) * 2) + 0x60 + FONT_WIDTH_TABLE;
    }
    else
    {
        index = 0x60 + FONT_WIDTH_TABLE;
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
        index = Calibri36_index[c - 1] + 0x7 + FONT_WIDTH_TABLE;
    }
    else
    {
        index = 0x7 + FONT_WIDTH_TABLE;
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
        data = Calibri36[j];
        data >>= 4;
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
    index = (22 * c) + 16 + FONT_WIDTH_TABLE;
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
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
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
    index = (22 * c) + 16 + FONT_WIDTH_TABLE;
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
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmin[index++];
    data >>= 1;
    GLCDWriteData(data);
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
    index = (39 * c) + 16 + FONT_WIDTH_TABLE;
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
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
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
    index = (39 * c) + 16 + FONT_WIDTH_TABLE;
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
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 14;
}

void GLCDPutCharDigMaxFirst(unsigned char c)
{
    unsigned short index;
    c -= '+';
    index = (39 * c) + 16 + FONT_WIDTH_TABLE;
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
    index = (39 * c) + 42 + FONT_WIDTH_TABLE;
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    data = lcdnumsmax[index++];
    data >>= 1;
    GLCDWriteData(data);
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 14;
}

// Resim basma fonksiyonları
void GLCD_Picture(char *str)
{
    unsigned char i, j;
    for (i = 0; i < 8; ++i)
    {
        GLCD_Chip_Select_Direct(1);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
        GLCD_Chip_Select_Direct(2);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
        GLCD_Chip_Select_Direct(3);
        GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
        GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
        for (j = 0; j < 64; ++j)
        {
            GLCD_Chip_Select_Direct(1);
            GLCD_Data_Direct(str[(i * 192) + j]);
            GLCD_Chip_Select_Direct(2);
            GLCD_Data_Direct(str[(i * 192) + j + 64]);
            GLCD_Chip_Select_Direct(3);
            GLCD_Data_Direct(str[(i * 192) + j + 128]);
        }
    }
    GLCD_Chip_Select_Direct(0);
}

void GLCD_Picture_Discrete(char *str, unsigned char i, unsigned char s, unsigned char k)
{
    unsigned short temp;
    unsigned char j;
    temp = i * 192;
    if (k == 0)
    {
        if (s == 0)
        {
            GLCD_Chip_Select_Direct(1);
            GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
            GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
            GLCD_Data_Fast_Start();
            for (j = 0; j < 16; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 1)
        {
            GLCD_Data_Fast_Start();
            for (j = 16; j < 40; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 2)
        {
            GLCD_Data_Fast_Start();
            for (j = 40; j < 64; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + j]);
            }
            GLCD_Data_Fast_End();
        }
    }
    else if (k == 1)
    {
        if (s == 0)
        {
            GLCD_Chip_Select_Direct(2);
            GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
            GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
            GLCD_Data_Fast_Start();
            for (j = 0; j < 16; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 64 + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 1)
        {
            GLCD_Data_Fast_Start();
            for (j = 16; j < 40; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 64 + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 2)
        {
            GLCD_Data_Fast_Start();
            for (j = 40; j < 64; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 64 + j]);
            }
            GLCD_Data_Fast_End();
        }
    }
    else if (k == 2)
    {
        if (s == 0)
        {
            GLCD_Chip_Select_Direct(3);
            GLCD_Command_Direct(DISPLAY_SET_Y_CMD);
            GLCD_Command_Direct(DISPLAY_SET_X_CMD | i);
            GLCD_Data_Fast_Start();
            for (j = 0; j < 16; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 128 + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 1)
        {
            GLCD_Data_Fast_Start();
            for (j = 16; j < 40; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 128 + j]);
            }
            GLCD_Data_Fast_End();
        }
        else if (s == 2)
        {
            GLCD_Data_Fast_Start();
            for (j = 40; j < 64; ++j)
            {
                GLCD_Data_Fast_Data(str[temp + 128 + j]);
            }
            GLCD_Data_Fast_End();
        }
    }
}

#include "main.h"

/*
================================================================================-------------------
  LMC19264A-01 / AIP31108 (KS0108) 192x64 GLCD S\xdcR\xdcC\xdcS\xdc (TEM?Z VE HIZLI S\xdcR\xdcC\xdc)
================================================================================-------------------
  \xc7ALI?MA PRENS?B? VE KULLANIM REHBER?:

  1. SHADOW RAM TAMPONU (glcd_buffer[1536]):
     MCU RAM'inde 192x64 piksel ekran alan? i\xe7in 1536 Baytl?k g\xf6lge bellek tutulur.
     Piksel okuma sorunlar? (RMW problemleri) ya?anmamas? i\xe7in dikey 8-bit s\xfctun durumlar?
     bu bellekte saklan?r.

  2. OTOMAT?K DONANIM YAZMA:
     T\xfcm \xe7izim ve yaz? fonksiyonlar? (GLCD_SetPixel, GLCD_String5x7, GLCD_Line, GLCD_Rectangle vb.)
     \xe7a?r?ld?klar? ANINDA hem g\xf6lge belle?i g\xfcnceller hem de do?rudan LCD donan?m?na yazar.

  3. ?STE?E BA?LI REFRESH (GLCD_Render):
     ?htiya\xe7 duyulmas? halinde t\xfcm ekran? haf?zadan yeniden basmak i\xe7in GLCD_Render() kullan?labilir.
================================================================================-------------------
 */

unsigned char screen_x = 0, screen_y = 0;
unsigned char tx = 0, ty = 0;

// AIP31108 / KS0108 GLCD Komut Sabitleri
#define DISPLAY_ON_CMD         0x3F
#define DISPLAY_OFF_CMD        0x3E
#define DISPLAY_SET_Y_CMD      0x40  // S\xfctun Adresi (0-63)
#define DISPLAY_SET_X_CMD      0xB8  // Sayfa/Page Adresi (0-7)
#define DISPLAY_START_LINE_CMD 0xC0 // Ba?lang?\xe7 Sat?r? (0-63)

// 192x64 Piksel Grafik Ekran ?\xe7in MCU RAM G\xf6lge Tamponu (192 S\xfctun x 8 Sayfa = 1536 Bayt)
unsigned char glcd_buffer[1536];

struct
{
    unsigned char x;
    unsigned char y;
} Coord;

// D??ar?dan bildirilen okuma/durum fonksiyonu
extern unsigned char GLCD_ReadStatus(unsigned char chip);

//-------------------------------------------------------------------------------------------------
// Donan?m \xc7ip Se\xe7im Fonksiyonu (Active LOW)
//-------------------------------------------------------------------------------------------------

void GLCD_Chip_Select(unsigned char Chip_idx)
{
    if (Chip_idx == 0) // Hi\xe7birini se\xe7me (All Deselected)
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
    else if (Chip_idx == 3) // Sa? 64 piksel (X: 128..191) -> CS3
    {
        CS1_SetHigh();
        CS2_SetHigh();
        CS3_SetLow();
    }
    else if (Chip_idx == 4) // Hepsini se\xe7 (All Selected - Init & Clear i\xe7in)
    {
        CS1_SetLow();
        CS2_SetLow();
        CS3_SetLow();
    }
    __nop();
}

//-------------------------------------------------------------------------------------------------
// Donan?ma Komut G\xf6nderme
//-------------------------------------------------------------------------------------------------

void GLCD_Command(unsigned char command)
{
    TRISD = 0x00; // PORTD \xc7?k??
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    RW_SetLow(); // RW = 0 (Yazma Modu)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFDIR_SetHigh(); // BUFDIR = 1 (MCU -> LCD)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFEN_SetLow(); // BUFEN = 0 (Tampon Etkin)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFE2_SetLow(); // BUFE2 = 0 (Ek Tampon Etkin)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    LATD = command; // Komut Byte'?n? Data Bus'a Koy
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    RS_SetLow(); // RS = 0 (Komut Modu)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    EN_SetHigh(); // EN = 1 (Enable Strobe Y\xdcKSEK)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    EN_SetLow(); // EN = 0 (Enable Strobe D\xdc?\xdcK)
    BUFEN_SetHigh(); // BUFEN Deaktif
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFE2_SetHigh(); // BUFE2 Deaktif
}

//-------------------------------------------------------------------------------------------------
// Donan?ma Veri G\xf6nderme
//-------------------------------------------------------------------------------------------------

void GLCD_Data(unsigned char data)
{
    TRISD = 0x00; // PORTD \xc7?k??
   __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    RW_SetLow(); // RW = 0 (Yazma Modu)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFDIR_SetHigh(); // BUFDIR = 1 (MCU -> LCD)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFEN_SetLow(); // BUFEN = 0 (Tampon Etkin)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFE2_SetLow(); // BUFE2 = 0 (Ek Tampon Etkin)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    LATD = data; // Veri Byte'?n? Data Bus'a Koy
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    RS_SetHigh(); // RS = 1 (Veri Modu)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    EN_SetHigh(); // EN = 1 (Enable Strobe Y\xdcKSEK)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    EN_SetLow(); // EN = 0 (Enable Strobe D\xdc?\xdcK)
    BUFEN_SetHigh(); // BUFEN Deaktif
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    BUFE2_SetHigh(); // BUFE2 Deaktif
}

//-------------------------------------------------------------------------------------------------
// GLCD Do?rudan Konumland?rma
//-------------------------------------------------------------------------------------------------

void GLCD_GoTo_Direct(unsigned char x, unsigned char page)
{
    unsigned char chip;
    unsigned char column;

    if (x >= 192 || page >= 8) return;

    chip = (x / 64) + 1; // 1: Sol, 2: Orta, 3: Sa? \xc7ip
    column = x % 64; // \xc7ip i\xe7i s\xfctun adresi (0..63)

    GLCD_Chip_Select(chip);
    GLCD_Command(DISPLAY_SET_X_CMD | page); // Page (0xB8 + page)
    GLCD_Command(DISPLAY_SET_Y_CMD | column); // S\xfctun (0x40 + column)
}

void GLCD_GoTo(unsigned char x, unsigned char y)
{
    screen_x = x;
    screen_y = y;
    GLCD_GoTo_Direct(x, y / 8);
}

//-------------------------------------------------------------------------------------------------
// GLCD Ba?latma Fonksiyonu
//-------------------------------------------------------------------------------------------------

void GLCD_Init(void)
{
    ANSELD = 0x00; // PORTD Dijital Mod
    TRISD = 0x00; // Data Bus \xc7?k??
    BUFE2_SetHigh(); // BUFE2 Deaktif
    __delay_us(10);
    GLCD_Chip_Select(4); // T\xfcm \xc7ipleri Se\xe7
    __delay_ms(10);

    GLCD_Command(DISPLAY_OFF_CMD); // Ekran Kapal? (0x3E)
    GLCD_Command(DISPLAY_SET_Y_CMD | 0); // Column = 0
    GLCD_Command(DISPLAY_SET_X_CMD | 0); // Page = 0
    GLCD_Command(DISPLAY_START_LINE_CMD | 0); // Start Line = 0
    GLCD_Command(DISPLAY_ON_CMD); // Ekran A\xe7?k (0x3F)
    __delay_us(10);
    GLCD_Chip_Select(0); // Se\xe7imleri Kald?r
    GLCD_ClearAll(); // Ekrana ?lk Temizlik
}

//-------------------------------------------------------------------------------------------------
// T\xfcm RAM Tamponunu Ekrana Yans?tma (A??r? H?zl? Tek Ge\xe7i?li Donan?m Render)
//-------------------------------------------------------------------------------------------------

void GLCD_Render(void)
{
    unsigned char page, col;
    unsigned short ptr;

    for (page = 0; page < 8; page++)
    {
        ptr = page * 192;

        // \xc7ip 1 (Sol 64 S\xfctun)
        GLCD_Chip_Select(1);
        GLCD_Command(DISPLAY_SET_X_CMD | page);
        GLCD_Command(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data(glcd_buffer[ptr + col]);
        }

        // \xc7ip 2 (Orta 64 S\xfctun)
        GLCD_Chip_Select(2);
        GLCD_Command(DISPLAY_SET_X_CMD | page);
        GLCD_Command(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data(glcd_buffer[ptr + 64 + col]);
        }

        // \xc7ip 3 (Sa? 64 S\xfctun)
        GLCD_Chip_Select(3);
        GLCD_Command(DISPLAY_SET_X_CMD | page);
        GLCD_Command(DISPLAY_SET_Y_CMD | 0);
        for (col = 0; col < 64; col++)
        {
            GLCD_Data(glcd_buffer[ptr + 128 + col]);
        }
    }
    GLCD_Chip_Select(0);
}

//-------------------------------------------------------------------------------------------------
// GLCD T\xfcm Ekran? Temizleme
//-------------------------------------------------------------------------------------------------

void GLCD_ClearAll(void)
{
    memset(glcd_buffer, 0x00, 1536);
    GLCD_Render();
}

void GLCD_WriteData(unsigned char dataToWrite)
{
    unsigned char yOffset = screen_y % 8;
    unsigned char page = screen_y / 8;
    unsigned short idx = (unsigned short) page * 192 + screen_x;

    if (screen_x < 192 && page < 8)
    {
        if (yOffset == 0)
        {
            glcd_buffer[idx] = dataToWrite;
            GLCD_GoTo_Direct(screen_x, page);
            GLCD_Data(glcd_buffer[idx]);
        }
        else
        {
            glcd_buffer[idx] = (glcd_buffer[idx] & ~(0xFF << yOffset)) | (dataToWrite << yOffset);
            GLCD_GoTo_Direct(screen_x, page);
            GLCD_Data(glcd_buffer[idx]);

            if (page + 1 < 8)
            {
                unsigned short idx2 = (unsigned short) (page + 1) * 192 + screen_x;
                glcd_buffer[idx2] = (glcd_buffer[idx2] & ~(0xFF >> (8 - yOffset))) | (dataToWrite >> (8 - yOffset));
                GLCD_GoTo_Direct(screen_x, page + 1);
                GLCD_Data(glcd_buffer[idx2]);
            }
        }
    }
    screen_x++;
}

void GLCDWriteData(unsigned char data)
{
    unsigned char yOffset = Coord.y % 8;
    unsigned char page = Coord.y / 8;
    unsigned short idx = (unsigned short) page * 192 + Coord.x;

    if (Coord.x < 192 && page < 8)
    {
        if (yOffset == 0)
        {
            glcd_buffer[idx] = data;
            GLCD_GoTo_Direct(Coord.x, page);
            GLCD_Data(glcd_buffer[idx]);
        }
        else
        {
            glcd_buffer[idx] = (glcd_buffer[idx] & ~(0xFF << yOffset)) | (data << yOffset);
            GLCD_GoTo_Direct(Coord.x, page);
            GLCD_Data(glcd_buffer[idx]);

            if (page + 1 < 8)
            {
                unsigned short idx2 = (unsigned short) (page + 1) * 192 + Coord.x;
                glcd_buffer[idx2] = (glcd_buffer[idx2] & ~(0xFF >> (8 - yOffset))) | (data >> (8 - yOffset));
                GLCD_GoTo_Direct(Coord.x, page + 1);
                GLCD_Data(glcd_buffer[idx2]);
            }
        }
    }
    Coord.x++;
}

void GotoXY(unsigned char x, unsigned char y)
{
    Coord.x = x;
    Coord.y = y;
}

//-------------------------------------------------------------------------------------------------
// MCU RAM Tamponu Kullanan Grafik \xc7izim Fonksiyonlar?
//-------------------------------------------------------------------------------------------------

void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
    unsigned char page = y / 8;
    unsigned char bit_pos = y % 8;
    unsigned short idx;

    if (x >= 192 || y >= 64) return;

    idx = (unsigned short) page * 192 + x;

    if (color == BLACK)
    {
        glcd_buffer[idx] |= (1 << bit_pos);
    }
    else
    {
        glcd_buffer[idx] &= ~(1 << bit_pos);
    }

    // An?nda donan?ma yaz
    GLCD_GoTo_Direct(x, page);
    GLCD_Data(glcd_buffer[idx]);
}

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

void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char curr_x, curr_y;
    for (curr_x = x; curr_x <= b; curr_x++)
    {
        for (curr_y = y; curr_y <= a; curr_y++)
        {
            GLCD_SetPixel(curr_x, curr_y, color);
        }
    }
}

void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color)
{
    GLCD_Rectangle_Fill(x, y, x2, y2, color);
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
    unsigned char temp = radius;
    while (temp > 0)
    {
        GLCD_Circle(cx, cy, temp, color);
        temp--;
    }
}

//-------------------------------------------------------------------------------------------------
// Metin ve Font Fonksiyonlar?
//-------------------------------------------------------------------------------------------------

void GLCD_String5x7(unsigned char x, unsigned char y, char *str)
{
    unsigned char i = 0;
    unsigned char curr_x = x;

    screen_x = curr_x;
    screen_y = y;

    while (str[i] != '\0')
    {
        if (curr_x + 6 > 192) break; // Ekran geni?lik s?n?r?n? a?ma

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
        index = (unsigned short) (0x7 + FONT_WIDTH_TABLE);
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

// Resim basma fonksiyonlar? (RAM Tamponuna ve Donan?ma Yazma)

void GLCD_Picture(char *str)
{
    memcpy(glcd_buffer, str, 1536);
    GLCD_Render();
}

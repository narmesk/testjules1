#include <xc.h>
#include <string.h>

void GLCD_Data_Fast_Data (char Data) /* GLCD data function */
{
  LATD = Data;
    __delay_us (3);
  EN_LAT = 1;
  //__nop ();
  __delay_us (3);
  EN_LAT = 0;
    __delay_us (3);
}


void GLCD_Command (char Command) /* GLCD command function */
{
  RW_LAT = 0;
  BUFDIR_LAT = 1;
  BUFEN_LAT = 0;
  __delay_us (3);
  LATD = Command;
  __delay_us (3);
  RS_LAT = 0;
  EN_LAT = 1;
  __delay_us (3);
  EN_LAT = 0;
  BUFEN_LAT = 1;
}

void GLCD_Data (char Data) /* GLCD data function */
{
  RW_LAT = 0;
  BUFDIR_LAT = 1;
  BUFEN_LAT = 0;
  __delay_us (3);
  LATD = Data;
  __delay_us (3);
  RS_LAT = 1;
  EN_LAT = 1;
  __delay_us (3);
  EN_LAT = 0;
  BUFEN_LAT = 1;
}

void GLCD_Chip_Select(char Chip_idx)
{
    if (Chip_idx == 0)
    {
        CS1_LAT = 1;
        CS2_LAT = 1;
        CS3_LAT = 1;
    }
    else if (Chip_idx == 1)
    {
        CS1_LAT = 0;
        CS2_LAT = 1;
        CS3_LAT = 1;
    }
    else if (Chip_idx == 2)
    {
        CS1_LAT = 1;
        CS2_LAT = 0;
        CS3_LAT = 1;
    }
    else if (Chip_idx == 3)
    {
        CS1_LAT = 1;
        CS2_LAT = 1;
        CS3_LAT = 0;
    }
    else if (Chip_idx == 4)
    {
        CS1_LAT = 0;
        CS2_LAT = 0;
        CS3_LAT = 0;
    }
    __delay_us(1);
}

void GLCD_GoTo(unsigned char x, unsigned char y)
{
    screen_x = x;
    screen_y = y;
    GLCD_Chip_Select(1);
    GLCD_Command(DISPLAY_SET_Y | 0);
    GLCD_Command(DISPLAY_SET_X | y);
    GLCD_Command(DISPLAY_START_LINE | 0);
    GLCD_Chip_Select(2);
    GLCD_Command(DISPLAY_SET_Y | 0);
    GLCD_Command(DISPLAY_SET_X | y);
    GLCD_Command(DISPLAY_START_LINE | 0);
    GLCD_Chip_Select(3);
    GLCD_Command(DISPLAY_SET_Y | 0);
    GLCD_Command(DISPLAY_SET_X | y);
    GLCD_Command(DISPLAY_START_LINE | 0);
    GLCD_Chip_Select(((x / 64) + 1));
    GLCD_Command(DISPLAY_SET_Y | (x % 64));
    GLCD_Command(DISPLAY_SET_X | y);
}

//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------

void GLCD_WriteData(unsigned char dataToWrite)
{
    while (GLCD_ReadStatus((screen_x / 64) + 1) & DISPLAY_STATUS_BUSY);
    RW_LAT = 0;
    BUFDIR_LAT = 1;
    BUFEN_LAT = 0;
    LATD = dataToWrite;
    GLCD_Chip_Select(((screen_x / 64) + 1));
    RS_LAT = 1;
    EN_LAT = 1;
    __nop();
    EN_LAT = 0;
    BUFEN_LAT = 1;
    GLCD_Chip_Select(0);
    screen_x++;
}

//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------

void GLCD_Init() /* GLCD initialize function */
{
    GLCD_Chip_Select(4);
    GLCD_Command(0x3E); /* Display OFF */
    GLCD_Command(DISPLAY_SET_Y); /* Set Y address (column=0) */
    GLCD_Command(DISPLAY_SET_X); /* Set x address (page=0) */
    GLCD_Command(DISPLAY_START_LINE); /* Set z address (start line=0) */
    GLCD_Command(0x3F); /* Display ON */
    dumyglcd = glcd_rdcache;
}

void GLCD_ClearAll() /* GLCD all display clear function */
{
    unsigned char i, j;
    GLCD_Chip_Select(4); /* Select both left & right half of display */
    for (i = 0; i < 8; i++)
    {
        GLCD_Command((DISPLAY_SET_X) + i); /* Increment page each time after 64 column */
        for (j = 0; j < 64; j++)
        {
            GLCD_Data(0); /* Write zeros to all 64 column */
        }
    }
    GLCD_Command(DISPLAY_SET_Y); /* Set Y address (column=0) */
    GLCD_Command(DISPLAY_SET_X); /* Set x address (page=0) */
    GLCD_Chip_Select(0);
}

void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
    if (color == BLACK)
    {
        bitset(dumyglcd[(y / 8)*192 + x], y % 8);
    }
    else
    {
        bitclr(dumyglcd[(y / 8)*192 + x], y % 8);
    }
}

void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color)
{
    unsigned char j = 0; // zmienna pomocnicza
    // rysowanie linii pionowych (boki)
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
    //unsigned short tend;
    a = a + 1;
    b = b + 1;
    for (y; y < a; y++)
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
    //yakla??k 381-400 sürüyor x720ns
    if (color == BLACK)
    {
        memset(glcd_rdcache[zone], 0xFF, 192);
        //memset (dumyglcd, 0xFF, 1536);
    }
    if (color == WHITE)
    {
        memset(glcd_rdcache[zone], 0x00, 192);
        //memset (dumyglcd, 0x00, 1536);
    }
}

void GLCD_Rectangle_Fill_Fast_White(unsigned char x, unsigned char y, unsigned char b, unsigned char a)
{
    unsigned char j, k;
    unsigned short t;
    a = a + 1;
    b = b + 1;
    for (y; y < a; y++)
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

    Dx = (X2 - X1); // obliczenie sk³adowej poziomej
    Dy = (Y2 - Y1); // obliczenie sk³adowej pionowej

    TwoDx = Dx + Dx; // podwojona sk³adowa pozioma
    TwoDy = Dy + Dy; // podwojona sk³adowa pionowa

    CurrentX = X1; // zaczynamy od X1
    CurrentY = Y1; // oraz Y1

    Xinc = 1; // ustalamy krok zwiêkszania pozycji w poziomie
    Yinc = 1; // ustalamy krok zwiêkszania pozycji w pionie

    if (Dx < 0) // jesli sk³adowa pozioma jest ujemna
    {
        Xinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
        Dx = -Dx; // zmieniamy znak sk³adowej na dodatni
        TwoDx = -TwoDx; // jak równie¿ podwojonej sk³adowej
    }

    if (Dy < 0) // je?li sk³adowa pionowa jest ujemna
    {
        Yinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
        Dy = -Dy; // zmieniamy znak sk³adowej na dodatki
        TwoDy = -TwoDy; // jak równiez podwojonej sk³adowej
    }

    GLCD_SetPixel(X1, Y1, color); // stawiamy pierwszy krok (zapalamy pierwszy piksel)

    if ((Dx != 0) || (Dy != 0)) // sprawdzamy czy linia sk³ada siê z wiêcej ni¿ jednego punktu ;)
    {
        // sprawdzamy czy sk³adowa pionowa jest mniejsza lub równa sk³adowej poziomej
        if (Dy <= Dx) // je?li tak, to idziemy "po iksach"
        {
            TwoDxAccumulatedError = 0; // zerujemy zmienn¹
            do // ruszamy w drogê
            {
                CurrentX += Xinc; // do aktualnej pozycji dodajemy krok
                TwoDxAccumulatedError += TwoDy; // a tu dodajemy podwojon¹ sk³adow¹ pionow¹
                if (TwoDxAccumulatedError > Dx) // je?li TwoDxAccumulatedError jest wiêkszy od Dx
                {
                    CurrentY += Yinc; // zwiêkszamy aktualn¹ pozycjê w pionie
                    TwoDxAccumulatedError -= TwoDx; // i odejmujemy TwoDx
                }
                GLCD_SetPixel(CurrentX, CurrentY, color); // stawiamy nastêpny krok (zapalamy piksel)
            }
            while (CurrentX != X2); // idziemy tak d³ugo, a¿ osi¹gniemy punkt docelowy
        }
        else // w przeciwnym razie idziemy "po igrekach"
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

void GLCD_StringArialBold14(unsigned char x, unsigned char y, char *str)/* GLCD string write function */
{
    //GotoXY (x, y);
    tx = x;
    ty = y;
    unsigned char i;

    for (i = 0; i < strlen(str); i++) /* Print char in string till null */
    {
        GLCDPutChar_ArialBold14(str[i]);
    }
}

void GLCD_String5x7(unsigned char x, unsigned char y, char *str)/* GLCD string write function */
{
    //GotoXY (x, y);
    tx = x;
    ty = y;
    unsigned char i;

    for (i = 0; i < strlen(str); i++) /* Print char in string till null */
    {
        GLCDPutChar5x7(str[i]);
    }
}

void GLCD_Picture(char *str) /* GLCD string write function */
{
    unsigned char i, j;
    // Loop through the vertical pages
    for (i = 0; i < 8; ++i)
    {
        GLCD_Chip_Select(1);
        GLCD_Command(DISPLAY_SET_Y);
        GLCD_Command(DISPLAY_SET_X | i);
        GLCD_Chip_Select(2);
        GLCD_Command(DISPLAY_SET_Y);
        GLCD_Command(DISPLAY_SET_X | i);
        GLCD_Chip_Select(3);
        GLCD_Command(DISPLAY_SET_Y);
        GLCD_Command(DISPLAY_SET_X | i);
        // Loop through the horizontal sections
        for (j = 0; j < 64; ++j)
        {
            GLCD_Chip_Select(1);
            GLCD_Data(str[(i * 192) + j]);
            GLCD_Chip_Select(2);
            GLCD_Data(str[(i * 192) + j + 64]);
            GLCD_Chip_Select(3);
            GLCD_Data(str[(i * 192) + j + 128]);
        }
    }
    GLCD_Chip_Select(0);
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
            GLCD_Chip_Select(1);
            GLCD_Command(DISPLAY_SET_Y);
            GLCD_Command(DISPLAY_SET_X | i);
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
            GLCD_Chip_Select(2);
            GLCD_Command(DISPLAY_SET_Y);
            GLCD_Command(DISPLAY_SET_X | i);
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
            GLCD_Chip_Select(3);
            GLCD_Command(DISPLAY_SET_Y);
            GLCD_Command(DISPLAY_SET_X | i);
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

void GLCDWriteData(unsigned char data)
{
    unsigned char yOffset;
    unsigned short t;
    unsigned char ysave1;
    yOffset = Coord.y % 8;
    if (yOffset != 0)
    {
        // first page
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
    Coord.x = x; // save new coordinates
    Coord.y = y;
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

void GLCDPutChar_ArialBold14(unsigned char c)
{
    unsigned char width;
    unsigned short index;
    unsigned char data;
    unsigned short j;
    unsigned short jj;
    unsigned short page; // page must be 16 bit to prevent overflow
    c -= 0x20; // first char
    if (c != 0)
    {
        index = ((Arial_bold_14_index[c - 1])*2) + 0x60 + FONT_WIDTH_TABLE; // char count
    }
    else
    {
        index = 0x60 + FONT_WIDTH_TABLE; // char count
    }
    width = Arial_bold_14[FONT_WIDTH_TABLE + c];
    Coord.x = tx;
    Coord.y = ty;
    jj = index + width;
    for (j = index; j < jj; j++) /* each column */
    {
        data = Arial_bold_14 [j];
        GLCDWriteData(data);
    }
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    page = width + index; // page must be 16 bit to prevent overflow
    jj += width;
    for (j = page; j < jj; j++) /* each column */
    {
        data = Arial_bold_14 [j];
        data >>= 2;
        GLCDWriteData(data);
    }
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    tx = tx + width + 1;
}

void GLCD_StringCalibri36(unsigned char x, unsigned char y, char *str)/* GLCD string write function */
{
    //GotoXY (x, y);
    tx = x;
    ty = y;
    unsigned char i;

    for (i = 0; i < strlen(str); i++) /* Print char in string till null */
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
    c -= 0x20; // first char
    if (c != 0)
    {
        index = Calibri36_index[c - 1] + 0x7 + FONT_WIDTH_TABLE;
    }
    else
    {
        index = 0x7 + FONT_WIDTH_TABLE; // char count
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
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    page = index + width;
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;

    page = index + (2 * width);
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;

    page = index + (3 * width);
    jj = page + width;
    for (j = page; j < jj; j++)
    {
        GLCDWriteData(Calibri36[j]);
    }
    // 1px gap between chars
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
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + width + 1;
}

void GLCD_StringHead8x8(unsigned char x, unsigned char y, char *str)/* GLCD string write function */
{
    //GotoXY (x, y);
    tx = x;
    ty = y;
    unsigned char i;
    for (i = 0; i < strlen(str); i++) /* Print char in string till null */
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    tx = tx + 5; //+ 12;
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
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
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
}


//255 second firstten sonraki durumda

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
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y = Coord.y + 8;
    tx = tx + 14;
}

void GLCDPutChar5x7(unsigned char c)
{
    c -= 0x20;
    unsigned short index = c * 5;
    Coord.x = tx;
    Coord.y = ty;
    GLCDWriteData(font[index++]);
    GLCDWriteData(font[index++]);
    GLCDWriteData(font[index++]);
    GLCDWriteData(font[index++]);
    GLCDWriteData(font[index++]);
    // 1px gap between chars
    GLCDWriteData(0x00);
    Coord.x = tx;
    Coord.y += 8;
    tx = tx + 6;
}

#ifndef GLCD_H
#define GLCD_H

#include <xc.h>

#define BLACK 1
#define WHITE 0

// Donanım Fonksiyonları
void GLCD_Init(void);
void GLCD_Render(void);
void GLCD_ClearAll(void);
void GLCD_Chip_Select(unsigned char Chip_idx);
void GLCD_Command(unsigned char command);
void GLCD_Data(unsigned char data);
void GLCD_GoTo(unsigned char x, unsigned char y);
void GLCDWriteData(unsigned char data);
void GLCD_WriteData(unsigned char dataToWrite);
void GotoXY(unsigned char x, unsigned char y);
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color);

// Grafik Çizim Fonksiyonları
void GLCD_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color);
void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char color);
void SetPixels(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char color);
void GLCD_Circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);
void GLCD_Circle_Fill(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char color);

// Metin ve Font Fonksiyonları
void GLCD_String5x7(unsigned char x, unsigned char y, char *str);
void GLCD_StringArialBold14(unsigned char x, unsigned char y, char *str);
void GLCD_StringCalibri36(unsigned char x, unsigned char y, char *str);
void GLCD_StringHead8x8(unsigned char x, unsigned char y, char *str);

void GLCDPutChar5x7(unsigned char c);
void GLCDPutChar_ArialBold14(unsigned char c);
void GLCDPutCharCalibri36(unsigned char c);
void GLCDPutCharHead8x8(unsigned char c);
void GLCDPutCharDigMin(unsigned char c);
void GLCDPutSpecialCharDigMin(unsigned char c);
void GLCDPutSpecialCharDigMax(unsigned char c);
void GLCDPutCharDigMax(unsigned char c);
void GLCDPutCharDigMaxFirst(unsigned char c);
void GLCDPutCharDigMaxSecond(unsigned char c);

// Resim Fonksiyonu
void GLCD_Picture(char *str);

#endif // GLCD_H

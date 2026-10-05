#include "mcc_generated_files/mcc.h"
#include "remotecontroller.h"
#include "glcd.h"
#include <string.h>

/*
================================================================================-------------------
  PIC18FREMOTE YENİ NESİL TEK GEÇİŞLİ (DIRECT BUFFER) HMI MENÜ SÜRÜCÜSÜ (menu.c)
================================================================================-------------------
  MİMARİ YENİLİKLER VE AVANTAJLARI:
  1. Parçalı (case 0..63 switch-case) taranan eski sistem yerine, donanıma doğrudan gölge bellek
     (glcd_buffer[1536]) üzerinden TEK GEÇİŞTE (Single-pass Direct Render) çizim yapan fonksiyonlar
     geliştirilmiştir.
  2. Ekran titremesini (flicker) engellemek için CreateMenu* (statik çerçeveler) ve RefreshMenu*
     (dinamik değerler, imleç yanıp sönme) mantığı korunmuş, ilk çağrıda (updatecnt == 0) anında
     çizilecek şekilde optimize edilmiştir.
  3. Donanım ekran tazelemesi `GLCD_Render()` fonksiyonu ile tek seferde 1536 bayt olarak basılır.
================================================================================-------------------
*/

extern ScreenCommand Screen_Vals[12];
extern ScreenCommand Config_Vals[13];
extern unsigned char MenuProductTable[9][8];
extern unsigned char ConfigMenuMessageTable[15][24];
extern unsigned char ParameterNameTable[15][16];
extern unsigned char SqeunceIdTable[24][2];
extern unsigned char MessageTable[8][8];

extern unsigned char config_menu_messageidx;
extern unsigned char squence_table_idx;
extern unsigned char slaveid_table_idx;
extern unsigned char subid_table_idx;
extern unsigned char subparam_table_idx;
extern unsigned char product_table_idx;
extern unsigned char entering_mode;
extern unsigned char entering_val;
extern unsigned char cursor_pos;
extern unsigned char cursor_blink;

extern unsigned char SLAVEIDLIST[24];
extern unsigned char SLAVESUBIDLIST[24];
extern unsigned char SUBPARAMS[23];
extern unsigned char TryIdx[24];
extern unsigned char SLAVEIDUPDATESTATE[24];
extern unsigned char slaveid_cnt;
extern unsigned char loop_cnt;
extern unsigned char copy_repeater_respond_stat;
extern unsigned short ActPassVal;

extern unsigned char val10, val11, val12, val13, val14, val15;
extern unsigned char valc10, valc11, valc12;
extern unsigned char tx, ty;

extern void WriteDecimalStringShort(short value);
extern void WriteDecimalStringUChar(unsigned char value);

//-------------------------------------------------------------------------------------------------
// Yardımcı Dijit / İmleç Çizim Fonksiyonu (Digit Render with Blink/Cursor support)
//-------------------------------------------------------------------------------------------------
static void DrawDigitBlink(unsigned char x, unsigned char y, unsigned char digit_val, unsigned char is_target_digit, unsigned char mode_active)
{
    tx = x;
    ty = y;

    if (mode_active && is_target_digit && cursor_blink)
    {
        GLCD_Rectangle_Fill(x + 1, y + 1, x + 6, y + 22, WHITE);
    }
    else
    {
        GLCDPutCharDigMaxFirst(digit_val);
        GLCDPutCharDigMaxSecond(digit_val);
    }
}

//-------------------------------------------------------------------------------------------------
// 1. IDLE MENÜSÜ (Çerçeve Oluşturma ve Fiyat Yenileme)
//-------------------------------------------------------------------------------------------------

void CreateMenuIdle_Direct(unsigned char menucount)
{
    GLCD_ClearAll();

    // Başlık
    GLCD_String5x7(3, 0, "PRICE TABLE");

    // Sayfa Numarası
    if (menucount == IDLE_PAGE1)      GLCD_String5x7(69, 0, "1");
    else if (menucount == IDLE_PAGE2) GLCD_String5x7(69, 0, "2");
    else if (menucount == IDLE_PAGE3) GLCD_String5x7(69, 0, "3");

    // Kolon Başlıkları
    GLCD_String5x7(91, 0, "CASH");
    GLCD_String5x7(147, 0, "CREDIT");

    // Ürün Adları
    unsigned char base_idx = 0;
    if (menucount == IDLE_PAGE2)      base_idx = 3;
    else if (menucount == IDLE_PAGE3) base_idx = 6;

    char prod_str[9];
    for (unsigned char row = 0; row < 3; row++)
    {
        memcpy(prod_str, MenuProductTable[base_idx + row], 8);
        prod_str[8] = '\0';
        GLCD_StringArialBold14(3, 10 + (row * 15), prod_str);
    }

    // Ayırıcı Çizgiler ve Izgara Tablosu
    GLCD_Rectangle(79, 0, 79, 55, BLACK);
    GLCD_Rectangle(135, 0, 135, 55, BLACK);
    GLCD_Rectangle(0, 8, 191, 8, BLACK);
    GLCD_Rectangle(0, 24, 191, 24, BLACK);
    GLCD_Rectangle(0, 39, 191, 39, BLACK);
    GLCD_Rectangle(0, 55, 191, 55, BLACK);

    GLCD_Render();
}

void RefreshMenuIdle_Direct(unsigned char menucount)
{
    unsigned char base_idx = 0;
    if (menucount == IDLE_PAGE2)      base_idx = 3;
    else if (menucount == IDLE_PAGE3) base_idx = 6;

    // 3 Satır Fiyat Değerlerini Yaz
    for (unsigned char row = 0; row < 3; row++)
    {
        unsigned char screen_idx = base_idx + row;
        unsigned char y_pos = 9 + (row * 15);

        // CASH Fiyatı
        WriteDecimalStringShort(Screen_Vals[screen_idx].Val_Pos1);
        tx = 85; ty = y_pos;
        GLCDPutCharDigMin(val13);
        tx = 97; ty = y_pos;
        GLCDPutSpecialCharDigMin('.');
        tx = 102; ty = y_pos;
        GLCDPutCharDigMin(val14);
        tx = 114; ty = y_pos;
        GLCDPutCharDigMin(val15);

        // CREDIT Fiyatı
        WriteDecimalStringShort(Screen_Vals[screen_idx].Val_Pos2);
        tx = 141; ty = y_pos;
        GLCDPutCharDigMin(val13);
        tx = 153; ty = y_pos;
        GLCDPutSpecialCharDigMin('.');
        tx = 158; ty = y_pos;
        GLCDPutCharDigMin(val14);
        tx = 170; ty = y_pos;
        GLCDPutCharDigMin(val15);
    }

    // Alt Mesaj Satırı (Status Bar)
    char msg_str[25];
    memcpy(msg_str, ConfigMenuMessageTable[config_menu_messageidx], 24);
    msg_str[24] = '\0';
    GLCD_String5x7(3, 57, msg_str);

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// 2. SENDER / GÖNDERİM DURUM MENÜSÜ
//-------------------------------------------------------------------------------------------------

void CreateMenuSender_Direct(unsigned char menucount)
{
    GLCD_ClearAll();

    GLCD_String5x7(3, 2, "SCREENS:");
    GLCD_String5x7(68, 2, "LOOPS:");
    GLCD_String5x7(170, 2, "P:");

    if (menucount == SENDING_PAGE1)      GLCD_String5x7(182, 2, "1");
    else if (menucount == SENDING_PAGE2) GLCD_String5x7(182, 2, "2");
    else if (menucount == SENDING_PAGE3) GLCD_String5x7(182, 2, "3");

    // Tablo Başlıkları
    GLCD_String5x7(2, 14, "ID TRY UP STATUS");
    GLCD_String5x7(98, 14, "ID TRY UP STATUS");

    // Tablo Çerçeveleri
    GLCD_Rectangle(0, 11, 191, 11, BLACK);
    GLCD_Rectangle(0, 22, 191, 22, BLACK);
    GLCD_Rectangle(0, 33, 191, 33, BLACK);
    GLCD_Rectangle(0, 43, 191, 43, BLACK);
    GLCD_Rectangle(0, 53, 191, 53, BLACK);
    GLCD_Rectangle(0, 63, 191, 63, BLACK);
    GLCD_Rectangle(96, 11, 96, 63, BLACK);

    // ID Listesi Etiketleri
    unsigned char start_id = 1;
    if (menucount == SENDING_PAGE2)      start_id = 9;
    else if (menucount == SENDING_PAGE3) start_id = 17;

    for (unsigned char row = 0; row < 4; row++)
    {
        unsigned char id1 = start_id + (row * 2);
        unsigned char id2 = id1 + 1;
        unsigned char y_p = 25 + (row * 10);

        WriteDecimalStringUChar(id1);
        tx = 3; ty = y_p;
        GLCDPutChar5x7(valc11); GLCDPutChar5x7(valc10);

        WriteDecimalStringUChar(id2);
        tx = 99; ty = y_p;
        GLCDPutChar5x7(valc11); GLCDPutChar5x7(valc10);
    }

    GLCD_Render();
}

void RefreshMenuSender_Direct(unsigned char menucount)
{
    // Cihaz ve Döngü Sayısı
    WriteDecimalStringUChar(slaveid_cnt);
    tx = 51; ty = 2;
    GLCDPutChar5x7(valc11); GLCDPutChar5x7(valc10);

    WriteDecimalStringUChar(loop_cnt);
    tx = 104; ty = 2;
    GLCDPutChar5x7(valc11); GLCDPutChar5x7(valc10);

    unsigned char base_idx = 0;
    if (menucount == SENDING_PAGE2)      base_idx = 8;
    else if (menucount == SENDING_PAGE3) base_idx = 16;

    char status_msg[9];
    for (unsigned char i = 0; i < 8; i++)
    {
        unsigned char slave_idx = base_idx + i;
        unsigned char col = i % 2;
        unsigned char row = i / 2;
        unsigned char x_try = (col == 0) ? 20 : 116;
        unsigned char x_stat = (col == 0) ? 39 : 135;
        unsigned char y_p = 25 + (row * 10);

        // Deneme Sayısı
        WriteDecimalStringUChar(TryIdx[slave_idx]);
        tx = x_try; ty = y_p;
        GLCDPutChar5x7(valc11); GLCDPutChar5x7(valc10);

        // Durum Metni
        memcpy(status_msg, MessageTable[SLAVEIDUPDATESTATE[slave_idx]], 8);
        status_msg[8] = '\0';
        GLCD_String5x7(x_stat, y_p, status_msg);
    }

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// 3. ID VE SUB-ID KONFİGÜRASYON MENÜSÜ
//-------------------------------------------------------------------------------------------------

void CreateMenuConfigID_Direct(void)
{
    GLCD_ClearAll();

    GLCD_Rectangle(0, 0, 191, 55, BLACK);
    GLCD_Rectangle(0, 13, 191, 13, BLACK);
    GLCD_Rectangle(0, 26, 191, 26, BLACK);
    GLCD_Rectangle(16, 26, 16, 55, BLACK);
    GLCD_Rectangle(96, 26, 96, 55, BLACK);
    GLCD_Rectangle(175, 26, 175, 55, BLACK);

    // Başlıklar
    char seq_str[3];
    memcpy(seq_str, SqeunceIdTable[squence_table_idx], 2);
    seq_str[2] = '\0';

    char header[20];
    strcpy(header, "SEQUENCE ID : ");
    strcat(header, seq_str);
    GLCD_StringArialBold14(34, 1, header);

    GLCD_StringArialBold14(28, 14, "SLAVE ID");
    GLCD_StringArialBold14(108, 14, "SUB ID");

    // Ok Simgeleri
    GLCD_StringArialBold14(5, 35, "<");
    GLCD_StringArialBold14(180, 35, ">");

    GLCD_Render();
}

void RefreshMenuConfigID_Direct(void)
{
    unsigned char mode_active = (entering_mode == 1);

    // Seçim Vurgu Çerçeveleri
    if (mode_active)
    {
        if (entering_val == 0)
        {
            GLCD_Rectangle(33, 28, 80, 53, BLACK);
            GLCD_Rectangle(105, 28, 152, 53, WHITE);
        }
        else
        {
            GLCD_Rectangle(33, 28, 80, 53, WHITE);
            GLCD_Rectangle(105, 28, 152, 53, BLACK);
        }
    }
    else
    {
        GLCD_Rectangle(33, 28, 80, 53, WHITE);
        GLCD_Rectangle(105, 28, 152, 53, WHITE);
    }

    // Slave ID Rakamları
    WriteDecimalStringUChar(SLAVEIDLIST[slaveid_table_idx]);
    DrawDigitBlink(36, 29, valc12, (cursor_pos == 2), mode_active && (entering_val == 0));
    DrawDigitBlink(50, 29, valc11, (cursor_pos == 1), mode_active && (entering_val == 0));
    DrawDigitBlink(64, 29, valc10, (cursor_pos == 0), mode_active && (entering_val == 0));

    // Sub ID Rakamları
    WriteDecimalStringUChar(SLAVESUBIDLIST[subid_table_idx]);
    DrawDigitBlink(108, 29, valc12, (cursor_pos == 2), mode_active && (entering_val == 1));
    DrawDigitBlink(122, 29, valc11, (cursor_pos == 1), mode_active && (entering_val == 1));
    DrawDigitBlink(136, 29, valc10, (cursor_pos == 0), mode_active && (entering_val == 1));

    // Alt Mesaj Satırı
    char msg_str[25];
    memcpy(msg_str, ConfigMenuMessageTable[config_menu_messageidx], 24);
    msg_str[24] = '\0';
    GLCD_String5x7(3, 57, msg_str);

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// 4. PARAMETRE KONFİGÜRASYON MENÜSÜ
//-------------------------------------------------------------------------------------------------

void CreateMenuConfigParam_Direct(void)
{
    GLCD_ClearAll();

    GLCD_Rectangle(0, 0, 191, 55, BLACK);
    GLCD_Rectangle(0, 13, 191, 13, BLACK);
    GLCD_Rectangle(0, 26, 191, 26, BLACK);

    GLCD_StringArialBold14(34, 1, "SUB PARAMETERS");

    char param_name[17];
    memcpy(param_name, ParameterNameTable[subparam_table_idx], 16);
    param_name[16] = '\0';
    GLCD_StringArialBold14(20, 14, param_name);

    GLCD_Render();
}

void RefreshMenuConfigParam_Direct(void)
{
    unsigned char mode_active = (entering_mode == 1);

    if (mode_active)
    {
        GLCD_Rectangle(73, 28, 120, 53, BLACK);
    }
    else
    {
        GLCD_Rectangle(73, 28, 120, 53, WHITE);
    }

    WriteDecimalStringUChar(SUBPARAMS[subparam_table_idx]);
    DrawDigitBlink(76, 29, valc12, (cursor_pos == 2), mode_active && (entering_val == 0));
    DrawDigitBlink(90, 29, valc11, (cursor_pos == 1), mode_active && (entering_val == 0));
    DrawDigitBlink(104, 29, valc10, (cursor_pos == 0), mode_active && (entering_val == 0));

    char msg_str[25];
    memcpy(msg_str, ConfigMenuMessageTable[config_menu_messageidx], 24);
    msg_str[24] = '\0';
    GLCD_String5x7(3, 57, msg_str);

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// 5. FİYAT KONFİGÜRASYON MENÜSÜ
//-------------------------------------------------------------------------------------------------

void CreateMenuConfigPrice_Direct(void)
{
    GLCD_ClearAll();

    GLCD_Rectangle(0, 0, 191, 55, BLACK);
    GLCD_Rectangle(0, 13, 191, 13, BLACK);
    GLCD_Rectangle(0, 26, 191, 26, BLACK);
    GLCD_Rectangle(16, 26, 16, 55, BLACK);
    GLCD_Rectangle(96, 26, 96, 55, BLACK);
    GLCD_Rectangle(175, 26, 175, 55, BLACK);

    char prod_str[9];
    memcpy(prod_str, MenuProductTable[product_table_idx], 8);
    prod_str[8] = '\0';
    GLCD_StringArialBold14(56, 1, prod_str);

    GLCD_StringArialBold14(42, 14, "CASH");
    GLCD_StringArialBold14(108, 14, "CREDIT");

    GLCD_StringArialBold14(5, 35, "<");
    GLCD_StringArialBold14(180, 35, ">");

    GLCD_Render();
}

void RefreshMenuConfigPrice_Direct(void)
{
    unsigned char mode_active = (entering_mode == 1);

    if (mode_active)
    {
        if (entering_val == 0)
        {
            GLCD_Rectangle(33, 28, 87, 53, BLACK);
            GLCD_Rectangle(105, 28, 159, 53, WHITE);
        }
        else
        {
            GLCD_Rectangle(33, 28, 87, 53, WHITE);
            GLCD_Rectangle(105, 28, 159, 53, BLACK);
        }
    }
    else
    {
        GLCD_Rectangle(33, 28, 87, 53, WHITE);
        GLCD_Rectangle(105, 28, 159, 53, WHITE);
    }

    // CASH Fiyatı
    WriteDecimalStringShort(Screen_Vals[product_table_idx].Val_Pos1);
    DrawDigitBlink(36, 29, val13, (cursor_pos == 2), mode_active && (entering_val == 0));
    tx = 50; ty = 29;
    GLCDPutSpecialCharDigMax('.');
    DrawDigitBlink(56, 29, val14, (cursor_pos == 1), mode_active && (entering_val == 0));
    DrawDigitBlink(70, 29, val15, (cursor_pos == 0), mode_active && (entering_val == 0));

    // CREDIT Fiyatı
    WriteDecimalStringShort(Screen_Vals[product_table_idx].Val_Pos2);
    DrawDigitBlink(108, 29, val13, (cursor_pos == 2), mode_active && (entering_val == 1));
    tx = 122; ty = 29;
    GLCDPutSpecialCharDigMax('.');
    DrawDigitBlink(128, 29, val14, (cursor_pos == 1), mode_active && (entering_val == 1));
    DrawDigitBlink(142, 29, val15, (cursor_pos == 0), mode_active && (entering_val == 1));

    char msg_str[25];
    memcpy(msg_str, ConfigMenuMessageTable[config_menu_messageidx], 24);
    msg_str[24] = '\0';
    GLCD_String5x7(3, 57, msg_str);

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// 6. ŞİFRE GİRİŞ MENÜSÜ
//-------------------------------------------------------------------------------------------------

void CreateMenuPassword_Direct(void)
{
    GLCD_ClearAll();

    GLCD_String5x7(3, 17, "ESC");
    GLCD_StringArialBold14(32, 14, "ENTER PASSWORD");
    GLCD_String5x7(171, 17, "ENT");

    GLCD_Rectangle(0, 13, 191, 26, BLACK);
    GLCD_Rectangle(24, 13, 24, 26, BLACK);
    GLCD_Rectangle(167, 13, 167, 26, BLACK);
    GLCD_Rectangle(67, 28, 128, 53, BLACK);

    GLCD_Render();
}

void RefreshMenuPassWord_Direct(void)
{
    WriteDecimalStringShort(ActPassVal);

    unsigned char c1 = (val12 == '0') ? '-' : ':';
    unsigned char c2 = (val13 == '0') ? '-' : ':';
    unsigned char c3 = (val14 == '0') ? '-' : ':';
    unsigned char c4 = (val15 == '0') ? '-' : ':';

    DrawDigitBlink(70, 29, c1, (cursor_pos == 3), 1);
    DrawDigitBlink(84, 29, c2, (cursor_pos == 2), 1);
    DrawDigitBlink(98, 29, c3, (cursor_pos == 1), 1);
    DrawDigitBlink(112, 29, c4, (cursor_pos == 0), 1);

    char msg_str[25];
    memcpy(msg_str, ConfigMenuMessageTable[config_menu_messageidx], 24);
    msg_str[24] = '\0';
    GLCD_String5x7(3, 57, msg_str);

    GLCD_Render();
}

//-------------------------------------------------------------------------------------------------
// ANINDA ÇAĞRI KÖPRÜLERİ (ANINDA RENDER)
//-------------------------------------------------------------------------------------------------

void CreateMenuIdle(unsigned char menucount, unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuIdle_Direct(menucount);
    menu_state = Menu_Created;
}

void RefreshMenuIdle(unsigned char menucount, unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuIdle_Direct(menucount);
    menu_state = Menu_Wait;
}

void CreateMenuSender(unsigned char menucount, unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuSender_Direct(menucount);
    menu_state = Menu_Created;
}

void RefreshMenuSender(unsigned char menucount, unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuSender_Direct(menucount);
    menu_state = Menu_Wait;
}

void CreateMenuConfigID(unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuConfigID_Direct();
    menu_state = Menu_Created;
}

void RefreshMenuConfigID(unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuConfigID_Direct();
    menu_state = Menu_Wait;
}

void CreateMenuConfigParam(unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuConfigParam_Direct();
    menu_state = Menu_Created;
}

void RefreshMenuConfigParam(unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuConfigParam_Direct();
    menu_state = Menu_Wait;
}

void CreateMenuConfigPrice(unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuConfigPrice_Direct();
    menu_state = Menu_Created;
}

void RefreshMenuConfigPrice(unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuConfigPrice_Direct();
    menu_state = Menu_Wait;
}

void CreateMenuPassword(unsigned char updatecnt)
{
    (void)updatecnt;
    CreateMenuPassword_Direct();
    menu_state = Menu_Created;
}

void RefreshMenuPassWord(unsigned char updatecnt)
{
    (void)updatecnt;
    RefreshMenuPassWord_Direct();
    menu_state = Menu_Wait;
}

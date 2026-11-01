#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define Menu_Creating 0
#define Menu_Created 1
#define Menu_Wait 2

#define IDLE 0
#define GOSLEEP 1
#define CONFIGID 2
#define CONFIGPARAM 3
#define CONFIGPRICE 4
#define ENTERPASSWORD 5
#define SENDING 6

#define IDLE_PAGE1 11
#define IDLE_PAGE2 12
#define IDLE_PAGE3 13
#define CONFIG_ID_PAGE 14
#define CONFIG_PARAM_PAGE 15
#define CONFIG_PRICE_PAGE 16
#define PASSWORD_PAGE 17
#define SENDING_PAGE1 18
#define SENDING_PAGE2 19
#define SENDING_PAGE3 20

#define RESEND_ENABLE 127
#define POSLOCK_ENABLE 127

typedef struct
{
  unsigned int Val_Pos1;
  unsigned int Val_Pos2;
} ScreenCommand;

extern unsigned char ProductCSwapTable[8];
extern unsigned char PosCSwapTable[16];

extern unsigned char cursor_pos;
extern unsigned char cursor_blink;
extern unsigned short cursor_blink_time;
extern unsigned char entering_mode;
extern unsigned char entering_val;
extern unsigned char config_menu_messageidx;
extern unsigned char product_table_idx;
extern unsigned char squence_table_idx;
extern unsigned char slaveid_table_idx;
extern unsigned char subid_table_idx;
extern unsigned char subparam_table_idx;
extern unsigned short ActPassVal;

extern const unsigned char MessageTable[4][8];
extern const unsigned char MenuProductTable[9][8];
extern const unsigned char MenuProviderTable1[9][8];
extern const unsigned char MenuProviderTable2[9][8];
extern const unsigned char MenuProviderTable3[9][8];
extern const unsigned char MenuProviderTable4[9][8];
extern const unsigned char MenuProviderTable5[9][8];
extern const unsigned char SqeunceIdTable[24][2];
extern const unsigned char ConfigMenuMessageTable[24][24];
extern const unsigned char ParameterNameTable[27][16];

void REMOTE_CONTROLLER_init(void);

// Sürükleyici ve Menü Prototipleri
void CreateMenuIdle_Direct(unsigned char menucount);
void RefreshMenuIdle_Direct(unsigned char menucount);
void CreateMenuSender_Direct(unsigned char menucount);
void RefreshMenuSender_Direct(unsigned char menucount);
void CreateMenuConfigID_Direct(void);
void RefreshMenuConfigID_Direct(void);
void CreateMenuConfigParam_Direct(void);
void RefreshMenuConfigParam_Direct(void);
void CreateMenuConfigPrice_Direct(void);
void RefreshMenuConfigPrice_Direct(void);
void CreateMenuPassword_Direct(void);
void RefreshMenuPassWord_Direct(void);
void read_keyb(void);

void WriteDecimalStringShort(short value);
void WriteDecimalStringUChar(unsigned char value);

#endif /* REMOTECONTROLLER_H */

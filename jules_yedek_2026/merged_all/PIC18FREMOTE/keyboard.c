// Keyboard Driver
#include "main.h"

unsigned char ButtonNum;
unsigned char ButtonKeyNum = 20;
unsigned char ButtonKeyPress = 0;
static unsigned char LastButtonKeyNum = 20;

unsigned int SetButtonPressTime = 0;
unsigned int StopButtonPressTime = 0;

extern unsigned char main_state;

void read_keyb(void)
{
  BUFE2_LAT = 0;
  __nop();
  RS_LAT = 0;
  EN_LAT = 0;
  RW_LAT = 0;
  BUFEN_LAT = 0;
  __nop();
  BUFDIR_LAT = 1;
  TRISD = 0x00;
  __nop();
  LATD = 0xFF;
  __nop();
  ButtonKeyNum = 20; // BASILI DEĞİL (Default: No Key)

  // Satır 1 Taraması
  D0_LAT = 0; D1_LAT = 1; D2_LAT = 1; D3_LAT = 1;
  if (C0_PORT == 0) { ButtonNum = 1;  ButtonKeyNum = 18; } // SOLA OK
  if (C1_PORT == 0) { ButtonNum = 3;  ButtonKeyNum = 16; } // YUKARI OK
  if (C2_PORT == 0) { ButtonNum = 2;  ButtonKeyNum = 17; } // AŞAĞI OK
  if (C3_PORT == 0) { ButtonNum = 4;  ButtonKeyNum = 13; } // ALM
  if (C4_PORT == 0) { ButtonNum = 5;  ButtonKeyNum = 12; } // ESC

  // Satır 2 Taraması
  D0_LAT = 1; D1_LAT = 0; D2_LAT = 1; D3_LAT = 1;
  if (C0_PORT == 0) { ButtonNum = 6;  ButtonKeyNum = 14; } // +-
  if (C1_PORT == 0) { ButtonNum = 8;  ButtonKeyNum = 15; } // CLR
  if (C2_PORT == 0) { ButtonNum = 7;  ButtonKeyNum = 10; } // Ent
  if (C3_PORT == 0) { ButtonNum = 9;  ButtonKeyNum = 19; } // SAĞA OK
  if (C4_PORT == 0) { ButtonNum = 10; ButtonKeyNum = 11; } // Set

  // Satır 3 Taraması
  D1_LAT = 1; D2_LAT = 0; D0_LAT = 1; D3_LAT = 1;
  if (C0_PORT == 0) { ButtonNum = 11; ButtonKeyNum = 6; }
  if (C1_PORT == 0) { ButtonNum = 13; ButtonKeyNum = 1; }
  if (C2_PORT == 0) { ButtonNum = 12; ButtonKeyNum = 2; }
  if (C3_PORT == 0) { ButtonNum = 14; ButtonKeyNum = 3; }
  if (C4_PORT == 0) { ButtonNum = 15; ButtonKeyNum = 0; }

  // Satır 4 Taraması
  D2_LAT = 1; D3_LAT = 0; D0_LAT = 1; D1_LAT = 1;
  if (C0_PORT == 0) { ButtonNum = 16; ButtonKeyNum = 5; }
  if (C1_PORT == 0) { ButtonNum = 18; ButtonKeyNum = 4; }
  if (C2_PORT == 0) { ButtonNum = 17; ButtonKeyNum = 9; }
  if (C3_PORT == 0) { ButtonNum = 19; ButtonKeyNum = 8; }
  if (C4_PORT == 0) { ButtonNum = 20; ButtonKeyNum = 7; }

  // Pinleri Tekrar Serbest Bırak
  D0_LAT = 1; D1_LAT = 1; D2_LAT = 1; D3_LAT = 1;
  BUFEN_LAT = 1; BUFE2_LAT = 1;
  TRISD = 0x00;

  // TUŞ BASIM DUMUMU / KENAR TETİKLEME (EDGE TRIGGER) ALGORİTMASI
  if (ButtonKeyNum != 20)
  {
      if (LastButtonKeyNum == 20)
      {
          ButtonKeyPress = 1; // Yeni tuş basıldı (Rising Edge)
      }
      else
      {
          ButtonKeyPress = 0; // Basılı tutuluyor (Hold State)
      }
  }
  else
  {
      ButtonKeyPress = 0; // Tuş bırakıldı
  }
  LastButtonKeyNum = ButtonKeyNum;

  // Uzun Basma İşlemleri (Set / Esc)
  if (ButtonKeyNum == 20)
  {
      SetButtonPressTime = 0;
      StopButtonPressTime = 0;
  }
  else if (ButtonKeyNum == 11) // Set Tuşu
  {
      if (SetButtonPressTime > 6555)
      {
          ButtonKeyPress = 0;
      }
      else
      {
          SetButtonPressTime++;
          if (main_state == IDLE)
          {
              if (SetButtonPressTime > 6000)      BUZZ_LAT = 0;
              else if (SetButtonPressTime > 5000) BUZZ_LAT = 1;
              else if (SetButtonPressTime > 4000) BUZZ_LAT = 0;
              else if (SetButtonPressTime > 3000) BUZZ_LAT = 1;
              else if (SetButtonPressTime > 2000) BUZZ_LAT = 0;
              else if (SetButtonPressTime > 1000) BUZZ_LAT = 1;
          }
      }
  }
  else if (ButtonKeyNum == 12) // Esc Tuşu
  {
      if (StopButtonPressTime > 2750)
      {
          ButtonKeyPress = 0;
      }
      else
      {
          StopButtonPressTime++;
          if (main_state == IDLE)
          {
              if (StopButtonPressTime > 2500)      BUZZ_LAT = 1;
              else if (StopButtonPressTime > 2100) BUZZ_LAT = 0;
              else if (StopButtonPressTime > 1700) BUZZ_LAT = 1;
              else if (StopButtonPressTime > 1350) BUZZ_LAT = 0;
              else if (StopButtonPressTime > 999)  BUZZ_LAT = 1;
              else if (StopButtonPressTime > 666)  BUZZ_LAT = 0;
              else if (StopButtonPressTime > 333)  BUZZ_LAT = 1;
          }
      }
  }
}

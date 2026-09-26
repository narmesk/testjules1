// Keyboard
#include "Keyboard_Header.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"
#include "remotecontroller.h"

unsigned char ButtonNum;
unsigned char ButtonKeyNum;
unsigned int SetButtonPressTime = 0;
unsigned int StopButtonPressTime = 0;
extern unsigned char ButtonKeyPress;
extern unsigned char send_ack;

extern unsigned char main_state;

void read_keyb (void)
{
  BUFE2_LAT = 0;
  __nop ();
  RS_LAT = 0;
  EN_LAT = 0;
  RW_LAT = 0;
  BUFEN_LAT = 0;
  __nop ();
  BUFDIR_LAT = 1;
  TRISD = 0x00;
  __nop ();
  LATD = 0xFF;
  __nop ();
  ButtonKeyNum = 20; //BASILI DE??L
  //alttaki 2 adet 1 leme ayn? zamanda nop i?lemi görür
  //0 a set edilen bir sonraki iterasyonda 1 e set edilip seti set sonradan yap
  D0_LAT = 0;
  D1_LAT = 1;
  D2_LAT = 1;
  D3_LAT = 1;

  if (C0_PORT == 0)
    {
      ButtonNum = 1;
      ButtonKeyNum = 18; //SOLA OK
    }

  if (C1_PORT == 0)
    {
      ButtonNum = 3;
      ButtonKeyNum = 16; //YUKARI OK
    }

  if (C2_PORT == 0)
    {
      ButtonNum = 2;
      ButtonKeyNum = 17; //A?A?I OK
    }

  if (C3_PORT == 0)
    {
      ButtonNum = 4;
      ButtonKeyNum = 13; //ALM
    }

  if (C4_PORT == 0)
    {
      ButtonNum = 5;
      ButtonKeyNum = 12; //ESC
    }

  D0_LAT = 1;
  D1_LAT = 0;
  D2_LAT = 1;
  D3_LAT = 1;

  if (C0_PORT == 0)
    {
      ButtonNum = 6;
      ButtonKeyNum = 14; //+-
    }

  if (C1_PORT == 0)
    {
      ButtonNum = 8;
      ButtonKeyNum = 15; //CLR

    }

  if (C2_PORT == 0)
    {
      ButtonNum = 7;
      ButtonKeyNum = 10; //Ent
    }

  if (C3_PORT == 0)
    {
      ButtonNum = 9;
      ButtonKeyNum = 19; //SA?A OK
    }

  if (C4_PORT == 0)
    {
      ButtonNum = 10;
      ButtonKeyNum = 11; //Set
    }

  D1_LAT = 1;
  D2_LAT = 0;
  D0_LAT = 1;
  D3_LAT = 1;

  if (C0_PORT == 0)
    {
      ButtonNum = 11;
      ButtonKeyNum = 6;
    }

  if (C1_PORT == 0)
    {
      ButtonNum = 13;
      ButtonKeyNum = 1;
    }

  if (C2_PORT == 0)
    {
      ButtonNum = 12;
      ButtonKeyNum = 2;
    }

  if (C3_PORT == 0)
    {
      ButtonNum = 14;
      ButtonKeyNum = 3;
    }

  if (C4_PORT == 0)
    {
      ButtonNum = 15;
      ButtonKeyNum = 0;
    }

  D2_LAT = 1;
  D3_LAT = 0;
  D0_LAT = 1;
  D1_LAT = 1;

  if (C0_PORT == 0)
    {
      ButtonNum = 16;
      ButtonKeyNum = 5;
    }

  if (C1_PORT == 0)
    {
      ButtonNum = 18;
      ButtonKeyNum = 4;
    }

  if (C2_PORT == 0)
    {
      ButtonNum = 17;
      ButtonKeyNum = 9;
    }

  if (C3_PORT == 0)
    {
      ButtonNum = 19;
      ButtonKeyNum = 8;
    }

  if (C4_PORT == 0)
    {
      ButtonNum = 20;
      ButtonKeyNum = 7;
    }

  D0_LAT = 1;
  D1_LAT = 1;
  D2_LAT = 1;
  D3_LAT = 1;
  BUFEN_LAT = 1;
  BUFE2_LAT = 1;
  TRISD = 0x00;
  if (ButtonKeyNum == 20)
    {
      SetButtonPressTime = 0;
      StopButtonPressTime = 0;
      send_ack = 0;
    }
  else if (ButtonKeyNum == 11)//set
    {
      if (SetButtonPressTime > 6555)
        {
          ButtonKeyPress = 0;
          //BUZZ_LAT = 0;
        }
      else
        {
          SetButtonPressTime++;
          if (main_state == IDLE)
            {
                if (SetButtonPressTime > 6000)
                {
                  BUZZ_LAT = 0;
                }
              else if (SetButtonPressTime > 5000)
                {
                  BUZZ_LAT = 1;
                }
              else if (SetButtonPressTime > 4000)
                {
                  BUZZ_LAT = 0;
                }
              else if (SetButtonPressTime > 3000)
                {
                  BUZZ_LAT = 1;
                }
              else if (SetButtonPressTime > 2000)
                {
                  BUZZ_LAT = 0;
                }
              else if (SetButtonPressTime > 1000)
                {
                  BUZZ_LAT = 1;
                }
            }
        }
    }
  else if (ButtonKeyNum == 12)//Esc
    {
      if (StopButtonPressTime > 2750)
        {
          ButtonKeyPress = 0;
          //BUZZ_LAT = 0;
        }
      else
        {
          StopButtonPressTime++;
          if (main_state == IDLE)
            {
              if (StopButtonPressTime > 2500)
                {
                  BUZZ_LAT = 1;
                }
              else if (StopButtonPressTime > 2100)
                {
                  BUZZ_LAT = 0;
                }
              else if (StopButtonPressTime > 1700)
                {
                  BUZZ_LAT = 1;
                }
              else if (StopButtonPressTime > 1350)
                {
                  BUZZ_LAT = 0;
                }
              else if (StopButtonPressTime > 999)
                {
                  BUZZ_LAT = 1;
                }
              else if (StopButtonPressTime > 666)
                {
                  BUZZ_LAT = 0;
                }
              else if (StopButtonPressTime > 333)
                {
                  BUZZ_LAT = 1;
                }
            }
        }
    }
}

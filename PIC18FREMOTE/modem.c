#include "mcc_generated_files/mcc.h"
#include "remotecontroller.h"
#include <string.h>
#include "mcc_generated_files/memory.h"
#include "modem.h"
#include "modbus.h"

extern unsigned int setting_update_wait_timer; // 2.5 saniye kadar
extern unsigned char SYS_STATE; //0 idle

extern unsigned char receive_line_number;
extern unsigned int receive_line_val[24];

unsigned char uart1_receive_data = 0;
unsigned char uart1_receive_idx = 0;
unsigned char uart1_receive_buffer[64]; //57byte
unsigned char uart1_transmit_buffer[64];
unsigned char uart1_header_ok_flag = 0;

unsigned char uart1_receive_timeout_en = 0;
unsigned char uart1_receive_timeout_cnt = 0;

unsigned char modem_send_modbus_desp = 0;

unsigned char modem_send_sync_flg = 0;
unsigned char modem_send_sync_desp = 0;
unsigned char modem_send_status_desp = 0;
unsigned char modem_send_config_get_desp = 0;
unsigned char modem_send_master_desp = 0;
unsigned char wait_slave_flag = 0;

extern unsigned char reset_flag;
extern unsigned char config_flag;
extern unsigned char resend_flag;
extern unsigned char success_flag;
extern unsigned long success_timer;
extern unsigned char syncstat;

extern unsigned char SLAVEIDLIST[24];
extern unsigned char SLAVESUBIDLIST[24];
extern unsigned char slaveid_cnt;
extern unsigned char slaveid_control;
extern unsigned char loop_cnt;

extern unsigned char RECEIVEVALFLG2;

extern ScreenCommand Screen_Vals[12];
extern ScreenCommand Config_Vals[13];

extern unsigned char RXCRC_Low, RXCRC_High;

unsigned char modem_sync_packet[8] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00};
unsigned char modem_config_get_packet[8] = {0x01, 0x06, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00};
unsigned char modem_status_packet[8] = {0x02, 0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00};



extern unsigned char modbus_receive_complete;
extern unsigned char modem_receive_silence_signal;

extern unsigned char first_screen_received_flag;
extern unsigned short first_screen_received_timer; // 2.5 saniye kadar


extern unsigned char config_menu_messageidx;

extern unsigned char repeaterloopcnt;

extern unsigned int receive_beeper_timer;
extern unsigned int receiveended_beeper_timer;

unsigned char testidx;
extern unsigned char MIRRORSLAVEIDUPDATESTATE[24];
unsigned char repeater_respond_stat = 255;

extern unsigned char posidentifier;

void Modem_Send_Config_Get(void)
{
  modem_send_config_get_desp = 1;
  ModBus_Get_CRC16_Value(modem_config_get_packet, 6);
  modem_config_get_packet[6] = RXCRC_High;
  modem_config_get_packet[7] = RXCRC_Low;
}

void Modem_Check_Config_Get_Req(void)
{
  if (modem_send_config_get_desp > 0)
    {
      switch (modem_send_config_get_desp)
        {
        case 1:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[0]);
            modem_send_config_get_desp++;
          }
          break;
        case 2:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[1]);
            modem_send_config_get_desp++;
          }
          break;
        case 3:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[2]);
            modem_send_config_get_desp++;
          }
          break;
        case 4:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[3]);
            modem_send_config_get_desp++;
          }
          break;
        case 5:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[4]);
            modem_send_config_get_desp++;
          }
          break;
        case 6:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[5]);
            modem_send_config_get_desp++;
          }
          break;
        case 7:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[6]);
            modem_send_config_get_desp++;
          }
          break;
        case 8:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_config_get_packet[7]);
            modem_send_config_get_desp++;
          }
          break;
        case 9:
          if (UART1_is_tx_done() == 1)
            {
              modem_send_config_get_desp = 0;
            }
          break;
        }
    }
}

void Modem_Send_Status(unsigned char status1, unsigned char status2, unsigned char status3)
{
  modem_send_status_desp = 1;
  modem_status_packet[2] = status1;
  modem_status_packet[3] = status2;
  modem_status_packet[4] = status3;
  ModBus_Get_CRC16_Value(modem_status_packet, 6);
  modem_status_packet[6] = RXCRC_High;
  modem_status_packet[7] = RXCRC_Low;
}

void Modem_Check_Send_Status_Req(void)
{
  if (modem_send_status_desp > 0)
    {
      switch (modem_send_status_desp)
        {
        case 1:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[0]);
            modem_send_status_desp++;
          }
          break;
        case 2:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[1]);
            modem_send_status_desp++;
          }
          break;
        case 3:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[2]);
            modem_send_status_desp++;
          }
          break;
        case 4:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[3]);
            modem_send_status_desp++;
          }
          break;
        case 5:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[4]);
            modem_send_status_desp++;
          }
          break;
        case 6:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[5]);
            modem_send_status_desp++;
          }
          break;
        case 7:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[6]);
            modem_send_status_desp++;
          }
          break;
        case 8:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_status_packet[7]);
            modem_send_status_desp++;
          }
          break;
        case 9:
          if (UART1_is_tx_done() == 1)
            {
              modem_send_status_desp = 0;
            }
          break;
        }
    }
}

void Modem_Send_Sync(void)
{
  modem_send_sync_desp = 1;
  modem_sync_packet[4] = syncstat;
  modem_sync_packet[5] = 0x03;
  ModBus_Get_CRC16_Value(modem_sync_packet, 6);
  modem_sync_packet[6] = RXCRC_High;
  modem_sync_packet[7] = RXCRC_Low;
}

void Modem_Check_Send_Sync_Req(void)
{
  if (modem_send_sync_desp > 0)
    {
      switch (modem_send_sync_desp)
        {
        case 1:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[0]);
            modem_send_sync_desp++;
          }
          break;
        case 2:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[1]);
            modem_send_sync_desp++;
          }
          break;
        case 3:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[2]);
            modem_send_sync_desp++;
          }
          break;
        case 4:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[3]);
            modem_send_sync_desp++;
          }
          break;
        case 5:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[4]);
            modem_send_sync_desp++;
          }
          break;
        case 6:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[5]);
            modem_send_sync_desp++;
          }
          break;
        case 7:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[6]);
            modem_send_sync_desp++;
          }
          break;
        case 8:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[7]);
            modem_send_sync_desp++;
          }
          break;
        case 9:
          if (UART1_is_tx_done() == 1)
            {
              modem_send_sync_desp = 0;
            }
          break;
        }
    }
}

void Modem_Send_Master(void)
{
  modem_send_master_desp = 1;
  modem_sync_packet[4] = 0x00;
  modem_sync_packet[5] = 0x37;
  ModBus_Get_CRC16_Value(modem_sync_packet, 6);
  modem_sync_packet[6] = RXCRC_High;
  modem_sync_packet[7] = RXCRC_Low;
}

void Modem_Send_Slave(void)
{
  modem_send_master_desp = 1;
  modem_sync_packet[4] = 0x00;
  modem_sync_packet[5] = 0x39;
  ModBus_Get_CRC16_Value(modem_sync_packet, 6);
  modem_sync_packet[6] = RXCRC_High;
  modem_sync_packet[7] = RXCRC_Low;
}

void Modem_Check_Send_Master(void)
{
  if (modem_send_master_desp > 0)
    {
      switch (modem_send_master_desp)
        {
        case 1:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[0]);
            modem_send_master_desp++;
          }
          break;
        case 2:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[1]);
            modem_send_master_desp++;
          }
          break;
        case 3:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[2]);
            modem_send_master_desp++;
          }
          break;
        case 4:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[3]);
            modem_send_master_desp++;
          }
          break;
        case 5:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[4]);
            modem_send_master_desp++;
          }
          break;
        case 6:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[5]);
            modem_send_master_desp++;
          }
          break;
        case 7:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[6]);
            modem_send_master_desp++;
          }
          break;
        case 8:
          //if (UART1_is_tx_done () == 1)
          {
            UART1_Write(modem_sync_packet[7]);
            modem_send_master_desp++;
          }
          break;
        case 9:
          if (UART1_is_tx_done() == 1)
            {
              modem_send_master_desp = 0;
            }
          break;
        }
    }
}

void Modem_Check_Idle_Receive_Req(void)
{
  if (UART1_DataReady == 1)
    {
      uart1_receive_data = (unsigned char) UART1_Read();
      if (uart1_header_ok_flag == 1)
        {
          uart1_receive_timeout_en = 1;
          uart1_receive_timeout_cnt = 0;
          uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
          uart1_receive_idx++;
          if (uart1_receive_idx >= 8)
            {
              uart1_receive_idx = 0;
              uart1_header_ok_flag = 0;
              uart1_receive_timeout_en = 0;
              uart1_receive_timeout_cnt = 0;
              ModBus_Get_CRC16_Value(uart1_receive_buffer, 6);
              if ((RXCRC_High == uart1_receive_buffer[6]) && (RXCRC_Low == uart1_receive_buffer[7]))
                {
                  if (posidentifier == 11)
                    {
                      if (uart1_receive_buffer[5] == 0x17)
                        {
                          config_flag = 1;
                        }
                    }
                  else if (posidentifier == 12)
                    {
                      if (uart1_receive_buffer[5] == 0x27)
                        {
                          config_flag = 1;
                        }
                    }
                  else //other
                    {
                      if (uart1_receive_buffer[5] == 0x07)
                        {
                          config_flag = 1;
                        }
                    }
                  if (uart1_receive_buffer[5] == 0x09)
                    {
                      reset_flag = 1;
                    }
                  if (uart1_receive_buffer[5] == 0x0A)
                    {
                      resend_flag = 1;
                      success_flag = 1;
                      success_timer = 0;
                    }
                  if ((uart1_receive_buffer[5] == 0x0B) || (uart1_receive_buffer[5] == 0x37))
                    {
                      receiveended_beeper_timer = 300;
                      receive_beeper_timer = 150;
                      start_procedure();
                    }
                  if (uart1_receive_buffer[5] == 0x39)
                    {
                      wait_slave_flag = 1;
                    }
                  if (uart1_receive_buffer[5] == 0x0C)
                    {
                      success_flag = 1;
                      success_timer = 0;
                      config_menu_messageidx = 11; //ba?ar?l? mesaj? bas?labiliyorsa bas?ls?n bu i?lem delphi ba?ard???ndaki durumdur
                    }
                  if (uart1_receive_buffer[5] == 0x0D)
                    {
                      stop_procedure();
                    }
                  if (uart1_receive_buffer[5] == 0x0E)
                    {
                      first_screen_received_flag = 1;
                      first_screen_received_timer = 0;
                    }
                  if (uart1_receive_buffer[5] == 0x08)
                    {
                      if (SYS_STATE != 4)
                        {
                          Modem_Send_Status(0xFF, 0xFF, 0x08);
                          SYS_STATE = 4;
                        }
                    }
                }
              memset((void*) uart1_receive_buffer, 0x00, 8);
            }
        }
      else
        {
          if ((uart1_receive_idx == 0) && (uart1_receive_data == 0x01))
            {
              uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
              uart1_receive_idx++;
              uart1_header_ok_flag = 1;
              uart1_receive_timeout_en = 1;
              uart1_receive_timeout_cnt = 0;
            }
        }
    }
}

void Modem_Check_Config_Receive_Req(unsigned char id)
{
  unsigned int idx;
  if (UART1_DataReady == 1)
    {
      uart1_receive_data = (unsigned char) UART1_Read();
      if (uart1_header_ok_flag == 1)
        {
          uart1_receive_timeout_en = 1;
          uart1_receive_timeout_cnt = 0;
          uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
          uart1_receive_idx++;
          if (uart1_receive_idx >= 57)
            {
              uart1_receive_idx = 0;
              uart1_header_ok_flag = 0;
              uart1_receive_timeout_en = 0;
              uart1_receive_timeout_cnt = 0;
              ModBus_Get_CRC16_Value(uart1_receive_buffer, 55);
              if ((RXCRC_High == uart1_receive_buffer[55]) && (RXCRC_Low == uart1_receive_buffer[56]))
                {
                  slaveid_cnt = uart1_receive_buffer[2];
                  loop_cnt = uart1_receive_buffer[3];
                  Config_Vals[0].Val_Pos1 = (unsigned int) (uart1_receive_buffer[7] << 8) | (unsigned int) uart1_receive_buffer[8];
                  Config_Vals[0].Val_Pos2 = (unsigned int) (uart1_receive_buffer[9] << 8) | (unsigned int) uart1_receive_buffer[10];
                  Config_Vals[1].Val_Pos1 = (unsigned int) (uart1_receive_buffer[11] << 8) | (unsigned int) uart1_receive_buffer[12];
                  Config_Vals[1].Val_Pos2 = (unsigned int) (uart1_receive_buffer[13] << 8) | (unsigned int) uart1_receive_buffer[14];
                  Config_Vals[2].Val_Pos1 = (unsigned int) (uart1_receive_buffer[15] << 8) | (unsigned int) uart1_receive_buffer[16];
                  Config_Vals[2].Val_Pos2 = (unsigned int) (uart1_receive_buffer[17] << 8) | (unsigned int) uart1_receive_buffer[18];
                  Config_Vals[3].Val_Pos1 = (unsigned int) (uart1_receive_buffer[19] << 8) | (unsigned int) uart1_receive_buffer[20];
                  Config_Vals[3].Val_Pos2 = (unsigned int) (uart1_receive_buffer[21] << 8) | (unsigned int) uart1_receive_buffer[22];
                  Config_Vals[4].Val_Pos1 = (unsigned int) (uart1_receive_buffer[23] << 8) | (unsigned int) uart1_receive_buffer[24];
                  Config_Vals[4].Val_Pos2 = (unsigned int) (uart1_receive_buffer[25] << 8) | (unsigned int) uart1_receive_buffer[26];
                  Config_Vals[5].Val_Pos1 = (unsigned int) (uart1_receive_buffer[27] << 8) | (unsigned int) uart1_receive_buffer[28];
                  Config_Vals[5].Val_Pos2 = (unsigned int) (uart1_receive_buffer[29] << 8) | (unsigned int) uart1_receive_buffer[30];
                  Config_Vals[6].Val_Pos1 = (unsigned int) (uart1_receive_buffer[31] << 8) | (unsigned int) uart1_receive_buffer[32];
                  Config_Vals[6].Val_Pos2 = (unsigned int) (uart1_receive_buffer[33] << 8) | (unsigned int) uart1_receive_buffer[34];
                  Config_Vals[7].Val_Pos1 = (unsigned int) (uart1_receive_buffer[35] << 8) | (unsigned int) uart1_receive_buffer[36];
                  Config_Vals[7].Val_Pos2 = (unsigned int) (uart1_receive_buffer[37] << 8) | (unsigned int) uart1_receive_buffer[38];
                  Config_Vals[8].Val_Pos1 = (unsigned int) (uart1_receive_buffer[39] << 8) | (unsigned int) uart1_receive_buffer[40];
                  Config_Vals[8].Val_Pos2 = (unsigned int) (uart1_receive_buffer[41] << 8) | (unsigned int) uart1_receive_buffer[42];
                  Config_Vals[9].Val_Pos1 = (unsigned int) (uart1_receive_buffer[43] << 8) | (unsigned int) uart1_receive_buffer[44];
                  Config_Vals[9].Val_Pos2 = (unsigned int) (uart1_receive_buffer[45] << 8) | (unsigned int) uart1_receive_buffer[46];
                  Config_Vals[10].Val_Pos1 = (unsigned int) (uart1_receive_buffer[47] << 8) | (unsigned int) uart1_receive_buffer[48];
                  Config_Vals[10].Val_Pos2 = (unsigned int) (uart1_receive_buffer[49] << 8) | (unsigned int) uart1_receive_buffer[50];
                  Config_Vals[11].Val_Pos1 = (unsigned int) (uart1_receive_buffer[51] << 8) | (unsigned int) uart1_receive_buffer[52];
                  Config_Vals[11].Val_Pos2 = (unsigned int) (uart1_receive_buffer[53] << 8) | (unsigned int) uart1_receive_buffer[54];
                  Config_Vals[12].Val_Pos1 = (unsigned int) slaveid_cnt;
                  Config_Vals[12].Val_Pos2 = (unsigned int) loop_cnt;
                  ClrWdt();
                  INTERRUPT_GlobalInterruptHighDisable();
                  INTERRUPT_GlobalInterruptLowDisable();
                  //    INTERRUPT_PeripheralInterruptDisable ();
                  write_config_paremeter_eeprom();
                  write_slaveidcnt_paremeter_eeprom();
                  write_loopcnt_paremeter_eeprom();
                  read_config_paremeter_eeprom();
                  read_slaveidcnt_paremeter_eeprom();
                  read_loopcnt_paremeter_eeprom();
                  ClrWdt();
                  eeprom_to_slave_idlist();
                  ClrWdt();
                  ModBus_Get_CRC16_Value(modem_config_get_packet, 6);
                  modem_config_get_packet[6] = RXCRC_High;
                  modem_config_get_packet[7] = RXCRC_Low;
                  for (idx = 0; idx < 8; idx++)
                    {
                      UART1_Write(modem_config_get_packet[idx]);
                      while (!UART1_is_tx_done());
                    }
                  while (!UART1_is_tx_done());
                  setting_update_wait_timer = 0;
                  SYS_STATE = 0;
                  INTERRUPT_GlobalInterruptHighEnable();
                  INTERRUPT_GlobalInterruptLowEnable();
                  //INTERRUPT_PeripheralInterruptEnable ();
                }
              memset((void*) uart1_receive_buffer, 0x00, sizeof (uart1_receive_buffer));
            }
        }
    }
  else
    {
      if ((uart1_receive_idx == 0) && (uart1_receive_data == id)) //0x01))
        {
          uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
          uart1_receive_idx++;
          uart1_header_ok_flag = 1;
          uart1_receive_timeout_en = 1;
          uart1_receive_timeout_cnt = 0;
        }
    }
}

void transfer_pos_to_price() {
  /*Screen_Vals[0].Val_Pos1 = receive_line_val[0];
  Screen_Vals[0].Val_Pos2 = receive_line_val[1];
  Screen_Vals[1].Val_Pos1 = receive_line_val[2];
  Screen_Vals[1].Val_Pos2 = receive_line_val[3];
  Screen_Vals[2].Val_Pos1 = receive_line_val[4];
  Screen_Vals[2].Val_Pos2 = receive_line_val[5];
  Screen_Vals[3].Val_Pos1 = receive_line_val[6];
  Screen_Vals[3].Val_Pos2 = receive_line_val[7];
  Screen_Vals[4].Val_Pos1 = receive_line_val[8];
  Screen_Vals[4].Val_Pos2 = receive_line_val[9];
  Screen_Vals[5].Val_Pos1 = receive_line_val[10];
  Screen_Vals[5].Val_Pos2 = receive_line_val[11];
  Screen_Vals[6].Val_Pos1 = receive_line_val[12];
  Screen_Vals[6].Val_Pos2 = receive_line_val[13];*/
  /* Screen_Vals[7].Val_Pos1 = receive_line_val[14];
   Screen_Vals[7].Val_Pos2 = receive_line_val[15];
   Screen_Vals[8].Val_Pos1 = receive_line_val[16];
   Screen_Vals[8].Val_Pos2 = receive_line_val[17];
   Screen_Vals[9].Val_Pos1 = receive_line_val[18];
   Screen_Vals[9].Val_Pos2 = receive_line_val[19];
   Screen_Vals[10].Val_Pos1 = receive_line_val[20];
   Screen_Vals[10].Val_Pos2 = receive_line_val[21];
   Screen_Vals[11].Val_Pos1 = receive_line_val[22];
   Screen_Vals[11].Val_Pos2 = receive_line_val[23];*/ }


//Burda hangi glcd de?erine ne kar??l?k gelir onu yazaca??z
//9 adet fiyat?m?z var indis 0 dan ba?l?yor 8 e kadar
//E?er hiç ellemez isek diesel,regular,plus supreme gözüküyor
//sebebi 3 nolu indise sahip dieselin ilk yap?lan pumptoperlarda 0 a yaz?lmas?
//bu kaymadan dolay? 3-0 0-1 1-2 2-3 e dos?ru kay?yor ilk 4 de?eri kayd?r?p def
//kerosen diye devam ediyoruz diye devam ediyoruz ilk 11 sat?r kod iptal olursa
//eski haline döner geriye do?ru uyumluluk için

void transfer_price_to_uart()
{
  uart1_transmit_buffer[7] = (unsigned char) (Screen_Vals[ProductCSwapTable[0]].Val_Pos1 >> 8); //0x00; //1 dizel cash olabilir ama a?a??da var normalde yol
  uart1_transmit_buffer[8] = (unsigned char) (Screen_Vals[ProductCSwapTable[0]].Val_Pos1 & 0x00FF); //
  uart1_transmit_buffer[9] = (unsigned char) (Screen_Vals[ProductCSwapTable[0]].Val_Pos2 >> 8); //dizel credit
  uart1_transmit_buffer[10] = (unsigned char) (Screen_Vals[ProductCSwapTable[0]].Val_Pos2 & 0x00FF);
  uart1_transmit_buffer[11] = (unsigned char) (Screen_Vals[ProductCSwapTable[1]].Val_Pos1 >> 8); //0x03; //2 sol cash regular
  uart1_transmit_buffer[12] = (unsigned char) (Screen_Vals[ProductCSwapTable[1]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[13] = (unsigned char) (Screen_Vals[ProductCSwapTable[1]].Val_Pos2 >> 8); // 0x03; //sol credit
  uart1_transmit_buffer[14] = (unsigned char) (Screen_Vals[ProductCSwapTable[1]].Val_Pos2 & 0x00FF); // 0x6C; //
  uart1_transmit_buffer[15] = (unsigned char) (Screen_Vals[ProductCSwapTable[2]].Val_Pos1 >> 8); //0x02; //3 orta cash   plus
  uart1_transmit_buffer[16] = (unsigned char) (Screen_Vals[ProductCSwapTable[2]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[17] = (unsigned char) (Screen_Vals[ProductCSwapTable[2]].Val_Pos2 >> 8); //0x02; //orta credit
  uart1_transmit_buffer[18] = (unsigned char) (Screen_Vals[ProductCSwapTable[2]].Val_Pos2 & 0x00FF); //0x6C; //
  uart1_transmit_buffer[19] = (unsigned char) (Screen_Vals[ProductCSwapTable[3]].Val_Pos1 >> 8); //0x01; //4 sa? cash   supreme
  uart1_transmit_buffer[20] = (unsigned char) (Screen_Vals[ProductCSwapTable[3]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[21] = (unsigned char) (Screen_Vals[ProductCSwapTable[3]].Val_Pos2 >> 8); //0x01; //sa? credit
  uart1_transmit_buffer[22] = (unsigned char) (Screen_Vals[ProductCSwapTable[3]].Val_Pos2 & 0x00FF); //0x6C; //
  uart1_transmit_buffer[23] = (unsigned char) (Screen_Vals[ProductCSwapTable[4]].Val_Pos1 >> 8); //0x00; //1 dizel cash olabilir ama a?a??da var normalde yol
  uart1_transmit_buffer[24] = (unsigned char) (Screen_Vals[ProductCSwapTable[4]].Val_Pos1 & 0x00FF); //
  uart1_transmit_buffer[25] = (unsigned char) (Screen_Vals[ProductCSwapTable[4]].Val_Pos2 >> 8); //dizel credit
  uart1_transmit_buffer[26] = (unsigned char) (Screen_Vals[ProductCSwapTable[4]].Val_Pos2 & 0x00FF);
  uart1_transmit_buffer[27] = (unsigned char) (Screen_Vals[ProductCSwapTable[5]].Val_Pos1 >> 8); //0x03; //2 sol cash regular
  uart1_transmit_buffer[28] = (unsigned char) (Screen_Vals[ProductCSwapTable[5]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[29] = (unsigned char) (Screen_Vals[ProductCSwapTable[5]].Val_Pos2 >> 8); // 0x03; //sol credit
  uart1_transmit_buffer[30] = (unsigned char) (Screen_Vals[ProductCSwapTable[5]].Val_Pos2 & 0x00FF); // 0x6C; //
  uart1_transmit_buffer[31] = (unsigned char) (Screen_Vals[ProductCSwapTable[6]].Val_Pos1 >> 8); //0x02; //3 orta cash   plus
  uart1_transmit_buffer[32] = (unsigned char) (Screen_Vals[ProductCSwapTable[6]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[33] = (unsigned char) (Screen_Vals[ProductCSwapTable[6]].Val_Pos2 >> 8); //0x02; //orta credit
  uart1_transmit_buffer[34] = (unsigned char) (Screen_Vals[ProductCSwapTable[6]].Val_Pos2 & 0x00FF); //0x6C; //
  uart1_transmit_buffer[35] = (unsigned char) (Screen_Vals[ProductCSwapTable[7]].Val_Pos1 >> 8); //0x01; //4 sa? cash   supreme
  uart1_transmit_buffer[36] = (unsigned char) (Screen_Vals[ProductCSwapTable[7]].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[37] = (unsigned char) (Screen_Vals[ProductCSwapTable[7]].Val_Pos2 >> 8); //0x01; //sa? credit
  uart1_transmit_buffer[38] = (unsigned char) (Screen_Vals[ProductCSwapTable[7]].Val_Pos2 & 0x00FF); //0x6C; //
  uart1_transmit_buffer[39] = (unsigned char) (Screen_Vals[ProductCSwapTable[8]].Val_Pos1 >> 8); //0x00; //1 dizel cash olabilir ama a?a??da var normalde yol
  uart1_transmit_buffer[40] = (unsigned char) (Screen_Vals[ProductCSwapTable[8]].Val_Pos1 & 0x00FF); //
  uart1_transmit_buffer[41] = (unsigned char) (Screen_Vals[ProductCSwapTable[8]].Val_Pos2 >> 8); //dizel credit
  uart1_transmit_buffer[42] = (unsigned char) (Screen_Vals[ProductCSwapTable[8]].Val_Pos2 & 0x00FF);
  uart1_transmit_buffer[43] = (unsigned char) (Screen_Vals[9].Val_Pos1 >> 8); //0x03; //2 sol cash regular
  uart1_transmit_buffer[44] = (unsigned char) (Screen_Vals[9].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[45] = (unsigned char) (Screen_Vals[9].Val_Pos2 >> 8); // 0x03; //sol credit
  uart1_transmit_buffer[46] = (unsigned char) (Screen_Vals[9].Val_Pos2 & 0x00FF); // 0x6C; //
  uart1_transmit_buffer[47] = (unsigned char) (Screen_Vals[10].Val_Pos1 >> 8); //0x02; //3 orta cash   plus
  uart1_transmit_buffer[48] = (unsigned char) (Screen_Vals[10].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[49] = (unsigned char) (Screen_Vals[10].Val_Pos2 >> 8); //0x02; //orta credit
  uart1_transmit_buffer[50] = (unsigned char) (Screen_Vals[10].Val_Pos2 & 0x00FF); //0x6C; //
  uart1_transmit_buffer[51] = (unsigned char) (Screen_Vals[11].Val_Pos1 >> 8); //0x01; //4 sa? cash   supreme
  uart1_transmit_buffer[52] = (unsigned char) (Screen_Vals[11].Val_Pos1 & 0x00FF); //0xDB; //
  uart1_transmit_buffer[53] = (unsigned char) (Screen_Vals[11].Val_Pos2 >> 8); //0x01; //sa? credit
  uart1_transmit_buffer[54] = (unsigned char) (Screen_Vals[11].Val_Pos2 & 0x00FF); //0x6C; //
  for (testidx = 0; testidx < 8; testidx++)
    {
      if (MIRRORSLAVEIDUPDATESTATE[testidx] != 1)
        {
          CLEAR_BIT(uart1_transmit_buffer[51], testidx);
        }
      else
        {
          SET_BIT(uart1_transmit_buffer[51], testidx);
        }
    }
  for (testidx = 0; testidx < 8; testidx++)
    {
      if (MIRRORSLAVEIDUPDATESTATE[testidx + 8] != 1)
        {
          CLEAR_BIT(uart1_transmit_buffer[52], testidx);
        }
      else
        {
          SET_BIT(uart1_transmit_buffer[52], testidx);
        }
    }
  for (testidx = 0; testidx < 8; testidx++)
    {
      if (MIRRORSLAVEIDUPDATESTATE[testidx + 16] != 1)
        {
          CLEAR_BIT(uart1_transmit_buffer[53], testidx);
        }
      else
        {
          SET_BIT(uart1_transmit_buffer[53], testidx);
        }
    }
  uart1_transmit_buffer[54] = 0x00; //repeater_respond_stat;
  ModBus_Get_CRC16_Value(uart1_transmit_buffer, 55);
  uart1_transmit_buffer[55] = RXCRC_High;
  uart1_transmit_buffer[56] = RXCRC_Low;
}

void modem_send_modbus_request(unsigned char Send_Slave_Id)
{
  uart1_transmit_buffer[0] = Send_Slave_Id; //0x25; //37
  uart1_transmit_buffer[1] = 0x10; //16
  uart1_transmit_buffer[2] = 0x00; //adresh
  uart1_transmit_buffer[3] = 0x00; //adresl
  uart1_transmit_buffer[4] = 0x00; //word quanty h
  uart1_transmit_buffer[5] = 0x18; //word quanty l
  uart1_transmit_buffer[6] = 0x30; //bytecount
  if (RECEIVEVALFLG2 == 1)//ekran verileri commander gelmezse ekrandan gelecek verileri
    {
      transfer_pos_to_price();
    }
  transfer_price_to_uart();
  modem_send_modbus_desp = 1;
}

void modem_send_modbus_transmit_buffer(void)
{
  // if (UART1_is_tx_done () == 1)
  {
    if (modem_send_modbus_desp < 58)
      {
        if (modem_send_modbus_desp != 0)
          {
            UART1_Write((unsigned int) uart1_transmit_buffer[modem_send_modbus_desp - 1]);
            modem_send_modbus_desp++;
          }
      }
  }
}

void modem_send_modbus_repeater_request(unsigned char Send_Slave_Id, unsigned char Send_SubSlave_Id)
{
  uart1_transmit_buffer[0] = Send_Slave_Id; //0x25; //37
  uart1_transmit_buffer[1] = 0x10; //16
  uart1_transmit_buffer[2] = repeaterloopcnt; //repeaterdaki loop cnt/0x00; //adresh
  uart1_transmit_buffer[3] = Send_SubSlave_Id; //adresl
  uart1_transmit_buffer[4] = 0x00; //word quanty h
  uart1_transmit_buffer[5] = 0x18; //word quanty l
  uart1_transmit_buffer[6] = 0x30; //bytecount
  if (RECEIVEVALFLG2 == 1)//ekran verileri commander gelmezse ekrandan gelecek verileri
    {
      transfer_pos_to_price();
    }
  transfer_price_to_uart();
  modem_send_modbus_desp = 1;
}

void modem_receive_modbus_request(unsigned char Send_Slave_Id)
{
  if (UART1_DataReady == 1)
    {
      uart1_receive_data = (unsigned char) UART1_Read();
      if (uart1_header_ok_flag == 1)
        {
          uart1_receive_timeout_en = 1;
          uart1_receive_timeout_cnt = 0;
          uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
          uart1_receive_idx++;
          if (uart1_receive_idx >= 8)
            {
              uart1_receive_idx = 0;
              uart1_header_ok_flag = 0;
              uart1_receive_timeout_en = 0;
              uart1_receive_timeout_cnt = 0;
              ModBus_Get_CRC16_Value(uart1_receive_buffer, 6);
              if ((RXCRC_High == uart1_receive_buffer[6]) && (RXCRC_Low == uart1_receive_buffer[7]))
                {
                  modbus_receive_complete = 1;
                  repeater_respond_stat = uart1_receive_buffer[2]; //actloop
                }
              memset((void*) uart1_receive_buffer, 0x00, 8);
            }
        }
      else
        {
          if ((uart1_receive_idx == 0) && (uart1_receive_data == Send_Slave_Id))
            {
              uart1_receive_buffer[uart1_receive_idx] = uart1_receive_data;
              uart1_receive_idx++;
              uart1_header_ok_flag = 1;
              uart1_receive_timeout_en = 1;
              uart1_receive_timeout_cnt = 0;
            }
        }
    }
}

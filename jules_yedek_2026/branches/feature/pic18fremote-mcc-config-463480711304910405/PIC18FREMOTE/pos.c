#include "mcc_generated_files/mcc.h"
#include "remotecontroller.h"
#include <string.h>
#include "pos.h"


//uart için

unsigned char uart2_receive_data = 0;
unsigned char uart2_receive_idx = 0;
unsigned char uart2_receive_buffer[32];
unsigned char uart2_header_ok_flag = 0;

unsigned char uart2_receive_timeout_en = 0;
unsigned char uart2_receive_timeout_cnt = 0;


//commander haberle?mesi
unsigned char pos_send_ack_flg = 0;
unsigned char pos_send_nack_flg = 0;
unsigned char pos_send_sync_flg = 0;
unsigned char pos_send_sync_desp = 0;

unsigned char receive_line_number = 0;
unsigned int receive_line_val[24];
unsigned int receive_line_val_temp[24];
unsigned int last_receive_line_val[24];
unsigned int last_receive_line_val_temp[24];
unsigned long receive_count;
unsigned long receive_countsetval1 = 10000; //10000yakla??k 5 sn

extern unsigned char posidentifier;


extern unsigned char syncstat;
extern unsigned char RECEIVEVALFLG1;
extern unsigned char RECEIVEVALFLG2;
extern unsigned char TIMERFLG1;

extern unsigned int receive_beeper_timer;
extern unsigned int receiveended_beeper_timer;

extern ScreenCommand Screen_Vals[12];

void Pos_Send_Ack(void)
{
  //if (UART2_is_tx_done () == 1)
  {
    if (posidentifier == 3)
      {
        UART2_Write((0x06 + 0x80));
      }
    else
      {
        UART2_Write(0x06);
      }
  }
}

void Pos_Send_Nack(void)
{
  //if (UART2_is_tx_done () == 1)
  {
    if (posidentifier == 3)
      {
        UART2_Write((0x15 + 0x80));
      }
    else
      {

        UART2_Write(0x15);
      }
  }
}

void Pos_Send_Sync(void)
{
  pos_send_sync_desp = 1;
}

void Pos_Check_Send_Sync_Req(void)
{
  if (pos_send_sync_desp > 0)
    {
      switch (pos_send_sync_desp)
        {
        case 1:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x02);
            pos_send_sync_desp++;
          }
          break;
        case 2:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x31);
            pos_send_sync_desp++;
          }
          break;
        case 3:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x30);
            pos_send_sync_desp++;
          }
          break;
        case 4:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x30);
            pos_send_sync_desp++;
          }
          break;
        case 5:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x30);
            pos_send_sync_desp++;
          }
          break;
        case 6:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x30);
            pos_send_sync_desp++;
          }
          break;
        case 7:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x31);
            pos_send_sync_desp++;
          }
          break;
        case 8:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x03);
            pos_send_sync_desp++;
          }
          break;
        case 9:
          //if (UART2_is_tx_done () == 1)
          {
            UART2_Write(0x03);
            pos_send_sync_desp++;
          }
          break;
        case 10:
          if (UART2_is_tx_done() == 1)
            {

              pos_send_sync_desp = 0;
            }
          break;
        }
    }
}

void Pos_Check_Receive_Req(void)
{
  if (UART2_DataReady == 1)
    {
      uart2_receive_data = (unsigned char) UART2_Read();
      if (uart2_header_ok_flag == 1)
        {
          uart2_receive_timeout_en = 1;
          uart2_receive_timeout_cnt = 0;
          uart2_receive_buffer[uart2_receive_idx] = uart2_receive_data;
          uart2_receive_idx++;
          if (uart2_receive_idx >= 15)
            {
              uart2_receive_idx = 0;
              uart2_header_ok_flag = 0;
              if (uart2_receive_buffer[14] == (uart2_receive_buffer[1]^uart2_receive_buffer[2]^uart2_receive_buffer[3]^uart2_receive_buffer[4]^uart2_receive_buffer[5]^uart2_receive_buffer[6]^uart2_receive_buffer[7]^uart2_receive_buffer[8]^uart2_receive_buffer[9]^uart2_receive_buffer[10]^uart2_receive_buffer[11]^uart2_receive_buffer[12]^uart2_receive_buffer[13]))
                {
                  //buras? bir önceki byteonlar dahnesini gösterir diye dü?ünüldü
                  receive_line_number = (uart2_receive_buffer[4] - 0x30)*10;
                  receive_line_number = receive_line_number + (uart2_receive_buffer[5] - 0x30);
                  syncstat = receive_line_number + 1;
                  receive_line_val_temp[receive_line_number ] = (uart2_receive_buffer[8] - 0x30)*1000 + (uart2_receive_buffer[9] - 0x30)*100 + (uart2_receive_buffer[10] - 0x30)*10 + (uart2_receive_buffer[11] - 0x30);
                  receive_line_val_temp[receive_line_number ] = (receive_line_val_temp[receive_line_number] / 10);

                  if (receive_line_val_temp[receive_line_number] != last_receive_line_val_temp[receive_line_number])
                    {
                      last_receive_line_val_temp[receive_line_number] = receive_line_val_temp[receive_line_number];
                      receive_line_val[PosCSwapTable[receive_line_number]] = receive_line_val_temp[receive_line_number];
                      switch (PosCSwapTable[receive_line_number])
                        {
                        case 0:
                          Screen_Vals[0].Val_Pos1 = receive_line_val[0];
                          break;
                        case 1:
                          Screen_Vals[0].Val_Pos2 = receive_line_val[1];
                          break;
                        case 2:
                          Screen_Vals[1].Val_Pos1 = receive_line_val[2];
                          break;
                        case 3:
                          Screen_Vals[1].Val_Pos2 = receive_line_val[3];
                          break;
                        case 4:
                          Screen_Vals[2].Val_Pos1 = receive_line_val[4];
                          break;
                        case 5:
                          Screen_Vals[2].Val_Pos2 = receive_line_val[5];
                          break;
                        case 6:
                          Screen_Vals[3].Val_Pos1 = receive_line_val[6];
                          break;
                        case 7:
                          Screen_Vals[3].Val_Pos2 = receive_line_val[7];
                          break;
                        case 8:
                          Screen_Vals[4].Val_Pos1 = receive_line_val[8];
                          break;
                        case 9:
                          Screen_Vals[4].Val_Pos2 = receive_line_val[9];
                          break;
                        case 10:
                          Screen_Vals[5].Val_Pos1 = receive_line_val[10];
                          break;
                        case 11:
                          Screen_Vals[5].Val_Pos2 = receive_line_val[11];
                          break;
                        case 12:
                          Screen_Vals[6].Val_Pos1 = receive_line_val[12];
                          break;
                        case 13:
                          Screen_Vals[6].Val_Pos2 = receive_line_val[13];
                          break;
                        }
                      //ilk gelen veri ile 5 sn lik timeri ba?lat?r 5 sn sonunda timer göndermeyi ba?lats?n
                      if (RECEIVEVALFLG1 == 1)
                        {
                          receive_count = 0; //yeni veride 5 snlik timer resetlenir
                        }
                      receiveended_beeper_timer = 10000;
                      receive_beeper_timer = 150;
                      RECEIVEVALFLG1 = 1;
                      RECEIVEVALFLG2 = 1;
                    }
                  //gönderme iste?i
                  pos_send_ack_flg = 1;
                  uart2_receive_timeout_en = 0;
                  uart2_receive_timeout_cnt = 0;
                  //ack
                }
              else
                {
                  //nack
                  pos_send_nack_flg = 1;
                  uart2_receive_idx = 0;
                  uart2_header_ok_flag = 0;
                  memset((void*) uart2_receive_buffer, 0x00, sizeof (uart2_receive_buffer));
                  syncstat = 0;
                  uart2_receive_timeout_en = 0;
                  uart2_receive_timeout_cnt = 0;
                }
            }
          else
            {
              if ((uart2_receive_idx >= 6) && (uart2_receive_buffer[3] == 0x30))
                {
                  uart2_receive_idx = 0;
                  uart2_header_ok_flag = 0;
                  if (uart2_receive_buffer[5] == (uart2_receive_buffer[1]^uart2_receive_buffer[2]^uart2_receive_buffer[3]^uart2_receive_buffer[4]))
                    {
                      pos_send_sync_flg = 1;
                      if (syncstat < 103)
                        {
                          syncstat = 103;
                        }
                      syncstat++;
                      if (syncstat >= 199)
                        {
                          syncstat = 103;
                        }
                      //sync
                      uart2_receive_timeout_en = 0;
                      uart2_receive_timeout_cnt = 0;
                    }
                  else
                    {

                      uart2_receive_idx = 0;
                      uart2_header_ok_flag = 0;
                      memset((void*) uart2_receive_buffer, 0x00, sizeof (uart2_receive_buffer));
                      syncstat = 0;
                      uart2_receive_timeout_en = 0;
                      uart2_receive_timeout_cnt = 0;
                    }
                }
            }
        }
      else
        {
          if ((uart2_receive_idx == 0) && (uart2_receive_data == 0x02))
            {

              uart2_receive_buffer[uart2_receive_idx] = uart2_receive_data;
              uart2_receive_idx++;
              uart2_header_ok_flag = 1;
              uart2_receive_timeout_en = 1;
              uart2_receive_timeout_cnt = 0;
            }
        }
    }
}

void Pos_Check_Gilbar_Receive_Req(void)
{
  if (UART2_DataReady == 1)
    {
      uart2_receive_data = (unsigned char) UART2_Read();
      if (uart2_header_ok_flag == 1)
        {
          uart2_receive_timeout_en = 1;
          uart2_receive_timeout_cnt = 0;
          if (posidentifier == 2)
            {
              uart2_receive_buffer[uart2_receive_idx] = uart2_receive_data;
            }
          else
            {
              uart2_receive_buffer[uart2_receive_idx] = 0x80 + uart2_receive_data;
            }
          uart2_receive_idx++;
          if (uart2_receive_idx >= 9)
            {
              uart2_receive_idx = 0;
              uart2_header_ok_flag = 0;
              if (uart2_receive_buffer[8] == (uart2_receive_buffer[0]^uart2_receive_buffer[1]^uart2_receive_buffer[2]^uart2_receive_buffer[3]^uart2_receive_buffer[4]^uart2_receive_buffer[5]^uart2_receive_buffer[6]^uart2_receive_buffer[7]))
                {
                  //buras? bir önceki byteonlar dahnesini gösterir diye dü?ünüldü
                  receive_line_number = (uart2_receive_buffer[2] - 0x31);

                  receive_line_val[receive_line_number ] = (uart2_receive_buffer[3] - 0x30)*1000 + (uart2_receive_buffer[4] - 0x30)*100 + (uart2_receive_buffer[5] - 0x30)*10 + (uart2_receive_buffer[6] - 0x30);
                  receive_line_val[receive_line_number ] = (receive_line_val[receive_line_number] / 10);
                  if (receive_line_val[receive_line_number ] != last_receive_line_val[receive_line_number ])
                    {
                      last_receive_line_val[receive_line_number ] = receive_line_val[receive_line_number ];
                      syncstat = receive_line_number + 1;
                      switch (PosCSwapTable[receive_line_number])
                        {
                        case 0:
                          Screen_Vals[0].Val_Pos1 = receive_line_val[0];
                          break;
                        case 1:
                          Screen_Vals[0].Val_Pos2 = receive_line_val[1];
                          break;
                        case 2:
                          Screen_Vals[1].Val_Pos1 = receive_line_val[2];
                          break;
                        case 3:
                          Screen_Vals[1].Val_Pos2 = receive_line_val[3];
                          break;
                        case 4:
                          Screen_Vals[2].Val_Pos1 = receive_line_val[4];
                          break;
                        case 5:
                          Screen_Vals[2].Val_Pos2 = receive_line_val[5];
                          break;
                        case 6:
                          Screen_Vals[3].Val_Pos1 = receive_line_val[6];
                          break;
                        case 7:
                          Screen_Vals[3].Val_Pos2 = receive_line_val[7];
                          break;
                        case 8:
                          Screen_Vals[4].Val_Pos1 = receive_line_val[8];
                          break;
                        case 9:
                          Screen_Vals[4].Val_Pos2 = receive_line_val[9];
                          break;
                        case 10:
                          Screen_Vals[5].Val_Pos1 = receive_line_val[10];
                          break;
                        case 11:
                          Screen_Vals[5].Val_Pos2 = receive_line_val[11];
                          break;
                        case 12:
                          Screen_Vals[6].Val_Pos1 = receive_line_val[12];
                          break;
                        case 13:
                          Screen_Vals[6].Val_Pos2 = receive_line_val[13];
                          break;
                        }

                      //ilk gelen veri ile 5 sn lik timeri ba?lat?r 5 sn sonunda timer göndermeyi ba?lats?n
                      if (RECEIVEVALFLG1 == 1)
                        {
                          receive_count = 0; //yeni veride 5 snlik timer resetlenir
                        }
                      receiveended_beeper_timer = 10000;
                      receive_beeper_timer = 150;
                      RECEIVEVALFLG1 = 1;
                      RECEIVEVALFLG2 = 1;
                    }
                  //gönderme iste?i
                  pos_send_ack_flg = 1;
                  uart2_receive_idx = 0;
                  uart2_header_ok_flag = 0;
                  memset((void*) uart2_receive_buffer, 0x00, sizeof (uart2_receive_buffer));
                  uart2_receive_timeout_en = 0;
                  uart2_receive_timeout_cnt = 0;
                  //ack
                }
              else
                {
                  //nack
                  pos_send_nack_flg = 1;
                  uart2_receive_idx = 0;
                  uart2_header_ok_flag = 0;
                  memset((void*) uart2_receive_buffer, 0x00, sizeof (uart2_receive_buffer));
                  syncstat = 0;
                  uart2_receive_timeout_en = 0;
                  uart2_receive_timeout_cnt = 0;
                }
            }
        }
      else
        {
          if (posidentifier == 2)
            {
              if ((uart2_receive_idx == 0) && (uart2_receive_data == 0x02))
                {
                  uart2_receive_buffer[uart2_receive_idx] = uart2_receive_data;
                  uart2_receive_idx++;
                  uart2_header_ok_flag = 1;
                  uart2_receive_timeout_en = 1;
                  uart2_receive_timeout_cnt = 0;
                }
            }
          else
            {
              if ((uart2_receive_idx == 0) && (uart2_receive_data == 0x82))
                {
                  uart2_receive_buffer[uart2_receive_idx] = uart2_receive_data + 0x80;
                  uart2_receive_idx++;
                  uart2_header_ok_flag = 1;
                  uart2_receive_timeout_en = 1;
                  uart2_receive_timeout_cnt = 0;
                }
            }
        }
    }
}

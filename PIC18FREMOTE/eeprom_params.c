#include "mcc_generated_files/mcc.h"
#include "remotecontroller.h"

unsigned char SUBPARAMS[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char SLAVEIDLIST[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char SLAVEIDUPDATESTATE[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char MIRRORSLAVEIDUPDATESTATE[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char SLAVESUBIDLIST[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //buraya id girilirse ayni indise repeater id girilmesi gerekir
unsigned char TryIdx[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char slaveid_cnt = 0;
unsigned char loop_cnt = 0;
unsigned char provider_id = 0;
unsigned char retrytime = 10;
unsigned char repeaterloopcnt = 1;
unsigned char posidentifier = 1;
unsigned char delayedpostime = 0;
unsigned char resend_request = 0;
unsigned char useposflag = 0;

ScreenCommand Screen_Vals[12];
ScreenCommand Config_Vals[13];

unsigned short ActPassVal = 0;
unsigned short ActPassVal1 = 0;
unsigned short ActPassVal2 = 0;
unsigned short ActPassVal3 = 0;
unsigned short ActPassVal4 = 0;

unsigned short ActPrice1Val = 0;
unsigned short ActPrice1Val1 = 0;
unsigned short ActPrice1Val2 = 0;
unsigned short ActPrice1Val3 = 0;
unsigned short ActPrice1Val4 = 0;

unsigned short ActPrice2Val = 0;
unsigned short ActPrice2Val1 = 0;
unsigned short ActPrice2Val2 = 0;
unsigned short ActPrice2Val3 = 0;
unsigned short ActPrice2Val4 = 0;

unsigned short ActParamVal = 0;
unsigned short ActParamVal1 = 0;
unsigned short ActParamVal2 = 0;
unsigned short ActParamVal3 = 0;
unsigned short ActParamVal4 = 0;

unsigned short ActIDVal = 0;
unsigned short ActIDVal1 = 0;
unsigned short ActIDVal2 = 0;
unsigned short ActIDVal3 = 0;
unsigned short ActIDVal4 = 0;

unsigned short ActSUBIDVal = 0;
unsigned short ActSUBIDVal1 = 0;
unsigned short ActSUBIDVal2 = 0;
unsigned short ActSUBIDVal3 = 0;
unsigned short ActSUBIDVal4 = 0;

unsigned char ButtonKeyPress = 0;

unsigned char val10 = '+';
unsigned char val11 = '0';
unsigned char val12 = '0';
unsigned char val13 = '0';
unsigned char val14 = '0';
unsigned char val15 = '0';

unsigned char valc10 = '0';
unsigned char valc11 = '0';
unsigned char valc12 = '0';

void slave_idlist_to_eeprom(void)
{
  Config_Vals[0].Val_Pos1 = (((unsigned int) SLAVEIDLIST[0] << 8) | (unsigned int) SLAVEIDLIST[1]);
  Config_Vals[1].Val_Pos1 = (((unsigned int) SLAVEIDLIST[2] << 8) | (unsigned int) SLAVEIDLIST[3]);
  Config_Vals[2].Val_Pos1 = (((unsigned int) SLAVEIDLIST[4] << 8) | (unsigned int) SLAVEIDLIST[5]);
  Config_Vals[3].Val_Pos1 = (((unsigned int) SLAVEIDLIST[6] << 8) | (unsigned int) SLAVEIDLIST[6]);
  Config_Vals[4].Val_Pos1 = (((unsigned int) SLAVEIDLIST[8] << 8) | (unsigned int) SLAVEIDLIST[8]);
  Config_Vals[5].Val_Pos1 = (((unsigned int) SLAVEIDLIST[10] << 8) | (unsigned int) SLAVEIDLIST[11]);
  Config_Vals[6].Val_Pos1 = (((unsigned int) SLAVEIDLIST[12] << 8) | (unsigned int) SLAVEIDLIST[13]);
  Config_Vals[7].Val_Pos1 = (((unsigned int) SLAVEIDLIST[14] << 8) | (unsigned int) SLAVEIDLIST[15]);
  Config_Vals[8].Val_Pos1 = (((unsigned int) SLAVEIDLIST[16] << 8) | (unsigned int) SLAVEIDLIST[17]);
  Config_Vals[9].Val_Pos1 = (((unsigned int) SLAVEIDLIST[18] << 8) | (unsigned int) SLAVEIDLIST[19]);
  Config_Vals[10].Val_Pos1 = (((unsigned int) SLAVEIDLIST[20] << 8) | (unsigned int) SLAVEIDLIST[21]);
  Config_Vals[11].Val_Pos1 = (((unsigned int) SLAVEIDLIST[22] << 8) | (unsigned int) SLAVEIDLIST[23]);
  Config_Vals[0].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[0] << 8) | (unsigned int) SLAVESUBIDLIST[1]);
  Config_Vals[1].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[2] << 8) | (unsigned int) SLAVESUBIDLIST[3]);
  Config_Vals[2].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[4] << 8) | (unsigned int) SLAVESUBIDLIST[5]);
  Config_Vals[3].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[6] << 8) | (unsigned int) SLAVESUBIDLIST[7]);
  Config_Vals[4].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[8] << 8) | (unsigned int) SLAVESUBIDLIST[9]);
  Config_Vals[5].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[10] << 8) | (unsigned int) SLAVESUBIDLIST[11]);
  Config_Vals[6].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[12] << 8) | (unsigned int) SLAVESUBIDLIST[13]);
  Config_Vals[7].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[14] << 8) | (unsigned int) SLAVESUBIDLIST[15]);
  Config_Vals[8].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[16] << 8) | (unsigned int) SLAVESUBIDLIST[17]);
  Config_Vals[9].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[18] << 8) | (unsigned int) SLAVESUBIDLIST[19]);
  Config_Vals[10].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[20] << 8) | (unsigned int) SLAVESUBIDLIST[21]);
  Config_Vals[11].Val_Pos2 = (((unsigned int) SLAVESUBIDLIST[22] << 8) | (unsigned int) SLAVESUBIDLIST[23]);
}

void eeprom_to_slave_idlist(void)
{
  SLAVEIDLIST[0] = (unsigned char) ((Config_Vals[0].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[1] = (unsigned char) (Config_Vals[0].Val_Pos1); //L
  SLAVEIDLIST[2] = (unsigned char) ((Config_Vals[1].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[3] = (unsigned char) (Config_Vals[1].Val_Pos1); //L
  SLAVEIDLIST[4] = (unsigned char) ((Config_Vals[2].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[5] = (unsigned char) (Config_Vals[2].Val_Pos1); //L
  SLAVEIDLIST[6] = (unsigned char) ((Config_Vals[3].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[7] = (unsigned char) (Config_Vals[3].Val_Pos1); //L
  SLAVEIDLIST[8] = (unsigned char) ((Config_Vals[4].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[9] = (unsigned char) (Config_Vals[4].Val_Pos1); //L
  SLAVEIDLIST[10] = (unsigned char) ((Config_Vals[5].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[11] = (unsigned char) (Config_Vals[5].Val_Pos1); //L
  SLAVEIDLIST[12] = (unsigned char) ((Config_Vals[6].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[13] = (unsigned char) (Config_Vals[6].Val_Pos1); //L
  SLAVEIDLIST[14] = (unsigned char) ((Config_Vals[7].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[15] = (unsigned char) (Config_Vals[7].Val_Pos1); //L
  SLAVEIDLIST[16] = (unsigned char) ((Config_Vals[8].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[17] = (unsigned char) (Config_Vals[8].Val_Pos1); //L
  SLAVEIDLIST[18] = (unsigned char) ((Config_Vals[9].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[19] = (unsigned char) (Config_Vals[9].Val_Pos1); //L
  SLAVEIDLIST[20] = (unsigned char) ((Config_Vals[10].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[21] = (unsigned char) (Config_Vals[10].Val_Pos1); //L
  SLAVEIDLIST[22] = (unsigned char) ((Config_Vals[11].Val_Pos1 >> 8) & 0x00FF); //H
  SLAVEIDLIST[23] = (unsigned char) (Config_Vals[11].Val_Pos1); //L
  SLAVESUBIDLIST[0] = (unsigned char) ((Config_Vals[0].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[1] = (unsigned char) Config_Vals[0].Val_Pos2; //L
  SLAVESUBIDLIST[2] = (unsigned char) ((Config_Vals[1].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[3] = (unsigned char) Config_Vals[1].Val_Pos2; //L
  SLAVESUBIDLIST[4] = (unsigned char) ((Config_Vals[2].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[5] = (unsigned char) Config_Vals[2].Val_Pos2; //L
  SLAVESUBIDLIST[6] = (unsigned char) ((Config_Vals[3].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[7] = (unsigned char) Config_Vals[3].Val_Pos2; //L
  SLAVESUBIDLIST[8] = (unsigned char) ((Config_Vals[4].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[9] = (unsigned char) Config_Vals[4].Val_Pos2; //L
  SLAVESUBIDLIST[10] = (unsigned char) ((Config_Vals[5].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[11] = (unsigned char) Config_Vals[5].Val_Pos2; //L
  SLAVESUBIDLIST[12] = (unsigned char) ((Config_Vals[6].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[13] = (unsigned char) Config_Vals[6].Val_Pos2; //L
  SLAVESUBIDLIST[14] = (unsigned char) ((Config_Vals[7].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[15] = (unsigned char) Config_Vals[7].Val_Pos2; //L
  SLAVESUBIDLIST[16] = (unsigned char) ((Config_Vals[8].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[17] = (unsigned char) Config_Vals[8].Val_Pos2; //L
  SLAVESUBIDLIST[18] = (unsigned char) ((Config_Vals[9].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[19] = (unsigned char) Config_Vals[9].Val_Pos2; //L
  SLAVESUBIDLIST[20] = (unsigned char) ((Config_Vals[10].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[21] = (unsigned char) Config_Vals[10].Val_Pos2; //L
  SLAVESUBIDLIST[22] = (unsigned char) ((Config_Vals[11].Val_Pos2 >> 8) & 0x00FF); //H
  SLAVESUBIDLIST[23] = (unsigned char) Config_Vals[11].Val_Pos2; //L
}

void read_price_eeprom(void)
{
  ClrWdt();
  Screen_Vals[0].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(11) << 8) | (unsigned int) DATAEE_ReadByte(12);
  Screen_Vals[0].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(13) << 8) | (unsigned int) DATAEE_ReadByte(14);
  Screen_Vals[1].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(15) << 8) | (unsigned int) DATAEE_ReadByte(16);
  Screen_Vals[1].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(17) << 8) | (unsigned int) DATAEE_ReadByte(18);
  Screen_Vals[2].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(19) << 8) | (unsigned int) DATAEE_ReadByte(20);
  Screen_Vals[2].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(21) << 8) | (unsigned int) DATAEE_ReadByte(22);
  Screen_Vals[3].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(23) << 8) | (unsigned int) DATAEE_ReadByte(24);
  Screen_Vals[3].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(25) << 8) | (unsigned int) DATAEE_ReadByte(26);
  Screen_Vals[4].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(27) << 8) | (unsigned int) DATAEE_ReadByte(28);
  Screen_Vals[4].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(29) << 8) | (unsigned int) DATAEE_ReadByte(30);
  Screen_Vals[5].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(31) << 8) | (unsigned int) DATAEE_ReadByte(32);
  Screen_Vals[5].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(33) << 8) | (unsigned int) DATAEE_ReadByte(34);
  Screen_Vals[6].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(35) << 8) | (unsigned int) DATAEE_ReadByte(36);
  Screen_Vals[6].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(37) << 8) | (unsigned int) DATAEE_ReadByte(38);
  Screen_Vals[7].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(39) << 8) | (unsigned int) DATAEE_ReadByte(40);
  Screen_Vals[7].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(41) << 8) | (unsigned int) DATAEE_ReadByte(42);
  Screen_Vals[8].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(43) << 8) | (unsigned int) DATAEE_ReadByte(44);
  Screen_Vals[8].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(45) << 8) | (unsigned int) DATAEE_ReadByte(46);
  Screen_Vals[9].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(47) << 8) | (unsigned int) DATAEE_ReadByte(48);
  Screen_Vals[9].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(49) << 8) | (unsigned int) DATAEE_ReadByte(50);
  Screen_Vals[10].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(51) << 8) | (unsigned int) DATAEE_ReadByte(52);
  Screen_Vals[10].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(53) << 8) | (unsigned int) DATAEE_ReadByte(54);
  Screen_Vals[11].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(55) << 8) | (unsigned int) DATAEE_ReadByte(56);
  Screen_Vals[11].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(57) << 8) | (unsigned int) DATAEE_ReadByte(58);
}

void write_price_eeprom(void)
{
  ClrWdt();
  DATAEE_WriteByte(11, (unsigned char) ((Screen_Vals[0].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(12, (unsigned char) (Screen_Vals[0].Val_Pos1));
  DATAEE_WriteByte(13, (unsigned char) ((Screen_Vals[0].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(14, (unsigned char) (Screen_Vals[0].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(15, (unsigned char) ((Screen_Vals[1].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(16, (unsigned char) (Screen_Vals[1].Val_Pos1));
  DATAEE_WriteByte(17, (unsigned char) ((Screen_Vals[1].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(18, (unsigned char) (Screen_Vals[1].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(19, (unsigned char) ((Screen_Vals[2].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(20, (unsigned char) (Screen_Vals[2].Val_Pos1));
  DATAEE_WriteByte(21, (unsigned char) ((Screen_Vals[2].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(22, (unsigned char) (Screen_Vals[2].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(23, (unsigned char) ((Screen_Vals[3].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(24, (unsigned char) (Screen_Vals[3].Val_Pos1));
  DATAEE_WriteByte(25, (unsigned char) ((Screen_Vals[3].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(26, (unsigned char) (Screen_Vals[3].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(27, (unsigned char) ((Screen_Vals[4].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(28, (unsigned char) (Screen_Vals[4].Val_Pos1));
  DATAEE_WriteByte(29, (unsigned char) ((Screen_Vals[4].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(30, (unsigned char) (Screen_Vals[4].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(31, (unsigned char) ((Screen_Vals[5].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(32, (unsigned char) (Screen_Vals[5].Val_Pos1));
  DATAEE_WriteByte(33, (unsigned char) ((Screen_Vals[5].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(34, (unsigned char) (Screen_Vals[5].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(35, (unsigned char) ((Screen_Vals[6].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(36, (unsigned char) (Screen_Vals[6].Val_Pos1));
  DATAEE_WriteByte(37, (unsigned char) ((Screen_Vals[6].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(38, (unsigned char) (Screen_Vals[6].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(39, (unsigned char) ((Screen_Vals[7].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(40, (unsigned char) (Screen_Vals[7].Val_Pos1));
  DATAEE_WriteByte(41, (unsigned char) ((Screen_Vals[7].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(42, (unsigned char) (Screen_Vals[7].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(43, (unsigned char) ((Screen_Vals[8].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(44, (unsigned char) (Screen_Vals[8].Val_Pos1));
  DATAEE_WriteByte(45, (unsigned char) ((Screen_Vals[8].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(46, (unsigned char) (Screen_Vals[8].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(47, (unsigned char) ((Screen_Vals[9].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(48, (unsigned char) (Screen_Vals[9].Val_Pos1));
  DATAEE_WriteByte(49, (unsigned char) ((Screen_Vals[9].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(50, (unsigned char) (Screen_Vals[9].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(51, (unsigned char) ((Screen_Vals[10].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(52, (unsigned char) (Screen_Vals[10].Val_Pos1));
  DATAEE_WriteByte(53, (unsigned char) ((Screen_Vals[10].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(54, (unsigned char) (Screen_Vals[10].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(55, (unsigned char) ((Screen_Vals[11].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(56, (unsigned char) (Screen_Vals[11].Val_Pos1));
  DATAEE_WriteByte(57, (unsigned char) ((Screen_Vals[11].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(58, (unsigned char) (Screen_Vals[11].Val_Pos2));
  ClrWdt();
}

void read_config_paremeter_eeprom(void)
{
  ClrWdt();
  Config_Vals[0].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(101) << 8) | (unsigned int) DATAEE_ReadByte(102);
  Config_Vals[0].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(103) << 8) | (unsigned int) DATAEE_ReadByte(104);
  Config_Vals[1].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(105) << 8) | (unsigned int) DATAEE_ReadByte(106);
  Config_Vals[1].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(107) << 8) | (unsigned int) DATAEE_ReadByte(108);
  Config_Vals[2].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(109) << 8) | (unsigned int) DATAEE_ReadByte(110);
  Config_Vals[2].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(111) << 8) | (unsigned int) DATAEE_ReadByte(112);
  Config_Vals[3].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(113) << 8) | (unsigned int) DATAEE_ReadByte(114);
  Config_Vals[3].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(115) << 8) | (unsigned int) DATAEE_ReadByte(116);
  Config_Vals[4].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(117) << 8) | (unsigned int) DATAEE_ReadByte(118);
  Config_Vals[4].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(119) << 8) | (unsigned int) DATAEE_ReadByte(120);
  Config_Vals[5].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(121) << 8) | (unsigned int) DATAEE_ReadByte(122);
  Config_Vals[5].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(123) << 8) | (unsigned int) DATAEE_ReadByte(124);
  Config_Vals[6].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(125) << 8) | (unsigned int) DATAEE_ReadByte(126);
  Config_Vals[6].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(127) << 8) | (unsigned int) DATAEE_ReadByte(128);
  Config_Vals[7].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(129) << 8) | (unsigned int) DATAEE_ReadByte(130);
  Config_Vals[7].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(131) << 8) | (unsigned int) DATAEE_ReadByte(132);
  Config_Vals[8].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(133) << 8) | (unsigned int) DATAEE_ReadByte(134);
  Config_Vals[8].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(135) << 8) | (unsigned int) DATAEE_ReadByte(136);
  Config_Vals[9].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(137) << 8) | (unsigned int) DATAEE_ReadByte(138);
  Config_Vals[9].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(139) << 8) | (unsigned int) DATAEE_ReadByte(140);
  Config_Vals[10].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(141) << 8) | (unsigned int) DATAEE_ReadByte(142);
  Config_Vals[10].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(143) << 8) | (unsigned int) DATAEE_ReadByte(144);
  Config_Vals[11].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(145) << 8) | (unsigned int) DATAEE_ReadByte(146);
  Config_Vals[11].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(147) << 8) | (unsigned int) DATAEE_ReadByte(148);
}

void write_slaveidcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos1 = (unsigned int) slaveid_cnt;
  DATAEE_WriteByte(149, (unsigned char) ((Config_Vals[12].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(150, (unsigned char) (Config_Vals[12].Val_Pos1));
  ClrWdt();
}

void write_loopcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos2 = (unsigned int) loop_cnt;
  DATAEE_WriteByte(151, (unsigned char) ((Config_Vals[12].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(152, (unsigned char) (Config_Vals[12].Val_Pos2));
  ClrWdt();
}

void read_slaveidcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos1 = (unsigned int) (DATAEE_ReadByte(149) << 8) | (unsigned int) DATAEE_ReadByte(150);
  slaveid_cnt = (unsigned char) Config_Vals[12].Val_Pos1;
}

void read_loopcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos2 = (unsigned int) (DATAEE_ReadByte(151) << 8) | (unsigned int) DATAEE_ReadByte(152);
  loop_cnt = (unsigned char) Config_Vals[12].Val_Pos2;
}

void read_providerid_paremeter_eeprom(void)
{
  provider_id = DATAEE_ReadByte(201);
}

void write_providerid_paremeter_eeprom(void)
{
  DATAEE_WriteByte(201, provider_id);
  ClrWdt();
}

void read_retrytime_paremeter_eeprom(void)
{
  retrytime = DATAEE_ReadByte(202);
}

void write_retrytime_paremeter_eeprom(void)
{
  DATAEE_WriteByte(202, retrytime);
  ClrWdt();
}

void read_repeaterloopcnt_paremeter_eeprom(void)
{
  repeaterloopcnt = DATAEE_ReadByte(203);
}

void write_repeaterloopcnt_paremeter_eeprom(void)
{
  DATAEE_WriteByte(203, repeaterloopcnt);
  ClrWdt();
}

void read_posidentifier_paremeter_eeprom(void)
{
  posidentifier = DATAEE_ReadByte(205);
}

void write_posidentifier_paremeter_eeprom(void)
{
  DATAEE_WriteByte(205, posidentifier);
  ClrWdt();
}

void read_delayedpostime_paremeter_eeprom(void)
{
  delayedpostime = DATAEE_ReadByte(204);
}

void write_delayedpostime_paremeter_eeprom(void)
{
  DATAEE_WriteByte(204, delayedpostime);
  ClrWdt();
}

void read_posswap_paremeter_eeprom(void)
{
  if (DATAEE_ReadByte(210) < 8)
    {
      PosCSwapTable[0] = DATAEE_ReadByte(210);
      PosCSwapTable[1] = PosCSwapTable[0] + 1;
    }
  if (DATAEE_ReadByte(211) < 8)
    {
      PosCSwapTable[2] = DATAEE_ReadByte(211);
      PosCSwapTable[3] = PosCSwapTable[2] + 1;
    }
  if (DATAEE_ReadByte(212) < 8)
    {
      PosCSwapTable[4] = DATAEE_ReadByte(212);
      PosCSwapTable[5] = PosCSwapTable[4] + 1;
    }
  if (DATAEE_ReadByte(213) < 8)
    {
      PosCSwapTable[6] = DATAEE_ReadByte(213);
      PosCSwapTable[7] = PosCSwapTable[6] + 1;
    }
  if (DATAEE_ReadByte(214) < 8)
    {
      PosCSwapTable[8] = DATAEE_ReadByte(214);
      PosCSwapTable[9] = PosCSwapTable[8] + 1;
    }
  if (DATAEE_ReadByte(215) < 8)
    {
      PosCSwapTable[10] = DATAEE_ReadByte(215);
      PosCSwapTable[11] = PosCSwapTable[10] + 1;
    }
  if (DATAEE_ReadByte(216) < 8)
    {
      PosCSwapTable[12] = DATAEE_ReadByte(216);
      PosCSwapTable[13] = PosCSwapTable[12] + 1;
    }
  if (DATAEE_ReadByte(217) < 8)
    {
      PosCSwapTable[14] = DATAEE_ReadByte(217);
      PosCSwapTable[15] = PosCSwapTable[14] + 1;
    }
}

void write_posswap_paremeter_eeprom(void)
{
  DATAEE_WriteByte(210, PosCSwapTable[0]);
  DATAEE_WriteByte(211, PosCSwapTable[2]);
  DATAEE_WriteByte(212, PosCSwapTable[4]);
  DATAEE_WriteByte(213, PosCSwapTable[6]);
  DATAEE_WriteByte(214, PosCSwapTable[8]);
  DATAEE_WriteByte(215, PosCSwapTable[10]);
  DATAEE_WriteByte(216, PosCSwapTable[12]);
  DATAEE_WriteByte(217, PosCSwapTable[14]);
  ClrWdt();
}

void read_productswap_paremeter_eeprom(void)
{
  if (DATAEE_ReadByte(218) < 12)
    {
      ProductCSwapTable[0] = DATAEE_ReadByte(218);
    }
  if (DATAEE_ReadByte(219) < 12)
    {
      ProductCSwapTable[1] = DATAEE_ReadByte(219);
    }
  if (DATAEE_ReadByte(220) < 12)
    {
      ProductCSwapTable[2] = DATAEE_ReadByte(220);
    }
  if (DATAEE_ReadByte(221) < 12)
    {
      ProductCSwapTable[3] = DATAEE_ReadByte(221);
    }
  if (DATAEE_ReadByte(222) < 12)
    {
      ProductCSwapTable[4] = DATAEE_ReadByte(222);
    }
  if (DATAEE_ReadByte(223) < 12)
    {
      ProductCSwapTable[5] = DATAEE_ReadByte(223);
    }
  if (DATAEE_ReadByte(224) < 12)
    {
      ProductCSwapTable[6] = DATAEE_ReadByte(224);
    }
  if (DATAEE_ReadByte(225) < 12)
    {
      ProductCSwapTable[7] = DATAEE_ReadByte(225);
    }
}

void write_productswap_paremeter_eeprom(void)
{
  DATAEE_WriteByte(218, ProductCSwapTable[0]);
  DATAEE_WriteByte(219, ProductCSwapTable[1]);
  DATAEE_WriteByte(220, ProductCSwapTable[2]);
  DATAEE_WriteByte(221, ProductCSwapTable[3]);
  DATAEE_WriteByte(222, ProductCSwapTable[4]);
  DATAEE_WriteByte(223, ProductCSwapTable[5]);
  DATAEE_WriteByte(224, ProductCSwapTable[6]);
  DATAEE_WriteByte(225, ProductCSwapTable[7]);
  ClrWdt();
}

void write_config_paremeter_eeprom(void)
{
  ClrWdt();
  DATAEE_WriteByte(101, (unsigned char) ((Config_Vals[0].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(102, (unsigned char) (Config_Vals[0].Val_Pos1));
  DATAEE_WriteByte(103, (unsigned char) ((Config_Vals[0].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(104, (unsigned char) (Config_Vals[0].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(105, (unsigned char) ((Config_Vals[1].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(106, (unsigned char) (Config_Vals[1].Val_Pos1));
  DATAEE_WriteByte(107, (unsigned char) ((Config_Vals[1].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(108, (unsigned char) (Config_Vals[1].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(109, (unsigned char) ((Config_Vals[2].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(110, (unsigned char) (Config_Vals[2].Val_Pos1));
  DATAEE_WriteByte(111, (unsigned char) ((Config_Vals[2].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(112, (unsigned char) (Config_Vals[2].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(113, (unsigned char) ((Config_Vals[3].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(114, (unsigned char) (Config_Vals[3].Val_Pos1));
  DATAEE_WriteByte(115, (unsigned char) ((Config_Vals[3].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(116, (unsigned char) (Config_Vals[3].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(117, (unsigned char) ((Config_Vals[4].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(118, (unsigned char) (Config_Vals[4].Val_Pos1));
  DATAEE_WriteByte(119, (unsigned char) ((Config_Vals[4].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(120, (unsigned char) (Config_Vals[4].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(121, (unsigned char) ((Config_Vals[5].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(122, (unsigned char) (Config_Vals[5].Val_Pos1));
  DATAEE_WriteByte(123, (unsigned char) ((Config_Vals[5].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(124, (unsigned char) (Config_Vals[5].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(125, (unsigned char) ((Config_Vals[6].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(126, (unsigned char) (Config_Vals[6].Val_Pos1));
  DATAEE_WriteByte(127, (unsigned char) ((Config_Vals[6].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(128, (unsigned char) (Config_Vals[6].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(129, (unsigned char) ((Config_Vals[7].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(130, (unsigned char) (Config_Vals[7].Val_Pos1));
  DATAEE_WriteByte(131, (unsigned char) ((Config_Vals[7].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(132, (unsigned char) (Config_Vals[7].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(133, (unsigned char) ((Config_Vals[8].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(134, (unsigned char) (Config_Vals[8].Val_Pos1));
  DATAEE_WriteByte(135, (unsigned char) ((Config_Vals[8].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(136, (unsigned char) (Config_Vals[8].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(137, (unsigned char) ((Config_Vals[9].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(138, (unsigned char) (Config_Vals[9].Val_Pos1));
  DATAEE_WriteByte(139, (unsigned char) ((Config_Vals[9].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(140, (unsigned char) (Config_Vals[9].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(141, (unsigned char) ((Config_Vals[10].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(142, (unsigned char) (Config_Vals[10].Val_Pos1));
  DATAEE_WriteByte(143, (unsigned char) ((Config_Vals[10].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(144, (unsigned char) (Config_Vals[10].Val_Pos2));
  ClrWdt();
  DATAEE_WriteByte(145, (unsigned char) ((Config_Vals[11].Val_Pos1 >> 8) & 0x00FF));
  DATAEE_WriteByte(146, (unsigned char) (Config_Vals[11].Val_Pos1));
  DATAEE_WriteByte(147, (unsigned char) ((Config_Vals[11].Val_Pos2 >> 8) & 0x00FF));
  DATAEE_WriteByte(148, (unsigned char) (Config_Vals[11].Val_Pos2));
  ClrWdt();
}

void WriteDecimalStringShort(short value)
{
  short temp1, temp2, temp3, temp4, temp5;
  if (value < 0)
    {
      val10 = '-';
      value = value*-1;
    }
  else
    {
      val10 = '+';
      value = value * 1;
    }
  temp1 = value / 10000; //32123
  val11 = (unsigned char) temp1 + 0x30; //3
  temp2 = temp1 * 10000; //30000
  temp2 = value - temp2; //2123
  temp3 = temp2 / 1000; //2
  val12 = (unsigned char) (temp3) + 0x30; //2
  temp4 = temp2 - (temp3 * 1000); //123
  temp5 = temp4 / 100; //1
  val13 = (unsigned char) temp5 + 0x30;
  temp2 = temp4 - (temp5 * 100); //23
  temp3 = temp2 / 10; //2
  val14 = (unsigned char) temp3 + 0x30;
  temp1 = temp2 - (temp3 * 10); //3
  val15 = (unsigned char) temp1 + 0x30;
}

void WriteDecimalStringUChar(unsigned char value)
{
  unsigned char temp1, temp2, temp3;
  temp1 = value / 100; //1
  valc12 = temp1 + 0x30;
  temp2 = value - (temp1 * 100); //23
  temp3 = temp2 / 10; //2
  valc11 = temp3 + 0x30;
  temp1 = temp2 - (temp3 * 10); //3
  valc10 = temp1 + 0x30;
}

void read_resendrequest_eeprom(void)
{
  resend_request = DATAEE_ReadByte(241);
}

void write_resendrequest_eeprom(unsigned char val)
{
  resend_request = val;
  DATAEE_WriteByte(241, resend_request);
  ClrWdt();
}

void read_useposflag_eeprom(void)
{
  useposflag = DATAEE_ReadByte(242);
}

void write_useposflag_eeprom(unsigned char val)
{
  useposflag = val;
  DATAEE_WriteByte(242, useposflag);
  ClrWdt();
}

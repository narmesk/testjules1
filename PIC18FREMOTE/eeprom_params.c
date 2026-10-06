#include "main.h"

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
  Screen_Vals[0].Val_Pos1 = (unsigned int) (EEPROM_Read(11) << 8) | (unsigned int) EEPROM_Read(12);
  Screen_Vals[0].Val_Pos2 = (unsigned int) (EEPROM_Read(13) << 8) | (unsigned int) EEPROM_Read(14);
  Screen_Vals[1].Val_Pos1 = (unsigned int) (EEPROM_Read(15) << 8) | (unsigned int) EEPROM_Read(16);
  Screen_Vals[1].Val_Pos2 = (unsigned int) (EEPROM_Read(17) << 8) | (unsigned int) EEPROM_Read(18);
  Screen_Vals[2].Val_Pos1 = (unsigned int) (EEPROM_Read(19) << 8) | (unsigned int) EEPROM_Read(20);
  Screen_Vals[2].Val_Pos2 = (unsigned int) (EEPROM_Read(21) << 8) | (unsigned int) EEPROM_Read(22);
  Screen_Vals[3].Val_Pos1 = (unsigned int) (EEPROM_Read(23) << 8) | (unsigned int) EEPROM_Read(24);
  Screen_Vals[3].Val_Pos2 = (unsigned int) (EEPROM_Read(25) << 8) | (unsigned int) EEPROM_Read(26);
  Screen_Vals[4].Val_Pos1 = (unsigned int) (EEPROM_Read(27) << 8) | (unsigned int) EEPROM_Read(28);
  Screen_Vals[4].Val_Pos2 = (unsigned int) (EEPROM_Read(29) << 8) | (unsigned int) EEPROM_Read(30);
  Screen_Vals[5].Val_Pos1 = (unsigned int) (EEPROM_Read(31) << 8) | (unsigned int) EEPROM_Read(32);
  Screen_Vals[5].Val_Pos2 = (unsigned int) (EEPROM_Read(33) << 8) | (unsigned int) EEPROM_Read(34);
  Screen_Vals[6].Val_Pos1 = (unsigned int) (EEPROM_Read(35) << 8) | (unsigned int) EEPROM_Read(36);
  Screen_Vals[6].Val_Pos2 = (unsigned int) (EEPROM_Read(37) << 8) | (unsigned int) EEPROM_Read(38);
  Screen_Vals[7].Val_Pos1 = (unsigned int) (EEPROM_Read(39) << 8) | (unsigned int) EEPROM_Read(40);
  Screen_Vals[7].Val_Pos2 = (unsigned int) (EEPROM_Read(41) << 8) | (unsigned int) EEPROM_Read(42);
  Screen_Vals[8].Val_Pos1 = (unsigned int) (EEPROM_Read(43) << 8) | (unsigned int) EEPROM_Read(44);
  Screen_Vals[8].Val_Pos2 = (unsigned int) (EEPROM_Read(45) << 8) | (unsigned int) EEPROM_Read(46);
  Screen_Vals[9].Val_Pos1 = (unsigned int) (EEPROM_Read(47) << 8) | (unsigned int) EEPROM_Read(48);
  Screen_Vals[9].Val_Pos2 = (unsigned int) (EEPROM_Read(49) << 8) | (unsigned int) EEPROM_Read(50);
  Screen_Vals[10].Val_Pos1 = (unsigned int) (EEPROM_Read(51) << 8) | (unsigned int) EEPROM_Read(52);
  Screen_Vals[10].Val_Pos2 = (unsigned int) (EEPROM_Read(53) << 8) | (unsigned int) EEPROM_Read(54);
  Screen_Vals[11].Val_Pos1 = (unsigned int) (EEPROM_Read(55) << 8) | (unsigned int) EEPROM_Read(56);
  Screen_Vals[11].Val_Pos2 = (unsigned int) (EEPROM_Read(57) << 8) | (unsigned int) EEPROM_Read(58);
}

void write_price_eeprom(void)
{
  ClrWdt();
  EEPROM_Write(11, (unsigned char) ((Screen_Vals[0].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(12, (unsigned char) (Screen_Vals[0].Val_Pos1));
  EEPROM_Write(13, (unsigned char) ((Screen_Vals[0].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(14, (unsigned char) (Screen_Vals[0].Val_Pos2));
  ClrWdt();
  EEPROM_Write(15, (unsigned char) ((Screen_Vals[1].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(16, (unsigned char) (Screen_Vals[1].Val_Pos1));
  EEPROM_Write(17, (unsigned char) ((Screen_Vals[1].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(18, (unsigned char) (Screen_Vals[1].Val_Pos2));
  ClrWdt();
  EEPROM_Write(19, (unsigned char) ((Screen_Vals[2].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(20, (unsigned char) (Screen_Vals[2].Val_Pos1));
  EEPROM_Write(21, (unsigned char) ((Screen_Vals[2].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(22, (unsigned char) (Screen_Vals[2].Val_Pos2));
  ClrWdt();
  EEPROM_Write(23, (unsigned char) ((Screen_Vals[3].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(24, (unsigned char) (Screen_Vals[3].Val_Pos1));
  EEPROM_Write(25, (unsigned char) ((Screen_Vals[3].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(26, (unsigned char) (Screen_Vals[3].Val_Pos2));
  ClrWdt();
  EEPROM_Write(27, (unsigned char) ((Screen_Vals[4].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(28, (unsigned char) (Screen_Vals[4].Val_Pos1));
  EEPROM_Write(29, (unsigned char) ((Screen_Vals[4].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(30, (unsigned char) (Screen_Vals[4].Val_Pos2));
  ClrWdt();
  EEPROM_Write(31, (unsigned char) ((Screen_Vals[5].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(32, (unsigned char) (Screen_Vals[5].Val_Pos1));
  EEPROM_Write(33, (unsigned char) ((Screen_Vals[5].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(34, (unsigned char) (Screen_Vals[5].Val_Pos2));
  ClrWdt();
  EEPROM_Write(35, (unsigned char) ((Screen_Vals[6].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(36, (unsigned char) (Screen_Vals[6].Val_Pos1));
  EEPROM_Write(37, (unsigned char) ((Screen_Vals[6].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(38, (unsigned char) (Screen_Vals[6].Val_Pos2));
  ClrWdt();
  EEPROM_Write(39, (unsigned char) ((Screen_Vals[7].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(40, (unsigned char) (Screen_Vals[7].Val_Pos1));
  EEPROM_Write(41, (unsigned char) ((Screen_Vals[7].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(42, (unsigned char) (Screen_Vals[7].Val_Pos2));
  ClrWdt();
  EEPROM_Write(43, (unsigned char) ((Screen_Vals[8].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(44, (unsigned char) (Screen_Vals[8].Val_Pos1));
  EEPROM_Write(45, (unsigned char) ((Screen_Vals[8].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(46, (unsigned char) (Screen_Vals[8].Val_Pos2));
  ClrWdt();
  EEPROM_Write(47, (unsigned char) ((Screen_Vals[9].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(48, (unsigned char) (Screen_Vals[9].Val_Pos1));
  EEPROM_Write(49, (unsigned char) ((Screen_Vals[9].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(50, (unsigned char) (Screen_Vals[9].Val_Pos2));
  ClrWdt();
  EEPROM_Write(51, (unsigned char) ((Screen_Vals[10].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(52, (unsigned char) (Screen_Vals[10].Val_Pos1));
  EEPROM_Write(53, (unsigned char) ((Screen_Vals[10].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(54, (unsigned char) (Screen_Vals[10].Val_Pos2));
  ClrWdt();
  EEPROM_Write(55, (unsigned char) ((Screen_Vals[11].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(56, (unsigned char) (Screen_Vals[11].Val_Pos1));
  EEPROM_Write(57, (unsigned char) ((Screen_Vals[11].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(58, (unsigned char) (Screen_Vals[11].Val_Pos2));
  ClrWdt();
}

void read_config_paremeter_eeprom(void)
{
  ClrWdt();
  Config_Vals[0].Val_Pos1 = (unsigned int) (EEPROM_Read(101) << 8) | (unsigned int) EEPROM_Read(102);
  Config_Vals[0].Val_Pos2 = (unsigned int) (EEPROM_Read(103) << 8) | (unsigned int) EEPROM_Read(104);
  Config_Vals[1].Val_Pos1 = (unsigned int) (EEPROM_Read(105) << 8) | (unsigned int) EEPROM_Read(106);
  Config_Vals[1].Val_Pos2 = (unsigned int) (EEPROM_Read(107) << 8) | (unsigned int) EEPROM_Read(108);
  Config_Vals[2].Val_Pos1 = (unsigned int) (EEPROM_Read(109) << 8) | (unsigned int) EEPROM_Read(110);
  Config_Vals[2].Val_Pos2 = (unsigned int) (EEPROM_Read(111) << 8) | (unsigned int) EEPROM_Read(112);
  Config_Vals[3].Val_Pos1 = (unsigned int) (EEPROM_Read(113) << 8) | (unsigned int) EEPROM_Read(114);
  Config_Vals[3].Val_Pos2 = (unsigned int) (EEPROM_Read(115) << 8) | (unsigned int) EEPROM_Read(116);
  Config_Vals[4].Val_Pos1 = (unsigned int) (EEPROM_Read(117) << 8) | (unsigned int) EEPROM_Read(118);
  Config_Vals[4].Val_Pos2 = (unsigned int) (EEPROM_Read(119) << 8) | (unsigned int) EEPROM_Read(120);
  Config_Vals[5].Val_Pos1 = (unsigned int) (EEPROM_Read(121) << 8) | (unsigned int) EEPROM_Read(122);
  Config_Vals[5].Val_Pos2 = (unsigned int) (EEPROM_Read(123) << 8) | (unsigned int) EEPROM_Read(124);
  Config_Vals[6].Val_Pos1 = (unsigned int) (EEPROM_Read(125) << 8) | (unsigned int) EEPROM_Read(126);
  Config_Vals[6].Val_Pos2 = (unsigned int) (EEPROM_Read(127) << 8) | (unsigned int) EEPROM_Read(128);
  Config_Vals[7].Val_Pos1 = (unsigned int) (EEPROM_Read(129) << 8) | (unsigned int) EEPROM_Read(130);
  Config_Vals[7].Val_Pos2 = (unsigned int) (EEPROM_Read(131) << 8) | (unsigned int) EEPROM_Read(132);
  Config_Vals[8].Val_Pos1 = (unsigned int) (EEPROM_Read(133) << 8) | (unsigned int) EEPROM_Read(134);
  Config_Vals[8].Val_Pos2 = (unsigned int) (EEPROM_Read(135) << 8) | (unsigned int) EEPROM_Read(136);
  Config_Vals[9].Val_Pos1 = (unsigned int) (EEPROM_Read(137) << 8) | (unsigned int) EEPROM_Read(138);
  Config_Vals[9].Val_Pos2 = (unsigned int) (EEPROM_Read(139) << 8) | (unsigned int) EEPROM_Read(140);
  Config_Vals[10].Val_Pos1 = (unsigned int) (EEPROM_Read(141) << 8) | (unsigned int) EEPROM_Read(142);
  Config_Vals[10].Val_Pos2 = (unsigned int) (EEPROM_Read(143) << 8) | (unsigned int) EEPROM_Read(144);
  Config_Vals[11].Val_Pos1 = (unsigned int) (EEPROM_Read(145) << 8) | (unsigned int) EEPROM_Read(146);
  Config_Vals[11].Val_Pos2 = (unsigned int) (EEPROM_Read(147) << 8) | (unsigned int) EEPROM_Read(148);
}

void write_slaveidcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos1 = (unsigned int) slaveid_cnt;
  EEPROM_Write(149, (unsigned char) ((Config_Vals[12].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(150, (unsigned char) (Config_Vals[12].Val_Pos1));
  ClrWdt();
}

void write_loopcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos2 = (unsigned int) loop_cnt;
  EEPROM_Write(151, (unsigned char) ((Config_Vals[12].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(152, (unsigned char) (Config_Vals[12].Val_Pos2));
  ClrWdt();
}

void read_slaveidcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos1 = (unsigned int) (EEPROM_Read(149) << 8) | (unsigned int) EEPROM_Read(150);
  slaveid_cnt = (unsigned char) Config_Vals[12].Val_Pos1;
}

void read_loopcnt_paremeter_eeprom(void)
{
  Config_Vals[12].Val_Pos2 = (unsigned int) (EEPROM_Read(151) << 8) | (unsigned int) EEPROM_Read(152);
  loop_cnt = (unsigned char) Config_Vals[12].Val_Pos2;
}

void read_providerid_paremeter_eeprom(void)
{
  provider_id = EEPROM_Read(201);
}

void write_providerid_paremeter_eeprom(void)
{
  EEPROM_Write(201, provider_id);
  ClrWdt();
}

void read_retrytime_paremeter_eeprom(void)
{
  retrytime = EEPROM_Read(202);
}

void write_retrytime_paremeter_eeprom(void)
{
  EEPROM_Write(202, retrytime);
  ClrWdt();
}

void read_repeaterloopcnt_paremeter_eeprom(void)
{
  repeaterloopcnt = EEPROM_Read(203);
}

void write_repeaterloopcnt_paremeter_eeprom(void)
{
  EEPROM_Write(203, repeaterloopcnt);
  ClrWdt();
}

void read_posidentifier_paremeter_eeprom(void)
{
  posidentifier = EEPROM_Read(205);
}

void write_posidentifier_paremeter_eeprom(void)
{
  EEPROM_Write(205, posidentifier);
  ClrWdt();
}

void read_delayedpostime_paremeter_eeprom(void)
{
  delayedpostime = EEPROM_Read(204);
}

void write_delayedpostime_paremeter_eeprom(void)
{
  EEPROM_Write(204, delayedpostime);
  ClrWdt();
}

void read_posswap_paremeter_eeprom(void)
{
  if (EEPROM_Read(210) < 8)
    {
      PosCSwapTable[0] = EEPROM_Read(210);
      PosCSwapTable[1] = PosCSwapTable[0] + 1;
    }
  if (EEPROM_Read(211) < 8)
    {
      PosCSwapTable[2] = EEPROM_Read(211);
      PosCSwapTable[3] = PosCSwapTable[2] + 1;
    }
  if (EEPROM_Read(212) < 8)
    {
      PosCSwapTable[4] = EEPROM_Read(212);
      PosCSwapTable[5] = PosCSwapTable[4] + 1;
    }
  if (EEPROM_Read(213) < 8)
    {
      PosCSwapTable[6] = EEPROM_Read(213);
      PosCSwapTable[7] = PosCSwapTable[6] + 1;
    }
  if (EEPROM_Read(214) < 8)
    {
      PosCSwapTable[8] = EEPROM_Read(214);
      PosCSwapTable[9] = PosCSwapTable[8] + 1;
    }
  if (EEPROM_Read(215) < 8)
    {
      PosCSwapTable[10] = EEPROM_Read(215);
      PosCSwapTable[11] = PosCSwapTable[10] + 1;
    }
  if (EEPROM_Read(216) < 8)
    {
      PosCSwapTable[12] = EEPROM_Read(216);
      PosCSwapTable[13] = PosCSwapTable[12] + 1;
    }
  if (EEPROM_Read(217) < 8)
    {
      PosCSwapTable[14] = EEPROM_Read(217);
      PosCSwapTable[15] = PosCSwapTable[14] + 1;
    }
}

void write_posswap_paremeter_eeprom(void)
{
  EEPROM_Write(210, PosCSwapTable[0]);
  EEPROM_Write(211, PosCSwapTable[2]);
  EEPROM_Write(212, PosCSwapTable[4]);
  EEPROM_Write(213, PosCSwapTable[6]);
  EEPROM_Write(214, PosCSwapTable[8]);
  EEPROM_Write(215, PosCSwapTable[10]);
  EEPROM_Write(216, PosCSwapTable[12]);
  EEPROM_Write(217, PosCSwapTable[14]);
  ClrWdt();
}

void read_productswap_paremeter_eeprom(void)
{
  if (EEPROM_Read(218) < 12)
    {
      ProductCSwapTable[0] = EEPROM_Read(218);
    }
  if (EEPROM_Read(219) < 12)
    {
      ProductCSwapTable[1] = EEPROM_Read(219);
    }
  if (EEPROM_Read(220) < 12)
    {
      ProductCSwapTable[2] = EEPROM_Read(220);
    }
  if (EEPROM_Read(221) < 12)
    {
      ProductCSwapTable[3] = EEPROM_Read(221);
    }
  if (EEPROM_Read(222) < 12)
    {
      ProductCSwapTable[4] = EEPROM_Read(222);
    }
  if (EEPROM_Read(223) < 12)
    {
      ProductCSwapTable[5] = EEPROM_Read(223);
    }
  if (EEPROM_Read(224) < 12)
    {
      ProductCSwapTable[6] = EEPROM_Read(224);
    }
  if (EEPROM_Read(225) < 12)
    {
      ProductCSwapTable[7] = EEPROM_Read(225);
    }
}

void write_productswap_paremeter_eeprom(void)
{
  EEPROM_Write(218, ProductCSwapTable[0]);
  EEPROM_Write(219, ProductCSwapTable[1]);
  EEPROM_Write(220, ProductCSwapTable[2]);
  EEPROM_Write(221, ProductCSwapTable[3]);
  EEPROM_Write(222, ProductCSwapTable[4]);
  EEPROM_Write(223, ProductCSwapTable[5]);
  EEPROM_Write(224, ProductCSwapTable[6]);
  EEPROM_Write(225, ProductCSwapTable[7]);
  ClrWdt();
}

void write_config_paremeter_eeprom(void)
{
  ClrWdt();
  EEPROM_Write(101, (unsigned char) ((Config_Vals[0].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(102, (unsigned char) (Config_Vals[0].Val_Pos1));
  EEPROM_Write(103, (unsigned char) ((Config_Vals[0].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(104, (unsigned char) (Config_Vals[0].Val_Pos2));
  ClrWdt();
  EEPROM_Write(105, (unsigned char) ((Config_Vals[1].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(106, (unsigned char) (Config_Vals[1].Val_Pos1));
  EEPROM_Write(107, (unsigned char) ((Config_Vals[1].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(108, (unsigned char) (Config_Vals[1].Val_Pos2));
  ClrWdt();
  EEPROM_Write(109, (unsigned char) ((Config_Vals[2].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(110, (unsigned char) (Config_Vals[2].Val_Pos1));
  EEPROM_Write(111, (unsigned char) ((Config_Vals[2].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(112, (unsigned char) (Config_Vals[2].Val_Pos2));
  ClrWdt();
  EEPROM_Write(113, (unsigned char) ((Config_Vals[3].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(114, (unsigned char) (Config_Vals[3].Val_Pos1));
  EEPROM_Write(115, (unsigned char) ((Config_Vals[3].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(116, (unsigned char) (Config_Vals[3].Val_Pos2));
  ClrWdt();
  EEPROM_Write(117, (unsigned char) ((Config_Vals[4].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(118, (unsigned char) (Config_Vals[4].Val_Pos1));
  EEPROM_Write(119, (unsigned char) ((Config_Vals[4].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(120, (unsigned char) (Config_Vals[4].Val_Pos2));
  ClrWdt();
  EEPROM_Write(121, (unsigned char) ((Config_Vals[5].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(122, (unsigned char) (Config_Vals[5].Val_Pos1));
  EEPROM_Write(123, (unsigned char) ((Config_Vals[5].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(124, (unsigned char) (Config_Vals[5].Val_Pos2));
  ClrWdt();
  EEPROM_Write(125, (unsigned char) ((Config_Vals[6].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(126, (unsigned char) (Config_Vals[6].Val_Pos1));
  EEPROM_Write(127, (unsigned char) ((Config_Vals[6].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(128, (unsigned char) (Config_Vals[6].Val_Pos2));
  ClrWdt();
  EEPROM_Write(129, (unsigned char) ((Config_Vals[7].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(130, (unsigned char) (Config_Vals[7].Val_Pos1));
  EEPROM_Write(131, (unsigned char) ((Config_Vals[7].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(132, (unsigned char) (Config_Vals[7].Val_Pos2));
  ClrWdt();
  EEPROM_Write(133, (unsigned char) ((Config_Vals[8].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(134, (unsigned char) (Config_Vals[8].Val_Pos1));
  EEPROM_Write(135, (unsigned char) ((Config_Vals[8].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(136, (unsigned char) (Config_Vals[8].Val_Pos2));
  ClrWdt();
  EEPROM_Write(137, (unsigned char) ((Config_Vals[9].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(138, (unsigned char) (Config_Vals[9].Val_Pos1));
  EEPROM_Write(139, (unsigned char) ((Config_Vals[9].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(140, (unsigned char) (Config_Vals[9].Val_Pos2));
  ClrWdt();
  EEPROM_Write(141, (unsigned char) ((Config_Vals[10].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(142, (unsigned char) (Config_Vals[10].Val_Pos1));
  EEPROM_Write(143, (unsigned char) ((Config_Vals[10].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(144, (unsigned char) (Config_Vals[10].Val_Pos2));
  ClrWdt();
  EEPROM_Write(145, (unsigned char) ((Config_Vals[11].Val_Pos1 >> 8) & 0x00FF));
  EEPROM_Write(146, (unsigned char) (Config_Vals[11].Val_Pos1));
  EEPROM_Write(147, (unsigned char) ((Config_Vals[11].Val_Pos2 >> 8) & 0x00FF));
  EEPROM_Write(148, (unsigned char) (Config_Vals[11].Val_Pos2));
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
  resend_request = EEPROM_Read(241);
}

void write_resendrequest_eeprom(unsigned char val)
{
  resend_request = val;
  EEPROM_Write(241, resend_request);
  ClrWdt();
}

void read_useposflag_eeprom(void)
{
  useposflag = EEPROM_Read(242);
}

void write_useposflag_eeprom(unsigned char val)
{
  useposflag = val;
  EEPROM_Write(242, useposflag);
  ClrWdt();
}

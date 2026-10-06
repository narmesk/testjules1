/*
 * File:   main.h
 * Author: armag
 *
 * Created on September 21, 2026, 10:15 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "mcc_generated_files/system/system.h"

#include "Font_Header.h"
#include "Keyboard_Header.h"
#include "Arial_bold_14.h"
#include "Calibri36.h"
#include "lcdnums.h"
#include "remotecontroller.h"
#include "glcd.h"
#include <string.h>

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



#define bitset(var, bit)  ((var) |= 1uL << (bit))
#define bitclr(var, bit)  ((var) &= ~(1uL << (bit)))
#define bitflip(var, bit) ((var) ^= 1uL << (bit))
#define bittest(var, bit) ((var) & 1uL << (bit))

#endif	/* MAIN_H */

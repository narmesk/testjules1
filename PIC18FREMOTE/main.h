#ifndef MAIN_H
#define MAIN_H

#include "remotecontroller.h"
#include "glcd.h"

// -----------------------------------------------------------------------------
// Donanım Pin Tanımları ve Makroları (Hardware Pin Aliases)
// -----------------------------------------------------------------------------
#ifndef BUZZ_LAT
#define BUZZ_LAT LATBbits.LATB0
#endif

#ifndef BCKLG_Toggle
#define BCKLG_Toggle() LATBbits.LATB1 ^= 1
#endif

// Central Header Includes for Project
extern unsigned char menu_state;
extern unsigned char main_state;

#endif // MAIN_H

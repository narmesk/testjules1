/**
 * Generated Pins header File
 *
 * @file pins.h
 *
 * @defgroup  pinsdriver Pins Driver
 *
 * @brief This is generated driver header for pins.
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.1
*/

/*
\xa9 [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 aliases
#define C0_TRIS                 TRISAbits.TRISA0
#define C0_LAT                  LATAbits.LATA0
#define C0_PORT                 PORTAbits.RA0
#define C0_WPU                  WPUAbits.WPUA0
#define C0_OD                   ODCONAbits.ODCA0
#define C0_ANS                  ANSELAbits.ANSELA0
#define C0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define C0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define C0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define C0_GetValue()           PORTAbits.RA0
#define C0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define C0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define C0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define C0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define C0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define C0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define C0_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define C0_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RA1 aliases
#define C1_TRIS                 TRISAbits.TRISA1
#define C1_LAT                  LATAbits.LATA1
#define C1_PORT                 PORTAbits.RA1
#define C1_WPU                  WPUAbits.WPUA1
#define C1_OD                   ODCONAbits.ODCA1
#define C1_ANS                  ANSELAbits.ANSELA1
#define C1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define C1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define C1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define C1_GetValue()           PORTAbits.RA1
#define C1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define C1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define C1_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define C1_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define C1_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define C1_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define C1_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define C1_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set RA2 aliases
#define C2_TRIS                 TRISAbits.TRISA2
#define C2_LAT                  LATAbits.LATA2
#define C2_PORT                 PORTAbits.RA2
#define C2_WPU                  WPUAbits.WPUA2
#define C2_OD                   ODCONAbits.ODCA2
#define C2_ANS                  ANSELAbits.ANSELA2
#define C2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define C2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define C2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define C2_GetValue()           PORTAbits.RA2
#define C2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define C2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define C2_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define C2_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define C2_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define C2_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define C2_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define C2_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 aliases
#define C3_TRIS                 TRISAbits.TRISA3
#define C3_LAT                  LATAbits.LATA3
#define C3_PORT                 PORTAbits.RA3
#define C3_WPU                  WPUAbits.WPUA3
#define C3_OD                   ODCONAbits.ODCA3
#define C3_ANS                  ANSELAbits.ANSELA3
#define C3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define C3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define C3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define C3_GetValue()           PORTAbits.RA3
#define C3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define C3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define C3_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define C3_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define C3_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define C3_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define C3_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define C3_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set RA4 aliases
#define C4_TRIS                 TRISAbits.TRISA4
#define C4_LAT                  LATAbits.LATA4
#define C4_PORT                 PORTAbits.RA4
#define C4_WPU                  WPUAbits.WPUA4
#define C4_OD                   ODCONAbits.ODCA4
#define C4_ANS                  ANSELAbits.ANSELA4
#define C4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define C4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define C4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define C4_GetValue()           PORTAbits.RA4
#define C4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define C4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define C4_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define C4_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define C4_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define C4_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define C4_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define C4_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set RA5 aliases
#define BUFE2_TRIS                 TRISAbits.TRISA5
#define BUFE2_LAT                  LATAbits.LATA5
#define BUFE2_PORT                 PORTAbits.RA5
#define BUFE2_WPU                  WPUAbits.WPUA5
#define BUFE2_OD                   ODCONAbits.ODCA5
#define BUFE2_ANS                  ANSELAbits.ANSELA5
#define BUFE2_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define BUFE2_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define BUFE2_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define BUFE2_GetValue()           PORTAbits.RA5
#define BUFE2_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define BUFE2_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define BUFE2_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define BUFE2_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define BUFE2_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define BUFE2_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define BUFE2_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define BUFE2_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set RB0 aliases
#define BUZZ_TRIS                 TRISBbits.TRISB0
#define BUZZ_LAT                  LATBbits.LATB0
#define BUZZ_PORT                 PORTBbits.RB0
#define BUZZ_WPU                  WPUBbits.WPUB0
#define BUZZ_OD                   ODCONBbits.ODCB0
#define BUZZ_ANS                  ANSELBbits.ANSELB0
#define BUZZ_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define BUZZ_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define BUZZ_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define BUZZ_GetValue()           PORTBbits.RB0
#define BUZZ_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define BUZZ_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define BUZZ_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define BUZZ_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define BUZZ_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define BUZZ_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define BUZZ_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define BUZZ_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set RB1 aliases
#define BCKLG_TRIS                 TRISBbits.TRISB1
#define BCKLG_LAT                  LATBbits.LATB1
#define BCKLG_PORT                 PORTBbits.RB1
#define BCKLG_WPU                  WPUBbits.WPUB1
#define BCKLG_OD                   ODCONBbits.ODCB1
#define BCKLG_ANS                  ANSELBbits.ANSELB1
#define BCKLG_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define BCKLG_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define BCKLG_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define BCKLG_GetValue()           PORTBbits.RB1
#define BCKLG_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define BCKLG_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define BCKLG_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define BCKLG_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define BCKLG_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define BCKLG_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define BCKLG_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define BCKLG_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RB2 aliases
#define IO_RB2_TRIS                 TRISBbits.TRISB2
#define IO_RB2_LAT                  LATBbits.LATB2
#define IO_RB2_PORT                 PORTBbits.RB2
#define IO_RB2_WPU                  WPUBbits.WPUB2
#define IO_RB2_OD                   ODCONBbits.ODCB2
#define IO_RB2_ANS                  ANSELBbits.ANSELB2
#define IO_RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define IO_RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define IO_RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define IO_RB2_GetValue()           PORTBbits.RB2
#define IO_RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define IO_RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define IO_RB2_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define IO_RB2_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define IO_RB2_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define IO_RB2_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define IO_RB2_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define IO_RB2_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RB3 aliases
#define IO_RB3_TRIS                 TRISBbits.TRISB3
#define IO_RB3_LAT                  LATBbits.LATB3
#define IO_RB3_PORT                 PORTBbits.RB3
#define IO_RB3_WPU                  WPUBbits.WPUB3
#define IO_RB3_OD                   ODCONBbits.ODCB3
#define IO_RB3_ANS                  ANSELBbits.ANSELB3
#define IO_RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define IO_RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define IO_RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define IO_RB3_GetValue()           PORTBbits.RB3
#define IO_RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define IO_RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define IO_RB3_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define IO_RB3_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define IO_RB3_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define IO_RB3_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define IO_RB3_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define IO_RB3_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RB4 aliases
#define BUFDIR_TRIS                 TRISBbits.TRISB4
#define BUFDIR_LAT                  LATBbits.LATB4
#define BUFDIR_PORT                 PORTBbits.RB4
#define BUFDIR_WPU                  WPUBbits.WPUB4
#define BUFDIR_OD                   ODCONBbits.ODCB4
#define BUFDIR_ANS                  ANSELBbits.ANSELB4
#define BUFDIR_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define BUFDIR_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define BUFDIR_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define BUFDIR_GetValue()           PORTBbits.RB4
#define BUFDIR_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define BUFDIR_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define BUFDIR_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define BUFDIR_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define BUFDIR_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define BUFDIR_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define BUFDIR_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define BUFDIR_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set RB5 aliases
#define BUFEN_TRIS                 TRISBbits.TRISB5
#define BUFEN_LAT                  LATBbits.LATB5
#define BUFEN_PORT                 PORTBbits.RB5
#define BUFEN_WPU                  WPUBbits.WPUB5
#define BUFEN_OD                   ODCONBbits.ODCB5
#define BUFEN_ANS                  ANSELBbits.ANSELB5
#define BUFEN_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define BUFEN_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define BUFEN_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define BUFEN_GetValue()           PORTBbits.RB5
#define BUFEN_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define BUFEN_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define BUFEN_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define BUFEN_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define BUFEN_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define BUFEN_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define BUFEN_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define BUFEN_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RB6 aliases
#define R43PGC_TRIS                 TRISBbits.TRISB6
#define R43PGC_LAT                  LATBbits.LATB6
#define R43PGC_PORT                 PORTBbits.RB6
#define R43PGC_WPU                  WPUBbits.WPUB6
#define R43PGC_OD                   ODCONBbits.ODCB6
#define R43PGC_ANS                  ANSELBbits.ANSELB6
#define R43PGC_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define R43PGC_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define R43PGC_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define R43PGC_GetValue()           PORTBbits.RB6
#define R43PGC_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define R43PGC_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define R43PGC_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define R43PGC_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define R43PGC_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define R43PGC_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define R43PGC_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define R43PGC_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set RB7 aliases
#define R40PGD_TRIS                 TRISBbits.TRISB7
#define R40PGD_LAT                  LATBbits.LATB7
#define R40PGD_PORT                 PORTBbits.RB7
#define R40PGD_WPU                  WPUBbits.WPUB7
#define R40PGD_OD                   ODCONBbits.ODCB7
#define R40PGD_ANS                  ANSELBbits.ANSELB7
#define R40PGD_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define R40PGD_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define R40PGD_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define R40PGD_GetValue()           PORTBbits.RB7
#define R40PGD_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define R40PGD_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define R40PGD_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define R40PGD_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define R40PGD_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define R40PGD_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define R40PGD_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define R40PGD_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set RC0 aliases
#define EN_TRIS                 TRISCbits.TRISC0
#define EN_LAT                  LATCbits.LATC0
#define EN_PORT                 PORTCbits.RC0
#define EN_WPU                  WPUCbits.WPUC0
#define EN_OD                   ODCONCbits.ODCC0
#define EN_ANS                  ANSELCbits.ANSELC0
#define EN_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define EN_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define EN_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define EN_GetValue()           PORTCbits.RC0
#define EN_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define EN_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define EN_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define EN_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define EN_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define EN_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define EN_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define EN_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set RC1 aliases
#define RW_TRIS                 TRISCbits.TRISC1
#define RW_LAT                  LATCbits.LATC1
#define RW_PORT                 PORTCbits.RC1
#define RW_WPU                  WPUCbits.WPUC1
#define RW_OD                   ODCONCbits.ODCC1
#define RW_ANS                  ANSELCbits.ANSELC1
#define RW_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RW_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RW_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RW_GetValue()           PORTCbits.RC1
#define RW_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RW_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RW_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define RW_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define RW_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define RW_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define RW_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define RW_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set RC2 aliases
#define RS_TRIS                 TRISCbits.TRISC2
#define RS_LAT                  LATCbits.LATC2
#define RS_PORT                 PORTCbits.RC2
#define RS_WPU                  WPUCbits.WPUC2
#define RS_OD                   ODCONCbits.ODCC2
#define RS_ANS                  ANSELCbits.ANSELC2
#define RS_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RS_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RS_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RS_GetValue()           PORTCbits.RC2
#define RS_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RS_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RS_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define RS_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define RS_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define RS_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define RS_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define RS_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 aliases
#define IO_RC3_TRIS                 TRISCbits.TRISC3
#define IO_RC3_LAT                  LATCbits.LATC3
#define IO_RC3_PORT                 PORTCbits.RC3
#define IO_RC3_WPU                  WPUCbits.WPUC3
#define IO_RC3_OD                   ODCONCbits.ODCC3
#define IO_RC3_ANS                  ANSELCbits.ANSELC3
#define IO_RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_RC3_GetValue()           PORTCbits.RC3
#define IO_RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_RC3_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_RC3_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_RC3_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define IO_RC3_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define IO_RC3_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define IO_RC3_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 aliases
#define IO_RC4_TRIS                 TRISCbits.TRISC4
#define IO_RC4_LAT                  LATCbits.LATC4
#define IO_RC4_PORT                 PORTCbits.RC4
#define IO_RC4_WPU                  WPUCbits.WPUC4
#define IO_RC4_OD                   ODCONCbits.ODCC4
#define IO_RC4_ANS                  ANSELCbits.ANSELC4
#define IO_RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_RC4_GetValue()           PORTCbits.RC4
#define IO_RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_RC4_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_RC4_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define IO_RC4_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define IO_RC4_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define IO_RC4_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define IO_RC4_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 aliases
#define DIR485_TRIS                 TRISCbits.TRISC5
#define DIR485_LAT                  LATCbits.LATC5
#define DIR485_PORT                 PORTCbits.RC5
#define DIR485_WPU                  WPUCbits.WPUC5
#define DIR485_OD                   ODCONCbits.ODCC5
#define DIR485_ANS                  ANSELCbits.ANSELC5
#define DIR485_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define DIR485_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define DIR485_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define DIR485_GetValue()           PORTCbits.RC5
#define DIR485_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define DIR485_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define DIR485_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define DIR485_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define DIR485_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define DIR485_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define DIR485_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define DIR485_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 aliases
#define IO_RC6_TRIS                 TRISCbits.TRISC6
#define IO_RC6_LAT                  LATCbits.LATC6
#define IO_RC6_PORT                 PORTCbits.RC6
#define IO_RC6_WPU                  WPUCbits.WPUC6
#define IO_RC6_OD                   ODCONCbits.ODCC6
#define IO_RC6_ANS                  ANSELCbits.ANSELC6
#define IO_RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define IO_RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define IO_RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define IO_RC6_GetValue()           PORTCbits.RC6
#define IO_RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define IO_RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define IO_RC6_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define IO_RC6_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define IO_RC6_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define IO_RC6_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define IO_RC6_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define IO_RC6_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 aliases
#define R47RX1_TRIS                 TRISCbits.TRISC7
#define R47RX1_LAT                  LATCbits.LATC7
#define R47RX1_PORT                 PORTCbits.RC7
#define R47RX1_WPU                  WPUCbits.WPUC7
#define R47RX1_OD                   ODCONCbits.ODCC7
#define R47RX1_ANS                  ANSELCbits.ANSELC7
#define R47RX1_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define R47RX1_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define R47RX1_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define R47RX1_GetValue()           PORTCbits.RC7
#define R47RX1_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define R47RX1_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define R47RX1_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define R47RX1_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define R47RX1_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define R47RX1_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define R47RX1_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define R47RX1_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set RD0 aliases
#define D0_TRIS                 TRISDbits.TRISD0
#define D0_LAT                  LATDbits.LATD0
#define D0_PORT                 PORTDbits.RD0
#define D0_WPU                  WPUDbits.WPUD0
#define D0_OD                   ODCONDbits.ODCD0
#define D0_ANS                  ANSELDbits.ANSELD0
#define D0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define D0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define D0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define D0_GetValue()           PORTDbits.RD0
#define D0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define D0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define D0_SetPullup()          do { WPUDbits.WPUD0 = 1; } while(0)
#define D0_ResetPullup()        do { WPUDbits.WPUD0 = 0; } while(0)
#define D0_SetPushPull()        do { ODCONDbits.ODCD0 = 0; } while(0)
#define D0_SetOpenDrain()       do { ODCONDbits.ODCD0 = 1; } while(0)
#define D0_SetAnalogMode()      do { ANSELDbits.ANSELD0 = 1; } while(0)
#define D0_SetDigitalMode()     do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set RD1 aliases
#define D1_TRIS                 TRISDbits.TRISD1
#define D1_LAT                  LATDbits.LATD1
#define D1_PORT                 PORTDbits.RD1
#define D1_WPU                  WPUDbits.WPUD1
#define D1_OD                   ODCONDbits.ODCD1
#define D1_ANS                  ANSELDbits.ANSELD1
#define D1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define D1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define D1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define D1_GetValue()           PORTDbits.RD1
#define D1_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define D1_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define D1_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define D1_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define D1_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define D1_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define D1_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define D1_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)

// get/set RD2 aliases
#define D2_TRIS                 TRISDbits.TRISD2
#define D2_LAT                  LATDbits.LATD2
#define D2_PORT                 PORTDbits.RD2
#define D2_WPU                  WPUDbits.WPUD2
#define D2_OD                   ODCONDbits.ODCD2
#define D2_ANS                  ANSELDbits.ANSELD2
#define D2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define D2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define D2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define D2_GetValue()           PORTDbits.RD2
#define D2_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define D2_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define D2_SetPullup()          do { WPUDbits.WPUD2 = 1; } while(0)
#define D2_ResetPullup()        do { WPUDbits.WPUD2 = 0; } while(0)
#define D2_SetPushPull()        do { ODCONDbits.ODCD2 = 0; } while(0)
#define D2_SetOpenDrain()       do { ODCONDbits.ODCD2 = 1; } while(0)
#define D2_SetAnalogMode()      do { ANSELDbits.ANSELD2 = 1; } while(0)
#define D2_SetDigitalMode()     do { ANSELDbits.ANSELD2 = 0; } while(0)

// get/set RD3 aliases
#define D3_TRIS                 TRISDbits.TRISD3
#define D3_LAT                  LATDbits.LATD3
#define D3_PORT                 PORTDbits.RD3
#define D3_WPU                  WPUDbits.WPUD3
#define D3_OD                   ODCONDbits.ODCD3
#define D3_ANS                  ANSELDbits.ANSELD3
#define D3_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define D3_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define D3_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define D3_GetValue()           PORTDbits.RD3
#define D3_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define D3_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define D3_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define D3_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define D3_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define D3_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define D3_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define D3_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)

// get/set RD4 aliases
#define D4_TRIS                 TRISDbits.TRISD4
#define D4_LAT                  LATDbits.LATD4
#define D4_PORT                 PORTDbits.RD4
#define D4_WPU                  WPUDbits.WPUD4
#define D4_OD                   ODCONDbits.ODCD4
#define D4_ANS                  ANSELDbits.ANSELD4
#define D4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define D4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define D4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define D4_GetValue()           PORTDbits.RD4
#define D4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define D4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define D4_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define D4_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define D4_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define D4_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define D4_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define D4_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set RD5 aliases
#define D5_TRIS                 TRISDbits.TRISD5
#define D5_LAT                  LATDbits.LATD5
#define D5_PORT                 PORTDbits.RD5
#define D5_WPU                  WPUDbits.WPUD5
#define D5_OD                   ODCONDbits.ODCD5
#define D5_ANS                  ANSELDbits.ANSELD5
#define D5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define D5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define D5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define D5_GetValue()           PORTDbits.RD5
#define D5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define D5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define D5_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define D5_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define D5_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define D5_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define D5_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define D5_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set RD6 aliases
#define D6_TRIS                 TRISDbits.TRISD6
#define D6_LAT                  LATDbits.LATD6
#define D6_PORT                 PORTDbits.RD6
#define D6_WPU                  WPUDbits.WPUD6
#define D6_OD                   ODCONDbits.ODCD6
#define D6_ANS                  ANSELDbits.ANSELD6
#define D6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define D6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define D6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define D6_GetValue()           PORTDbits.RD6
#define D6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define D6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define D6_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define D6_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define D6_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define D6_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define D6_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define D6_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set RD7 aliases
#define D7_TRIS                 TRISDbits.TRISD7
#define D7_LAT                  LATDbits.LATD7
#define D7_PORT                 PORTDbits.RD7
#define D7_WPU                  WPUDbits.WPUD7
#define D7_OD                   ODCONDbits.ODCD7
#define D7_ANS                  ANSELDbits.ANSELD7
#define D7_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define D7_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define D7_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define D7_GetValue()           PORTDbits.RD7
#define D7_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define D7_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define D7_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define D7_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define D7_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define D7_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define D7_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define D7_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

// get/set RE0 aliases
#define CS3_TRIS                 TRISEbits.TRISE0
#define CS3_LAT                  LATEbits.LATE0
#define CS3_PORT                 PORTEbits.RE0
#define CS3_WPU                  WPUEbits.WPUE0
#define CS3_OD                   ODCONEbits.ODCE0
#define CS3_ANS                  ANSELEbits.ANSELE0
#define CS3_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define CS3_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define CS3_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define CS3_GetValue()           PORTEbits.RE0
#define CS3_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define CS3_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define CS3_SetPullup()          do { WPUEbits.WPUE0 = 1; } while(0)
#define CS3_ResetPullup()        do { WPUEbits.WPUE0 = 0; } while(0)
#define CS3_SetPushPull()        do { ODCONEbits.ODCE0 = 0; } while(0)
#define CS3_SetOpenDrain()       do { ODCONEbits.ODCE0 = 1; } while(0)
#define CS3_SetAnalogMode()      do { ANSELEbits.ANSELE0 = 1; } while(0)
#define CS3_SetDigitalMode()     do { ANSELEbits.ANSELE0 = 0; } while(0)

// get/set RE1 aliases
#define CS2_TRIS                 TRISEbits.TRISE1
#define CS2_LAT                  LATEbits.LATE1
#define CS2_PORT                 PORTEbits.RE1
#define CS2_WPU                  WPUEbits.WPUE1
#define CS2_OD                   ODCONEbits.ODCE1
#define CS2_ANS                  ANSELEbits.ANSELE1
#define CS2_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define CS2_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define CS2_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define CS2_GetValue()           PORTEbits.RE1
#define CS2_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define CS2_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define CS2_SetPullup()          do { WPUEbits.WPUE1 = 1; } while(0)
#define CS2_ResetPullup()        do { WPUEbits.WPUE1 = 0; } while(0)
#define CS2_SetPushPull()        do { ODCONEbits.ODCE1 = 0; } while(0)
#define CS2_SetOpenDrain()       do { ODCONEbits.ODCE1 = 1; } while(0)
#define CS2_SetAnalogMode()      do { ANSELEbits.ANSELE1 = 1; } while(0)
#define CS2_SetDigitalMode()     do { ANSELEbits.ANSELE1 = 0; } while(0)

// get/set RE2 aliases
#define CS1_TRIS                 TRISEbits.TRISE2
#define CS1_LAT                  LATEbits.LATE2
#define CS1_PORT                 PORTEbits.RE2
#define CS1_WPU                  WPUEbits.WPUE2
#define CS1_OD                   ODCONEbits.ODCE2
#define CS1_ANS                  ANSELEbits.ANSELE2
#define CS1_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define CS1_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define CS1_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define CS1_GetValue()           PORTEbits.RE2
#define CS1_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define CS1_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define CS1_SetPullup()          do { WPUEbits.WPUE2 = 1; } while(0)
#define CS1_ResetPullup()        do { WPUEbits.WPUE2 = 0; } while(0)
#define CS1_SetPushPull()        do { ODCONEbits.ODCE2 = 0; } while(0)
#define CS1_SetOpenDrain()       do { ODCONEbits.ODCE2 = 1; } while(0)
#define CS1_SetAnalogMode()      do { ANSELEbits.ANSELE2 = 1; } while(0)
#define CS1_SetDigitalMode()     do { ANSELEbits.ANSELE2 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/
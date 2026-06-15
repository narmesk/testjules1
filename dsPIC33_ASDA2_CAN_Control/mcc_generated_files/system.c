#include "system.h"
#include "mcc.h"
#include "can1.h"
#include "dma.h"

// FICD
#pragma config ICS = PGD1
#pragma config JTAGEN = OFF

// FPOR
#pragma config ALTI2C1 = OFF
#pragma config ALTI2C2 = OFF
#pragma config WDTWIN = WIN25

// FWDT
#pragma config WDTPOST = PS32768
#pragma config WDTPRE = PR128
#pragma config PLLKEN = ON
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF

// FOSC
#pragma config POSCMD = XT              // 8MHz Kristal Modu
#pragma config OSCIOFNC = OFF
#pragma config IOL1WAY = OFF
#pragma config FCKSM = CSECME

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Primary Oscillator (XT) with PLL
#pragma config PWMLOCK = OFF
#pragma config IESO = ON

// FGS
#pragma config GWRP = OFF
#pragma config GCP = OFF

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();

    // Şimdilik sadece LED için donanımı başlatıyoruz
}

void OSCILLATOR_Initialize(void)
{
    // 8 MHz XT -> 64 MIPS
    CLKDIVbits.PLLPRE = 0;  // N1=2
    PLLFBDbits.PLLDIV = 62; // M=64
    CLKDIVbits.PLLPOST = 0; // N2=2

    // Explicit switch logic for safety
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 0b011);
    while (OSCCONbits.LOCK != 1);
}

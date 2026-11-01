#include "system.h"
#include "mcc.h"
#include "can1.h"
#include "dma.h"

// Configuration Bits (Korumaya devam ediyoruz, LED'i yakan ayarlar bunlar)
#pragma config ICS = PGD1
#pragma config JTAGEN = OFF
#pragma config ALTI2C1 = OFF
#pragma config ALTI2C2 = OFF
#pragma config WDTWIN = WIN25
#pragma config WDTPOST = PS32768
#pragma config WDTPRE = PR128
#pragma config PLLKEN = ON
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF
#pragma config POSCMD = XT
#pragma config OSCIOFNC = OFF
#pragma config IOL1WAY = OFF
#pragma config FCKSM = CSECME
#pragma config FNOSC = PRIPLL
#pragma config PWMLOCK = OFF
#pragma config IESO = ON
#pragma config GWRP = OFF
#pragma config GCP = OFF

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();

    // CAN ve DMA Katmanını Geri Getiriyoruz
    DMA_Initialize();
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
}

void OSCILLATOR_Initialize(void)
{
    // 8 MHz XT -> 64 MIPS
    CLKDIV = 0x0000;
    CLKDIVbits.PLLPRE = 0;  // N1=2
    PLLFBDbits.PLLDIV = 62; // M=64
    CLKDIVbits.PLLPOST = 0; // N2=2

    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 0b011);
    while (OSCCONbits.LOCK != 1);
}

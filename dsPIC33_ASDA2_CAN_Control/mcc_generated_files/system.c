#include "system.h"
#include "mcc.h"
#include "can1.h"
#include "dma.h"

/**
 * CONFIGURATION BITS - KRİTİK AYARLAR
 * Bu ayarlar olmadan MCU doğru hızda çalışmaz veya hiç başlamaz.
 */
// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF            // Watchdog Timer Enable bit (WDT disabled)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECME           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator w/ PLL)
#pragma config PWMLOCK = OFF            // Peripheral Pin Select Lock (PWM registers may be written without unlock)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protection is disabled)

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();
    DMA_Initialize();

    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
}

void OSCILLATOR_Initialize(void)
{
    // 8 MHz XT Kristal -> 64 MIPS
    CLKDIVbits.PLLPRE = 0;  // N1=2
    PLLFBDbits.PLLDIV = 62; // M=64
    CLKDIVbits.PLLPOST = 0; // N2=2 (128MHz Fosc)

    // Clock Switch (Already done by FNOSC=PRIPLL config, but explicit call for safety)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 0b011);
    while (OSCCONbits.LOCK != 1);
}

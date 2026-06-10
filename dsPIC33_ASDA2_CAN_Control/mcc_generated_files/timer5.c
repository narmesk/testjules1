#include "timer5.h"
#include "../asda2_control.h"

void Timer5_Initialize(void)
{
    // Assuming 70 MIPS (Fcy = 70MHz)
    // TMR5 period = 4ms
    // Prescaler 1:64 -> 70,000,000 / 64 = 1,093,750 Hz
    // 4ms * 1,093,750 = 4,375

    T5CONbits.TON = 0;      // Stop Timer
    T5CONbits.TCS = 0;      // Internal Clock (Fcy)
    T5CONbits.TCKPS = 0b10; // Prescaler 1:64
    TMR5 = 0x00;            // Clear Timer
    PR5 = 4375;             // Period match for 4ms

    IPC7bits.T5IP = 5;      // Priority
    IFS1bits.T5IF = 0;      // Clear Flag
    IEC1bits.T5IE = 1;      // Enable Interrupt

    T5CONbits.TON = 1;      // Start Timer
}

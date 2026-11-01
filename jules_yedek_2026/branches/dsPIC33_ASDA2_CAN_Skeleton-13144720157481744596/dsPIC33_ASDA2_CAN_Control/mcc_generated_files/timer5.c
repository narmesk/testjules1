#include "timer5.h"
#include "../asda2_control.h"

void Timer5_Initialize(void)
{
    // dsPIC33EP @ 64 MIPS (Fcy = 64MHz)
    // TMR5 period = 4ms
    // Prescaler 1:64 -> 64,000,000 / 64 = 1,000,000 Hz
    // 4ms * 1,000,000 = 4,000 cycles

    T5CONbits.TON = 0;
    T5CONbits.TCS = 0;
    T5CONbits.TCKPS = 0b10; // 1:64
    TMR5 = 0x00;
    PR5 = 3999;             // 4ms match

    IPC7bits.T5IP = 5;
    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 1;

    T5CONbits.TON = 1;
}

#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    /* PPS Eşlemesi - Kullanıcının Donanım Ayarları ile Senkronize */

    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 RX: RB12 (RP44) -> 0x2C
    RPINR26bits.C1RXR = 0x002C;

    // ECAN1 TX: RB10 (RP42) -> Function 0x0E
    RPOR4bits.RP42R = 0x000E;

    // SPI2 ve diğer PPS ayarların (ihtiyaç varsa eklenir)
    // RPOR1bits.RP37R = 0x0009; // RB5 -> SPI2 SCK
    // RPOR1bits.RP36R = 0x0008; // RB4 -> SPI2 SDO

    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

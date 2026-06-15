#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // Pinleri Çıkış Olarak Ayarla (TRIS bits)
    TRISBbits.TRISB15 = 0; // Heartbeat LED / Debug Pin
    TRISBbits.TRISB10 = 0; // ECAN1 TX

    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    // ECAN1 Mapping
    RPINR26bits.C1RXR = 0x002C; // RB12 (RP44) -> RX
    RPOR4bits.RP42R = 0x000E;   // RB10 (RP42) -> TX

    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

#include "mcc.h"

void PIN_MANAGER_Initialize(void)
{
    // dsPIC33EP64MC504 Pin Başlatma

    // 1. ANALOG ÖZELLİKLERİ KAPAT (KRİTİK!)
    // Eğer bu register sıfırlanmazsa RB9 dahil birçok pin analog kalır ve çıkış vermez.
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // 2. PIN YÖNLENDİRME (TRIS)
    TRISBbits.TRISB9 = 0;  // RB9 Çıkış (Senin LED'in)
    TRISBbits.TRISB10 = 0; // ECAN1 TX Çıkış

    // 3. PPS (Peripheral Pin Select)
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    RPINR26bits.C1RXR = 0x002C; // RB12 (RP44) -> RX
    RPOR4bits.RP42R = 0x000E;   // RB10 (RP42) -> TX

    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

#include "system.h"
#include "mcc.h"
#include "can1.h"
#include "dma.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    PIN_MANAGER_Initialize();
    DMA_Initialize();

    // CAN initialization
    CAN1_Initialize();
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
}

void OSCILLATOR_Initialize(void)
{
    // Donanımsal 8 MHz Kristal (Primary Oscillator) -> 64 MIPS (Fcy) Hedefi
    // Fosc = Fin * (M / (N1 * N2))
    // Fosc = 8 * (64 / (2 * 2)) = 128 MHz
    // Fcy = Fosc / 2 = 64 MIPS

    CLKDIVbits.PLLPRE = 0;  // N1=2
    PLLFBDbits.PLLDIV = 62; // M=64
    CLKDIVbits.PLLPOST = 0; // N2=2

    // Güvenli Switch: Primary Oscillator w/ PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 0b011); // Switch bekle
    while (OSCCONbits.LOCK != 1);     // PLL kilidini bekle
}

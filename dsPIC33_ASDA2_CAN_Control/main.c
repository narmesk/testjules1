#include "mcc_generated_files/system.h"
#include "mcc_generated_files/timer5.h"
#include "asda2_control.h"

int main(void)
{
    SYSTEM_Initialize();
    ASDA2_Initialize();
    CANopen_Start_Nodes();
    Timer5_Initialize();

    __builtin_enable_interrupts();

    while (1)
    {
        // CPU YAŞIYOR MU? - Ana Döngü Kalp Atışı
        // RB15'i toggle et. Eğer lojik analizörde burayı görüyorsan kod buraya kadar geliyor demektir.
        LATBbits.LATB15 = !LATBbits.LATB15;

        // Basit bir gecikme ekle ki çok hızlı toggle etmesin (Test amaçlı)
        for(volatile uint32_t i=0; i<100000; i++);

        // CAN Test: Periyodik SYNC gönder
        Send_SYNC_Message();
    }

    return 0;
}

#include <xc.h>

void PIN_MANAGER_Initialize(void)
{
   /**
    LATx registers
    */
    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x18;
    LATD = 0x0;
    LATE = 0x0;

    /**
    ODx registers
    */
    ODCONA = 0x0;
    ODCONB = 0x0;
    ODCONC = 0x18;
    ODCOND = 0x0;
    ODCONE = 0x0;

    /**
    TRISx registers
    */
    TRISA = 0xDF;
    TRISB = 0x4;
    TRISC = 0x80;
    TRISD = 0x0;
    TRISE = 0x0;

    /**
    ANSELx registers
    */
    ANSELA = 0xC0;
    ANSELB = 0x0;
    ANSELC = 0x0;
    ANSELD = 0x0;
    ANSELE = 0x0;

    /**
    WPUx registers
    */
    WPUA = 0x0;
    WPUB = 0x0;
    WPUC = 0x0;
    WPUD = 0x0;
    WPUE = 0x0;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;
    SLRCOND = 0xFF;
    SLRCONE = 0x7;

    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;
    INLVLD = 0xFF;
    INLVLE = 0xF;

   /**
    RxyI2C | RxyFEAT registers
    */
    RB1I2C = 0x0;
    RB2I2C = 0x0;
    RC3I2C = 0x0;
    RC4I2C = 0x0;
    RD0I2C = 0x0;
    RD1I2C = 0x0;

    /**
    PPS registers
    */
    U2RXPPS = 0xA; //RB2->UART2:RX2;
    U1RXPPS = 0x17; //RC7->UART1:RX1;
    RB3PPS = 0x16;  //RB3->UART2:TX2;
    RC6PPS = 0x13;  //RC6->UART1:TX1;
    I2C1SCLPPS = 0x13;  //RC3->I2C1:SCL1;
    RC3PPS = 0x21;  //RC3->I2C1:SCL1;
    I2C1SDAPPS = 0x14;  //RC4->I2C1:SDA1;
    RC4PPS = 0x22;  //RC4->I2C1:SDA1;

   /**
    IOCx registers
    */
    IOCAP = 0x0;
    IOCAN = 0x0;
    IOCAF = 0x0;
    IOCBP = 0x0;
    IOCBN = 0x0;
    IOCBF = 0x0;
    IOCCP = 0x0;
    IOCCN = 0x0;
    IOCCF = 0x0;
    IOCEP = 0x0;
    IOCEN = 0x0;
    IOCEF = 0x0;
}

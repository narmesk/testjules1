/*
 * File:   kayankapi.c
 * Author: Haser Otomasyon
 *
 * Created on 10 Ocak 2017 Sal?, 16:29
 */


#include "kayankapi.h"

signed long currentLocation = 0; // store current location in steps so we can use absolute moves
signed int revolutions = 0;
unsigned char state = 0;
unsigned char last_button_timer = 0;
unsigned char last_button_flag = 0;
unsigned char trigger_close = 0;

// --------- Global variables ----------------------
unsigned int ADval; // new AD value
unsigned int ADvalBuf[32];
unsigned char ADvalidx = 0;
unsigned int ADval_avg; // Average value
unsigned char ADval_Ready = 0;

unsigned int PWMval; // PWM value

unsigned int TempCnt;
unsigned char TimerCnt1;
unsigned char AlmCnt1 = 0;
unsigned char AlmCnt2 = 0;
unsigned char AlmCnt3 = 0;
unsigned int TimerCnt11;
unsigned char buffer[8];
unsigned char Txbuffer[8];
unsigned char rcvbuffidx;
unsigned char temp;
unsigned char receive_flag;
unsigned char transmit_flag;
unsigned char header_ok;
unsigned char RXCRC_Low, RXCRC_High;
unsigned char Addr_Low, Addr_High;
unsigned char Cnt_Low, Cnt_High;
unsigned int StatVal = 0;
unsigned char Button_Stat = 0; //Bütün butonlar bas?l? de?il

int main(int argc, char** argv)
{
    delay_ms(250);
    init();
    delay_ms(1000);
    ADvalidx = 0;
    ADval_avg = 0;
    ENB = 1;
    while (1)
    {
        /*if (state == Stat_Wait_Button)
        {
            if (last_button_flag == 1)
            {
                last_button_timer++;
                if (last_button_timer > 127)//5sn
                {
                    last_button_timer = 0;
                    Button_Stat = BUTTONCLOSE_PRESSED;
                }
                last_button_flag = 0;
            }
        }*/
        if (BUTTONCLOSE == 0)
            Button_Stat = BUTTONCLOSE_PRESSED;
        if (BUTTONOPEN == 0)
            Button_Stat = BUTTONOPEN_PRESSED;
        if ((transmit_flag == 1) || (transmit_flag == 2))//diff addr read
        {
            CharToRS232(Txbuffer[0]);
            CharToRS232(Txbuffer[1]);
            CharToRS232(Txbuffer[2]);
            CharToRS232(Txbuffer[3]);
            CharToRS232(Txbuffer[4]);
            CharToRS232(Txbuffer[5]);
            CharToRS232(Txbuffer[6]);
            transmit_flag = 0;
        }
        else if (transmit_flag == 4)//write
        {
            CharToRS232(Txbuffer[0]);
            CharToRS232(Txbuffer[1]);
            CharToRS232(Txbuffer[2]);
            CharToRS232(Txbuffer[3]);
            CharToRS232(Txbuffer[4]);
            CharToRS232(Txbuffer[5]);
            CharToRS232(Txbuffer[6]);
            CharToRS232(Txbuffer[7]);
            transmit_flag = 0;
            if (StatVal == 1)
                Button_Stat = BUTTONOPEN_PRESSED;
            else if (StatVal == 2)
                Button_Stat = BUTTONCLOSE_PRESSED;
        }
        if (state == Stat_Start)
        {
            PDC1 = Zero_Speed;
            PDC2 = Home_Speed; //yakla??k yüzde 30 ile aç?lma
            state = Stat_Search_Home;
        }
        else if (state == Stat_Search_Home)
        {
            if (ADval_Ready == Current_Trig_Low)
            {
                PDC1 = Zero_Speed;
                PDC2 = Zero_Speed; //yakla??k yüzde 30 ile aç?lma
                POSCNT = 32767;
                ADval_Ready = Current_Trig_Zero;
                AlmCnt1 = 0;
                AlmCnt2 = 0;
                AlmCnt3 = 0;
                Button_Stat = 0;
                state = Stat_Wait_Button;
            }
        }
        else if (state == Stat_Wait_Button)
        {
            if (Button_Stat == BUTTONCLOSE_PRESSED)
            {
                PDC1 = Max_Speed; //Kapanma
                PDC2 = Zero_Speed;
                state = Stat_Start_Close;
                ADval_Ready = Current_Trig_Zero;
                Button_Stat = 0;
            }
        }
        else if (state == Stat_Start_Close)
        {
            if (POSCNT >= 46500)
            {
                PDC1 = Home_Speed; //Kapanma
                PDC2 = Zero_Speed;
                state = Stat_Start_Close_Low;
            }
            if (ADval_Ready == Current_Trig_High)
            {
                if (AlmCnt2 > 19)
                {
                    state = Stat_Alarm_Fault;
                    ADval_Ready = Current_Trig_Zero;
                    AlmCnt2 = 0;
                }
            }
            else
            {
                AlmCnt2 = 0;
            }

            if (Button_Stat == BUTTONOPEN_PRESSED)
            {
                PDC1 = Zero_Speed; //
                PDC2 = Max_Speed; //Aç?lma
                state = Stat_Start_Open;
                Button_Stat = 0;
            }
        }
        else if (state == Stat_Start_Close_Low)
        {
            if (ADval_Ready == Current_Trig_Low)
            {
                if (AlmCnt1 > 9)
                {
                    PDC1 = Zero_Speed;
                    PDC2 = Zero_Speed; //yakla??k yüzde 30 ile aç?lma
                    TempCnt = POSCNT;
                    state = Stat_Start_Closed;
                    Button_Stat = 0;
                    ADval_Ready = Current_Trig_Zero;
                    AlmCnt1 = 0;
                }
            }
            else
            {
                AlmCnt1 = 0;
            }
            if (Button_Stat == BUTTONOPEN_PRESSED)
            {
                PDC1 = Zero_Speed; //
                PDC2 = Max_Speed; //Aç?lma
                state = Stat_Start_Open;
                Button_Stat = 0;
            }
        }
        else if (state == Stat_Start_Closed)
        {
            if (Button_Stat == BUTTONOPEN_PRESSED)
            {
                PDC1 = Zero_Speed; //
                PDC2 = Max_Speed; //Aç?lma
                state = Stat_Start_Open;
                Button_Stat = 0;
            }
        }
        else if (state == Stat_Start_Open)
        {
            if (POSCNT < (TempCnt - 13000))
            {
                PDC1 = Zero_Speed; //Kapanma
                PDC2 = Home_Speed;
                state = Stat_Start_Stop_Search;
            }
            if (ADval_Ready == Current_Trig_High)
            {
                if (AlmCnt2 > 19)
                {
                    state = Stat_Alarm_Fault;
                    ADval_Ready = Current_Trig_Zero;
                    AlmCnt2 = 0;
                }
            }
            else
            {
                AlmCnt2 = 0;
            }
        }
        else if (state == Stat_Start_Stop_Search)
        {
            if (POSCNT < (TempCnt - 15000))
            {
                PDC1 = Zero_Speed; //Kapanma
                PDC2 = Zero_Speed;
                state = Stat_Wait_Button;
                Button_Stat = 0;
            }
            if (ADval_Ready == Current_Trig_Low)
            {
                if (AlmCnt1 > 19)
                {
                    state = Stat_Alarm_Fault;
                    ADval_Ready = Current_Trig_Zero;
                    AlmCnt1 = 0;
                }
            }
            else
            {
                AlmCnt1 = 0;
            }
        }
        else if (state == Stat_Alarm_Fault)
        {
            PDC1 = Zero_Speed;
            PDC2 = Zero_Speed; //yakla??k yüzde 30 ile aç?lma
            LED = 0;
            delay_ms(500);
            LED = 1;
            delay_ms(500);
            state = Stat_Start;
            Button_Stat = 0;
        }
        if (state == Stat_Start_Closed)
        {
            if (ADval_Ready == Current_Trig_Fault)
            {
                if (AlmCnt3 > 5)
                {
                    state = Stat_Alarm_Fault;
                    ADval_Ready = Current_Trig_Zero;
                    AlmCnt3 = 0;
                }
            }
            else
            {
                AlmCnt3 = 0;
            }
        }
    }
    return (EXIT_SUCCESS);
}

void init(void)
{
    TRISE = 0x0000; // Portb nin tümü ç?k??
    TRISC = 0;
    TRISB = 0x0000; // Portb nin tümü ç?k??
    _TRISB2 = 1; //Set as Input pin BUTTON1 open
    _TRISE4 = 1; //Button2 Close
    _TRISB4 = 1; //QA
    _TRISB5 = 1; //QB
    _TRISB3 = 1; //INDX
    TRISF = 0b00000000;
    TRISD = 0x0000; // Portb nin tümü ç?k??
    PORTE = 0x00; // tüm portlar? sil
    LATE = 0x00;
    SPI1STATbits.SPIEN = 0; // Enable SPI port
    PORTB = 0; // tüm portlar? sil
    PORTD = 0; // tüm portlar? sil
    qei_init();
    T1CONbits.TCKPS = 0;
    PR1 = TIMER1_INTERVAL;
    POSCNT = 32767;
    T1CONbits.TON = 0; // Enable timer
    IEC0bits.T1IE = 0; // Enable interrupt
    initADC();
    init_pwm();
    SetupUART();
}

void initADC(void)
{
    _TRISB0 = 1; //Set as Input pin
    _TRISB1 = 1; //Set as Input pin
    ADPCFG = 0b1111111111111100; // tüm portlar digital
    ADCON1bits.ADON = 0; //ADC module OFF
    ADCON1bits.FORM = 0b00; //Integer Output
    ADCON1bits.SSRC = 0b111; //Clearing SAMP bit ends sampling and starts conversion
    ADCON1bits.SIMSAM = 0; //Sample multiple channels individually in sequence
    ADCON1bits.ASAM = 0; //Sampling begins when SAMP bit is set
    ADCON2bits.VCFG = 0b000; //voltage ref is AVDD & AVSS
    ADCON2bits.CSCNA = 0; //Do not scan input
    ADCON2bits.CHPS = 0b01; //converts CH0 & CH1
    ADCON2bits.SMPI = 0b0000; //For DMA
    ADCON2bits.BUFM = 0; //Buffer fills from the start address
    ADCON2bits.ALTS = 0; //Use channel A only
    ADCON3bits.ADRC = 0; //Clock derived from system clock
    ADCON3bits.SAMC = 0b11111; //Auto sample time bits (SAMC * Tad)
    ADCON3bits.ADCS = 0b111111; //ADC conversion clock time [Tcy*(ADCS +1) = Tad]
    ADCHSbits.CH0SA = 0; //Channel 0 positive input is AN0
    ADCON1bits.ADON = 1; // turn ADC ON
}

void init_pwm(void)
{
    PWM1 = 0;
    PWM2 = 0;
    PWMCON1 = 0; // Enable all PWM pairs in complementary mode
    PWMCON2 = 0;
    PTCON = 0;
    // Configure PWM for 10kHz frequency
    //
    //   PWM period = Tcy * prescale * PTPER = 0.333ns * 64 * PTPER
    //   PWM pulse width = (Tcy/2) * prescale * PDC
    //
    PWMCON1 = 0x0F03; //F; // Enable all PWM pairs in complementary mode
    PTCON = 0;
    _PTCKPS = 0; // prescale=1:64 (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    PTPER = 3000; // 100us PWM period (15-bit period value)
    PDC1 = PTPER; // 50% duty cycle on PWM channel 1
    PDC2 = PTPER; // 50% duty cycle on PWM channel 2
    PDC3 = PTPER; // 50% duty cycle on PWM channel 3
    PTMR = 0; // Clear 15-bit PWM timer counter
    _PTEN = 1; // Enable PWM time base

    // Configure Timer 1
    // In this example, I'm setting PR1 and TCKPS for 5kHz.
    // In other words, the Timer 1 interrupt service routine
    // will run 5000 times a second (i.e. every 200us).
    T1CON = 0; // Clear the Timer 1 configuration
    TMR1 = 0; // Reset Timer 1 counter
    PR1 = 6000; // Set the Timer 1 period (max 65535)
    T1CONbits.TCS = 0; // Select internal clock (Fosc/4)
    T1CONbits.TCKPS = 0; // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    _T1IP = 1; // Set the Timer 1 interrupt priority
    _T1IF = 0; // Clear the Timer 1 interrupt flag
    _T1IE = 1; // Enable Timer 1 interrupt
    T1CONbits.TON = 1; // Turn on Timer 1
}

unsigned int readADC(int ch)
{
    ADCHS = ch; // 1. select analog input channel
    ADCON1bits.SAMP = 1; // start sampling ...
    while (!ADCON1bits.DONE); //3. wait for the conversion to complete
    return ADCBUF0;
}

void ModBus_Get_CRC16_Value(unsigned char *puchMsg, unsigned char usDataLen)
{
    unsigned char uchCRCHi = 0xff;
    unsigned char uchCRCLo = 0xff;
    unsigned char uIndex;
    while (usDataLen--)
    {
        uIndex = uchCRCHi ^ *puchMsg++;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    RXCRC_Low = uchCRCLo;
    RXCRC_High = uchCRCHi;
}

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void)//????????uart1 TX interrupt
{
    _U1TXIF = 0; //Clear register uart1 TX ???? 0
}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)//???????? uart1 RX interrupt
{
    _U1RXIF = 0; //???????? uart1 RX interrupt
    temp = U1RXREG;
    if (header_ok == 1)
    {
        buffer[rcvbuffidx] = temp;
        rcvbuffidx++;
        if (rcvbuffidx >= 8)
        {
            rcvbuffidx = 0;
            header_ok = 0;
            ModBus_Get_CRC16_Value(buffer, 6);
            if ((RXCRC_High == buffer[6]) && (RXCRC_Low == buffer[7]))
            {
                receive_flag = 1;
                Addr_High = buffer[2]; //adres HI
                Addr_Low = buffer[3]; //adres LO
                Cnt_High = buffer[4]; //Count HI
                Cnt_Low = buffer[5]; //Count LO
                if (buffer[1] == 0x03) //read
                {
                    Txbuffer[2] = Cnt_Low * 2;
                    if (Cnt_Low == 1)
                    {
                        Txbuffer[0] = buffer[0];
                        Txbuffer[1] = buffer[1];
                        if (Addr_Low == 0x07)//Analog de?er sadece okunabilir
                        {
                            transmit_flag = 1;
                            Txbuffer[3] = ((ADval_avg >> 8)&0xff);
                            Txbuffer[4] = (ADval_avg & 0xff);
                        }
                        else if (Addr_Low == 0x09)//status de?eri hem yaz?l?r hem okunur
                        {
                            transmit_flag = 2;
                            Txbuffer[3] = ((StatVal >> 8)&0xff);
                            Txbuffer[4] = (StatVal & 0xff);
                        }
                        ModBus_Get_CRC16_Value(Txbuffer, 5);
                        Txbuffer[5] = RXCRC_High;
                        Txbuffer[6] = RXCRC_Low;
                    }
                }
                else if (buffer[1] == 0x06)//Write
                {
                    Txbuffer[0] = buffer[0];
                    Txbuffer[1] = buffer[1];
                    Txbuffer[2] = Addr_High;
                    Txbuffer[3] = Addr_Low;
                    if (Addr_Low == 0x09)//status de?eri hem yaz?labilir hem okunur
                    {
                        transmit_flag = 4;
                        StatVal = 0; //Status anl?k olarak komuta dönü?sün
                        StatVal = ((unsigned int) Cnt_High << 8) + (unsigned int) Cnt_Low;
                        Txbuffer[4] = ((StatVal >> 8)&0xff);
                        Txbuffer[5] = (StatVal & 0xff);
                    }
                    ModBus_Get_CRC16_Value(Txbuffer, 6);
                    Txbuffer[6] = RXCRC_High;
                    Txbuffer[7] = RXCRC_Low;
                }
            }
            else
            {

            }
        }
    }
    else
    {
        if ((rcvbuffidx == 0) && (temp == 0x03))
        {
            buffer[rcvbuffidx] = temp;
            rcvbuffidx++;
            header_ok = 1;
        }
    }

}

void __attribute__((__interrupt__, auto_psv)) _QEIInterrupt(void)
{
    if (QEICONbits.UPDN == 1)
    {
        POSCNT = 32768;
        revolutions++;
    }
    else
    {
        POSCNT = 32767;
        revolutions--;
    }
    IFS2bits.QEIIF = 0; // reset the if flag
    /*if (QEICONbits.CNTERR)
    {
        QEICONbits.CNTERR = 0;      // reset count error flag
    }*/
}

void qei_init(void)
{

    ConfigIntQEI(QEI_INT_PRI_1 & QEI_INT_ENABLE);
    OpenQEI(QEI_DIR_SEL_CNTRL & // UPDN bit decides direction count
            QEI_INT_CLK & // Use FCY for clocking
            QEI_INDEX_RESET_DISABLE &
            QEI_CLK_PRESCALE_1 & // 1:1 Prescale
            QEI_GATED_ACC_DISABLE &
            QEI_NORMAL_IO & // QEI Control state of IO pin
            QEI_INPUTS_NOSWAP & // Channels A and B are not swapped
            QEI_MODE_x4_MATCH & // X4 mode with counter reset by match
            QEI_UP_COUNT & // Read only for this mode
            QEI_IDLE_CON, // Stop in idle operationQEI_IDLE_STOP
            MATCH_INDEX_INPUT_PHASEA &
            QEI_QE_CLK_DIVIDE_1_16 & // Filter 1:16 divider
            QEI_QE_OUT_ENABLE); // Enable the input digital filter
    POSCNT = 32767; // Set Current Position to 0
    MAXCNT = 0xFFFF; //29999;//9999;                // Use the full 16bit counter
}

/* Engtimer */
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    TimerCnt1++; //203us
    if (TimerCnt1 > 5)//1.22ms
    {
        TimerCnt1 = 0;
        ADval = readADC(0);
        if (ADval > 1000)
            ADval = 1000;
        ADvalBuf[ADvalidx] = ADval;
        ADvalidx++;
        if (ADvalidx > 31)
        {
            ADval_avg = ADvalBuf[0] + ADvalBuf[1] + ADvalBuf[2] + ADvalBuf[3];
            ADval_avg = ADval_avg + ADvalBuf[4] + ADvalBuf[5] + ADvalBuf[6] + ADvalBuf[7];
            ADval_avg = ADval_avg + ADvalBuf[8] + ADvalBuf[9] + ADvalBuf[10] + ADvalBuf[11];
            ADval_avg = ADval_avg + ADvalBuf[12] + ADvalBuf[13] + ADvalBuf[14] + ADvalBuf[15];
            ADval_avg = ADval_avg + ADvalBuf[16] + ADvalBuf[17] + ADvalBuf[18] + ADvalBuf[19];
            ADval_avg = ADval_avg + ADvalBuf[20] + ADvalBuf[21] + ADvalBuf[22] + ADvalBuf[23];
            ADval_avg = ADval_avg + ADvalBuf[24] + ADvalBuf[25] + ADvalBuf[26] + ADvalBuf[27];
            ADval_avg = ADval_avg + ADvalBuf[28] + ADvalBuf[29] + ADvalBuf[30] + ADvalBuf[31];
            ADval_avg = ADval_avg / 32;
            ADvalidx = 0;
            if ((PDC1 == Home_Speed) || (PDC2 == Home_Speed))//dü?ük h?zla giderken alarm alg?lama
            {
                if (ADval_avg > Home_Current)
                {
                    ADval_Ready = Current_Trig_Low;
                    AlmCnt1++;
                }
            }
            else if ((PDC1 == Max_Speed) || (PDC2 == Max_Speed))//max h?zla giderken alarm alg?lama
            {
                if (ADval_avg > Max_Current)
                {
                    ADval_Ready = Current_Trig_High;
                    AlmCnt2++;
                }
            }
            else //gitme durumu yokken alarm alg?lama
            {
                if (ADval_avg > 15)
                {
                    ADval_Ready = Current_Trig_Fault;
                    AlmCnt3++;
                }
            }
            last_button_flag = 1; //39.06901ms
        }
    }
    // Update the duty cycle register
    //6000 max
}

void SetupUART(void)
{

    U1MODE = 0; // reset
    U1STA = 0;
    U1MODEbits.ALTIO = 1; // Enable Alternate I/O: U1ATX e non U1TX
    U1BRG = 31; //57600 baud measured!
    U1MODEbits.UARTEN = 1; // enableUART
    U1MODEbits.USIDL = 0; // run in idle mode
    U1MODEbits.WAKE = 0; // not wake
    U1MODEbits.LPBACK = 0; // Loopback disable
    U1MODEbits.ABAUD = 0; // auto baud disable
    U1MODEbits.PDSEL = 0; // 8 bit, no parity
    U1MODEbits.STSEL = 0; // 1 stop bit
    IFS0bits.U1RXIF = 0; // reset interrupt
    U1STAbits.URXISEL = 0; // interrupt with char in RX
    IEC0bits.U1RXIE = 1; // enable interrupt RX int
}

void CharToRS232(unsigned char x)
{
    U1STAbits.UTXEN = 1;
    while (U1STAbits.UTXBF)
    {
        //wait empy buffer
    }
    U1TXREG = x;
}

signed long dspicabs(signed long x)
{
    return (x < 0) ? -x : x;
}

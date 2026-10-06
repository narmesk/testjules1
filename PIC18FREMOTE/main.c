#include "mcc_generated_files/system/system.h"
#include "main.h"
#include "glcd.h"
#include <string.h>

/*
================================================================================-------------------
  PIC18FREMOTE YENİ NESİL HMI MENÜ SİSTEMİ ANA UYGULAMA KODU (main.c)
================================================================================-------------------
  ÇALIŞMA PRENSİBİ:
  1. Donanım Başlatma (SYSTEM_Initialize, GLCD_Init, I2C1, Timer0 0.5 ms).
  2. "Ledovate REMOTE PRICE CHANGER" Giriş/Açılış Animasyonu.
  3. Ana Döngüde (`while(1)`):
     - Her 500 µs Timer0 kesmesinde `update = 1` bayrağı ile senkronize çalışır.
     - Tuş takımı taranır (`read_keyb()`) ve kenar tetiklemeli `ButtonKeyPress == 1` ile menü geçişi yapılır.
     - Tuş 1: IDLE / Fiyat Tablosu Sayfaları (IDLE_PAGE1/2/3)
     - Tuş 2: SENDER / Pano Gönderim Sayfaları (SENDING_PAGE1/2/3)
     - Tuş 3: CONFIG ID / Sequence ID Ayarı
     - Tuş 4: CONFIG PARAM / Sub Parameters
     - Tuş 5: CONFIG PRICE / Ürün Fiyat Ayarı
     - Tuş 6: ENTER PASSWORD / Şifre Girişi
  4. Otomatik Zamanlayıcı (`demo_timer`): 3 saniye boyunca tuşa basılmazsa IDLE sayfaları otomatik döner.
  5. `HMI_Menu_Manager()` çağrılarak gölge bellek (`glcd_buffer[1536]`) üzerinden TEK GEÇİŞTE donanıma basılır.
================================================================================-------------------
*/

// Sistem ve Menü Durum Değişkenleri
unsigned char main_state = IDLE;
unsigned char page_state = IDLE_PAGE1;
unsigned char menu_state = Menu_Creating;

extern unsigned char ButtonKeyPress;
extern unsigned char ButtonKeyNum;
extern unsigned char cursor_blink;
extern unsigned short cursor_blink_time;

unsigned char update = 0;
unsigned char success_flag = 1;
unsigned long success_timer = 0;

// Test ve Sayfa Zamanlayıcıları
unsigned short demo_timer = 0;
unsigned char current_idle_page = IDLE_PAGE1;
unsigned char current_sender_page = SENDING_PAGE1;

extern unsigned char tx, ty;

// Sürücü ve Menü Prototipleri
extern void CreateMenuIdle_Direct(unsigned char menucount);
extern void RefreshMenuIdle_Direct(unsigned char menucount);
extern void CreateMenuSender_Direct(unsigned char menucount);
extern void RefreshMenuSender_Direct(unsigned char menucount);
extern void CreateMenuConfigID_Direct(void);
extern void RefreshMenuConfigID_Direct(void);
extern void CreateMenuConfigParam_Direct(void);
extern void RefreshMenuConfigParam_Direct(void);
extern void CreateMenuConfigPrice_Direct(void);
extern void RefreshMenuConfigPrice_Direct(void);
extern void CreateMenuPassword_Direct(void);
extern void RefreshMenuPassWord_Direct(void);
extern void read_keyb(void);
extern void REMOTE_CONTROLLER_init(void);

// I2C Bellek Simülasyonu
#define MEMORY_SIZE 255
volatile uint8_t memory[MEMORY_SIZE];
volatile uint8_t address = 0;
volatile uint8_t stage = 0; // 0 = adres bekleniyor, 1 = veri bekleniyor

bool User_I2C1_Callback(i2c_client_transfer_event_t event)
{
    switch (event)
    {
    case I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH:
        stage = 0;
        return true;

    case I2C_CLIENT_TRANSFER_EVENT_RX_READY:
        if (stage == 0)
        {
            address = I2C1_ReadByte();
            if (address >= MEMORY_SIZE) address = 0;
            stage = 1;
        }
        else
        {
            memory[address++] = I2C1_ReadByte();
            if (address >= MEMORY_SIZE) address = 0;
        }
        return true;

    case I2C_CLIENT_TRANSFER_EVENT_TX_READY:
        I2C1_WriteByte(memory[address++]);
        if (address >= MEMORY_SIZE) address = 0;
        return true;

    case I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED:
        stage = 0;
        return true;

    case I2C_CLIENT_TRANSFER_EVENT_ERROR:
        stage = 0;
        return false;

    default:
        return true;
    }
}

static void REMOTE_CONTROLLER_timer0Interrupt(void)
{
    update = 1;

    if (success_flag == 0)
    {
        success_timer++;
        if (success_timer >= 100000) // ~50 Saniye deneme bekleme
        {
            success_timer = 0;
            BCKLG_Toggle();
        }
    }
    else
    {
        success_timer = 0;
    }
}

// HMI Menü Çizim ve Güncelleme Yöneticisi
void HMI_Menu_Manager(void)
{
    switch (main_state)
    {
    case IDLE:
        if (menu_state == Menu_Creating)
        {
            CreateMenuIdle_Direct(current_idle_page);
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuIdle_Direct(current_idle_page);
            menu_state = Menu_Wait;
        }
        break;

    case SENDING:
        if (menu_state == Menu_Creating)
        {
            CreateMenuSender_Direct(current_sender_page);
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuSender_Direct(current_sender_page);
            menu_state = Menu_Wait;
        }
        break;

    case CONFIGID:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigID_Direct();
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuConfigID_Direct();
            menu_state = Menu_Wait;
        }
        break;

    case CONFIGPARAM:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigParam_Direct();
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuConfigParam_Direct();
            menu_state = Menu_Wait;
        }
        break;

    case CONFIGPRICE:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigPrice_Direct();
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuConfigPrice_Direct();
            menu_state = Menu_Wait;
        }
        break;

    case ENTERPASSWORD:
        if (menu_state == Menu_Creating)
        {
            CreateMenuPassword_Direct();
            menu_state = Menu_Created;
        }
        else if (menu_state == Menu_Created)
        {
            RefreshMenuPassWord_Direct();
            menu_state = Menu_Wait;
        }
        break;

    default:
        main_state = IDLE;
        menu_state = Menu_Creating;
        break;
    }
}

int main(void)
{
    SYSTEM_Initialize();
    __delay_ms(350); // GLCD donanım ekran açılış gecikmesi
    I2C1_CallbackRegister(User_I2C1_Callback);

    TMR0_PeriodMatchCallbackRegister(REMOTE_CONTROLLER_timer0Interrupt);
    TMR0_Start();

    INTERRUPT_GlobalInterruptEnable();

    for (uint8_t i = 0; i < MEMORY_SIZE; i++) memory[i] = 0;

    GLCD_Init();
    GLCD_ClearAll();

    REMOTE_CONTROLLER_init();

    // 1. AÇILIŞ / GİRİŞ ANİMASYONU
    tx = 10;
    ty = 5;
    GLCDPutCharCalibri36(0x20); // L
    __delay_ms(150);
    GLCDPutCharCalibri36(0x21); // e
    __delay_ms(150);
    GLCDPutCharCalibri36(0x22); // d
    __delay_ms(150);
    GLCDPutCharCalibri36(0x23); // o
    __delay_ms(150);
    GLCDPutCharCalibri36(0x24); // v
    __delay_ms(150);
    GLCDPutCharCalibri36(0x25); // a
    __delay_ms(150);
    GLCDPutCharCalibri36(0x26); // t
    __delay_ms(150);
    GLCDPutCharCalibri36(0x21); // e
    __delay_ms(150);
    GLCD_StringArialBold14(10, 40, "REMOTE PRICE CHANGER");

    __delay_ms(1000);

    // İlk Menüyü Oluştur
    main_state = IDLE;
    menu_state = Menu_Creating;

    // 2. ANA SÜREÇ DÖNGÜSÜ
    while (1)
    {
        if (update == 1)
        {
            update = 0;
            CLRWDT();
            read_keyb(); // Tuş takımını tara

            // Kenar Tetiklemeli Tuş Algılama / Menü Değişimi
            if (ButtonKeyPress == 1)
            {
                if (ButtonKeyNum == 1) // Tuş 1: IDLE / Fiyat Tablosu
                {
                    main_state = IDLE;
                    current_idle_page++;
                    if (current_idle_page > IDLE_PAGE3) current_idle_page = IDLE_PAGE1;
                    menu_state = Menu_Creating;
                }
                else if (ButtonKeyNum == 2) // Tuş 2: SENDER / Pano Gönderim
                {
                    main_state = SENDING;
                    current_sender_page++;
                    if (current_sender_page > SENDING_PAGE3) current_sender_page = SENDING_PAGE1;
                    menu_state = Menu_Creating;
                }
                else if (ButtonKeyNum == 3) // Tuş 3: CONFIG ID
                {
                    main_state = CONFIGID;
                    menu_state = Menu_Creating;
                }
                else if (ButtonKeyNum == 4) // Tuş 4: CONFIG PARAM
                {
                    main_state = CONFIGPARAM;
                    menu_state = Menu_Creating;
                }
                else if (ButtonKeyNum == 5) // Tuş 5: CONFIG PRICE
                {
                    main_state = CONFIGPRICE;
                    menu_state = Menu_Creating;
                }
                else if (ButtonKeyNum == 6) // Tuş 6: ENTER PASSWORD
                {
                    main_state = ENTERPASSWORD;
                    menu_state = Menu_Creating;
                }
            }

            // İmleç Yanıp Sönme (Blink) Zamanlaması
            cursor_blink_time++;
            if (cursor_blink_time >= 600) // ~300 ms
            {
                cursor_blink_time = 0;
                cursor_blink ^= 1;
                if (main_state != IDLE && main_state != SENDING && menu_state == Menu_Wait)
                {
                    menu_state = Menu_Created; // Yanıp sönme olan ekranlarda tazelemeyi tetikle
                }
            }

            // Otomatik Demo Sayfa Geçişi (3 saniyede bir tuşa basılmazsa IDLE sayfaları döner)
            demo_timer++;
            if (demo_timer >= 6000) // 6000 x 0.5 ms = 3 Saniye
            {
                demo_timer = 0;
                if (main_state == IDLE)
                {
                    current_idle_page++;
                    if (current_idle_page > IDLE_PAGE3) current_idle_page = IDLE_PAGE1;
                    menu_state = Menu_Creating;
                }
            }

            // HMI EKRAN DİZGİSİNİ BASS
            HMI_Menu_Manager();
        }
    }
}

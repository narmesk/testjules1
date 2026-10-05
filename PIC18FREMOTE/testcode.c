#include "mcc_generated_files/system/system.h"
#include "main.h"
#include "glcd.h"
#include <string.h>

/*
================================================================================-------------------
  PIC18FREMOTE YENİ NESİL HMI MENÜ SİSTEMİ TEST KODU (testcode.c)
================================================================================-------------------
  BU KOD NE YAPIYOR?
  1. Donanım Başlatma (SYSTEM_Initialize, GLCD, I2C1 Client Callback, Timer0 0.5 ms).
  2. "Ledovate REMOTE PRICE CHANGER" Giriş/Açılış Animasyonu (Calibri36 + ArialBold14 Fontları).
  3. Ana Döngüde (`while(1)`):
     - Her 500 µs Timer0 kesmesinde `update = 1` bayrağı ile senkronize çalışır.
     - Tuş takımı taranır (`read_keyb()`) ve basılan tuşa göre menüler arası geçiş simüle edilir.
     - `ButtonKeyNum == 1` (Tuş 1): Fiyat Tablosu (IDLE P1/P2/P3)
     - `ButtonKeyNum == 2` (Tuş 2): Gönderim / Pano Durumu (SENDER P1/P2/P3)
     - `ButtonKeyNum == 3` (Tuş 3): Slave ID / Sub ID Ayar Ekranı (CONFIG ID)
     - `ButtonKeyNum == 4` (Tuş 4): Alt Parametre Ayar Ekranı (CONFIG PARAM)
     - `ButtonKeyNum == 5` (Tuş 5): Ürün Fiyat Ayar Ekranı (CONFIG PRICE)
     - `ButtonKeyNum == 6` (Tuş 6): Şifre Giriş Ekranı (ENTER PASSWORD)
  4. Yeni nesil `CreateMenu*_Direct` ve `RefreshMenu*_Direct` fonksiyonları çağrılarak 1536-baytlık
     RAM gölge belleği (`glcd_buffer`) üzerinden ekran TEK GEÇİŞTE (Single-pass Direct Render) taranır.
================================================================================-------------------
*/

// Sistem ve Menü Durum Değişkenleri
unsigned char main_state = IDLE;
unsigned char page_state = IDLE_PAGE1;
unsigned char menu_state = Menu_Creating;
unsigned char ButtonKeyPress = 0;
extern unsigned char ButtonKeyNum;

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
        else
        {
            RefreshMenuIdle_Direct(current_idle_page);
        }
        break;

    case SENDING:
        if (menu_state == Menu_Creating)
        {
            CreateMenuSender_Direct(current_sender_page);
            menu_state = Menu_Created;
        }
        else
        {
            RefreshMenuSender_Direct(current_sender_page);
        }
        break;

    case CONFIGID:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigID_Direct();
            menu_state = Menu_Created;
        }
        else
        {
            RefreshMenuConfigID_Direct();
        }
        break;

    case CONFIGPARAM:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigParam_Direct();
            menu_state = Menu_Created;
        }
        else
        {
            RefreshMenuConfigParam_Direct();
        }
        break;

    case CONFIGPRICE:
        if (menu_state == Menu_Creating)
        {
            CreateMenuConfigPrice_Direct();
            menu_state = Menu_Created;
        }
        else
        {
            RefreshMenuConfigPrice_Direct();
        }
        break;

    case ENTERPASSWORD:
        if (menu_state == Menu_Creating)
        {
            CreateMenuPassword_Direct();
            menu_state = Menu_Created;
        }
        else
        {
            RefreshMenuPassWord_Direct();
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

            // Tuş Geçiş Simülasyonu / Menü Değişimi
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

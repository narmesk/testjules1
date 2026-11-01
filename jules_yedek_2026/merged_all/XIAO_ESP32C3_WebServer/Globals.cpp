#include "Config.h"

AsyncWebServer server(80);

KeyValuePair espsettings[12];
String price_arr[24] = {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};
String parameters[48] = {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};

String sid = "";
String pid = "";
String ptype = "";
String pmaster = "";
String devprod = "d";
String mode_apsta = "apsta";
String espsettings_sid = "";

String ap_ssid_buff_def = "ESP32_AP";
String ap_password_buff_def = "12345678";
int ap_maxconnections = 4;

int wifiretries = 0;
int wifimaxretries = 20;
int wifi_fail_try = 0;

String serverName_process = "http://example.com/process.php";
String serverName_pool = "http://example.com/pool/";
String serverName_poolp = "";
String cfjres = "";

String receivepacket = "";
String receivepacket_before = "";
String transmitpacket = "";

char is_modbus_hmi = 'm';
char modbustask = 'x';
char change_source = 'a';
char modbusPTres[24] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

unsigned long currentTime = 0;
unsigned long cTime_wificonnect = 0;
const long intv_wificonnect = 10000;

int serialbufferarr[60] = {0};
int flag1 = 0;
String pt_startingaddr = "1";
String pt_quantity = "1";
int pt_slave_ids[24] = {0};
int pt_target_count = 0;
String modbus_function = "16";
String modbus_startingaddr = "0";
String modbus_quantity = "10";
String modbus_bytecount = "20";
int serialtrynum = 3;
String msg_str = "";

const char* PARAM_params[48] = {
    "p0","p1","p2","p3","p4","p5","p6","p7","p8","p9",
    "p10","p11","p12","p13","p14","p15","p16","p17","p18","p19",
    "p20","p21","p22","p23","p24","p25","p26","p27","p28","p29",
    "p30","p31","p32","p33","p34","p35","p36","p37","p38","p39",
    "p40","p41","p42","p43","p44","p45","p46","p47"
};

const char* PARAM_leds[18] = {
    "l0","l1","l2","l3","l4","l5","l6","l7","l8","l9",
    "l10","l11","l12","l13","l14","l15","n1","n2"
};

String StringDeleteChar(String str, char ch) {
    String res = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != ch) res += str[i];
    }
    return res;
}

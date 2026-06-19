#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include <HTTPClient.h>
#include "driver/uart.h"
#include <WiFiClientSecure.h>

#define RXD2 20
#define TXD2 21

unsigned long currentTime = millis();
unsigned long cTime_wificonnect = millis(); //wifi status check
unsigned long cTime_serverpool = millis(); //Server Pool
unsigned long cTime_restartnormal = millis(); //Restart Normal
unsigned long cTime_modbuschecksend = millis(); //Restart Normal
unsigned long intv_wificonnect = 60000; //wifi status check
unsigned long intv_serverpool = 60000; //Server Pool 60000=60seconds
unsigned long intv_restartnormal = 3600000; //Restart Normal , one hour
unsigned long intv_modbuschecksend = 60000; //modbus Pool 60000=60seconds

String prices = "";

String price_arr[24]={"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};
int priceupdatefinish=0;
String transmitpacket = "";//"0350303111222333444555666777888999103215456170003";
String receivepacket = "";//"011000000000000000000000O";
String receivepacket_before = "000000000000000000000000N";//FOR SERVER SEND RECEİVEPACKET
String receivepacket2 = "";
String pmaster="";//FOR SERVER SEND RECEİVEPACKET
String pid="";//FOR SERVER SEND RECEİVEPACKET
String ptype="";//FOR SERVER SEND RECEİVEPACKET
String parameters[48] ;
//String espsettings[] ;
String sid=""; //gstationid
String devprod="p"; //d or p
String logs ="";
String mode_apsta ="apsta"; //ap:only AP, apsta:both AP and STA
String ap_ssid_buff_def= "Ledovate-Price-Change";
String ap_password_buff_def= "123456789";
int wifi_fail_try = 0;

String msg_str;

uint8_t wifiretries=0;
uint8_t wifimaxretries=3;
uint8_t ap_maxconnections=2;//Maximum Connection Limit for AP
int ap_currentstations=0, ap_newstations=0;

typedef struct espsettings_kv_ {
   String key;
   String value;
} espsettings_kv_t;

espsettings_kv_t espsettings[] = {
   {"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""},{"", ""}
};
String espsettings_sid = ""; //settingsde degişen sid , lokalde var sadece

const char *PARAM_leds[] = {"led1", "led2", "led3", "led4", "led5", "led6", "led7", "led8","led9", "led10", "led11", "led12", "led13", "led14", "led15", "led16", "brightness","speed"};
const char *PARAM_params[] = {"ledparam1", "ledparam2", "ledparam3", "ledparam4", "ledparam5", "ledparam6", "ledparam7", "ledparam8","ledparam9", "ledparam10", "ledparam11", "ledparam12", "ledparam13", "ledparam14", "ledparam15", "ledparam16","ledparam17", "ledparam18", "ledparam19", "ledparam20", "ledparam21", "ledparam22", "ledparam23", "ledparam24","ledparam25", "ledparam26", "ledparam27", "ledparam28", "ledparam29", "ledparam30", "ledparam31", "ledparam32","ledparam33", "ledparam34", "ledparam35", "ledparam36", "ledparam37", "ledparam38", "ledparam39", "ledparam40","ledparam41", "ledparam42", "ledparam43", "ledparam44", "ledparam45", "ledparam46", "ledparam47", "ledparam48"};

String serverName = "https://www.ledobe.com/app/";
String serverName_process = serverName+"process.php";
String serverName_pool = serverName+"cfg/ppool/";
String serverName_ledcfg = serverName+"cfg/ledcfg/";
String serverName_pres = serverName+"cfg/pres/";
String serverName_poolp = "";

String poolres = "";
String cfjres = "";

unsigned int flag1 = 0;
int incomingByte;      // a variable to read incoming serial data into

String modbus_function="16";
String modbus_startingaddr="0";
String modbus_startingnum="0";
String modbus_quantity="24";
String modbus_bytecount="48";
String pt_quantity="0";
String pt_startingaddr="0";
String waitForReply="";
int serialtrynum=3; // in "a" phase do 3 times
int serialtrynum_cur= 0;
char modbusPTres[24];//MAX PT yaptım buraya, değişir mi ?
char modbustask = 'x'; //x finished or not started, s started, f esp send itself, r send with stable RP, t send with PT RP
byte serialbufferarr[57];
char is_modbus_hmi = 'm'; //h hmi, m modbus
char change_source = 'a'; //l localnetwork, a accesspoint, s server //where change occured but a and  l same now

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

#define highByte(x) ( (x) >> (8) ) // keep upper 8 bits
#define lowByte(x) ( (x) & (0xff) ) // keep lower 8 bits

String processor(const String& var){
  return String();
}

/*
 * WiFiEvent_t için not: ESP32 Core 3.x sürümünde imza değişmiş olabilir.
 * Eğer derleme hatası alırsanız 'WiFiEvent_t event, WiFiEventInfo_t info' şeklinde güncelleyin.
 */
void OnWiFiEvent(WiFiEvent_t event){
     //Serial.printf("WiFi-event - %d : ",event);
      serverlog("WiFi-event - ","");
      serverlog((String)event,"");
      serverlog(": ","");

  switch (event) {
     case 0:
      serverlog("WIFI_READY,0,ESP32 wifi ready","ln");
      break;
    case 10:
      serverlog("AP_START,10,AP Started","ln");
      break;
    case 12:
      serverlog("AP_STACONNECTED,12,AP Station Connected","ln");
      break;
    case 14:
      serverlog("AP_STAIPASSIGNED,14,AP Station IP Assigned","ln");
      break;
     case 13:
      serverlog("AP_STADISCONNECTED,13,AP Station Disconnected","ln");
      break;
    default:
      serverlog("otherevent","ln");
    break;
  }/**/
}

String StringDeleteChar(String words, char no){
    char c;
    //char no = '.'; //character I want removed.
    for (int i=0; i<words.length();++i){
        c = words.charAt(i);
        if(c==no){
            words.remove(i, 1);
        }
    }
    return words;
}

String LedPrepareResult(String type="res"){
    if(is_modbus_hmi=='m'){
		String receivepacket_wanted = "";
		receivepacket = "";
		 for(int i = 0; i < 24; i++)
		{
			receivepacket += modbusPTres[i];
			receivepacket_wanted += '1';

		}

    if (receivepacket.substring(0,parameters[2].toInt()) == receivepacket_wanted.substring(0,parameters[2].toInt()))
			receivepacket += 'O';
	else
			receivepacket += 'N';

	}
  //receivepacket="110000000000000000000000O\n";
  String receivepacketfilter = "";
  receivepacketfilter = receivepacket;
  receivepacketfilter.trim();
  receivepacketfilter=receivepacketfilter.substring(receivepacketfilter.length()-25);
  serverlog("filtered:","ln");
  serverlog(receivepacketfilter,"");

String msg="{\"res\":\""+receivepacketfilter+"\"}";
    //msg = "{\"35\":\"csrok\"}";
   //msg+="{";

/*
for (int i = 0; i < receivepacket.length(); i++) {
  if (receivepacket.substring(i, i+1) == "1") {
       msg+="\""+(i+1)+"\":\"";
  }
}
 */
    if(type=="res")
     return msg;
    else if(type=="trim")
     return receivepacketfilter;
}

int FileWriteF(String txt, String path){

 File file = SPIFFS.open(path, FILE_WRITE);
  if(!file){
    serverlog("Failed to open file for reading","ln");
    //return "";
  }

  int bytesWritten = file.print(txt);

  if (bytesWritten > 0) {
    serverlog("ledconfig.json written success:","");
    serverlog((String)bytesWritten,"ln");
  } else {
    serverlog("ledconfig.json write failed","ln");
  }

  file.close();

  return bytesWritten;
}

String WriteLedConfig(const String proct){

/**/
File file = SPIFFS.open("/ledconfig.json");
  if(!file){
    serverlog("Failed to open ledconfig.json for reading","ln");
    //return "";
  }

String ledconfigstr;

  serverlog("ledconfig.json Read Success","ln");
  while(file.available()){
    //Serial.write(file.read());
    ledconfigstr+=String((char)file.read());
  }
  //Serial.println(ledconfigstr);
file.close();

DynamicJsonDocument doc(8192);
 //char json[] ="{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
// Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, ledconfigstr);
 // Test if parsing succeeds.
  if (error) {
    serverlog("deserializeJson() failed: ","");
    serverlog(error.f_str(),"ln");
    return "";
  }


//Station ID ve serverpool url hazırla
const char* gstationidc = doc["sid"] | "";
sid = gstationidc;
serverName_poolp =  serverName_pool + sid + ".json";

//server send receivepacket için
const char* pidc = doc["pid"] | "";
pid = pidc;
const char* ptypec = doc["ptype"] | "";
ptype = ptypec;
const char* pmasterc = doc["pmaster"] | "";
pmaster = pmasterc;

//const char* sensor = doc["leds"][0]["t"];
//Serial.println(sensor);
//Serial.println(doc.memoryUsage());  // 10 on AVR

if(proct == "p"){
   doc["ptype"]="pr";

/*
int m=0;
for (int i = 0; i < prices.length(); i++) {
  if (prices.substring(i, i+1) == ":") {
    //firstVal = input.substring(0, i).toInt();
     //Serial.println(prices.substring(i-4, i));
     doc["leds"][m]["v"]=prices.substring(i-4, i);
     m++;
    //break;
  }
}*/
for (int i = 0; i < 24; i++) {
     doc["leds"][i]["v"]=price_arr[i];
}
}
else if(proct=="m"){
     doc["ptype"]="pm";

  for (int z = 0; z < 48; z++) {
     doc["parameters"][z]["v"]=parameters[z];
    //break;
  }
}

else if(proct=="s"){
     doc["ptype"]="st";
     doc["sid"] = espsettings_sid;
for (int x = 0; x <= 11; x++) { //Serial.println(x);Serial.println(espsettings[x].key.c_str());
  if(espsettings[x].key!="")
    doc["esp32sett"][espsettings[x].key]=espsettings[x].value;
  }
}

if(proct == "p" || proct == "m" || proct == "s"){
   doc["pdate"]="";
   doc["pmaster"]="ln";
   doc["pid"]=random(11111111,99999999);
}

//doc["gstationid"]="2";
//doc.garbageCollect();
//Serial.println(doc.memoryUsage()); // 21

ledconfigstr="";
serializeJson(doc, ledconfigstr);
//Serial.println(ledconfigstr);

const char* vtmp="";
for(int i=0;i<48;i++)
  {
    vtmp = doc["parameters"][i]["v"];
      parameters[i]=vtmp;
  }

int x=0;
//JsonArray setts = doc["esp32sett"].as<JsonArray>();
JsonObject root = doc["esp32sett"].as<JsonObject>();

    for (JsonPair kv : root) {
      JsonString key = kv.key();
      espsettings[x].key = key.c_str();
      JsonString val = kv.value();
   espsettings[x].value = val.c_str();
    x+=1;
 //   JsonString key = kv.key();
 // Serial.println(key.c_str());
    }
devprod =  getvfrom_espsettings("devprod");

//IF ONLY READ OR SERVER RELATED CALL
if(proct == "x"){
	        //prices="";
const char* vtmp1="";
for(int i=0;i<24;i++)
  {
    vtmp1 = doc["leds"][i]["v"] | "0";
//prices+=String(vtmp1)+":";
           price_arr[i]=String(vtmp1);
  }

}
//IF SETTINGS, PARAMETERS, PICAES CHANGED
if(proct != "x"){

FileWriteF(ledconfigstr,"/ledconfig.json");

  //IF SETTINGS, PARAMETERS, PICAES CHANGED SEND CFJ TO SERVER
    String serverName_process2 = serverName_process + "?savecfj=t&sid=" + sid;
   sendhttp(ledconfigstr, "httpjson", serverName_process2);
}

    return "";

}

String SendSerialProcStart(){
  transmitpacket = "";
  int ledcount=36;
  String lastpart="170003";
  String pricedelm="";
  String paramdelm="";
  int priceleftc = 0;
  int priceleftctot = 36;

  //"0350303111222333444555666777888999103215456170003";
//035 03 03 -111 222-333  444-555 666-777 888-999  103-215 456-170-003
String prices = "";
for (int i = 0; i < 12; i++) {
     prices+=price_arr[i];
}

pricedelm = StringDeleteChar(prices, '.');    //Serial.println(pricedelm);
pricedelm = StringDeleteChar(pricedelm, ':');

priceleftc=priceleftctot-pricedelm.length();
for(int i=0;i<priceleftc;i++)
  pricedelm+="0";

if(parameters[1].length()==1)
  paramdelm="00"+parameters[1];
else if(parameters[1].length()==2)
    paramdelm="0"+parameters[1];
transmitpacket+=paramdelm;

if(parameters[2].length()==1)
  paramdelm="0"+parameters[2];
else
  paramdelm=parameters[2];
transmitpacket+=paramdelm;

if(parameters[5].length()==1)
  paramdelm="0"+parameters[5];
else
  paramdelm=parameters[5];
transmitpacket+=paramdelm;

//transmitpacket+=parameters[1];
//transmitpacket+=parameters[2];
//transmitpacket+=parameters[5];
 transmitpacket += pricedelm;
 transmitpacket+=lastpart;

    serverlog(transmitpacket,"ln");

  flag1=1;

return "";
}

void serverlog(String msg, String ln){
  if(devprod=="d" || devprod=="D"){
    if(ln=="ln")
      Serial.println(msg);
    else if(ln=="")
      Serial.print(msg);
  }

  else if(devprod=="p" || devprod=="P"){

  }
}

String getvfrom_espsettings(String key1){
   for (int x = 0; x < sizeof(espsettings)/sizeof(espsettings[0]); x++) {
    if (key1 == espsettings[x].key) {
     return espsettings[x].value;
    }
  }
  return "";
}

void esprestart(){
  serverlog("Restarting in 2 seconds","ln");
  delay(2000);
  ESP.restart();
}

void initap(){

	if(WiFi.status()!= WL_CONNECTED){

   WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  /*-----------WIFI not connected so try AP---------*/

    if(WiFi.status() != WL_CONNECTED)
	{

  String ap_ssid_buff="";
  String ap_password_buff="";

  if(getvfrom_espsettings("apid")==""){
     ap_ssid_buff= ap_ssid_buff_def;
     ap_password_buff= ap_password_buff_def;
  }
  else{
    ap_ssid_buff=getvfrom_espsettings("apid");
    ap_password_buff=getvfrom_espsettings("appw");
  }

    char ap_ssid[ap_ssid_buff.length()+1];
    ap_ssid_buff.toCharArray(ap_ssid, ap_ssid_buff.length()+1);

    char ap_password[ap_password_buff.length()+1];
    ap_password_buff.toCharArray(ap_password, ap_password_buff.length()+1);


    serverlog("Setting AP ","ln");
    //WiFi.mode(WIFI_AP);

    //Setting the AP Mode with SSID, Password, and Max Connection Limit
    if(WiFi.softAP(ap_ssid,ap_password,1,false,ap_maxconnections)==true)
    {
    delay(100);//seems like this delay is quite important.

    IPAddress Ip(7, 3, 2, 1);
    IPAddress NMask(255, 255, 255, 0);
    WiFi.softAPConfig(Ip, Ip, NMask);

    serverlog("AP: ","");
    serverlog(ap_ssid,"");
    serverlog(" : ","");
    serverlog((String)WiFi.softAPIP().toString().c_str(),"ln");

    /*
    Serial.println(ap_ssid_buff);
    Serial.println(ap_password_buff);
    Serial.println(ap_ssid);
    Serial.println(ap_password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Print ESP8266 Local IP Address
    //Serial.println(WiFi.localIP());
    */

    }
    else
    {
    delay(100);//seems like this delay is quite important.
     serverlog("Unable to Create Access Point","ln");
    }


    }

}
}

void initwifi(){


	if(mode_apsta=="apsta"){

  WiFi.mode(WIFI_STA);

  /*-----------WIFI connect try first----------*/

    String wifi_ssid_buff="";
    String wifi_password_buff="";
    wifi_ssid_buff=getvfrom_espsettings("wifiid");
    wifi_password_buff=getvfrom_espsettings("wifipw");

    if(wifi_ssid_buff!="" && wifi_password_buff!=""){

    //WiFi.softAPdisconnect();

    char wifi_ssid[wifi_ssid_buff.length()+1];
    wifi_ssid_buff.toCharArray(wifi_ssid, wifi_ssid_buff.length()+1);

    char wifi_password[wifi_password_buff.length()+1];
    wifi_password_buff.toCharArray(wifi_password, wifi_password_buff.length()+1);



    WiFi.begin(wifi_ssid, wifi_password);

    serverlog("Connecting to ","");
    serverlog(wifi_ssid,"ln");

  while(WiFi.status()!=WL_CONNECTED && wifiretries<wifimaxretries)
    {
    serverlog(".","");
    wifiretries++;
    delay(1000);
    }

    /*//Inform the user whether the timeout has occured, or the ESP8266 is connected to the internet
    if(wifiretries==wifimaxretries)//Timeout has occured
    {
    serverlog("Unable to Connect to ","");
    serverlog(wifi_ssid,"ln");
	//initap();
    }*/

    if(WiFi.status()==WL_CONNECTED)//WiFi has succesfully Connected
    {
    // Connected to WiFi
    serverlog("","ln");
    serverlog("Connected!","ln");
    serverlog("IP address for localnetwork ","");
    serverlog(wifi_ssid,"");
    serverlog(" : ","");
    serverlog(WiFi.localIP().toString().c_str(),"ln");
    serverlog("wifi rssi:","");
    serverlog((String)WiFi.RSSI(),"ln");
	wifi_fail_try = 0;
	  WiFi.softAPdisconnect();
    }
    else{
		serverlog("Unable to Connect to ","");
		serverlog(wifi_ssid,"ln");
	}

  wifiretries = 0;

   }


}
}

String sendhttp(String httpRequestData, String httpType, String serverPath){

	if(WiFi.status()== WL_CONNECTED && mode_apsta=="apsta"){
      /**/
      HTTPClient http;

      //String serverPath = serverName + "?sid=1";

      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      int httpResponseCode = 0;
        String payload = "{}";
      // Send HTTP GET request
      if (httpType=="httpget")
       httpResponseCode = http.GET();
      else if (httpType=="httppost")
      {
       http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
       httpResponseCode = http.POST(httpRequestData);
      }
      else if (httpType=="httpjson")
      {
       http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
       httpResponseCode = http.POST(httpRequestData);

      }


      if (httpResponseCode>0) {
        serverlog("HTTP Response code: ","");
        serverlog((String)httpResponseCode,"ln");
        payload = http.getString();
        serverlog(payload,"ln");
      }
      else {
        serverlog("Error code: ","");
        serverlog((String)httpResponseCode,"ln");
      }
      // Free resources
      http.end();

  return payload;
}
else{
	return "";
}
}

String checkncfj(){
  //cfjres="{}";
  if (cfjres.substring(0,8) == "{\"pdate\""){
    serverlog("cfj download success","ln");
    return "1";
  }
  return "0";
}

//SEND receivepacket TO SERVER--------------------------------------------
void receivepacketsend(){
  String receivepacket_trim = LedPrepareResult("trim");
  String receivepacket_res = LedPrepareResult("res");

   serverlog("receivepacket:"+receivepacket_trim+"  receivepacket_before:"+receivepacket_before,"ln");
  if(pmaster=="sv" && receivepacket_before!=receivepacket_trim){
    cfjres = "";
    String serverName_process1 = serverName_process + "?procres="+receivepacket_trim+"&pid=" + pid;
    cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process1);
    receivepacket_before = receivepacket_trim;
     serverlog("receivepacketsend success","ln");
  }

}


void initserver(){

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /","ln");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/parameters", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /parameters","ln");
    request->send(SPIFFS, "/parameters.html", String(), false, processor);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /settings","ln");
    request->send(SPIFFS, "/settings.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /style.css","ln");
    request->send(SPIFFS, "/style.css", "text/css");
  });

    // Route to load style.css file
  server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /js.js","ln");
    request->send(SPIFFS, "/js.js", "text/javascript");
  });

    // Route to load style.css file
  server.on("/getledconfig", HTTP_GET, [](AsyncWebServerRequest *request){
    serverlog("HTTP_GET /getledconfig","ln");
    request->send(SPIFFS, "/ledconfig.json", "text/plain");
  });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/sendparam", HTTP_POST, [](AsyncWebServerRequest *request){
          serverlog("HTTP_POST /sendparam","ln");
        String param = "";
        if (request->hasParam(PARAM_params[0], true)) {
          for(int i=0;i<48;i++)
            {
              parameters[i] = request->getParam(PARAM_params[i], true)->value();
              serverlog(parameters[i],"");
            }
        serverlog("","ln");

			change_source ='a'; //change made from AP
            WriteLedConfig(String("m"));
            //SendSerialProcStart();

            //Serial.println(parameters[0]);

        } else {
            //message = "No message sent";
        }
             //Serial.println("receivedprices:"+prices);

    request->send(200, "text/plain", "{\"res\":\"paramok\"}");
    //request->send(200, "text/plain", "{\"35\":\"csrok\"}");
    });

    //espsettings_arr
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/sendsett", HTTP_POST, [](AsyncWebServerRequest *request){
          serverlog("HTTP_POST /sendsett","ln");

          int paramsNr = request->params();

        if (paramsNr>0) {

          int z=0;
          for(int i=0;i<paramsNr;i++){

               AsyncWebParameter* p = request->getParam(i);
               serverlog(p->name(),"");
               serverlog(":","");
               serverlog(p->value(),"");
                serverlog("  ","");


           if(p->name()=="gstationid"){
		   }
		   else if(p->name()=="sid"){
			   espsettings_sid = p->value();
		   }
           else{
               espsettings[z].key = p->name();
               espsettings[z].value = p->value();
               z+=1;
          }
          }

/*        Serial.println("x");
for (int x = 0; x <= sizeof(espsettings)/sizeof(espsettings[0]); x++) {
               Serial.print(espsettings[x].key);
               Serial.print(":");
               Serial.println(espsettings[x].value);
  }
*/
        serverlog("","ln");

			change_source ='a'; //change made from AP
           WriteLedConfig(String("s"));
          request->send(200, "text/plain", "{\"res\":\"paramok\"}");
           esprestart();

            //SendSerialProcStart();

            //Serial.println(parameters[0]);
            //prices = StringDeleteChar(prices, '.');

        } else {
                request->send(200, "text/plain", "{\"res\":\"0\"}");
        }
             //Serial.println("receivedprices:"+prices);

    //request->send(200, "text/plain", "{\"35\":\"csrok\"}");
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/sendserial", HTTP_POST, [](AsyncWebServerRequest *request){
        serverlog("HTTP_POST /sendserial","ln");
        //String message;
        String price = "";

        //prices="";
        if (request->hasParam(PARAM_leds[0], true)) {
			for(int i=0; i<18;i++){
			if (request->hasParam(PARAM_leds[i], true))
				price_arr[i] = request->getParam(PARAM_leds[i], true)->value();

			}
           /* price = request->getParam(PARAM_leds[0], true)->value();price_arr[0] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[1], true)->value();price_arr[1] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[2], true)->value();price_arr[2] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[3], true)->value();price_arr[3] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[4], true)->value();price_arr[4] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[5], true)->value();price_arr[5] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[6], true)->value();price_arr[6] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[7], true)->value();price_arr[7] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[8], true)->value();price_arr[8] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[9], true)->value();price_arr[9] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[10], true)->value();price_arr[10] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[11], true)->value();price_arr[11] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[16], true)->value();price_arr[16] = price;//prices+=price+":";
            price = request->getParam(PARAM_leds[17], true)->value();price_arr[17] = price;//prices+=price+":";*/
            WriteLedConfig(String("p"));

			if(is_modbus_hmi=='h')
				SendSerialProcStart();
			else if(is_modbus_hmi=='m')
			    modbustask = 's';

			change_source ='a'; //change made from AP

            //Serial.println(parameters[0]);
            //prices = StringDeleteChar(prices, '.');

        } else {
            //message = "No message sent";
        }
             //Serial.println("receivedprices:"+prices);

    request->send(200, "text/plain", "{\"res\":\"ok\"}");
    //request->send(200, "text/plain", "{\"35\":\"csrok\"}");
    });

   // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/checkserial", HTTP_GET, [](AsyncWebServerRequest *request){
          serverlog("HTTP_GET /checkserial","");
         String ledmessage = LedPrepareResult();
         serverlog(ledmessage,"ln");

    request->send(200, "text/plain", ledmessage);
    //request->send(200, "text/plain", "{\"35\":\"csrok\"}");
    });


  // Start server
  server.begin();

}

//MODBUS CRC16--------------------------------------------
static uint16_t MODBUS_CRC16_v1( const unsigned char *buf, unsigned int len )
{
	uint16_t crc = 0xFFFF;
	unsigned int i = 0;
	char bit = 0;

	for( i = 0; i < len; i++ )
	{
		crc ^= buf[i];

		for( bit = 0; bit < 8; bit++ )
		{
			if( crc & 0x0001 )
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	return  (crc>>8) | (crc<<8);
}


/*--------MODBUSTASKSTART--------------------------------------------*/

void ModbusTaskStart(){
	if(modbustask=='s'){ // if started first
		 for(int i = 0; i < 24; i++)
		{
		 modbusPTres[i]='0';
		}
		modbustask = 'f';
	}
	if(modbustask=='f')// ESP SEND SELF
		ModbusTaskSend_Self();
	if(modbustask=='r')// SEND WITH STABLE REPEATER
		ModbusTaskSend_RP();
	if(modbustask=='t')// SEND WITH PT REPEATER
		ModbusTaskSend_RPPT();
}


/*--------MODBUSTASKSTART--------------------------------------------*/

void ModbusTaskSend_Self(){
	ModbusTaskSend();
	if(change_source == 's')
	receivepacketsend();
}

/*--------ModbusTaskSend_RP--------------------------------------------*/

void ModbusTaskSend_RP(){
}

/*--------ModbusTaskSend_RPPT--------------------------------------------*/

void ModbusTaskSend_RPPT(){
}

/*--------ModbusTaskSend_RPPT--------------------------------------------*/

char CheckSerialRes(){
	if(serialbufferarr[0]!=0){ //if first byte is not 0 , continue
		if(serialbufferarr[8]==0){ //if 8. byte is 0 , it is 8 byte
			uint16_t crcr = MODBUS_CRC16_v1(serialbufferarr,sizeof(serialbufferarr)-51);
			//Serial.println(crcr);
			//Serial.println(highByte(crcr),HEX);
			//Serial.println(lowByte(crcr),HEX);
			if(highByte(crcr)==serialbufferarr[6] && lowByte(crcr)==serialbufferarr[7])
				return '1';

		}
		else{ //if 8. byte is not 0 , it is 57 byte
			return 'n';

		}
	}
	return 'n';
}

char  CheckModbusTask(){
 for(int k = 0; k < parameters[2].toInt(); k++){
	 if(modbusPTres[k]!='1'){
		 return '0';
	 }

 }
		modbustask = 'x';
		return '1';

}

/*--------MODBUSTASKSEND---------------------------------------------------*/

String ModbusTaskSend(){

	//modbus routine started

//byte CardNumber[]={0x24,0x10,0x00,0x00,0x00,0x18,0x30,0x03,0x09,0x03,0x78,0x00,0x6f,0x00,0xde,0x01,0x4d,0x01,0xbc,0x02,0x2b,0x02,0x9a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x34,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0xf8,0xff};
pt_startingaddr=parameters[1];
pt_quantity=parameters[2];  //Serial.println(pt_quantity);
//pt_quantity=1;

//String waitForReply="";
//String serialtrynum="";
unsigned char vals[58];
byte  valsbyte[58];
int rowb=0;

String price_arr2[24]={"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};
for (int i = 0; i < 24; i++) {
     price_arr2[i]=StringDeleteChar(price_arr[i], '.');
}

 for(int i = 0; i < 58; i++)
{
 valsbyte[i]=0;
}

for(int tryn=0;tryn<serialtrynum;tryn++)
for(int m=0;m<pt_quantity.toInt();m++)
	if(modbusPTres[m]=='0') //if not ok
{
	rowb=0;

msg_str = "SerialTry: " + String(tryn+1) + "-" + String(m+1);
serverlog(msg_str,"ln");

valsbyte[rowb++]=(pt_startingaddr.toInt())+m;//CardNumber[0]=(pt_startingaddr.toInt())+m;
valsbyte[rowb++]=modbus_function.toInt();
valsbyte[rowb++]=0;
valsbyte[rowb++]=modbus_startingaddr.toInt();
valsbyte[rowb++]=0;
valsbyte[rowb++]=modbus_quantity.toInt();
valsbyte[rowb++]=modbus_bytecount.toInt();


  //transmitpacket = "";
  //int ledcount=36;
  //String lastpart="170003";
  //String pricedelm="";
  //String paramdelm="";
  //int priceleftc = 0;
  //int priceleftctot = 36;

  //"0350303111222333444555666777888999103215456170003";
//035 03 03 -111 222-333  444-555 666-777 888-999  103-215 456-170-003

//pricedelm = StringDeleteChar(prices, '.');    Serial.println(pricedelm); //111:222:333:444:555:666:777:888:
//pricedelm = StringDeleteChar(pricedelm, ':');
//pricedelm = pricedelm.substring(0, pricedelm.length() - 1); //Serial.println(pricedelm);


/*
int a=0;
for (int i = 0; i < pricedelm.length(); i++) {
  if (pricedelm.substring(i, i+1) == ":") {
    //firstVal = input.substring(0, i).toInt();
     //Serial.println(prices.substring(i-4, i));
     price_arr[a]=pricedelm.substring(i-3, i);Serial.println(price_arr[a]);
     a++;
    //break;
  }
}  */



valsbyte[rowb++]=highByte(price_arr2[6].toInt());
valsbyte[rowb++]=lowByte(price_arr2[6].toInt());
valsbyte[rowb++]=highByte(price_arr2[7].toInt());
valsbyte[rowb++]=lowByte(price_arr2[7].toInt());
valsbyte[rowb++]=highByte(price_arr2[0].toInt());
valsbyte[rowb++]=lowByte(price_arr2[0].toInt());
valsbyte[rowb++]=highByte(price_arr2[1].toInt());
valsbyte[rowb++]=lowByte(price_arr2[1].toInt());
valsbyte[rowb++]=highByte(price_arr2[2].toInt());
valsbyte[rowb++]=lowByte(price_arr2[2].toInt());
valsbyte[rowb++]=highByte(price_arr2[3].toInt());
valsbyte[rowb++]=lowByte(price_arr2[3].toInt());
valsbyte[rowb++]=highByte(price_arr2[4].toInt());
valsbyte[rowb++]=lowByte(price_arr2[4].toInt());
valsbyte[rowb++]=highByte(price_arr2[5].toInt());
valsbyte[rowb++]=lowByte(price_arr2[5].toInt());
valsbyte[rowb++]=highByte(price_arr2[8].toInt());
valsbyte[rowb++]=lowByte(price_arr2[8].toInt());
valsbyte[rowb++]=highByte(price_arr2[9].toInt());
valsbyte[rowb++]=lowByte(price_arr2[9].toInt());
valsbyte[rowb++]=highByte(price_arr2[10].toInt());
valsbyte[rowb++]=lowByte(price_arr2[10].toInt());
valsbyte[rowb++]=highByte(price_arr2[11].toInt());
valsbyte[rowb++]=lowByte(price_arr2[11].toInt());

valsbyte[rowb++]=0;
valsbyte[rowb++]=0;
valsbyte[rowb++]=0;
valsbyte[rowb++]=0;

valsbyte[rowb++]=highByte((((price_arr[17].toInt())-1)*200)+700);
valsbyte[rowb++]=lowByte((((price_arr[17].toInt())-1)*200)+700);
valsbyte[rowb++]=highByte((price_arr[16].toInt())-1);
valsbyte[rowb++]=lowByte((price_arr[16].toInt())-1);

uint16_t crcr = MODBUS_CRC16_v1(valsbyte,sizeof(valsbyte)-3);

valsbyte[55]=highByte(crcr);
valsbyte[56]=lowByte(crcr);
valsbyte[57]=0xff;

//Serial.println(MODBUS_CRC16_v1(valsbyte,STR_LEN-3), HEX);
    //serverlog(transmitpacket,"ln");

 msg_str ="";
 for(int k = 0; k < 58; k++)
{
	msg_str+=String(valsbyte[k], HEX) + "-";
// Serial.print(valsbyte[k],HEX);
 //Serial.print("-");
}
 //Serial.println("");
 serverlog(msg_str,"ln");

/*  for(int k = 0; k < 58; k++)
{
 Serial.print(CardNumber[k],HEX);
 Serial.print("-");
}
 Serial.println("");
*/

Serial1.write(valsbyte, 58);

delay(170); //100ms is not enough

ReadSerialBuffer();

 msg_str ="";
 for(int k = 0; k < 57; k++)
		msg_str+=String(serialbufferarr[k], HEX) + "-";
  serverlog(msg_str,"ln");


if(CheckSerialRes()=='1'){
		modbusPTres[m]='1';
}

 msg_str ="";
 for(int k = 0; k < 24; k++)
		msg_str+=String(modbusPTres[k]) + "-";
  serverlog(msg_str,"ln");

}

CheckModbusTask();
	//modbustask = 'x';


return "";
}


/*--------ReadSerialBuffer-------------------*/

void ReadSerialBuffer(){

 for(int k = 0; k < 57; k++)
{
  serialbufferarr[k] = 0;

}

int i=0;

  if (Serial1.available()){
   while (Serial1.available() > 0)
  {
       //serverlog("  hmic:","");
       //serverlog((String)Serial1.available(),"");
    // get the new byte:
    int inChar = (int)Serial1.read();
	  //Serial.println(inChar,HEX);
	  serialbufferarr[i] = inChar;
	  i++;
	  if(i==56) break;


  }

         //serverlog("receivepacketwoutfilter:","");
         //serverlog(receivepacket,"ln");
		 //receivepacketsend();
  }


}

//MODBUS TEST--------------------------------------------
String modbustest(){
	     serverlog("sendingserialtest:","ln");
		   const unsigned char dt[] = {0x24,0x10,0x00,0x00,0x00,0x18,0x30,0x03,0x09,0x03,0x78,0x00,0x6f,0x00,0xde,0x01,0x4d,0x01,0xbc,0x02,0x2b,0x02,0x9a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x34,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		    unsigned int STR_LEN = sizeof(dt);

Serial.println(MODBUS_CRC16_v1(dt,STR_LEN), HEX);
	//flag1=1;
	transmitpacket="2510000000183001b901ba006f007000dd00de014b014c0000000000000000000000000834000900000000000000000000000000000000832fff";
   // byte cmd[9] = {0x2b, 0x00, 0x01, 0x00, 0x01, 0xa1, 0x00, 0x00, 0x7e};

byte CardNumber[]={0x24,0x10,0x00,0x00,0x00,0x18,0x30,0x01,0xb9,0x01,0xba,0x00,0x6f,0x00,0x70,0x00,0xdd,0x00,0xde,0x01,0x4b,0x01,0x4c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x34,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x94,0xff};
//int CardNumber[]={6,16,0,0,0,24,48,1,185,1,186,0,111,0,112,0,221,0,222,1,75,1,76,0,0,0,0,0,0,0,0,0,0,0,0,8,52,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,148};
byte CardNumberByte[57];
 for(int i = 0; i < 58; i++)
{
 Serial.print(CardNumber[i],HEX);
}
 Serial.println("");
	 //Serial1.print(transmitpacket);
Serial.write(CardNumber, 58);
Serial1.write(CardNumber, 58);

	return "";
}


void setup(){

  //Hardware Serial of ESP32
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    serverlog("An Error has occurred while mounting SPIFFS","ln");
    return;
  }

  //CONFIG OKU
  WriteLedConfig(String("x"));

  // Serial port for debugging purposes
  if(devprod=="d" || devprod=="D")
	Serial.begin(115200);
  delay(100);

  /*if(Serial)
    serverlog("system started","ln");
  else
    devprod="p";  */

  //WIFI BASLAT
  //WiFi.onEvent(OnWiFiEvent);
  initwifi();
  initap();

  //WiFi.setAutoReconnect(false);
  //serverlog("getAutoReconnect:","");
  //serverlog(String(WiFi.getAutoReconnect()),"ln");


  initserver();
	//modbustest();
}

void loop(){

/*---------CHECK WIFI AND POOl SERVER IF apsta---------------*/

if(mode_apsta=="apsta"){


/*---------CHECK WIFI STATUS---------------*/

if ((millis() - cTime_wificonnect >= intv_wificonnect)) {
    cTime_wificonnect=millis();
    serverlog("Checking Wifi status.................","ln");

    if(WiFi.status()!= WL_CONNECTED){
      //WIFI BASLAT
   serverlog("Restart wifi before Pooling Server...","ln");
	//if(wifi_fail_try<2){
		//wifi_fail_try++;
		serverlog("wifi try:","");
		//serverlog((String)wifi_fail_try,"ln");
		initwifi();
	//}
	//else{
		//initap();
		//wifi_fail_try = 0;
	//}
    }
    else{
     serverlog("wifi is already connected.................","ln");
    }
}



/*--------POOL SERVER-------------------*/

if ((millis() - cTime_serverpool >= intv_serverpool)) {
  serverlog((String)millis(),"");
  cTime_serverpool=millis();

   /* if(WiFi.status()!= WL_CONNECTED){
      //WIFI BASLAT
   serverlog("Restart wifi before Pooling Server...","ln");
      initwifi();
    }*/

    if(WiFi.status()== WL_CONNECTED){

  serverlog("WİFİ is up, Pooling Server...","ln");

poolres = "";
poolres = sendhttp("", "httpget", serverName_poolp);
//server change start detected
if(poolres=="1")
{
  cfjres = "";
  String serverName_process1 = serverName_process + "?getcfj=t&sid=" + sid + "&pstart=t";
   cfjres = sendhttp("", "httpget", serverName_process1);
     serverlog("cfj:","ln");
     serverlog(cfjres,"ln");
    if(cfjres!="" && checkncfj()=="1")
    {
      //Save cfj downloaded from server
      FileWriteF(cfjres,"/ledconfig.json");
      //serial process start
	  change_source ='s'; //change made from server
      WriteLedConfig(String("x"));
      if(ptype=="pr") //server cfj price change process
      {
        receivepacket_before = "000000000000000000000000N";
         //SendSerialProcStart();
		 modbustask = 's';
        //Pool result prepare
      }
      else{
          String serverName_process1 = serverName_process + "?procres="+"t"+"&pid=" + pid;
          String receivepacket_res = "{\"res\":\"paramok\"}";
          cfjres = sendhttp(receivepacket_res, "httpjson", serverName_process1);
         }
    }

}

    }
    else {
      serverlog("WiFi Disconnected while pooling","ln");
    }
}

}


/*---------TEST MODBUS SERIAL EVRY 1 SEC---------------*/

if ((millis() - cTime_serverpool >= intv_serverpool)) {
  cTime_serverpool=millis();
   // modbustask = 's';
	//ModbusTaskStart();
}


/*---------RESTART NORMAL---------------*/

if ((millis() - cTime_restartnormal >= intv_restartnormal)) {
    cTime_restartnormal=millis();
    serverlog("Restart Normally..........","ln");

	esprestart();
}


 /*--------SEND SERIAL SIIMULATION---------
  if(millis() - currentTime >= 10000) {
  if (flag1 == 1){
    //currentlyOn=false;
    currentTime=millis(); // Reset timer
    receivepacket = "111000000000000000000000O";
    Serial.println("simulation:" + receivepacket);
    receivepacketsend();
    flag1 = 0;
  }
  }
*/

/*---------SEND SERIAL DATA------------

  if (flag1 == 1)  {
	serverlog("Sendingserials...:","");
	serverlog(transmitpacket,"ln");
    receivepacket = "";
    //webden send butonuna basınca çalışacak kod
    Serial1.println(transmitpacket);
    flag1 = 0;
  }
*/

/*---------CHECK AND SEND MODBUS SERIAL DATA------------*/

if ((millis() - cTime_modbuschecksend >= intv_modbuschecksend) ||  modbustask == 's') {
    cTime_modbuschecksend=millis();
  if (modbustask != 'x')  {
	serverlog("Sendingmodbusserials...:","");
	ModbusTaskStart();
  }
}

/*-------CHECK SERIAL DATA------------*/
/*
  if (Serial1.available()){
   while (Serial1.available() > 0)
  {
       //serverlog("  hmic:","");
       //serverlog((String)Serial1.available(),"");
    // get the new byte:
    int inChar = (int)Serial1.read();
	  Serial.println(inChar,HEX);

    if (inChar == '\n') {
      serverlog("","ln");
      serverlog("hmi:","");
      serverlog(receivepacket2,"ln");
      receivepacket = receivepacket2;

      receivepacket2 = "";
    }
    else
    {
      //serverlog("  hmichar:","");
      //serverlog((String)inChar,"");


      receivepacket2 += inChar;
    }
  }

         //serverlog("receivepacketwoutfilter:","");
         //serverlog(receivepacket,"ln");
		 //receivepacketsend();
  }
*/


}

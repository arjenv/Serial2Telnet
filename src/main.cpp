/*********************************************************
 * 
 * Arjen Vellekoop
 * 29 september 2025
 * 
 * Serial2Telnet
 * 
 * Wireless Serial monitor via Telnet
 * 
 * Starts initially with a WifiManager @ 192.168.4.1
 * Set your IP, password and SSID 
 * 
 * From telnet try 'H' for help
 * 
 * 
 * 
 * 
 *  
 ***********************************************************/

#include "main.h"

//Global variables
char     ch;

AsyncWebServer server(80);
TeePrint tee(Serial, TelnetStream); // stream print(f) statements to both Serial as Telnet.


char      static_ip[16] = "192.168.1.5";
char      static_gw[16] = "192.168.1.254";
char      static_sn[16] = "255.255.255.0";
char      ssid[30];
char      password[30];
uint32_t  Baudrate = 115200;
bool      SetupFinished;

/***********  SETUP       ******************/
void setup() {

  LittleFS.begin();
  ReadSettings(); // get baudrate

  
  Serial.begin(Baudrate);
  server.begin();
  TelnetStream.begin();  
  Serial.printf("\n\nVersion: %s\n", Version);
  
  //******************  wifi setup  *******************

  //read configuration from FS json

  if (LittleFS.begin()) {
    if (Readconfigfile()) { // does /config.json exists?
      // yes try and connect
      if (!initWiFi()) {
        Serial.printf("Failed to init Wifi, starting WiFi Manager as Access Point (AP).\nConnect to 192.168.4.1\n");
        SetupWifiManager();
      }
    }  
    else {
      SetupWifiManager();
    }
  }
  else {
    Serial.printf("Could not read Flash..  Abort\n");
    exit(1);
  } 

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  Serial.printf("Baudrate %i\n", Baudrate);


// *********************end wifi setup ********************


  InitArduinoOTA();

  // Route to load style.css file
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/css/style.css", "text/css");
  });
  Serial.printf("End Setup. Starting loop\n\n");

}

void loop () {

  ArduinoOTA.handle();
  HandleTelnet();

  while (Serial.available() > 0) {  //wait for data 
    ch = Serial.read();
    TelnetStream.print(ch);  //Send data received from software serial to hardware serial    
    Serial.print(ch);
  }
}

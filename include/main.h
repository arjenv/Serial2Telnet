#ifndef MYMAIN_H
#define MYMAIN_H

/* include all headers whose types you need */

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <TelnetStream.h>
#include <ArduinoOTA.h>
#include "TeePrint.h" 

/* define macros */

#define Version         "Serial2Telnet V1.0 29 September 2025. A.R.Vellekoop"
#define httpPort        80
#define FLASHBUTTON     0 //GPIO0

#define PO              1 // Debug PO=printer output

extern char                 ssid[30];
extern char                 password[30];
extern char                 static_ip[16];
extern char                 static_gw[16];
extern char                 static_sn[16];
extern bool                 SetupFinished;
extern uint32_t             Baudrate;
extern                      AsyncWebServer server;

/* declare global functions */

void        InitArduinoOTA(void);
void        HandleTelnet(void);
bool        initWiFi(void);
bool        Readconfigfile(void);
bool        SetupWifiManager(void);
bool        ReadSettings(void);  // get the variables from file.
void        StoreSettings(void);

#endif
/* Handle Telnet requests
*
*   Arjen Vellekoop
*
*   May 22, 2024
*
*   R: Reset
*   C: Restart Wifimanager
*   F: FreeHeap
*   V: Version
*   W: Wifi signal Strength
*   B: Set Baudrate (e.g. 'B9600'")
*   L: List latest info
*
*
*/

//#include <Arduino.h>
//#include <TelnetStream.h>       // Version 1.2.5 latest sept 17, 2023 //  https://github.com/jandrassy/TelnetStream
#include "main.h"

void   HandleTelnet(void) {

  char    input[30];
  int     newBaudrate;
  int     limitbaudrate[] = {50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
  uint8_t i;
  bool    foundBaudrate;

  switch (toupper(TelnetStream.read())) { // commands recognised by Telnet
    case 'R':
    TelnetStream.stop();
    delay(100);
    ESP.reset();
      break;
    case 'C':
      TelnetStream.println("Reset password in config file\nLogin via 192.168.4.1 and set parameters again\n");
	    tee.printf("WiFi Disconnecting: ");
      delay(2000);
      WiFi.disconnect();
  	  while (WiFi.status() == WL_CONNECTED) {
    	  delay(500);
    	  tee.print(".");
  	  }
  	  Serial.printf("\nWiFi disconnected\n");
      SetupWifiManager();
      break;
    case 'F':
      TelnetStream.printf("Freeheap: %d\n\r", ESP.getFreeHeap());
      break;
    case 'V':
      TelnetStream.printf("Version: %s\n\r", Version);
      break;
    case 'L':
      TelnetStream.println("local ip");
      TelnetStream.println(WiFi.localIP());
      TelnetStream.println(WiFi.gatewayIP());
      TelnetStream.println(WiFi.subnetMask());
      TelnetStream.printf("Baudrate %i\n", Baudrate);
      break;
    case 'B': // change Baudrate
      newBaudrate = TelnetStream.readBytesUntil('\n', input, 10);
      newBaudrate = atoi(input);

      foundBaudrate = false;
      for (i = 0; i<17 && foundBaudrate==false; i++) {
        if  (newBaudrate == limitbaudrate[i]) foundBaudrate = true;
      }
      if (foundBaudrate) {
        Baudrate = newBaudrate;
        StoreSettings();
        tee.printf("Set new Baudrate: %i\n", newBaudrate);
        Serial.end();
        Serial.begin(Baudrate);
      }
      else {
        tee.printf("Not a valid Baudrate. Value not set %i\n", newBaudrate);
      }
    break;
    case 'W': 
      TelnetStream.printf("RSSI: %i dB\n", WiFi.RSSI());
      break;
    case 'H':
      TelnetStream.println("R: Reset");
      TelnetStream.println("C: Restart Wifimanager");
      TelnetStream.println("F: FreeHeap");
      TelnetStream.println("V: Version");
      TelnetStream.println("W: Wifi signal Strength");
      TelnetStream.println("B: Set Baudrate (e.g. 'B9600'");
      TelnetStream.println("L: List latest info");
      break;
    default:
      while(TelnetStream.available())
        TelnetStream.read(); // empty buffer. Required because at startup the buffer is filled with garbage
                            // and after a single letter command there is still a return and Newline.
  } //end switch
}

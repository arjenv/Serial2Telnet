September 2025

Serial2Telnet

For ESP8266
Might work with ESP32 (not tested)

Program for wireless RS232 to Telnet.

When your project is too far away from your PC/Laptop
Connect a separate NodeMCU or equivalent to the output of your project.

GND -- GND
SerialTX -- SerialRX

Power this externally with a battery or PSU or from your project (mind the
5V/3V3)

It start with a WifiManager. Connect your phone/tablet to the AccessPoint (AP)
Open up a browser @ 192.168.4.1
Enter your settings in the Form. Hit Save

Then open a telnet session with the IP address you entered before.

Type 'H' for some help

Change Baudrate with (e.g.) 'B9600' or 'B115200'

Debug your project over WiFi.


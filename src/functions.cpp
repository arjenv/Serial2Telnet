#include "main.h"

bool Readconfigfile() {

  File configFile;
  size_t ConfigFile_size;
  JsonDocument json;

  if (LittleFS.exists("/config.json")) { //file exists, then reading and loading
    Serial.printf("reading config file\n");
    configFile = LittleFS.open("/config.json", "r");
    if (configFile) {
      Serial.printf("opened config file\n");
      ConfigFile_size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[ConfigFile_size]);

      configFile.readBytes(buf.get(), ConfigFile_size);
      auto deserializeError = deserializeJson(json, buf.get());
      serializeJson(json, Serial);
      if (!deserializeError) {
        Serial.printf("\nparsed json\n");
        if(json["ip"]) {
          Serial.printf("setting custom ip from config\n");
          strncpy(static_ip, json["ip"], 16);
          strncpy(static_gw, json["gateway"], 16);
          strncpy(static_sn, json["subnet"], 16);
          strncpy(ssid, json["ssid"], 30);
          strncpy(password, json["pass"], 30);
          Serial.printf("%s\n", static_ip);
        } 
        else {
          Serial.printf("no custom ip in config\n");
          return(false);
        }
      } 
      else {
        Serial.printf("deserialization error\n");
        return(false);
      }
    }
    else {
      Serial.printf("Failed to open /config.json\n");
      return(false);
    }
  }
  else {
    Serial.printf("/config.json does not exist\n");
    return(false);
  }
  //end read. success!
  Serial.printf("%s\n", static_ip);
  return(true);
}

bool   ReadSettings(void) {  // get the variables from file.
                              // Since this function is called before Serial.begin
                              // No use to Serial.print() stuff
  File      settingsFile;
  size_t    SettingsFileSize;
  JsonDocument json;


  if (LittleFS.exists("/settings.json")) { //file exists, then reading and loading
    //Serial.printf("reading settings file\n");
    settingsFile = LittleFS.open("/settings.json", "r");
    if (settingsFile) {
      //Serial.printf("opened settings.json file\n");
      SettingsFileSize = settingsFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[SettingsFileSize]);

      settingsFile.readBytes(buf.get(), SettingsFileSize);
      auto deserializeError = deserializeJson(json, buf.get());
      //serializeJson(json, Serial);
      //Serial.println();
      if (!deserializeError) {
        //Serial.printf("\nparsed settings.json\n");
        Baudrate = json["Baudrate"]; 
      } 
      else {
      //    Serial.printf("deserialization error\n");
        return(false);
      }
    }
    else {
    //  Serial.printf("Failed to open /settings.json\n");
      return(false);
    }
  }
  else {
    //Serial.printf("/settings.json does not exist\n");
    return(false);
  }
  //end read. success!
  //Serial.printf("Successfully read settings.json\n");
  return(true);
}

void StoreSettings(void) {
  //save the custom variables to FS
  tee.println("saving settings.json");
  JsonDocument json;
  json["Baudrate"] = Baudrate;

  File configFile = LittleFS.open("/settings.json", "w");
  if (!configFile) {
    tee.println("failed to open settings file for writing");
  }

  serializeJson(json, tee);
  serializeJson(json, configFile);
  tee.println();
  configFile.close();
  //end save
}



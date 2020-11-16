#ifndef config_schema_h
#define config_schema_h

#include <Arduino.h>

boolean increase = true;
boolean decrease = false;


struct Config {
  double desired_temp      = 25;
  double temp_offset       = 0.5;
  int target_intensity[7]  = { 0,0,0,0,0,0,0 };
  int current_intensity[7] = { 0,0,0,0,0,0,0 };
  int daynight             = 0;
};

Config config;


/** Config reader handler
  * Checks if we have a config.json in flash store 
  * if not, we use default values defined in header
  * if we have a valid config it reads it and feeds the values
  * into used variables that makes the controller tick
  */
void readConfig() {
  if (LittleFS.begin()) {
    if (LittleFS.exists("/config.json")) {
      //file exists, reading and loading
      printOnSerial("reading config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(1024);
        deserializeJson(json, buf.get());
        config.desired_temp = json["desired_temp"];
        config.temp_offset = json["temp_offset"];
        config.target_intensity[0] = json["desired_cold"];
        config.target_intensity[1] = json["desired_cold"];
        config.target_intensity[2] = json["desired_neutral"];
        config.target_intensity[3] = json["desired_neutral"];
        config.target_intensity[4] = json["desired_warm"];
        config.target_intensity[5] = json["desired_warm"];
        config.target_intensity[6] = json["desired_blue"];
        config.daynight = json["daynight"];
      }
    }
  } else {
    printOnSerial("failed to mount FS");
  }
}

/** Config save handler
  * Saves configuration into config.json file in flash
  */
void saveConfig(JsonDocument& json) {
  printOnSerial("saving config");
  serializeJson(json, Serial);
  
  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    printOnSerial("failed to open config file for writing");
  }

  serializeJson(json, configFile);
  //json.printTo(configFile);
  configFile.close();
  readConfig();
}

#endif

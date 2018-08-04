#ifndef wifi_setup_h
#define wifi_setup_h

#include <Arduino.h>

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include "html.h"

#include "config_schema.h" 

//init webserver
ESP8266WebServer server ( 80 );


String indexPage(){
  return main_page;
}

String settingsPage(){
  if ( server.hasArg("submit") ) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) != "submit") {
          json[server.argName(i)] = server.arg(i);
      }
    }
    saveConfig(json);
  }
  return settings_page;
}

String jsonApiHandler(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  JsonObject& fishtankdata = json.createNestedObject("fishtankdata");
    fishtankdata["temperature"] = tempInt;
    fishtankdata["fan_pwm"] = fan_pwm;
  JsonObject& fishtanksettings = json.createNestedObject("fishtanksettings");
    fishtanksettings["desired_temp"] = config.desired_temp;
    fishtanksettings["temp_offset"] = config.temp_offset;

  String output;
  json.printTo(output);
  return output;
}

// Handle request for root document ("/")
void rootHandler(){ 
  server.send ( 200, "text/html", indexPage() );
}

// Handle request for settings document ("/settings")
void SettingsHandler(){ 
  server.send ( 200, "text/html", settingsPage() );
}

// Handle request for jsonapi document ("/fishtank.json")
void apiHandler(){ 
  server.send ( 200, "application/json", jsonApiHandler() );
}
#endif

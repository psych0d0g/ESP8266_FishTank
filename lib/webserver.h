#ifndef webserver_h
#define webserver_h

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
    DynamicJsonDocument json(1024);
    //JsonObject json = doc.createObject();
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) != "submit") {
          json[server.argName(i)] = server.arg(i);
      }
    }
    saveConfig(json);
  }
  return settings_page;
}

String settingsScriptPage(){
  return settings_script;
}

String indexScriptPage(){
  return index_script;
}

String gaugesScriptPage(){
  return gauges_script;
}

String stylesheetPage(){
  return stylesheet;
}

String jsonApiHandler(){
  DynamicJsonDocument json(1024);
  //JsonObject json = doc.createObject();

  JsonObject fishtankdata = json.createNestedObject("fishtankdata");
    fishtankdata["temperature"] = tempInt;
    fishtankdata["level"] = "70";
    fishtankdata["fan_pwm"] = fan_pwm;
  JsonObject fishtanksettings = json.createNestedObject("fishtanksettings");
    fishtanksettings["desired_temp"] = config.desired_temp;
    fishtanksettings["temp_offset"] = config.temp_offset;
    fishtanksettings["desired_cold"] = config.target_intensity[0];
    for (channel=0; channel<=target_intensity_length; channel++) {
      fishtanksettings["current_brightness_"+String(channel)] = config.current_intensity[channel]/40;
    }
    fishtanksettings["desired_neutral"] = config.target_intensity[2];
    fishtanksettings["desired_warm"] = config.target_intensity[4];
    fishtanksettings["desired_blue"] = config.target_intensity[6];
    fishtanksettings["daynight"] = config.daynight;
    fishtanksettings["instant"] = config.instant;

  String output;
  serializeJson(json, output);
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

// Handle request for settings document ("/settings")
void SettingsScriptHandler(){ 
  server.send ( 200, "text/javascript", settingsScriptPage() );
}

// Handle request for settings document ("/settings")
void indexScriptHandler(){ 
  server.send ( 200, "text/javascript", indexScriptPage() );
}

// Handle request for settings document ("/settings")
void gaugesScriptHandler(){ 
  server.send ( 200, "text/javascript", gaugesScriptPage() );
}

// Handle request for settings document ("/settings")
void stylesheetHandler(){ 
  server.send ( 200, "text/css", stylesheetPage() );
}

// Handle request for jsonapi document ("/fishtank.json")
void apiHandler(){ 
  server.send ( 200, "application/json", jsonApiHandler() );
}

void webserverSetup(){
  server.on ( "/", rootHandler );
  server.on ( "/settings", SettingsHandler );
  server.on ( "/js/settings.js", SettingsScriptHandler );
  server.on ( "/js/index.js", indexScriptHandler );
  server.on ( "/js/gauges.js", gaugesScriptHandler );
  server.on ( "/css/style.css", stylesheetHandler );
  server.on ( "/fishtank.json", apiHandler );
  server.begin();
  printOnSerial( "HTTP server started" );
}
#endif

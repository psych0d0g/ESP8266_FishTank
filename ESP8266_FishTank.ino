#include "IOT_fishtank.h"
#include <ArduinoJson.h>
#include <FS.h>
#include "thermistor.h"
#include "HardwareSerial.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <PID_v1.h>

// pin Definitions
#define NTC_PIN A0
#define FAN_PIN D8

// Wifi Setup
#define host "Fishtank"

#define INTERVAL 5000   // time between reads
unsigned long lastRead = 0;

int avgLoop = 5;    //temp measurement loops

//PID parameters. I'm using defaults with quite nice results
double kp=20;   //proportional parameter
double ki=5;   //integral parameter
double kd=1;   //derivative parameter

//Minimum and Maximum PWM command, according fan specs and noise level required
double commandMin = 0;
double commandMax = 250;

double desired_temp, tempInt, maxTdiff, avgInt, tempDiff, command;

//init PID
PID myPID(&tempDiff, &command, &maxTdiff,kp,ki,kd, REVERSE);

boolean first = true;

ESP8266WebServer server ( 80 );


// Thermistor object
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      100000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      97500);         // Value of the series resistor

// Global temperature reading
float temp;

String indexPage(){
  return main_page;
}

String settingsPage(){
  return settings_page;
}

String getData(){
  String page = "{";
  page += "\"fishtankdata\": {";
  page += "  \"temperature\": \"";
  page += tempInt;
  page += "\",";
  page += "  \"fan_pwm\": \"";
  page += command;
  page += "\"";
  page += "}";
  page += "}";
  return page;
}

void handleRoot(){ 
  server.send ( 200, "text/html", indexPage() );
}

void handleSettings(){ 
  server.send ( 200, "text/html", settingsPage() );
}

void handleData(){ 
  server.send ( 200, "application/json", getData() );
}

void handleTempLoop(){
  if (millis() - lastRead >= INTERVAL){  // if INTERVAL has passed
    lastRead = millis(); 

    //this loop is a trick to get a more stable temp, under +5v LM35 is quite unprecise...
    avgInt = 0;
    for(int i=0; i < avgLoop; i++) {
      double tInt = thermistor.read() / 10.0;
      avgInt += tInt;
      delay(100);
    }

    //calculate setpoint (tempDiff)
    tempInt = avgInt / avgLoop;
    tempDiff = tempInt - desired_temp;
    if(tempDiff < 0) {
      tempDiff = 0;
    }
    Serial.print("Temp: ");Serial.println(tempInt);
    //process PID
    myPID.Compute();
    Serial.println(command);
    //apply PID processed command
    analogWrite(FAN_PIN, command);
  }
}

void readConfig() {
    if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          Serial.println("setting custom settings from config");
          desired_temp = json["desired_temp"];
          maxTdiff = json["maxTdiff"];
        } else {
          Serial.println("failed to load json config");
        }
      }
    } else {
      Serial.println("No config file, using defaults");
      Serial.println("desired_temp: 25");
      Serial.println("max temp diffrence: 0.5");
      desired_temp = 25.0;
      maxTdiff = 0.5;
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

void saveConfig() {
  Serial.println("saving config");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

//  json["desired_temp"] = 
//  json["maxTdiff"] = 

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.prettyPrintTo(Serial);
  json.printTo(configFile);
  configFile.close();
}
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  Serial.begin(115200);
  WiFi.hostname( host );
  WiFiManager wifiManager;

  wifiManager.setBreakAfterConfig(true);
  if (!wifiManager.autoConnect("ESP_FishTank")) {
    Serial.println("failed to connect, we will fire up config mode");
    delay(3000);
    wifiManager.startConfigPortal("ESP_FishTank");
    delay(5000);
  }

  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  readConfig();
  /*return index page which is stored in serverIndex */
  server.on ( "/", handleRoot );
  server.on ( "/settings", handleSettings );
  server.on ( "/fishtank.json", handleData);
  server.begin();
  Serial.println ( "HTTP server started" );
  pinMode(FAN_PIN, OUTPUT);
  tempDiff = tempInt - desired_temp;
  if(tempDiff < 0) {
    tempDiff = 0;
  }

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(commandMin, commandMax);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  handleTempLoop();
  server.handleClient();
}

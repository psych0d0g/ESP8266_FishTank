#include "ESP8266_FishTank.h"
#include "thermistor.h"
#include "HardwareSerial.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PID_v1.h>

// pin Definitions
#define NTC_PIN A0
#define FAN_PIN D8

// Wifi Setup
#define host "Fishtank"
#define ssid     "..."       // WiFi SSID
#define password "..."  // WiFi password

// Target temperature you want to have in your tank
double desired_temp = 25.0;

//Setpoint (Maximum difference between measured and desired temperature)
double maxTdiff = 0.5;

//Minimum and Maximum PWM command, according fan specs and noise level required
double commandMin = 0;
double commandMax = 250;

/**
  * You dont usually need to change anything below this line
 */

int avgLoop = 5;    //temp measurement loops

//PID parameters. I'm using defaults with quite nice results
double kp=20;   //proportional parameter
double ki=5;   //integral parameter
double kd=1;   //derivative parameter

#define INTERVAL 5000   // time between reads
unsigned long lastRead = 0;

double tempInt, avgInt, tempDiff, command;

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
  return file1;
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
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  Serial.begin(115200);
  WiFi.hostname( host );
  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
  /*return index page which is stored in serverIndex */
  server.on ( "/", handleRoot );
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


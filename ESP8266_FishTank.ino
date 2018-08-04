#include <ESP8266SSDP.h>
#include <DNSServer.h>
#include <FS.h>
#include <Wire.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
//#include <MQTT.h>

#include "lib/user_config.h"
#include "lib/display.h"
#include "lib/sensors.h"
#include "lib/webserver_setup.h"

//WiFiClient net;
//MQTTClient client; 

/**
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void connectMqtt() {
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  client.subscribe("/fishtank/command/maxtemp");
  client.subscribe("/fishtank/command/maxdiff");

  Serial.print ( "MQTT Connected to: " + String(mqtt_server) );
}

void publishMessage(String topic, String payload) {
  client.publish(topic, payload);
  Serial.println("Published: " + topic + " - " + payload);
}
*/
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

  Serial.println ( "IP address: " + String(WiFi.localIP()) );

 // client.begin(mqtt_server, net);
 // client.onMessage(messageReceived);
  mlx.begin();  
  //connectMqtt();
  readConfig();
  displaySetup();
  /*return index page which is stored in serverIndex */
  server.on ( "/", rootHandler );
  server.on ( "/settings", SettingsHandler );
  server.on ( "/fishtank.json", apiHandler );
  server.begin();
  Serial.println ( "HTTP server started" );



  pinMode(FAN_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  tempDiff = tempInt - config.desired_temp;
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
  handleWaterLevel();
  handleDisplay();
//  client.subscribe("/fishtank/command/maxtemp");
//  client.subscribe("/fishtank/command/maxdiff");
  server.handleClient();
}

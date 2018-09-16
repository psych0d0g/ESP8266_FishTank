#include <ESP8266SSDP.h>
#include <DNSServer.h>
#include <FS.h>
#include <Wire.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

#include "lib/user_config.h"
#include "lib/display.h"
#include "lib/sensors.h"
#include "lib/webserver.h"
#include "lib/led_control.h"
#include "lib/mqtt_handler.h"
#include "lib/time_handler.h"

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
  Wire.begin();
  Wire.setClock(400000);
  setupNTP();
  setupPWM();
  setupSensors();
  readConfig();
  displaySetup();
  webserverSetup();
  setupMqtt();
  /*return index page which is stored in serverIndex */
  pinMode(FAN_PIN, OUTPUT);

}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  loopNTP();
  handleLedState();
  handleTempLoop();
  handleWaterLevel();
  handleDisplay();
  //handleMQTT();
  //Serial.println(currentTime("local"));
  compute(calculateDayOfYear(currentTime("local")));
  server.handleClient();
}

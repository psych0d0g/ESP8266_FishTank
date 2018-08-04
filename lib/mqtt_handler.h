#ifndef mqtt_handler_h
#define mqtt_handler_h

#include <Arduino.h>
#include <MQTT.h>
#include <ESP8266WiFi.h>

WiFiClient net;
MQTTClient client; 

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setupMqtt() {
  client.begin(mqtt_server, net);
  client.onMessage(messageReceived);
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

void handleMQTT(){
  if (millis() - lastReadTemp >= INTERVAL){  // if INTERVAL has passed
    lastReadTemp = millis(); 
    publishMessage("/fishtank/status/temp", String(tempInt));
    publishMessage("/fishtank/status/pwm", String(fan_pwm/250*100));
    publishMessage("/fishtank/status/waterlvl", String(distance));
    client.subscribe("/fishtank/command/maxtemp");
    client.subscribe("/fishtank/command/maxdiff");
  }
}

#endif

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

  client.subscribe("cmnd/"+host+"/maxtemp");
  client.subscribe("cmnd/"+host+"/maxdiff");

  Serial.println ( "MQTT Connected to: " + String(mqtt_server) );
}

void publishMessage(String topic, String payload) {
  client.publish(topic, payload);
  Serial.println("Published: " + topic + " - " + payload);
}

void handleMQTT(){
  if (millis() - lastSendMqtt >= INTERVAL){  // if INTERVAL has passed
    lastSendMqtt = millis(); 
    setupMqtt();
    publishMessage("stat/"+host+"/temp", String(tempInt));
    publishMessage("stat/"+host+"/pwm", String(fan_pwm/250*100));
    publishMessage("stat/"+host+"/waterlvl", String(distance));
    client.subscribe("cmnd/"+host+"/maxtemp");
    client.subscribe("cmnd/"+host+"/maxdiff");
  }
}

#endif

#ifndef mqtt_handler_h
#define mqtt_handler_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient net;
PubSubClient client(net); 


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setupMqtt() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(host.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void handleMQTT(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (millis() - lastSendMqtt >= MQTT_INTERVAL){  // if INTERVAL has passed
    lastSendMqtt = millis(); 
    for (unsigned int i = 0; i < sizeof(sensorReading)/sizeof(sensorReading[0]); i++ ) {
      char topic[String("stat/"+host+"/"+sensorReading[i].sensor).length() + 1];
      char sensor[String(sensorReading[i].value).length() + 1];
      String("stat/"+host+"/"+sensorReading[i].sensor).toCharArray(topic, sizeof(topic));
      String(sensorReading[i].value).toCharArray(sensor, sizeof(sensor));
      Serial.print(topic);
      Serial.print(":");
      Serial.println(sensor );
      client.publish(topic, sensor);
    }
  }
}

#endif
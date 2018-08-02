
#include <FS.h>
#include <Wire.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <PID_v1.h>
//#include <MQTT.h>
#include "ESP8266_FishTank.h"
#include "html.h"
//#include "thermistor.h"
#include "HardwareSerial.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_MLX90614.h"

Adafruit_SSD1306 display(OLED_RESET);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Config config; 

//WiFiClient net;
//MQTTClient client; 

//init PID
PID myPID(&tempDiff, &fan_pwm, &config.temp_offset,kp,ki,kd, REVERSE);

//init webserver
ESP8266WebServer server ( 80 );


// Init Thermistor object
//THERMISTOR thermistor(NTC_PIN,        // Analog pin
//                      100000,          // Nominal resistance at 25 ºC
//                      3950,           // thermistor's beta coefficient
//                      97500);         // Value of the series resistor

// Handle request for root document ("/")
void rootHandler(){ 
  server.send ( 200, "text/html", indexPage() );
}

String indexPage(){
  return main_page;
}

// Handle request for settings document ("/settings")
void SettingsHandler(){ 
  server.send ( 200, "text/html", settingsPage() );
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

// Handle request for jsonapi document ("/fishtank.json")
void apiHandler(){ 
  server.send ( 200, "application/json", jsonApiHandler() );
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

void displaySetup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(2000);
  display.clearDisplay();
}

/** Temperature handler
  * Compages current temp with desired temp every 5 seconds 
  * (Configure via INTERVAL variable in header)
  * taking the max allowed offset into account.
  * generate a proper PID calculated PWM fan speed value
  */
void handleTempLoop(){
  if (millis() - lastReadTemp >= INTERVAL){  // if INTERVAL has passed
    lastReadTemp = millis(); 
/**
    //this loop is a trick to get a more stable temp by averaging over 5 readings (avgLoop in header)
    avgInt = 0;
    for(int i=0; i < avgLoop; i++) {
      double tInt = thermistor.read() / 10.0;
      avgInt += tInt;
      delay(100);
    }
*/
    //calculate setpoint (tempDiff)
    tempInt = mlx.readObjectTempC();
    tempDiff = tempInt - config.desired_temp;
    if(tempDiff < 0) {
      tempDiff = 0;
    }

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");

    //process PID
    myPID.Compute();
    Serial.println(fan_pwm);
   // connectMqtt();
    //apply PID processed fan_pwm
   // publishMessage("/fishtank/temperature", String(tempInt));
    //publishMessage("/fishtank/fan_pwm", String(fan_pwm/250*100));
    analogWrite(FAN_PIN, fan_pwm);
  }
}

void handleWaterLevel() {
  if (millis() - lastReadLevel >= INTERVAL){  // if INTERVAL has passed
    lastReadLevel = millis(); 
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration*0.034/2;
    if (distance >= 200 || distance <= 0){
      Serial.println("Distance Out of range");
    }
    Serial.println(distance);
  }
}

void handleDisplay() {
  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Water Temp: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(tempInt);
  display.println("C");
  display.setTextColor(WHITE);
  display.print("FAN Speed : ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(fan_pwm/250*100);
  display.println("%");
  display.setTextColor(WHITE);
  display.setTextColor(WHITE);
  display.print("Water Level : ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(distance);
  display.println("cm");
  display.setTextColor(WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}
/** Config reader handler
  * Checks if we have a config.json in flash store 
  * if not, we use default values defined in header
  * if we have a valid config it reads it and feeds the values
  * into used variables that makes the controller tick
  */
void readConfig() {
  if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        config.desired_temp = json["desired_temp"];
        config.temp_offset = json["temp_offset"];
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

/** Config save handler
  * Saves configuration into config.json file in flash
  */
void saveConfig(JsonObject& json) {
  Serial.println("saving config");
  json.prettyPrintTo(Serial);
  
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(configFile);
  configFile.close();
  readConfig();
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}
/**
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

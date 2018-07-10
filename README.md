# ESPtank
IOT ESP8266 Fishtank Controller

ESP8266 Powered Fishtank controller

Tested on LoLin NodeMCU 1.0 ESP8266 Board

### Simplified Schematics

![alt text](https://raw.githubusercontent.com/psych0d0g/ESP8266_FishTank/master/schematics/ESP8266_FishTank.png "Simplified Schematics")

The full fritzing schematic file can be downloaded [here](https://raw.githubusercontent.com/psych0d0g/ESP8266_FishTank/master/schematics/ESP8266_FishTank.fzz)

### Current Features

* Measures Temperature from connected NTC 100k termristor
* Controlls connected FAN using PID PWM
* Displays current Temperature and FAN speed on animated ajax gauges in Browser

### Sourcecode TODO

* Implement Dynamic WIFI Config

 Create an local Access Point if no valid WIFI to connect to is in reach and present a captive portal to any connected client that lets the user connect to a wifi of his choice by using a webbrowser

* Make Key parameters Configurable on the Web GUI

 Enable the user to set target temperature and maximum difference between measured and desired temperature via webinterface and store the choices in ESP's eeprom (use defaults if nothing has ben stored in eeprom so far)

* Create a proper API
 
 API can be used by 3. party app developers to communicate with the controller (Configuration, Current Status, Monitoring and alerting in certain conditions)

### Electronics and Additional features TODO

* Use a i2c IR contactless temperature sensor

 Benefits:
 * No electronics in the Fishtank
 * No electric interference for the sensor (the NTC Wires are affected for example by the usually used cathode light in fishtanks)

* Implement Water level sensing using a Ultrasonic sensor
 * Make key parameters configurable (Size of fishtank) to calculate water content of the fishtank in Litres
 * Display water volume in a nice ajaxified gauge on the webgui

* Add a Light Controlling feature to simulate Daylight via LED panel
 * Configurable Static Day/Night times
 * Option to Follow the Real Sunrise / Sundawn times
 * Maybe simulate real world weather conditions (Clouds anyone?)

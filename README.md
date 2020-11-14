# ESPtank
IOT ESP8266 Fishtank Controller

ESP8266 Powered Fishtank controller

Tested and developed on LoLin NodeMCU 1.0 ESP8266 Board

PLEASE NOTE: this software is licensed under the MIT license, but used librarys might use other licenses.

### Simplified Schematics

![alt text](https://raw.githubusercontent.com/psych0d0g/ESP8266_FishTank/master/schematics/ESP8266_FishTank.png "Simplified Schematics")

The full fritzing schematic file can be downloaded [here](https://raw.githubusercontent.com/psych0d0g/ESP8266_FishTank/master/schematics/ESP8266_FishTank.fzz)

### Current Features

* Measures Temperature from connected NTC 100k thermistor or i2c MLX IR Contactless Temp sensor
* Controls connected FAN using PID PWM
* Displays current Temperature and FAN speed on animated ajax gauges in Browser or on attached i2c Oled screen
* Reads connected Ultrasonic Distance sensor to report on water level (WiP)
* publishes sensor readings to a defined MQTT Server

### Dependencies

Check Platformio.ini for dependencies

#ifndef header_h
#define header_h
//#include "system.h"

// pin Definitions
#define NTC_PIN A0
#define FAN_PIN D8
#define TRIG_PIN D6
#define ECHO_PIN D7

#define OLED_RESET D0

// Wifi Setup
String host = "Fishtank";
const char* mqtt_server = "10.10.10.10";

struct Config {
  double desired_temp = 25;
  double temp_offset = 0.5;
};

int INTERVAL = 5000;   // time between reads

int avgLoop = 5;    //temp measurement loops

//PID parameters.
double kp=20;   //proportional parameter
double ki=5;   //integral parameter
double kd=1;   //derivative parameter

//Minimum and Maximum PWM command, according fan specs and noise level required
double commandMin = 0;
double commandMax = 250;

boolean first = true;
double temp, tempInt, avgInt, tempDiff, fan_pwm;

long duration, distance, lastReadTemp, lastReadLevel;
#endif

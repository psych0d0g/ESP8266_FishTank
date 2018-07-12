#ifndef header_h
#define header_h

// Wifi Setup
String host = "Fishtank";

struct Config {
  double desired_temp = 25;
  double temp_offset = 0.5;
};

int INTERVAL = 5000;   // time between reads
unsigned long lastRead = 0;

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

#endif

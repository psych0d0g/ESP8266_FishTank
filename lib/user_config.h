#ifndef user_config_h
#define user_config_h

// Sensor can either be MLX or NTC
#define SENSOR_MLX
//#define SENSOR_NTC
// pin Definitions
// If you use a NTC Sensor instead of i2c based uncomment this and the avgLoop below
#define NTC_PIN A0
// Pin where you connected the FAN Controlling Transistor
#define FAN_PIN D8
//Pin setup for the Ultrasonic Sensor
#define TRIG_PIN D6
#define ECHO_PIN D7

// Local Hostname (where you want to reach your controller at)
// Also used for mqtt and wifi AP name (Case sensitive)
String host = "fishtank";
//Connect to the following MQTT Server
const char* mqtt_server = "10.10.10.10";

// Update stuff every 5 seconds by default
unsigned long INTERVAL = 5000;   // time between reads

// Loop over NTC Readings
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



long duration, distance, lastReadTemp, lastReadLevel, lastSendMqtt, lastUpdateDisplay;
#endif

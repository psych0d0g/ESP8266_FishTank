#ifndef user_config_h
#define user_config_h
#include <Timezone.h>

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
String host = "fishtankdev";
//Connect to the following MQTT Server
const char* mqtt_server = "10.10.10.10";

int targetLightCold    = 128;
int targetLightNeutral = 200;
int targetLightWarm    = 180;
int targetLightBlue    = 64;

TimeChangeRule myDST = {"CEST", Last, Sun, Mar, 2, +120};    // Daylight time = UTC + 2 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 3, +60}; // Standard time = UTC +1 hours

int ntp_interval = 5000;

struct sensorReadings {
  //uint32_t timeStamp;
  String sensor;
  double value;
};

sensorReadings sensorReading[] = {
	{"water_temp", 0.0},
	{"water_level", 0.0},
	{"fan_speed", 0.0}
};

// Update stuff every 5 seconds by default
unsigned long INTERVAL = 5000;   // time between reads
unsigned long LED_INTERVAL = 500;   // time between LED updates (infuences the time a sunrise and sunset require to be completed)
unsigned long WATER_INTERVAL = 300000;   // time between reads 5 min
unsigned long MQTT_INTERVAL = 10000;   // time between MQTT publishes

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



long duration, distance, lastReadTemp, lastTimeCalc, lastSetLed, lastReadLevel, lastSendMqtt, lastUpdateDisplay;
int sunrise_at = 0;
int sunset_at = 0;
int i_cold = 0;
int i_neutral = 0;
int i_warm = 0;
int i_blue = 0;
#endif

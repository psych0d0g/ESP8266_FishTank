#ifndef sensors_h
#define sensors_h

#include <Arduino.h>
#include <PID_v1.h>

#include "config_schema.h" 
#include "user_config.h"

//make sure only one sensor at a time
#if not (defined SENSOR_MLX && defined SENSOR_NTC)
  #ifdef SENSOR_MLX
    #include <Adafruit_MLX90614.h>
  #elif defined SENSOR_NTC
    #include "HardwareSerial.h"
    #include <thermistor.h>
  #endif
#else
  #error 'please use either NTC or MLX thx ;-)'
#endif


//init PID
PID myPID(&tempDiff, &fan_pwm, &config.temp_offset,kp,ki,kd, REVERSE);



#ifdef SENSOR_MLX
  Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#elif defined SENSOR_NTC
  THERMISTOR thermistor(NTC_PIN,        // Analog pin
                        100000,          // Nominal resistance at 25 ºC
                        3950,           // thermistor's beta coefficient
                        97500);         // Value of the series resistor
#endif

void setupSensors() {
  #ifdef SENSOR_MLX
    mlx.begin();  
  #endif
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

/** Temperature handler
  * Compages current temp with desired temp every 5 seconds 
  * (Configure via INTERVAL variable in header)
  * taking the max allowed offset into account.
  * generate a proper PID calculated PWM fan speed value
  */
void handleTempLoop(){
  if (millis() - lastReadTemp >= INTERVAL){  // if INTERVAL has passed
    lastReadTemp = millis(); 
    #ifdef SENSOR_NTC
      //this loop is a trick to get a more stable temp by averaging over 5 readings (avgLoop in header)
      avgInt = 0;
      for(int i=0; i < avgLoop; i++) {
        double tInt = thermistor.read() / 10.0;
        avgInt += tInt;
        delay(100);
      }
      tempInt = avgInt/avgLoop;
    #elif defined SENSOR_MLX
      tempInt = mlx.readObjectTempC();
    #endif

    //calculate setpoint (tempDiff)
    tempDiff = tempInt - config.desired_temp;
    if(tempDiff < 0) {
      tempDiff = 0;
    }

  Serial.print("tObject = "); Serial.print(tempInt); Serial.println("*C");

    //process PID
    myPID.Compute();
    Serial.println(fan_pwm);
    sensorReading[0].value = tempInt;
    sensorReading[2].value = fan_pwm;
    analogWrite(FAN_PIN, fan_pwm);
  }
}


void handleWaterLevel() {
  if (millis() - lastReadLevel >= WATER_INTERVAL){  // if INTERVAL has passed
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
    sensorReading[1].value = distance;
  }
}

#endif

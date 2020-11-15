#ifndef led_control_h
#define led_control_h

#include <Arduino.h>
#include "PCA9685.h"
#include "time_handler.h"
#include "config_schema.h" 

PCA9685 pwmController; 

void setupPWM(){
    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    pwmController.init(B000000);        // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(100); // Default is 200Hz, supports 24Hz to 1526Hz
}

//WiP
void changeBrightness(boolean mode, String color){
  if(mode == increase){
    for (byte channel=0; channel<=(sizeof(color) / sizeof(color[0])); channel++) {
   //   if (pwmController.getChannelPWM(color[channel]) > ){

   //   }
      Serial.println("Array Pos:" + String(channel));
      Serial.println("Array Content: " + String(color[channel]));
      Serial.println(pwmController.getChannelPWM(color[channel])); // Blue
      yield();  // take a breather, required for ESP8266
    }
  }
  if(mode == decrease){

  }
}

bool isday(){
  if (config.daynight == 0){
    if (sunset_at > 10000 && sunrise_at > 10000 && sunset_at >= currentTime("local") && sunrise_at <= currentTime("local")) {
      return true;
    } else if ((sunset_at > 10000 && sunrise_at > 10000 && sunset_at > currentTime("local") && sunrise_at > currentTime("local")) || (sunset_at > 10000 && sunrise_at > 10000 && sunset_at < currentTime("local") && sunrise_at < currentTime("local"))) {
      return false;
    }
  } else if(config.daynight == 1){
    return true;
  } else if(config.daynight == 2){
    return false;
  }
}

void handleLedState(){
	// Only modify White LEDs while in Daylight TimeFrame
	if (millis() - lastSetLed >= LED_INTERVAL){  // if INTERVAL has passed
    lastSetLed = millis(); 
    //changeBrightness(increase, "warmwhite");
    if(isday()){
			if( i_blue > 0 ) {
				pwmController.setChannelPWM(4, i_blue); // Blue
				yield();  // take a breather, required for ESP8266
    		i_blue--;
        yield();
        if(i_blue < 2) {
          pwmController.setChannelPWM(4, 0); // Blue
          yield();
          i_blue=0;
        }
   			printOnSerial("value of i_blue:" + String(i_blue));
 			}
			if( i_warm < config.desired_warm*4 ) {
				pwmController.setChannelPWM(3, i_warm); // Warm White
				yield();  // take a breather, required for ESP8266
    		i_warm++;
        yield();
   			printOnSerial("value of i_warm:" + String(i_warm));
 			}
      if( i_warm > config.desired_warm*4 ) {
        pwmController.setChannelPWM(3, i_warm); // Warm White
        yield();  // take a breather, required for ESP8266
        i_warm--;
        yield();
        printOnSerial("value of i_warm:" + String(i_warm));
      }
			if( i_warm == config.desired_warm*4 && i_neutral < config.desired_neutral*4 ) {
				pwmController.setChannelPWM(2, i_neutral); // Neutral White
				yield();  // take a breather, required for ESP8266
    		i_neutral++;
        yield();
   			printOnSerial("value of i_neutral:" + String(i_neutral));
 			}
      if( i_warm == config.desired_warm*4 && i_neutral > config.desired_neutral*4 ) {
        pwmController.setChannelPWM(2, i_neutral); // Neutral White
        yield();  // take a breather, required for ESP8266
        i_neutral--;
        yield();
        printOnSerial("value of i_neutral:" + String(i_neutral));
      }
 			if( i_warm == config.desired_warm*4 && i_neutral == config.desired_neutral*4 && i_cold < config.desired_cold*4 ) {
				pwmController.setChannelPWM(0, i_cold); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold); // Cold White
				yield();  // take a breather, required for ESP8266
    		i_cold++;
        yield();
   			printOnSerial("value of i_cold:" + String(i_cold));
 			}
      if( i_warm == config.desired_warm*4 && i_neutral == config.desired_neutral*4 && i_cold > config.desired_cold*4 ) {
        pwmController.setChannelPWM(0, i_cold); // Cold White
        yield();  // take a breather, required for ESP8266
        pwmController.setChannelPWM(1, i_cold); // Cold White
        yield();  // take a breather, required for ESP8266
        i_cold--;
        yield();
        printOnSerial("value of i_cold:" + String(i_cold));
      }
		} else {
			if( i_cold > 0 ) {
				pwmController.setChannelPWM(0, i_cold); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold); // Cold White
				yield();  // take a breather, required for ESP8266
    		i_cold--;
        yield();
        if(i_cold < 2) {
          pwmController.setChannelPWM(0, 0); // Blue
          yield();
          pwmController.setChannelPWM(1, 0); // Blue
          yield();
          i_cold=0;
        }
   			printOnSerial("value of i_cold:" + String(i_cold));
 			}
 			if( i_cold == 0 && i_neutral > 0 ) {
				pwmController.setChannelPWM(2, i_neutral); // Neutral White
				yield();  // take a breather, required for ESP8266
    		i_neutral--;
        yield();
        if(i_neutral < 2) {
          pwmController.setChannelPWM(2, 0); // Blue
          yield();
          i_neutral=0;
        }
   			printOnSerial("value of i_neutral:" + String(i_neutral));
 			}
 			if( i_cold == 0 && i_neutral == 0 && i_warm > 0 ) {
				pwmController.setChannelPWM(3, i_warm); // Neutral White
				yield();  // take a breather, required for ESP8266
    		i_warm--;
        yield();
        if(i_warm < 2) {
          pwmController.setChannelPWM(3, 0); // Blue
          yield();
          i_warm=0;
        }
   			printOnSerial("value of i_warm:" + String(i_warm));
 			}
      if( i_cold == 0 && i_neutral == 0 && i_blue < config.desired_blue*4 ) {
        pwmController.setChannelPWM(4, i_blue); // Neutral White
        yield();  // take a breather, required for ESP8266
        i_blue++;
        yield();
        printOnSerial("value of i_blue:" + String(i_blue));
      }
      if( i_cold == 0 && i_neutral == 0 && i_blue > config.desired_blue*4 ) {
        pwmController.setChannelPWM(4, i_blue); // Neutral White
        yield();  // take a breather, required for ESP8266
        i_blue--;
        yield();
        if(i_blue < 2) {
          pwmController.setChannelPWM(4, 0); // Blue
          yield();
          i_blue=0;
        }
        printOnSerial("value of i_blue:" + String(i_blue));
      }
		}
	}
}


#endif
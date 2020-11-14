#ifndef led_control_h
#define led_control_h

#include <Arduino.h>
#include "PCA9685.h"
#include "time_handler.h"

PCA9685 pwmController; 

void setupPWM(){
    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    pwmController.init(B000000);        // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(100); // Default is 200Hz, supports 24Hz to 1526Hz
}

void handleLedState(){
	// Only modify White LEDs while in Daylight TimeFrame
	if (millis() - lastSetLed >= LED_INTERVAL){  // if INTERVAL has passed
    lastSetLed = millis(); 
		if (sunset_at > 10000 && sunrise_at > 10000 && sunset_at >= currentTime("local") && sunrise_at <= currentTime("local")) {
			if( i_blue > 0 ) {
				pwmController.setChannelPWM(4, i_blue << 4); // Blue
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_blue:" + String(i_blue));
    		i_blue--;
        yield();
        if(i_blue < 8) {
          pwmController.setChannelPWM(4, 0); // Blue
          yield();
          i_blue=0;
        }
 			}
			if( i_warm < targetLightWarm ) {
				pwmController.setChannelPWM(3, i_warm << 4); // Warm White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_warm:" + String(i_warm));
    		i_warm++;
        yield();
 			}
			if( i_warm == targetLightWarm && i_neutral < targetLightNeutral ) {
				pwmController.setChannelPWM(2, i_neutral << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_neutral:" + String(i_neutral));
    		i_neutral++;
        yield();
 			}
 			if( i_warm == targetLightWarm && i_neutral == targetLightNeutral && i_cold < targetLightCold ) {
				pwmController.setChannelPWM(0, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_cold:" + String(i_cold));
    		i_cold++;
        yield();
 			}
		} else if ((sunset_at > 10000 && sunrise_at > 10000 && sunset_at > currentTime("local") && sunrise_at > currentTime("local")) || (sunset_at > 10000 && sunrise_at > 10000 && sunset_at < currentTime("local") && sunrise_at < currentTime("local"))) {
			if( i_cold > 0 ) {
				pwmController.setChannelPWM(0, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_cold:" + String(i_cold));
    		i_cold--;
        yield();
        if(i_cold < 8) {
          pwmController.setChannelPWM(0, 0); // Blue
          yield();
          pwmController.setChannelPWM(1, 0); // Blue
          yield();
          i_cold=0;
        }
 			}
 			if( i_cold == 0 && i_neutral > 0 ) {
				pwmController.setChannelPWM(2, i_neutral << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_neutral:" + String(i_neutral));
    		i_neutral--;
        yield();
        if(i_neutral < 8) {
          pwmController.setChannelPWM(2, 0); // Blue
          yield();
          i_neutral=0;
        }
 			}
 			if( i_cold == 0 && i_neutral == 0 && i_warm > 0 ) {
				pwmController.setChannelPWM(3, i_warm << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_warm:" + String(i_warm));
    		i_warm--;
        yield();
        if(i_warm < 8) {
          pwmController.setChannelPWM(3, 0); // Blue
          yield();
          i_warm=0;
        }
 			}
 			if( i_cold == 0 && i_neutral == 0 && i_blue < targetLightBlue ) {
				pwmController.setChannelPWM(4, i_blue << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
   			printOnSerial("value of i_blue:" + String(i_blue));
    		i_blue++;
        yield();
 			}
		}
	}
}


#endif
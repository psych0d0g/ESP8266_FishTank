#ifndef led_control_h
#define led_control_h

#include <Arduino.h>
#include "PCA9685.h"
#include "time_handler.h"

PCA9685 pwmController; 

void setupPWM(){
    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    pwmController.init(B000000);        // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(1000); // Default is 200Hz, supports 24Hz to 1526Hz
}

void handleLedState(){
	// Only modify White LEDs while in Daylight TimeFrame
	if (millis() - lastSetLed >= LED_INTERVAL){  // if INTERVAL has passed
    lastSetLed = millis(); 
		if (sunset_at > 10000 && sunrise_at > 10000 && sunset_at >= currentTime("local") && sunrise_at <= currentTime("local")) {
			if( i_blue > 0 ) {
				pwmController.setChannelPWM(4, i_blue << 4); // Blue
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_blue:");Serial.println(i_blue);
      			i_blue--;
      			yield();
   			}
			if( i_warm < targetLightWarm ) {
				pwmController.setChannelPWM(3, i_warm << 4); // Warm White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_warm:");Serial.println(i_warm);
      			i_warm++;
      			yield();
   			}
			if( i_warm == targetLightWarm && i_neutral < targetLightNeutral ) {
				pwmController.setChannelPWM(2, i_neutral << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_neutral:");Serial.println(i_neutral);
      			i_neutral++;
      			yield();
   			}
   			if( i_warm == targetLightWarm && i_neutral == targetLightNeutral && i_cold < targetLightCold ) {
				pwmController.setChannelPWM(0, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_cold:");Serial.println(i_cold);
      			i_cold++;
      			yield();
   			}
		} else if ((sunset_at > 10000 && sunrise_at > 10000 && sunset_at > currentTime("local") && sunrise_at > currentTime("local")) || (sunset_at > 10000 && sunrise_at > 10000 && sunset_at < currentTime("local") && sunrise_at < currentTime("local"))) {
			if( i_cold > 0 ) {
				pwmController.setChannelPWM(0, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
				pwmController.setChannelPWM(1, i_cold << 4); // Cold White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_cold:");Serial.println(i_cold);
      			i_cold--;
      			yield();
   			}
   			if( i_cold == 0 && i_neutral > 0 ) {
				pwmController.setChannelPWM(2, i_neutral << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_neutral:");Serial.println(i_neutral);
      			i_neutral--;
      			yield();
   			}
   			if( i_cold == 0 && i_neutral == 0 && i_warm > 0 ) {
				pwmController.setChannelPWM(3, i_warm << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_warm:");Serial.println(i_warm);
      			i_warm--;
      			yield();
   			}
   			if( i_cold == 0 && i_neutral == 0 && i_blue < targetLightBlue ) {
				pwmController.setChannelPWM(4, i_blue << 4); // Neutral White
				yield();  // take a breather, required for ESP8266
     			Serial.print("value of i_blue:");Serial.println(i_blue);
      			i_blue++;
      			yield();
   			}
		}
	}
}


#endif
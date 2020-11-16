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

bool isday(){
  compute(calculateDayOfYear(currentTime("local")));
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
    if(isday()){
      // Loop through array from High to Low since we assume blue night light is at end of array
      // this way we can turn down the Blue light first, before increasing white light brightness
      for (channel=(sizeof(config.target_intensity) / sizeof(int)); channel>0; channel--) {
        if (channel == (sizeof(config.target_intensity) / sizeof(int))){
    			if( config.current_intensity[channel] > 0 ) {
    				pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Blue
    				yield();  // take a breather, required for ESP8266
        		config.current_intensity[channel]--;
            yield();
            if(config.current_intensity[channel] < 2) {
              pwmController.setChannelPWM(channel, 0); // Blue
              yield();
              config.current_intensity[channel]=0;
            }
       			printOnSerial("value of i_blue:" + String(config.current_intensity[channel]));
     			}
        }
        // Increase Brightness of channel if desired brightness was set higher
		    if( config.current_intensity[channel] < config.target_intensity[channel]*4 ) {
  				pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Warm White
  				yield();  // take a breather, required for ESP8266
      		config.current_intensity[channel]++;
          yield();
     			printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
   			}
        // Decrease Brightness of channel if desired brightness was set lower
        if( config.current_intensity[channel] > config.target_intensity[channel]*4 ) {
          pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Warm White
          yield();  // take a breather, required for ESP8266
          config.current_intensity[channel]--;
          yield();
          printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
        }
      }
		} else {
      // Loop through array forward for night mode, turn down white, before turning up blue
      for (channel=0; channel<(sizeof(config.target_intensity) / sizeof(int)); channel++) {
        if (channel < (sizeof(config.target_intensity) / sizeof(int))){
          if( config.current_intensity[channel] > 0 ) {
            pwmController.setChannelPWM(channel, config.current_intensity[channel]); // all diffrent non blue channels
            yield();  // take a breather, required for ESP8266
            config.current_intensity[channel]--;
            yield();
            if(config.current_intensity[channel] < 2) {
              pwmController.setChannelPWM(channel, 0); // whites
              yield();
              config.current_intensity[channel]=0;
            }
            printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
          }
        }
        if( config.current_intensity[channel] < config.target_intensity[channel]*4 ) {
          pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Probably Blue and UV
          yield();  // take a breather, required for ESP8266
          config.current_intensity[channel]++;
          yield();
          printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
        }
        if( config.current_intensity[channel] > config.target_intensity[channel]*4 ) {
          pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Probably Blue and UV
          yield();  // take a breather, required for ESP8266
          config.current_intensity[channel]--;
          yield();
          printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
        }
      }
		}
	}
}


#endif
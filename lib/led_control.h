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

void changeBrightness(boolean mode, int channel){
  if(mode == increase && config.current_intensity[channel] != config.target_intensity[channel]*4){
    config.current_intensity[channel]++;
    yield();
    pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Warm White
    yield();  // take a breather, required for ESP8266
    printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
  }
  if(mode == decrease && config.current_intensity[channel] != 0){
    config.current_intensity[channel]--;
    yield();
    pwmController.setChannelPWM(channel, config.current_intensity[channel]); // Blue
    yield();  // take a breather, required for ESP8266
    printOnSerial("value of channel "+String(channel)+":" + String(config.current_intensity[channel]));
  }
}

bool isday(){
  compute(calculateDayOfYear(currentTime("local")));
  if (config.daynight == 0){
    if (sunset_at > 10000 && sunrise_at > 10000 && sunset_at >= currentTime("local") && sunrise_at <= currentTime("local")) {
      // It is day
      return true;
    } else if ((sunset_at > 10000 && sunrise_at > 10000 && sunset_at > currentTime("local") && sunrise_at > currentTime("local")) || (sunset_at > 10000 && sunrise_at > 10000 && sunset_at < currentTime("local") && sunrise_at < currentTime("local"))) {
      //It is night
      return false;
    }
  } else if(config.daynight == 1){
    // manual ovverride to day active
    return true;
  } else if(config.daynight == 2){
    // manual override to night active
    return false;
  }
}

void handleLedState(){
	// Only modify White LEDs while in Daylight TimeFrame
  if (millis() - lastSetLed >= LED_INTERVAL){  // if INTERVAL has passed
    lastSetLed = millis(); 
    int target_intensity_length = sizeof(config.target_intensity) / sizeof(int)-1;
    // Sorry its getting ugly here, but we need some uglyness in the code
    // to make it beautyful in real world :/
    // Looping over configured channels
    for (channel=0; channel<=target_intensity_length; channel++) {
      // Do this when we have daylight outside
      if(isday()){
        // If Channels 6 and 7 (Night Light) are not off yet, turn them off before anything else
        if(config.current_intensity[6] != 0 || config.current_intensity[7] != 0){
          if (channel >= target_intensity_length-1){
            changeBrightness(decrease, channel);
          }
        }else{
          // If the loop has reached a channel that is to be handled at daytime
          if (channel < target_intensity_length-1){
            // Increase Brightness of channel if desired brightness was set higher
            if( config.current_intensity[channel] < config.target_intensity[channel]*4 ) {
                changeBrightness(increase, channel);
            }
            // Decrease Brightness of channel if desired brightness was set lower
            if( config.current_intensity[channel] > config.target_intensity[channel]*4 ) {
              changeBrightness(decrease, channel);
            }
          }
        }
      // Here we start our Nighttime routine
  		} else {
        // If channels 1 and 2 are not off yet, turn them off before anything else (Reducing COLD White first)
        if(config.current_intensity[0] != 0 || config.current_intensity[1] != 0){
          if (channel < target_intensity_length-5){
            changeBrightness(decrease, channel);
          }
        // If channels 3 and 4 are not off yet, turn them off next (Reducing Neutral white, getting gradually more warm white like in nature)
        } else if(config.current_intensity[2] != 0 || config.current_intensity[3] != 0){
          if (channel < target_intensity_length-3 && channel >= target_intensity_length-5){
            changeBrightness(decrease, channel);
          }
        } else {
          //If channels 5 and 6 are not off yet, finally turn them off together with adding blue
          // so we get a nice gradual move to blue night light
          if (channel < target_intensity_length-1 && channel >= target_intensity_length-3){
            changeBrightness(decrease, channel);
          }
          // If the loop has reached a channel that is handled at night time
          if (channel >= target_intensity_length-1){
            if( config.current_intensity[channel] < config.target_intensity[channel]*4 ) {
              changeBrightness(increase, channel);
            }
            if( config.current_intensity[channel] > config.target_intensity[channel]*4 ) {
              changeBrightness(decrease, channel);
            }
          }
        }
      }
    }
  }
}


#endif
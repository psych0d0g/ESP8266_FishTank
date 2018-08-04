#ifndef display_h
#define display_h

#include <Arduino.h>

#include <Adafruit_SSD1306.h>

#define OLED_RESET D0

Adafruit_SSD1306 display(OLED_RESET);

void displaySetup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(2000);
  display.clearDisplay();
}

void handleDisplay() {
  if (millis() - lastReadTemp >= INTERVAL){  // if INTERVAL has passed
  lastReadTemp = millis(); 
    // text display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Water Temp: ");
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(tempInt);
    display.println("C");
    display.setTextColor(WHITE);
    display.print("FAN Speed : ");
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(fan_pwm/250*100);
    display.println("%");
    display.setTextColor(WHITE);
    display.setTextColor(WHITE);
    display.print("Water Level : ");
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(distance);
    display.println("cm");
    display.setTextColor(WHITE);
    display.display();
    delay(2000);
    display.clearDisplay();
  }
}

#endif

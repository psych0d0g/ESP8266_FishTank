#ifndef display_h
#define display_h

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <math.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void displaySetup() {
     u8g2.setI2CAddress(0x3C*2);
     u8g2.begin();
}

// Some maths to get a percentage like Progress bar kinda thing for the FAN RPM
int calculateFlowBar(float flow) {
  int calc_flow = flow/250*100;
  // We cap at 25L/min for our used pump for the flowbar
  return calc_flow;
}

void flow_draw_prepare(void) {
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(2);
  u8g2.setFontMode(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.setBitmapMode(false /* solid */);
}

void draw_flowbar(float flow) {
  flow_draw_prepare();
  const uint8_t frame_size = 16;

  // Water Droplet icon
  u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
  u8g2.drawStr(frame_size * 0.5, 3, "G");
  // Frame for Flowrate
  u8g2.drawFrame(0, 0, 128, frame_size);
  // Fill Bar for Flowrate
  u8g2.drawBox(1, 1, calculateFlowBar(flow), frame_size-2);
  // Int Flowrate value
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(frame_size * 1.5, 4, String(calculateFlowBar(flow)).c_str());
  u8g2.drawStr(frame_size * 4, 4, "%");
}

void draw_temps(float tempA) {
  flow_draw_prepare();
  const uint8_t frame_size = 32;
  const uint8_t frame_y_start=16+16;
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(4, frame_y_start-11, "Water Temp");
// Frame for tempA
  u8g2.drawFrame(0, frame_y_start, 63, frame_size);
// Int tempA value
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.drawStr(10, frame_y_start+6, String(int(round(tempA))).c_str());
  u8g2.drawUTF8(30, frame_y_start+6, "°C");  
}

void printOnScreen(double curFlow, int tempA){
  curTime = millis();
  // Every 1000 milisecond (1 second), print out readings to display
  if(curTime >= (sloopTime + 1000))
  {
    sloopTime = curTime;              // Updates cloopTime
    Wire.setClock(400000L);
    u8g2.clearBuffer();
    draw_flowbar(curFlow);
    draw_temps(tempA);
    u8g2.sendBuffer();
    Wire.setClock(100000L);
  }
}

#endif

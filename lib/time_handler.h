#ifndef time_handler_h
#define time_handler_h

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Timezone.h>
#include <time.h>


WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 86400000 );

Timezone myTZ(myDST, mySTD);

TimeChangeRule *tcr;

void setupNTP(){
  timeClient.begin();
}  

void loopNTP(){
  curTime = millis();
  // Every 1000 milisecond (1 second), print out readings to display
  if(curTime >= (tloopTime + 300000))
  {
    tloopTime = curTime;              // Updates cloopTime
    timeClient.update();
  }
}

long currentTime(String type) {
  if (type == "local") {
    time_t utc = timeClient.getEpochTime();
    time_t local = myTZ.toLocal(utc, &tcr);
    return local;
  } else if (type == "utc") {
    time_t utc = timeClient.getEpochTime();
    return utc;
  }
  return 0;
}


// format and print a time_t value, with a time zone appended.
void dateTime(time_t t, const char *tz)
{
  char buf[32];
  char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
  Serial.println(buf);
}

//calculateDayOfYear(currentTime("utc"));
int calculateDayOfYear(time_t t) {
  
  // Given a day, month, and year (4 digit), returns 
  // the day of year. Errors return 999.
  int d = day(t);
  int m = month(t);
  int y = year(t);

  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  // Verify we got a 4-digit year
  if (y < 1000) {
    return 999;
  }
  
  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (y%4  == 0) {
    if (y%100 != 0) {
      daysInMonth[1] = 29;
    }
    else {
      if (y%400 == 0) {
        daysInMonth[1] = 29;
      }
    }
   }

  // Make sure we are on a valid day of the month
  if (d < 1) 
  {
    return 999;
  } else if (d > daysInMonth[m-1]) {
    return 999;
  }
  
  int doy = 0;
  for (int i = 0; i < m - 1; i++) {
    doy += daysInMonth[i];
  }
  
  doy += d;
  return doy;
}

// subfunction to compute solstice, found at https://www.arduinoforum.de/arduino-Thread-Sonnenaufgang-untergang-f%C3%BCr-Steuerungen-berechnen
float solstice(int T) {
    // Formula 2008 by Arnold(at)Barmettler.com, fit to 20 years of average declinations (2008-2017)
    return 0.409526325277017*sin(0.0169060504029192*(T-80.0856919827619));
}

// subfunction to compute Zeitdifferenz    
float timeDifference(float Deklination, float B) {
    // Dauer des halben Tagbogens in Stunden: Zeit von Sonnenaufgang (Hoehe h) bis zum hoechsten Stand im Sueden
    return 12.0*acos((sin(-(50.0/60.0)*PI/180.0) - sin(B)*sin(Deklination)) / (cos(B)*cos(Deklination)))/PI;
}

// subfunction to compute The Equation of Time 
float timeEquation(int T) {
    return -0.170869921174742*sin(0.0336997028793971 * T + 0.465419984181394) - 0.129890681040717*sin(0.0178674832556871*T - 0.167936777524864);
}

// subfunction to compute sunrise
float getSunrise(int T, float B) {
    float DK = solstice(T);
    return 12 - timeDifference(DK, B) - timeEquation(T);
}

// subfunction to compute sunset
float getSunset(int T, float B) {
    float DK = solstice(T);
    return 12 + timeDifference(DK, B) - timeEquation(T);
}

// subfunction to compute it all and return sunrise and sunset epoch's
void compute(int T) {
  if (millis() - lastTimeCalc >= INTERVAL){  // if INTERVAL has passed
    lastTimeCalc = millis(); 
    if (sunrise_at == 0 || sunset_at == 0 || currentTime("local") - previousMidnight(currentTime("local")) < 20) {
      float lon     = 8.59;
      float lat     = 49.12;
      int   zone       = 2;       // Difference from UTC
      float B = lat*PI/180.0;  // geogr. Breite in Radians

      // Berechnung von Sonnenauf- und -Untergang
      float sunrise    = getSunrise(T, B) - lon /15.0 + zone;    // Sonnenaufgang bei 0 Grad Laenge
      float sunset  = getSunset(T, B) - lon /15.0 + zone;  // Sonnenuntergang bei 0 Grad Laenge

      sunrise_at = previousMidnight(currentTime("local")) + (int)(sunrise*60*60);
      sunset_at = previousMidnight(currentTime("local")) + (int)(sunset*60*60);
      printOnSerial(" sunrise at: " + String(sunrise_at) + " sunset at: " + String(sunset_at));
    }
  }
}
#endif
/**
 * ClockInABox
 * https://github.com/boneskull/arduino-clock-in-a-box
 * Author: Christopher Hiller
 * License: MIT
 */

// TODO: TinyWire version
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS1307 RTC;

// if this remains false, the program loop will break and do nothing
boolean success = false;

// whether or not the colon should be displayed
boolean colon = false;

// used instead of delay
unsigned long int prevMillis = 0;

// displays current time
void displayTime(DateTime dateTime) {
  int hour = dateTime.hour();
  int min = dateTime.minute();
  Serial.print("Current time: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.println(min);

  // TODO this converts the time to an integer.  so if the time is 6:24p
  // it will convert to integer 1824.  this is because I couldn't figure out
  // how to print an integer in a given column using the LEDBackpack lib.
  // because I am dumb.  the drawback here is that once it's midnight, you will
  // see "0" displayed, then "10" at 12:10, and two digits until 1:00.
  int time = ((int)(hour / 10) * 1000) + ((int)(hour % 10) * 100) +
    ((int)(min / 10) * 10) + ((int)min % 10);

  matrix.println(time);
}

// toggles the colon
void blink() {
  colon = !colon;
  matrix.drawColon(colon);
}

void setup () {
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();

  // uncomment the following line initially to set the clock.
  // upload, then comment out, then upload again.
  // RTC.adjust(DateTime(__DATE__, __TIME__));

  if (! RTC.isrunning()) {
    // I imagine this happens if you do not have an RTC module connected.
    Serial.println("RTC is NOT running!");
  }
  else {
    success = true;
    matrix.begin(0x70);
    displayTime(RTC.now());
    matrix.writeDisplay();
  }

}

void loop () {
  if (!success) {
    break;
  }
  unsigned long int currentMillis = millis();
  if (currentMillis - prevMillis >= 500) {

    blink();
    // update time every second
    if (currentMillis - prevMillis >= 1000) {
      displayTime(RTC.now());
      prevMillis = currentMillis;
    }

    matrix.writeDisplay();
  }
}

#include <Arduino.h>
#include "configurations.h"

void setup() {
  Serial.begin(115200);
  
  // Setting up SHK pins
  for (int i = 0; i < numberOfLines; i++) {
    pinMode(SHKPins[i], INPUT_PULLUP);
  }
}


void SHKRead() {
  unsigned long currentMillis = millis();
  for (int i = 0; i < numberOfLines; i++) {
    int reading = digitalRead(SHKPins[i]);

    if (reading != hookStates[i] && (currentMillis - lastDebounceTime[i] > hookDebounceDelay)) {
      hookStates[i] = reading;
      lastDebounceTime[i] = currentMillis;
    }
  }
}


void loop() {
  SHKRead();

  delay(1);
}

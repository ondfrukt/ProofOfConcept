#include <Arduino.h>
#include "configurations.h"
#include <Adafruit_MCP23X17.h>

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

    if (reading != hookStates[i] && (currentMillis - lastSHKDebounceTime[i] > hookDebounceDelay)) {
      hookStates[i] = reading;
      lastSHKDebounceTime[i] = currentMillis;
    }
  }
}


void loop() {
  SHKRead();

  delay(1);
}

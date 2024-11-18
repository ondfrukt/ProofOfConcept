#ifndef PulsHandler_h
#define PulsHandler_h

#include <Arduino.h>
#include "config.h"

const unsigned pulseGapMin = 10;
const unsigned pulseGapMax = 60;

void pulseHandler(int i) {

  // State change detection med debounce
  if (Line[i].SHKState != Line[i].previousSHKState) {
    Serial.println("Edge detected");
    Line[i].lastSHKBounceTime = millis();
    Line[i].edgeTimestamp = millis();
    Serial.print("i "); Serial.print(i); Serial.print(" Pulsing: "); Serial.println(Line[i].pulsing);
    return;
  }

  // Debounce check
  if ((millis() - Line[i].lastSHKBounceTime) < pulseGapMin) {
    return;
  }

  // Beräkna gap om edgeTimeStamp är satt
  unsigned gap = Line[i].edgeTimestamp ? millis() - Line[i].edgeTimestamp : 0;

  if (Line[i].SHKState && Line[i].previousSHKState && gap <= pulseGapMax) {
    return;
  }

  if (!Line[i].SHKState && !Line[i].previousSHKState) {
    return;
  }

  // Falling edge detection
  if (!Line[i].SHKState && !Line[i].pulsing) {
    Serial.print("i "); Serial.print(i); Serial.println(" falling edge detected");
    Line[i].pulsing = true;
    Serial.print("i "); Serial.print(i); Serial.println(" pulsing");
    Line[i].edgeTimestamp = millis();
    return;
  }

  // Rising edge detection
  if (Line[i].SHKState && Line[i].pulsing) {
    Serial.print("i "); Serial.print(i); Serial.println(" rising edge detected");
    Line[i].pulsing = false;
    Line[i].pulsCount++;
    Serial.print("i "); Serial.print(i); Serial.print(" pulse count: "); Serial.println(Line[i].pulsCount);
    Line[i].edgeTimestamp = millis();

    if (Line[i].currentLineStatus != line_pulse_dialing) {
      Line[i].setLineStatus(line_pulse_dialing);
    }
    return;
  }

  // Digit completion detection
  if (Line[i].SHKState && !Line[i].pulsing && gap > pulseGapMax) {
    uint8_t pulseValue = Line[i].pulsCount % 10;
    char digit = pulseValue;  // Säkrare än String conversion

    Line[i].pulsing = false;
    Line[i].pulsCount = 0;
    Line[i].edgeTimestamp = millis();

    // Notify system of new digit
    Line[i].newDigitReceived(digit);
    return;
  }
}

#endif

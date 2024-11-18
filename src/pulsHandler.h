#ifndef PulsHandler_h
#define PulsHandler_h

#include <Arduino.h>
#include "config.h"

const unsigned pulseGapMin = 10;
const unsigned pulseGapMax = 60;

void pulseHandler(int line) {
  // Först, validera line-parametern för att undvika buffer overflow
  if (line < 0 || line >= activeLines) {  // MAX_LINES bör definieras i config.h
    Serial.println(F("Invalid line number in pulseHandler"));
    return;
  }

  // State change detection med debounce
  if (lineSystem.lineArray[line].SHKState != lineSystem.lineArray[line].lastSHKState) {
    Serial.println("Edge detected");
    lineSystem.lineArray[line].lastSHKBounceTime = millis();
    lineSystem.lineArray[line].edgeTimestamp = millis();
    Serial.print("Line "); Serial.print(line); Serial.print(" Pulsing: "); Serial.println(lineSystem.lineArray[line].pulsing);
    return;
  }

  // Debounce check
  if ((millis() - lineSystem.lineArray[line].lastSHKBounceTime) < pulseGapMin) {
    return;
  }

  // Beräkna gap om edgeTimeStamp är satt
  unsigned gap = lineSystem.lineArray[line].edgeTimestamp ? millis() - lineSystem.lineArray[line].edgeTimestamp : 0;

  if (lineSystem.lineArray[line].SHKState && lineSystem.lineArray[line].lastSHKState && gap <= pulseGapMax) {
    return;
  }

  if (!lineSystem.lineArray[line].SHKState && !lineSystem.lineArray[line].lastSHKState) {
    return;
  }

  // Falling edge detection
  if (!lineSystem.lineArray[line].SHKState && !lineSystem.lineArray[line].pulsing) {
    Serial.print("Line "); Serial.print(line); Serial.println(" falling edge detected");
    lineSystem.lineArray[line].pulsing = true;
    Serial.print("Line "); Serial.print(line); Serial.println(" pulsing");
    lineSystem.lineArray[line].edgeTimestamp = millis();
    return;
  }

  // Rising edge detection
  if (lineSystem.lineArray[line].SHKState && lineSystem.lineArray[line].pulsing) {
    Serial.print("Line "); Serial.print(line); Serial.println(" rising edge detected");
    lineSystem.lineArray[line].pulsing = false;
    lineSystem.lineArray[line].pulsCount++;
    Serial.print("Line "); Serial.print(line); Serial.print(" pulse count: "); Serial.println(lineSystem.lineArray[line].pulsCount);
    lineSystem.lineArray[line].edgeTimestamp = millis();

    if (lineSystem.lineArray[line].currentLineStatus != line_pulse_dialing) {
      lineSystem.setLineStatus(line, line_pulse_dialing);
    }
    return;
  }

  // Digit completion detection
  if (lineSystem.lineArray[line].SHKState && !lineSystem.lineArray[line].pulsing && gap > pulseGapMax) {
    uint8_t pulseValue = lineSystem.lineArray[line].pulsCount % 10;
    char digit = pulseValue;  // Säkrare än String conversion

    lineSystem.lineArray[line].pulsing = false;
    lineSystem.lineArray[line].pulsCount = 0;
    lineSystem.lineArray[line].edgeTimestamp = millis();

    // Notify system of new digit
    lineSystem.newDigitReceived(line, digit);
    return;
  }
}

#endif

#ifndef PulsHandler_h
#define PulsHandler_h

#include <Arduino.h>
#include "config.h"
#include "hookChange.h"

// Minsta och största tillåtna tidsintervall mellan pulser (i millisekunder)
const unsigned pulseGapMin = 10;  // Minimalt tidsintervall för att ignorera studs (debounce)
const unsigned pulseGapMax = 150; // Maximalt tidsintervall för att räkna en puls

// Funktion som hanterar pulssignaler
void pulseHandler(int line) {
  unsigned long currentTime = millis();
  // Detektering av tillståndsförändring (edge) med debounce
  if (Line[line].SHKState != Line[line].previousSHKState) {
    Serial.println("Edge detected");

    // Debounce: Ignorera kant om det har gått för kort tid
    unsigned debounceTime = currentTime - Line[line].lastSHKBounceTime;
    if (debounceTime < pulseGapMin) {
      Serial.print("Debounce active, ignoring edge (Debounce time: ");
      Serial.print(debounceTime);
      Serial.println(" ms)");
      return;
    }

    Line[line].lastSHKBounceTime = currentTime;  // Spara tiden för senaste förändring

    // Beräkna tidsgap sedan senaste kant
    unsigned gap = Line[line].edgeTimestamp ? currentTime - Line[line].edgeTimestamp : 0;
    Serial.print("Gap: "); Serial.println(gap);

    // Fallande kant (från hög till låg)
    if (!Line[line].SHKState) {
      Serial.print("Line "); Serial.print(line); Serial.println(" falling edge detected");
      Line[line].pulsing = true; // Indikerar att vi är i pulsläge
      Line[line].edgeTimestamp = currentTime; // Uppdatera kantens tid
      return;
    }

    // Stigande kant (från låg till hög)
    if (Line[line].SHKState && Line[line].pulsing) {
      Serial.print("i "); Serial.print(line); Serial.println(" rising edge detected");
      Line[line].pulsing = false; // Avsluta pulsläge
      Line[line].pulsCount++; // Öka antalet pulser
      Serial.print("i "); Serial.print(line); Serial.print(" pulse count: "); Serial.println(Line[line].pulsCount);
      Line[line].edgeTimestamp = currentTime; // Uppdatera kantens tid

      // Sätt linjestatus till "pulssignalering" om den inte redan är inställd
      if (Line[line].currentLineStatus != line_pulse_dialing) {
        Line[line].setLineStatus(line_pulse_dialing);
      }
      return;
    }
  }

  // Kontrollera om gapet indikerar färdigställd siffra
  if (!Line[line].pulsing && Line[line].pulsCount > 0 && (currentTime - Line[line].edgeTimestamp) > pulseGapMax) {
    Serial.println("Digit completion detected");
    uint8_t pulseValue = Line[line].pulsCount % 10; // Få den slutgiltiga siffran (1-9)
    char digit = pulseValue; // Konvertera till ett tecken

    Line[line].pulsing = false; // Nollställ pulsläge
    Line[line].pulsCount = 0; // Återställ pulsantalet
    Line[line].edgeTimestamp = currentTime; // Uppdatera kantens tid

    // Skicka vidare siffran till systemet
    Line[line].newDigitReceived(digit);
  }

  // Uppdatera föregående tillstånd
  Line[line].previousSHKState = Line[line].SHKState;
}

#endif

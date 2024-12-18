#ifndef SHKHandler_h
#define SHKHandler_h

#include <Arduino.h>
#include "config.h"
#include "hookChange.h"

// Minsta och största tillåtna tidsintervall mellan pulser (i millisekunder)
const unsigned pulseGapMin = 10;  // Minimalt tidsintervall för att ignorera studs (debounce)
const unsigned pulseGapMax = 150; // Maximalt tidsintervall för att räkna en puls


// Function to setup SHK pins with the MCP_KS083F
void setupSHKPins() {

  // Initialize MCP for KS830F
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
    Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
    //while(1);  // Stay in an infinite loop if initialization fails
    return;
  }
  Serial.println("MCP for KSF083f initialized successfully.");

  // Set all SHK pins as inputs with internal pull-up resistors
  for (int pinIndex = 0; pinIndex < activeLines; pinIndex++) {
    mcp_ks083f.pinMode(SHKPins[pinIndex], INPUT_PULLUP);
  }
  
}

void processSHKState() {
  // Läs GPIO-tillstånd från MCP-enheten
  uint16_t mcpState = mcp_ks083f.readGPIOAB();
  unsigned long currentTime = millis();

  // Loopa igenom alla aktiva linjer
  for (int line = 0; line < activeLines; line++) {
    bool newSHKState = bitRead(mcpState, SHKPins[line]);

    // Steg 1: Kontrollera om linjen är idle (ingen aktivitet pågår)
    if (Line[line].currentLineStatus == line_idle) {
      // Hantera långsamma ändringar (lur på/av)
      if (newSHKState != Line[line].previousSHKState) {
        Line[line].lastSHKBounceTime = currentTime;
      }

      // Kontrollera om ändringen är stabil
      if ((currentTime - Line[line].lastSHKBounceTime) > SHKDebouncingTime) {
        if (newSHKState == 1 && Line[line].hookStatus == hook_on) {
          Line[line].hookStatus = hook_off;
          hookChange(line, hook_off);
        } else if (newSHKState == 0 && Line[line].hookStatus == hook_off) {
          Line[line].hookStatus = hook_on;
          hookChange(line, hook_on);
        }
      }
      Line[line].previousSHKState = newSHKState;
      continue; // Ingen vidare bearbetning behövs om linjen är idle
    }

    // Steg 1.5: Kontrollera om linjen är redo för pulshantering
    if (!(Line[line].currentLineStatus == line_ready || Line[line].currentLineStatus == line_pulse_dialing)) {
      continue; // Hoppa över pulskontroll om båda statusvillkoren inte är uppfyllda
    }

    // Steg 2: Hantera pulser (snabba förändringar)
    if (newSHKState != Line[line].previousSHKState) {
      Serial.println("Edge detected");

      // Debounce: Ignorera kant om det har gått för kort tid
      unsigned debounceTime = currentTime - Line[line].lastSHKBounceTime;
      if (debounceTime < pulseGapMin) {
        Serial.print("Debounce active, ignoring edge (Debounce time: ");
        Serial.print(debounceTime);
        Serial.println(" ms)");
        continue;
      }

      Line[line].lastSHKBounceTime = currentTime;

      // Fallande kant (från hög till låg)
      if (!newSHKState && !Line[line].pulsing) {
        Serial.print("Line "); Serial.print(line); Serial.println(" falling edge detected");
        Line[line].pulsing = true;
        Line[line].edgeTimestamp = currentTime;
        continue;
      }

      // Stigande kant (från låg till hög)
      if (newSHKState && Line[line].pulsing) {
        Serial.print("Line "); Serial.print(line); Serial.println(" rising edge detected");
        Line[line].pulsing = false;
        Line[line].pulsCount++;
        Serial.print("Pulse count: "); Serial.println(Line[line].pulsCount);
        Line[line].edgeTimestamp = currentTime;

        // Sätt linjestatus till pulssignalering om det inte redan är inställt
        if (Line[line].currentLineStatus != line_pulse_dialing) {
          Line[line].setLineStatus(line_pulse_dialing);
        }
        continue;
      }
    }

    // Steg 3: Kontrollera om en siffra är klar (lång paus mellan pulser)
    unsigned gap = currentTime - Line[line].edgeTimestamp;
    if (!Line[line].pulsing && Line[line].pulsCount > 0 && gap > pulseGapMax) {
      Serial.println("Digit completion detected");
      char digit = (Line[line].pulsCount % 10) + '0'; // Konvertera till ASCII-siffra

      Line[line].pulsing = false;
      Line[line].pulsCount = 0;
      Line[line].edgeTimestamp = currentTime;

      // Skicka vidare siffran till systemet
      Line[line].newDigitReceived(digit);
    }

    // Uppdatera tidigare tillstånd
    Line[line].previousSHKState = newSHKState;
  }
}

#endif
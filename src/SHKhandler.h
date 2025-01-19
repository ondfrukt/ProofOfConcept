#ifndef SHKHandler_h
#define SHKHandler_h

#include <Arduino.h>
#include "config.h"

// Debouncing time in milliseconds
uint8_t SHKDebouncingTime = 10;

// Function to setup SHK pins with the MCP_KS083F 
void setupSHKPins() {

  // Initialize MCP for KS830F
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
    Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
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

  // Loopa igenom alla aktiva linjer
  for (int line = 0; line < activeLines; line++) {
    bool newSHK = bitRead(mcpState, SHKPins[line]);

    if (newSHK != Line[line].lastDebounceValue) {
      Line[line].lastDebounceTime = millis();
      Line[line].lastDebounceValue = newSHK;
      continue;
    }

    // Avbryter iterationen om inte tillräckligt med tid har passerat sedan föregående ändring
    if (millis() - Line[line].lastDebounceTime < SHKDebouncingTime) continue;
    
    pulsHandler(line, newSHK);


    if (newSHK && Line[line].hookStatus == hook_on && Line[line].pulsingFlag == false) {
        // Hook off detekterat
        
        Line[line].hookStatus = hook_off;
        hookChange(line, hook_off);
        
      } else if (!newSHK && Line[line].hookStatus == hook_off && Line[line].pulsingFlag == false) {
        // Hook on detekterat
        Line[line].hookStatus = hook_on;
        hookChange(line, hook_on);
        
      }
    }
  }


#endif
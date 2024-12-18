#ifndef SHKRead_h
#define SHKRead_h

#include <Arduino.h>
#include "config.h"
#include "hookChange.h"

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

// Function to read and debounce digital inputs from SHK pins
void SHKRead() {
  // Reads all GPIO pins in MCP_KS083F and stores the result in a 16-bit variable
  uint16_t mcpState = mcp_ks083f.readGPIOAB();
  unsigned long currentTime = millis();

  // Loop through all active is and store the current SHK state in currentSHKReading
  for (int line = 0; line < activeLines; line++) {

    Line[line].previousSHKState = Line[line].SHKState;
    Line[line].SHKState = bitRead(mcpState, SHKPins[line]);

    // Check if the pinIndex is pulsing, if true, skip the rest of the for-loop and continue with the next pinIndex
    if (Line[line].pulsing == true) {
      continue;
    }

    // Controlls if the current SHK input state has changed. If true, the current SHK state is updated and the last bounce time is updated
    if (Line[line].SHKState != Line[line].previousSHKState) {
      Line[line].lastSHKBounceTime = currentTime;
      continue; // Skip the rest of the for-loop
    }

    // Controlls if the debouncing time has passed, which means that the SHK state has changed and is stable
    if ((currentTime - Line[line].lastSHKBounceTime) > SHKDebouncingTime) {
      // If the debouncing time has passed, the SHK state has clearly changed and is stable and hook status can be updated
      if (Line[line].hookStatus == hook_on && Line[line].SHKState == 1) {
        Line[line].hookStatus = hook_off;
        hookChange(line, hook_off);
        
      } else if (Line[line].hookStatus == hook_off && Line[line].SHKState == 0) {
        Line[line].hookStatus = hook_on;
        hookChange(line, hook_on);
        
        }
    }
  }
}

#endif

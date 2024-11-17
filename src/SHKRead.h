#ifndef SHKRead_h
#define SHKRead_h

#include <Arduino.h>
#include "config.h"

// Function to setup SHK pins with the MCP_KS083F
void setupSHKPins() {

  // Initialize MCP for KS830F
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
    Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
    while(1);  // Stay in an infinite loop if initialization fails
  }
  Serial.println("MCP for KSF083f initialized successfully.");

  // Set all SHK pins as inputs with internal pull-up resistors
  for (int i = 0; i < activeLines; i++) {
    mcp_ks083f.pinMode(SHKPins[i], INPUT_PULLUP);
  }
  
}

// Function to read and debounce digital inputs from SHK pins
void SHKRead() {
  // Reads all GPIO pins in MCP_KS083F and stores the result in a 16-bit variable
  uint16_t mcpState = mcp_ks083f.readGPIOAB();
  unsigned long currentTime = millis();

  // Loop through all active lines and store the current SHK state in currentSHKReading
  for (int line = 0; line < activeLines; line++) {
    // Check if the line is pulsing, if true, skip the rest of the for-loop and continue with the next line
    if (lineSystem.lineArray[line].pulsing == true) {
      continue;
    }
    // Read the current SHK state from the MCP
    bool currentSHKReading = bitRead(mcpState, SHKPins[line]);

    // Get a reference to the current line data
    auto& lineData = lineSystem.lineArray[line]; // Get a reference to the current line data

    // Controlls if the current SHK input state has changed. If true, the current SHK state is updated and the last bounce time is updated
    if (lineData.SHKState != currentSHKReading) {
      lineSystem.lineArray[line].lastSHKBounceTime = currentTime; 
      lineSystem.lineArray[line].SHKState = currentSHKReading;
      continue; // Skip the rest of the for-loop
    }

    // Controlls if the debouncing time has passed, which means that the SHK state has changed and is stable
    if ((currentTime - lineData.lastSHKBounceTime) > SHKDebouncingTime) {
      // If the debouncing time has passed, the SHK state has clearly changed and is stable and hook status can be updated
      if (lineData.hookStatus == hook_on && currentSHKReading == 1) {
        lineSystem.lineArray[line].hookStatus = hook_off;
        hookChange(line, hook_off);
      } else if (lineData.hookStatus == hook_off && currentSHKReading == 0) {
        lineSystem.lineArray[line].hookStatus = hook_on;
        hookChange(line, hook_on);
        }
    }
  }
}

#endif

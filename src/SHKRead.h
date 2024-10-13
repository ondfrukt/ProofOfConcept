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

  // Loop through all active lines and store the current SHK state in pinRead
  for (int line = 0; line < activeLines; line++) {
    bool pinRead = bitRead(mcpState, SHKPins[line]);
    auto& lineData = lineSystem.lineVector[line]; // Get a reference to the current line data

    // Controlls if the current SHK input state has changed. If true, the current SHK state is updated and the last bounce time is updated
    if (lineData.currentSHKState != pinRead) {
      lineData.lastSHKBounceTime = currentTime; 
      lineData.currentSHKState = pinRead;
      continue; // Skip the rest of the for-loop
    }

    // Controlls if the debouncing time has passed, which means that the SHK state has changed and is stable
    if ((currentTime - lineData.lastSHKBounceTime) > SHKDebouncingTime) {
      if (lineData.hookStatus == hook_on && pinRead == 1) {
        lineSystem.lineVector[line].hookStatus = hook_off;
        hookChange(line, hook_off);
      } else if (lineData.hookStatus == hook_off && pinRead == 0) {
        lineSystem.lineVector[line].hookStatus = hook_on;
        hookChange(line, hook_on);
      }
    }
  }
}

#endif

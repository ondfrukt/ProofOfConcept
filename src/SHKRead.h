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


// Function to read and debounce digital inputs
void SHKRead(int line) {
  static uint32_t lastReadTime = 0;
  uint32_t currentTime = millis();  // Get the current time in milliseconds

  // Only read the MCP state if enough time has passed since the last read
  if (currentTime - lastReadTime >= SHKReadInterval) {
    lastReadTime = currentTime;

    // Read all GPIO pins in MCP_KS083F
    uint16_t mcpState = mcp_ks083f.readGPIOAB();
    bool pinRead = bitRead(mcpState, SHKPins[line]);

    // Check if the current SHK input state has changed
    if (lineSystem.lineVector[line].currentSHKState != pinRead) {
      lineSystem.lineVector[line].lastSHKBounceTime = currentTime;
      lineSystem.lineVector[line].currentSHKState = pinRead;  // Update the current state
    }

    // Check if the debounce time has passed
    if ((currentTime - lineSystem.lineVector[line].lastSHKBounceTime) > SHKDebouncingTime) {
      // If the state has changed, trigger the appropriate function
      if (bitRead(SHKState, line) == 1 && pinRead == 0) {
        lineSystem.lineVector[line].hookStatus = hook_off;
        hookChange(line, hook_off); // Call function for hook off
      }
      if (bitRead(SHKState, line) == 0 && pinRead == 1) {
        lineSystem.lineVector[line].hookStatus = hook_on;
        hookChange(line, hook_on); // Call function for hook on
      }
    }
  }
}

#endif

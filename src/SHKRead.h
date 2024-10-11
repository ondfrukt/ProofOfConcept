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
void SHKRead() {
  uint32_t currentTime = millis();  // Get the current time in milliseconds

  // Skip this read if the time since the last debounce is not enough
  if (currentTime - lastSHKDebounceTime < SHKDebouncing) {
    return;
  }

  // Read all GPIO pins in MCP_KS083F
  uint16_t mcpState = mcp_ks083f.readGPIOAB();

  uint8_t newSHKState = 0;
  bool changed = false;

  // Process only the specific SHK inputs
  for (int i = 0; i < activeLines; i++) {
    bool pinRead = bitRead(mcpState, SHKPins[i]);
    bitWrite(newSHKState, i, pinRead);
    
    // Check if the current SHK input state has changed
    if (bitRead(SHKState, i) != pinRead) {
      changed = true;
    }

    // If the state has changed, trigger the appropriate function
    if (bitRead(SHKState, i) == 1 && pinRead == 0) {
      hookChange(i, HOOK_OFF); // Call function for hook off
    }
    if (bitRead(SHKState, i) == 0 && pinRead == 1) {
      hookChange(i, HOOK_ON); // Call function for hook on
    }
  }

  // If any pin has changed, update the debounce time and state
  if (changed) {
    lastSHKDebounceTime = currentTime;
    SHKState = newSHKState;
    stateChanged = true;
  }
}

#endif

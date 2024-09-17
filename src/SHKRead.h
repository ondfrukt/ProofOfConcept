#ifndef SHKRead_h
#define SHKRead_h

#include <Arduino.h>
#include "config.h"

void setupSHKPins() {

  // Initiate MCP for KS830F
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
      Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
      while(1);
  }

  Serial.println("MCP for KSF083f initialized successfully.");

  for (int i = 0; i < NUM_INPUTS; i++) {
        mcp_ks083f.pinMode(SHKPins[i], INPUT_PULLUP);
  }
}

// Function to read and debounce digital inputs
void readSHK() {
  uint32_t currentTime = millis();
  if (currentTime - lastSHKDebounceTime < SHKDebouncing) {
    return; // Not enough time has passed, skip this read
  }

  // Reading all GPIO pins in MCP_KS083F
  uint16_t mcpState = mcp_ks083f.readGPIOAB();

  uint8_t newSHKState = 0;
  bool changed = false;

  // Process only the specific SHK inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    bool pinState = bitRead(mcpState, SHKPins[i]);
    bitWrite(newSHKState, i, pinState);
    
    // Check if the current SHK input state has changed
    if (bitRead(SHKState, i) != pinState) {
      changed = true;
    }

    if (bitRead(SHKState, i) == 1 && pinState == 0) {
        hookChange(i, HOOK_OFF); // Trigger function on hook off
      }
    if (bitRead(SHKState, i) == 0 && pinState == 1) {
        hookChange(i, HOOK_ON); // Trigger function on hook on
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
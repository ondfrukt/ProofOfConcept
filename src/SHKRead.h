#ifndef SHKRead_h
#define SHKRead_h

#include <Arduino.h>
#include "config.h"

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

  // Printing current SHK state for i 0 and 1 
  //Serial.print("i 0 SHK: "); Serial.print(bitRead(mcpState, SHKPins[0])); 
  //Serial.print(" | i 1 SHK: "); Serial.print(bitRead(mcpState, SHKPins[1]));
  //Serial.print(" | i 2 SHK: "); Serial.print(bitRead(mcpState, SHKPins[2]));
  //Serial.print(" | i 3 SHK: "); Serial.println(bitRead(mcpState, SHKPins[3]));


  // Loop through all active is and store the current SHK state in currentSHKReading
  for (int pinIndex = 0; pinIndex < activeLines; pinIndex++) {

    Line[pinIndex].previousSHKState = Line[pinIndex].SHKState;
    Line[pinIndex].SHKState = bitRead(mcpState, SHKPins[pinIndex]);

    // Check if the pinIndex is pulsing, if true, skip the rest of the for-loop and continue with the next pinIndex
    if (Line[pinIndex].pulsing == true) {
      continue;
    }

    // Controlls if the current SHK input state has changed. If true, the current SHK state is updated and the last bounce time is updated
    if (Line[pinIndex].SHKState != Line[pinIndex].previousSHKState) {
      Line[pinIndex].lastSHKBounceTime = currentTime;
      continue; // Skip the rest of the for-loop
    }

    // Controlls if the debouncing time has passed, which means that the SHK state has changed and is stable
    if ((currentTime - Line[pinIndex].lastSHKBounceTime) > SHKDebouncingTime) {
      // If the debouncing time has passed, the SHK state has clearly changed and is stable and hook status can be updated
      if (Line[pinIndex].hookStatus == hook_on && Line[pinIndex].SHKState == 1) {
        Line[pinIndex].hookStatus = hook_off;
        hookChange(pinIndex, hook_off);
        
      } else if (Line[pinIndex].hookStatus == hook_off && Line[pinIndex].SHKState == 0) {
        Line[pinIndex].hookStatus = hook_on;
        hookChange(pinIndex, hook_on);
        
        }
    }
  }
}

#endif

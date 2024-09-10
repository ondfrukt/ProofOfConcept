#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "config.h"

Adafruit_MCP23X17 mcp_ks083f;

void setupSHKPins() {
  // Initiate MCP for K
  for (int i = 0; i < NUM_INPUTS; i++) {
        mcp_ks083f.pinMode(SHKPins[i], INPUT_PULLUP);
  }
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
        Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
        return;
  }
    Serial.println("MCP for KSF083f initialized successfully.");
  Wire.begin();
}

// Function to read and debounce digital inputs
void readSHK() {
  unsigned long currentTime = millis();
  // Iterate through all inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    // Read the current state of the input
    int reading = mcp_ks083f.digitalRead(SHKPins[i]);

    // Check if the input has changed
    if (reading != lastSHKInputState[i]) {
      // Reset the debouncing timer
      lastSHKDebounceTime[i] = currentTime;
    }
    
    // If enough time has passed since the last change
    if ((currentTime - lastSHKDebounceTime[i]) > SHKDebouncing) {
      // If the input state has changed
      if (reading != SHKState[i]) {
        SHKState[i] = reading;  // Update the input state
        
        // Print the new state for debugging
        Serial.print("Input ");
        Serial.print(i);
        Serial.print(" changed to: ");
        Serial.println(SHKState[i]);
      }
    }
    // Save the current reading for the next loop iteration
    lastSHKInputState[i] = reading;
  }
}
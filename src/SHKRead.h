#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "config.h"

void setupSHKPins() {
  // Initialize all input pins as INPUT_PULLUP
    for (int i = 0; i < NUM_INPUTS; i++) {
        pinMode(inputPins[i], INPUT_PULLUP);
    }
    
  // Seting up MCP GPOI expander for KSF083F
  Adafruit_MCP23X17 mcp2;
  if (!mcp2.begin_I2C(mcp2_address)) {
        Serial.println("MCP23017 for KSF083f initialization failed. Check connections and address.");
        return;
    }
    Serial.println("MCP23017 for KSF083f initialized successfully.");
  Wire.begin();
}

// Function to read and debounce digital inputs
void readSHK() {
  unsigned long currentTime = millis();
  
  // Iterate through all inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    // Read the current state of the input
    int reading = digitalRead(inputPins[i]);

    // Check if the input has changed
    if (reading != lastInputState[i]) {
      // Reset the debouncing timer
      lastDebounceTime[i] = currentTime;
    }
    
    // If enough time has passed since the last change
    if ((currentTime - lastDebounceTime[i]) > SHKDebouncing) {
      // If the input state has changed
      if (reading != inputState[i]) {
        inputState[i] = reading;  // Update the input state
        
        // Print the new state for debugging
        Serial.print("Input ");
        Serial.print(i);
        Serial.print(" changed to: ");
        Serial.println(inputState[i]);
      }
    }
    // Save the current reading for the next loop iteration
    lastInputState[i] = reading;
  }
}
#include <Arduino.h>
#include "config.h"
#include "hookChange.h"
#include "SHKRead.h"
#include "lineAction.h"

LineSystem lineSystem;
Adafruit_MCP23X17 mcp_ks083f;
// MT8816 matrix;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupSHKPins();
}

// Function to handle line timer expiration
void lineTimerCheck() {
  for (int i = 0; i < activeLines; i++) {
    if (bitRead(lineTimerFlags, i) == 1) {
      if (millis() - lineSystem.getLineTimerStart(i) > lineSystem.getLineTimerLimit(i)) {
        lineTimerExpired(i);
      }
    }
  }
}

void loop() {
  readSHK();
  // Check if any line timers is active
  if (lineTimerFlags != 0) {
    lineTimerCheck();
  }
  delay(1);
}

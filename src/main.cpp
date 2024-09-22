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

void loop() {
  readSHK();

  // Checking if a timer is activ on a line
  if (lineSystem.lineTimerFlags != 0){
    for (int i=0; i < 8; i++){
      if (lineSystem.lineTimerFlags & (1 << i)){
        timerChecker(i);
      }
    }
  }






  delay(1);
}

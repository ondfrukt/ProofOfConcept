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


  //Reads SHK pins and triggers hookChange() if a change is detected. HookChange() will trigger lineAction() if a change is detected.
  SHKRead();

  // Check if any line timers is active and if it has expired and trigger the appropriate action in lineAction()
  if (lineTimerFlags != 0) {
    lineTimerCheck();
  }




  delay(1);

}

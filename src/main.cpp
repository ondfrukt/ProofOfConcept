#include <Arduino.h>
#include "main.h"
#include "config.h"
#include "hookChange.h"
#include "SHKRead.h"
#include "lineAction.h"
#include "pulsHandler.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Setup started");
  Wire.begin();
  i2CScanner();
  setupSHKPins();
  setupHookChecker();
  Serial.println("Setup complete");
}

void loop() {
  //Reads SHK pins and triggers hookChange() if a change is detected
  SHKRead();

  // Check if one or more lines are not idle
  if (lineSystem.allLinesIdle == false) {
    for (int i = 0; i < activeLines; i++) {
      auto& lineData = lineSystem.lineArray[i];
      
      // Check if the line status is not idle
      if (lineData.currentLineStatus != line_idle) {
        // Check if the line timer is active
        if (lineData.lineTimerActive) {
          // if the line timer has expired, trigger lineTimerExpired()
          if (millis() - lineData.lineTimerStart > lineData.lineTimerLimit) {
            lineTimerExpired(i);
          }
        }

        // Check if the line is ready or pulse dialing
        if (lineData.currentLineStatus == line_ready || lineData.currentLineStatus == line_pulse_dialing) {
        //Serial.print("Current SHK state: ");Serial.println(lineData.SHKState);
        pulseHandler(i);
        }
      }
    }
  }
}
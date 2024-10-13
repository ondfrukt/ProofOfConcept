#include <Arduino.h>
#include "main.h"
#include "config.h"
#include "hookChange.h"
#include "SHKRead.h"
#include "lineAction.h"

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupSHKPins();
}

void loop() {

  
  //Reads SHK pins and triggers hookChange() if a change is detected. HookChange() will trigger lineAction() if a change is detected.
  SHKRead();

  // Check if one or more lines are =! idle
  if (lineSystem.allLinesIdle == false) {
    for (int i = 0; i < activeLines; i++) {
      auto& lineData = lineSystem.lineVector[i];
      // Check if the line status is not idle
      if (lineData.currentLineStatus != line_idle) {
        // Check if the line timer is active
        if (lineData.lineTimerActive) {
          // if the line timer has expired, trigger lineTimerExpired()
          if (millis() - lineData.lineTimerStart > lineData.lineTimerLimit) {
            lineTimerExpired(i);
          }
        }

      // Check if the line status is ready to receive digits
      if (lineData.currentLineStatus == line_ready){

        }



      }
    }
  }

  delay(1);
}

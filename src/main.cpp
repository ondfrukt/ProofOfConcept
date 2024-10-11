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

  // Check if any line timers is active and if it has expired and trigger the appropriate action in lineAction()
  if (lineTimerFlags != 0) {
    lineTimerCheck();
  }

// loop over all lines and check if a status is not equal to idle
  for (int i = 0; i < activeLines; i++) {
    if (lineSystem.lineVector[i].currentStatus != line_idle) {
      // If the line is not idle, check if the line timer is active
      if (lineSystem.lineVector[i].lineTimerActive == true) {
        // If the line timer is active, check if the timer has expired
        if (millis() - lineSystem.lineVector[i].lineTimerStart > lineSystem.lineVector[i].lineTimerLimit) {
          // If the timer has expired, trigger the appropriate action in lineAction()
          lineTimerExpired(i);
        }
      }
  }
  }



  delay(1);

}

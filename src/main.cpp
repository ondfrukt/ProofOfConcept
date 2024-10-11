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
  for (int i = 0; i < activeLines; i++) {
    SHKRead(i);
  }

  // loop over all lines and check if a status is not equal to idle
  // If not idle, check if the line timer is active
  // If active, check if the timer has expired
  for (int i = 0; i < activeLines; i++) {
    if (lineSystem.lineVector[i].currentLineStatus != line_idle) {
      if (lineSystem.lineVector[i].lineTimerActive == true) {
        if (millis() - lineSystem.lineVector[i].lineTimerStart > lineSystem.lineVector[i].lineTimerLimit) {
          lineTimerExpired(i);
        }
      }
    }
  }
  delay(1);

}

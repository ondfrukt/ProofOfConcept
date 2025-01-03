#include <Arduino.h>
#include "config.h"
#include "hookChange.h"

#include "lineAction.h"
#include "pulsHandler.h"
#include "SHKHandler.h"

#include "LineHandler.h"
#include "mqttHandler.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");
  
  Wire.begin();
  i2CScanner();
  setupSHKPins();
  setupHookChecker();
  //mqttHandler.setupWiFi();
  //mqttHandler.setupMQTT();

  // Set the action callback for the MQTTHandler. This function will be called when MQTT message is received 
  //mqttHandler.setActionCallback(lineAction);
  Serial.println("Setup complete");
  Serial.println("");
}

void loop() {

  processSHKState();

  // Check if one or more lines are not idle
  if (allLinesIdle() == false) {
    digitalWrite(hookLED, true);
    for (int line = 0; line < activeLines; line++) {
      // Check if the line status is not idle
      if (Line[line].currentLineStatus != line_idle) {
        // Check if the line timer is active
        if (Line[line].lineTimerActive) {
          // if the line timer has expired, trigger lineTimerExpired()
          if (millis() - Line[line].lineTimerStart > Line[line].lineTimerLimit) {
            //lineTimerExpired(line);
            }
          }
        }
      }
    }
  // Handle MQTT messages
  //mqttHandler.loop();
  delay(1);
}
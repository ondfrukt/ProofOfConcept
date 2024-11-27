#include <Arduino.h>
#include "config.h"
#include "hookChange.h"
#include "SHKRead.h"
#include "lineAction.h"
#include "pulsHandler.h"
#include "LineHandler.h"
#include "mqttHandler.h"

void setup() {
  Serial.begin(115200);

  Serial.println("Setup started");
  Wire.begin();
  i2CScanner();
  setupSHKPins();
  setupHookChecker();

  mqttHandler.setupWiFi();
  mqttHandler.setupMQTT();

  // Set the action callback for the MQTTHandler. This function will be called when MQTT message is received 
  mqttHandler.setActionCallback(lineAction);
  Serial.println("Setup complete");
}


void loop() {
  //Reads SHK pins and triggers hookChange() if a change is detected
  SHKRead();
  // Check if one or more lines are not idle
  if (allLinesIdle() == false) {
    for (int i = 0; i < activeLines; i++) {
      // Check if the line status is not idle
      if (Line[i].currentLineStatus != line_idle) {
        // Check if the line timer is active
        if (Line[i].lineTimerActive) {
          // if the line timer has expired, trigger lineTimerExpired()
          if (millis() - Line[i].lineTimerStart > Line[i].lineTimerLimit) {
            //lineTimerExpired(i);
          }
        }

        // Check if the line is ready or pulse dialing
        if (Line[i].currentLineStatus == line_ready || Line[i].currentLineStatus == line_pulse_dialing) {

        //pulseHandler(i);

        }
      }
    }
  }

  // Handle MQTT messages
  mqttHandler.loop();
  delay(1);
}
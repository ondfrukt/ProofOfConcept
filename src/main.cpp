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
  setupMCPPins();
  mt8816.begin();

  ToneGen.begin(50000);

  setupHookChecker();
  //mqttHandler.setupWiFi();
  //mqttHandler.setupMQTT();
  

  // Pin modes
  pinMode(hookLED, OUTPUT);
  digitalWrite(hookLED, LOW);
  pinMode(testButton1, INPUT_PULLUP);

  // Set the action callback for the MQTTHandler. This function will be called when MQTT message is received 
  //mqttHandler.setActionCallback(lineAction);

  Serial.println("Setup complete");
  Serial.println("");
}


void loop() {

  ToneGen.update();
  processSHKState();

  if (allLinesIdle() == false) {
    for (int line = 0; line < activeLines; line++) {
      // Check if the line status is not idle
      if (Line[line].currentLineStatus != line_idle) {
        // Check if the line timer is active
        if (Line[line].lineTimerActive) {
          // if the line timer has expired, trigger lineTimerExpired()
          if (millis() - Line[line].lineTimerStart > Line[line].lineTimerLimit) {
            lineTimerExpired(line);
            }
          }
        }
      }
    }
  // Test button. Generates a call from line 2 to line 3 if line 3 is ready (Hook is off)
  if (digitalRead(testButton1) == LOW) {
    if (Line[2].currentLineStatus == line_ready){
         Line[2].dialedDigits = String(3);
        lineAction(2, line_ringing);
    }
  }
  // Handle MQTT messages
  //mqttHandler.loop();
  delay(1);
}
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


  setupHookChecker();
  //mqttHandler.setupWiFi();
  //mqttHandler.setupMQTT();
  

  // Pin modes
  pinMode(hookLED, OUTPUT);
  digitalWrite(hookLED, LOW);

  pinMode(testButton1, INPUT_PULLUP);

  // Set the action callback for the MQTTHandler. This function will be called when MQTT message is received 
  //mqttHandler.setActionCallback(lineAction);

  mt8816.connect(0, 1);
  mt8816.connect(1, 0);


  Serial.println("Setup complete");
  Serial.println("");
}


void loop() {

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


  if (digitalRead(testButton1) == LOW) {

    Serial.println("Test Ring");
    ringHandler.generateRingSignal(3);
  }


  // Handle MQTT messages
  //mqttHandler.loop();
  delay(1);
}
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

  // for (int x = 0; x < 8; x++) {
  //   for (int y = 0; y < 8; y++) {
  //     mt8816.connect(x, y);
  //   }
  // }
  mt8816.connect(2, 3);


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
    
    for (int i = 0; i < 10; i++) {
    digitalWrite(wifiLED, HIGH);
    delay(50);
    digitalWrite(wifiLED, LOW);
    delay(50);
    }
    mt8816.connect(0, 1);
    
  }


  // Handle MQTT messages
  //mqttHandler.loop();
  delay(1);
}
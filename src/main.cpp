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
  setupMT8870();

  setupHookChecker();
  //mqttHandler.setupWiFi();
  //mqttHandler.setupMQTT();
  

  // Pin modes
  pinMode(hookLED, OUTPUT);
  digitalWrite(hookLED, LOW);
  pinMode(testButton1, INPUT_PULLUP);

  // Set the action callback for the MQTTHandler. This function will be called when MQTT message is received 
  //mqttHandler.setActionCallback(lineAction);

  // connect all Aout_x to the X-line
  mt8816.connectAout_x();

  Serial.println("Setup complete");
  Serial.println("");
}


void loop() {

  processSHKState();

  if (allLinesIdle() == false) {

    // Line timer Check
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
    
    if (digitalRead(STD) == HIGH) {  // Om en DTMF-signal detekteras
      int q1 = digitalRead(Q1);
      int q2 = digitalRead(Q2);
      int q3 = digitalRead(Q3);
      int q4 = digitalRead(Q4);

      int dtmf_value = (q4 << 3) | (q3 << 2) | (q2 << 1) | q1; // Kombinera bitarna till en siffra

      Serial.print("Mottagen DTMF-siffra: ");
      Serial.println(dtmf_value);
      
      delay(300); // Vänta en stund för att undvika flera avläsningar av samma signal
  }


  if (digitalRead(testButton1) == LOW) {

    // Test Ring
    // Serial.println("Test Ring");
    // for (int i = 0; i < 10; i++) {
    // digitalWrite(wifiLED, HIGH);
    // delay(50);
    // digitalWrite(wifiLED, LOW);
    // delay(50);
    // }


    // Test Aout_x Connections
    // if (!Aout_x){
    //   mt8816.connectAout_x();
    //   Aout_x = true;
    // } else {
    //   mt8816.disconnectAout_x();
    //   Aout_x = false;
    // }


    mt8816.printConnections();
  }


  // Handle MQTT messages
  //mqttHandler.loop();
  delay(1);
}
#include <Arduino.h>
#include "config.h"

#include "lineAction.h"
#include "pulsHandler.h"
#include "SHKHandler.h"
#include "toneHandler.h"


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

  // Handle MQTT messages
  //mqttHandler.loop();

  SHKHandler();

  if (allLinesIdle() == false) {
    timerHandler();
  }

  toneHandler();

  if (digitalRead(testButton1) == LOW) {

    mt8816.printConnections();
    
  }
  delay(1);
}

#include <Arduino.h>
#include "configurations.h"
#include <Adafruit_MCP23X17.h>
#include "SHKRead.h"

void setup() {
  Serial.begin(115200);
  setupSHKPins();


}

void loop() {
  delay(1);
}

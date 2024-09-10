#include <Arduino.h>
#include "config.h"
#include "SHKRead.h"

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupSHKPins();

}

void loop() {
  delay(1);
}

#include <Arduino.h>
#include "config.h"
#include "hookChange"
#include "SHKRead.h"





void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupSHKPins();
}

void loop() {
  readSHK();

  delay(1);
}

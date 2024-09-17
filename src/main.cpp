#include <Arduino.h>
#include "config.h"
#include "hookChange"
#include "SHKRead.h"

LineSystem lineSystem;
Adafruit_MCP23X17 mcp_ks083f;
// MT8816 matrix;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupSHKPins();
}

void loop() {
  readSHK();

  delay(1);
}

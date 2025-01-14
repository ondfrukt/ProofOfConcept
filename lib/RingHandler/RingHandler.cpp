#include <Arduino.h>
#include <Wire.h>
#include "ringHandler.h"

RingHandler::RingHandler(Adafruit_MCP23X17& mcp, const uint8_t _activeLines, const uint8_t* _RMpins, const uint8_t* _FRpins, int _ringLength) 
    : mcp_ks083f(mcp) {
  activeLines = _activeLines;
  RMPins = _RMpins;
  FRPins = _FRpins;
  ringLength = _ringLength;
  iterations = ringLength / (2 * 25);  // 25 mS = half of 20 Hz period

  for (int i = 0; i < activeLines; i++) {
    mcp_ks083f.pinMode(RMPins[i], OUTPUT);
    mcp_ks083f.pinMode(FRPins[i], OUTPUT);
  }


}

// generate a test ring signal
void RingHandler::generateRingSignal(int line) {
  // check if phone is on-hook before ringing, then generate a short ring at 20 Hz
  // this should be implemented better, maybe with interrupts so that as soon as the
  // phone is picked up, ring generation stops but this is ok for testing
  
  if (line < 0 || line >= activeLines) {
    Serial.println("Fel: Ogiltigt linjeindex!");
    return;
  }

  Serial.println("Ringing line " + String(line));
  mcp_ks083f.digitalWrite(RMPins[line], HIGH);        // enable ring mode


  for (int i = 0; i < iterations; i++) {
    mcp_ks083f.digitalWrite(FRPins[line], HIGH);      // toggle fwd/rev pin to generate ring
    delay(25);  // 25 mS = half of 20 Hz period
    mcp_ks083f.digitalWrite(FRPins[line], LOW);
    delay(25);  // 25 mS = half of 20 Hz period
  }
  mcp_ks083f.digitalWrite(RMPins[line], LOW);         // disable ring mode
};
#ifndef PulsHandler_h
#define PulsHandler_h

#include <Arduino.h>
#include "config.h"

const unsigned pulseGapMin = 10;
const unsigned pulseGapMax = 60;


void pulseHandler(int line) {
  bool currentSHKReading = mcp_ks083f.digitalRead(SHKPins[line]);
  auto& lineData = lineSystem.lineArray[line];
  bool lastSHKState = lineData.SHKState;
  unsigned long lastSHKDebounceTime = lineData.lastSHKBounceTime;
  unsigned long edgeTimeStamp = lineData.edgeTimestamp;
  unsigned long currentTime = millis();
  
  // If SHK state has changed, update the last bounce time and the current SHK state and return
  if(currentSHKReading != lastSHKState) {
    lineSystem.lineArray[line].SHKState = currentSHKReading;
    lineSystem.lineArray[line].lastSHKBounceTime = currentTime;
    return;
  }

  // If the debouncing time has passed, the SHK state has changed and is stable. Else, return
  if((currentTime - lastSHKDebounceTime) < pulseGapMin) return;

  // If edgeTimeStamp is not 0, calculate the gap between the current time and the edgeTimeStamp. Else, set gap to 0
  unsigned gap = edgeTimeStamp ? currentTime - edgeTimeStamp : 0;

  // SHK pin is high and unchanged, skip until longer gap between digits
  if(currentSHKReading && lastSHKState && gap <= pulseGapMax) return;
  // SHK pin is low and unchanged, skip always
  if(!currentSHKReading && !lastSHKState) return;

  // If the line is pulsing it means that the SHK pin is currently low and the line is pulsing
  bool pulsing = lineData.pulsing;
  uint32_t currentLineStatus = lineData.currentLineStatus;
  
  // Falling edge
  if(!currentSHKReading && !pulsing) {
    lineSystem.lineArray[line].pulsing = true;
    lineSystem.lineArray[line].edgeTimestamp = currentTime;
    return;
  }

  // Rising edge
  if(currentSHKReading && pulsing){
    lineSystem.lineArray[line].pulsing = false;
    lineSystem.lineArray[line].pulsCount++;
    lineSystem.lineArray[line].edgeTimestamp = currentTime;
    
    // If the line is not dialing, set the line status to pulse dialing and trigger the dialingStartedCallback
    if(currentLineStatus != line_pulse_dialing){ 
      lineSystem.setLineStatus(line, line_pulse_dialing);
    }
    return;
  }

  // If the SHK pin is high and not pulsing and the gap between the current time and the edgeTimeStamp is greater than the pulseGapMax, 
    // set the pulsing flag to false, 
    // set the pulsCount to 0, 
    // set the edgeTimeStamp to 0 
    // and trigger the digitReceivedCallback
  
  if(currentSHKReading && !pulsing && gap > pulseGapMax){
    char digit = String(lineSystem.lineArray[line].pulsCount % 10)[0];
    pulsing = false;
    lineSystem.lineArray[line].pulsCount = 0;
    lineSystem.lineArray[line].edgeTimestamp = 0;
    lineSystem.newDigitReceived(line, digit);
    return;
  }
}

#endif
#ifndef hookChange_h
#define hookChange_h

#include <Arduino.h>
#include "config.h"
#include "lineAction.h"

// Function to handle changes in hook state
void hookChange(int line, hookStatuses newHookState) {

  // ------------- Hook OFF -------------------
  if (newHookState == hook_off) {

    // ---> Ready
    if (lineSystem.lineVector[line].currentLineStatus == line_idle) {
      lineSystem.setLineStatus(line, line_ready);
      Serial.print("Line "); Serial.print(line); Serial.println(" Ready");
      lineAction(line, line_ready);
      return;
    }

    // ---> Connected
    if (lineSystem.lineVector[line].currentLineStatus == line_incoming) {
      lineSystem.setLineStatus(line, line_connected);
      Serial.print("Line "); Serial.print(line); Serial.println(" Connected");
      lineAction(line, line_connected);
      return;
    }
  }

  // ------------- Hook ON -------------------
  if (newHookState == hook_on) {
    uint32_t currentLineStatus = lineSystem.lineVector[line].currentLineStatus;

    // ---> Idle
    if (
      currentLineStatus == line_connected || 
      currentLineStatus == line_disconnected || 
      currentLineStatus == line_abandoned || 
      currentLineStatus == line_busy ||
      currentLineStatus == line_fail ||
      currentLineStatus == line_timeout) {
      
      lineSystem.setLineStatus(line, line_idle);
      Serial.print("Line "); Serial.print(line); Serial.println(" Idle");
      lineAction(line, line_idle);
      return;
    }

    // ---> Disconnected
    if (currentLineStatus == line_ringing) {
      lineSystem.setLineStatus(line, line_disconnected);
      Serial.print("Line "); Serial.print(line); Serial.println(" Disconnected");
      lineAction(line, line_disconnected);
      return;
    }
  }
}

#endif

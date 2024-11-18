#ifndef hookChange_h
#define hookChange_h

#include <Arduino.h>
#include "config.h"
#include "lineAction.h"




// Read SHK pins and trigger hookChange() if one or more lines are off-hook

// Function to handle changes in hook state
void hookChange(int line, hookStatuses newHookState) {
  // ------------- Hook OFF -------------------
  if (newHookState == hook_off) {
    //Serial.print("Line "); Serial.print(line); Serial.println(" Hook OFF");

    // ---> Ready
    if (lineSystem.lineArray[line].currentLineStatus == line_idle) {
      lineSystem.setLineStatus(line, line_ready);
      lineAction(line, line_ready);
      return;
    }

    // ---> Connected
    if (lineSystem.lineArray[line].currentLineStatus == line_incoming) {
      lineSystem.setLineStatus(line, line_connected);
      lineAction(line, line_connected);
      return;
    }
  }

  // ------------- Hook ON -------------------
  if (newHookState == hook_on) {
    //Serial.print("Line "); Serial.print(line); Serial.println(" Hook ON");
    uint32_t currentLineStatus = lineSystem.lineArray[line].currentLineStatus;
    // ---> Idle
    if (
      currentLineStatus == line_ready || 
      currentLineStatus == line_connected || 
      currentLineStatus == line_disconnected || 
      currentLineStatus == line_abandoned || 
      currentLineStatus == line_busy ||
      currentLineStatus == line_fail ||
      currentLineStatus == line_timeout) {

      
      lineSystem.setLineStatus(line, line_idle);
      lineAction(line, line_idle);
      return;
    }

    // ---> Disconnected
    if (currentLineStatus == line_ringing) {
      lineSystem.setLineStatus(line, line_disconnected);
      lineAction(line, line_disconnected);
      return;
    }
  }
}

// Function to set correct hook status on boot
void setupHookChecker(){
  Serial.println("Initial hook statuses");
  uint16_t mcpState = mcp_ks083f.readGPIOAB();
  for (int i = 0; i < activeLines; i++) {
    bool currentSHKReading = bitRead(mcpState, SHKPins[i]);
    if (currentSHKReading == 0) {
      lineSystem.lineArray[i].hookStatus = hook_on;
      Serial.print("Line "); Serial.print(i); Serial.println(" Hook ON");
    } else {
      lineSystem.lineArray[i].hookStatus = hook_off;
      Serial.print("Line "); Serial.print(i); Serial.println(" Hook OFF");
    }
  }
}

#endif

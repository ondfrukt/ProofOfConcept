#ifndef hookChange_h
#define hookChange_h

#include <Arduino.h>
#include "config.h"
#include "lineAction.h"




// Read SHK pins and trigger hookChange() if one or more lines are off-hook

// Function to handle changes in hook state
void hookChange(int i, hookStatuses newHookState) {
  Serial.print("Line "); Serial.print(i); Serial.print(" Hook state changed to: "); Serial.println(newHookState);
  // ------------- Hook OFF -------------------
  if (newHookState == hook_off) {
    //Serial.print("Line "); Serial.print(line); Serial.println(" Hook OFF");

    // ---> Ready
    if (Line[i].currentLineStatus == line_idle) {
      Line[i].setLineStatus(line_ready);
      lineAction(i, line_ready);
      return;
    }

    // ---> Connected
    if (Line[i].currentLineStatus == line_incoming) {
      Line[i].setLineStatus(line_connected);
      lineAction(i, line_connected);
      return;
    }
  }

  // ------------- Hook ON -------------------
  if (newHookState == hook_on) {
    uint32_t currentLineStatus = Line[i].currentLineStatus;
    // ---> Idle
    if (
      currentLineStatus == line_ready || 
      currentLineStatus == line_connected || 
      currentLineStatus == line_disconnected || 
      currentLineStatus == line_abandoned || 
      currentLineStatus == line_busy ||
      currentLineStatus == line_fail ||
      currentLineStatus == line_timeout) {

      
      Line[i].setLineStatus(line_idle);
      lineAction(i, line_idle);
      return;
    }

    // ---> Disconnected
    if (currentLineStatus == line_ringing) {
      Line[i].setLineStatus(line_disconnected);
      lineAction(i, line_disconnected);
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
      Line[i].hookStatus = hook_on;
      Serial.print("Line "); Serial.print(i); Serial.println(" Hook ON");
    } else {
      Line[i].hookStatus = hook_off;
      Serial.print("Line "); Serial.print(i); Serial.println(" Hook OFF");
    }
  }
}

#endif

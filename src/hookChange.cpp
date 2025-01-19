#include "hookChange.h"

// Function to handle changes in hook state
void hookChange(int line, hookStatuses newHookState) {

  // ------------- Hook OFF -------------------
  if (newHookState == hook_off) {

    // ---> Ready
    if (Line[line].currentLineStatus == line_idle) {
      Line[line].setLineStatus(line_ready);
      lineAction(line, line_ready);
      return;
    }

    // ---> Connected
    if (Line[line].currentLineStatus == line_incoming) {
      Line[line].setLineStatus(line_connected);
      lineAction(line, line_connected);
      return;
    }
  }

  // ------------- Hook ON -------------------
  if (newHookState == hook_on) {
    uint32_t currentLineStatus = Line[line].currentLineStatus;
    // ---> Idle
    if (
      currentLineStatus == line_ready || 
      currentLineStatus == line_connected || 
      currentLineStatus == line_disconnected || 
      currentLineStatus == line_abandoned || 
      currentLineStatus == line_busy ||
      currentLineStatus == line_fail ||
      currentLineStatus == line_timeout ||
      currentLineStatus == line_pulse_dialing){

      
      Line[line].setLineStatus(line_idle);
      Line[line].dialedDigits = ""; // Clear dialed digits if any
      lineAction(line, line_idle);
      
      return;
    }

    // ---> Disconnected
    if (currentLineStatus == line_ringing) {
      Line[line].setLineStatus(line_disconnected);
      lineAction(line, line_disconnected);
      return;
    }
  }
}

// Function to set correct hook status on boot
void setupHookChecker(){

  // Read all GPIO pins from the MCP
  uint16_t mcpState = mcp_ks083f.readGPIOAB();

  // Loops through all SHK pins and sets the hook status
  for (int line = 0; line < activeLines; line++) {
    bool currentSHKReading = bitRead(mcpState, SHKPins[line]);
    if (currentSHKReading == 0) {
      Line[line].hookStatus = hook_on;
      Serial.print("Line "); Serial.print(line); Serial.println(" Hook ON");
    } else {
      Line[line].hookStatus = hook_off;
      Serial.print("Line "); Serial.print(line); Serial.println(" Hook OFF");
    }
  }
}
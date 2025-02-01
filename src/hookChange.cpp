#include "hookChange.h"

// Function to handle changes in hook state
void hookChange(int line, hookStatuses newHookState) {

  // ------------- Hook OFF -------------------
  if (newHookState == hook_off) {

    // ---> Ready
    if (Line[line].currentLineStatus == line_idle) {
      lineAction(line, line_ready);
      return;
    }

    // ---> Connected
    if (Line[line].currentLineStatus == line_incoming) {
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
      currentLineStatus == line_disconnected || 
      currentLineStatus == line_abandoned || 
      currentLineStatus == line_busy ||
      currentLineStatus == line_fail ||
      currentLineStatus == line_timeout ||
      currentLineStatus == line_pulse_dialing){

      lineAction(line, line_idle);
      return;
    }
    
    // Disconnect the line if it is connected
    if (currentLineStatus == line_connected){
      Serial.println("Connection between line " + String(line) + " and " + String(Line[line].connectedTo) + " disconnected");
      lineAction(Line[line].connectedTo, line_disconnected);
      lineAction(line, line_idle);
      return;
    }
    // Resets the status for the called and the calling line
    if (currentLineStatus == line_ringing){
      Serial.println("Connection between line " + String(line) + " and " + String(Line[line].connectedTo) + " disconnected");
      if (Line[Line[line].connectedTo].currentLineStatus == line_incoming){
        lineAction(Line[line].connectedTo, line_idle);
      }
      lineAction(line, line_idle);
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
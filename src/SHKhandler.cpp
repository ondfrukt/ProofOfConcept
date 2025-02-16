#include "SHKHandler.h"


// Function to handle changes in hook state
void SHKHandler() {
  // Läs GPIO-tillstånd från MCP-enheten
  uint16_t mcpState = mcp_ks083f.readGPIOAB();

  // Loopa igenom alla aktiva linjer
  for (int line = 0; line < activeLines; line++) {
    bool newSHK = bitRead(mcpState, SHKPins[line]);

    if (newSHK != Line[line].lastDebounceValue) {
      Line[line].lastDebounceTime = millis();
      Line[line].lastDebounceValue = newSHK;
      continue;
    }

    // Avbryter iterationen om inte tillräckligt med tid har passerat sedan föregående ändring
    if (millis() - Line[line].lastDebounceTime < SHKDebouncingTime) continue;
    
    pulsHandler(line, newSHK);


    if (newSHK && Line[line].hookStatus == hook_on && Line[line].pulsingFlag == false) {
        // Hook off detekterat
        
        Line[line].hookStatus = hook_off;
        hookChange(line, hook_off);
        
      } else if (!newSHK && Line[line].hookStatus == hook_off && Line[line].pulsingFlag == false) {
        // Hook on detekterat
        Line[line].hookStatus = hook_on;
        hookChange(line, hook_on);
        
      }
    }
  }

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
      currentLineStatus == line_pulse_dialing ||
      currentLineStatus == line_tone_dialing ||
      currentLineStatus == line_ringing){

      lineAction(line, line_idle);
      return;
    }
    
    if (currentLineStatus == line_connected){
      Serial.println("Line " + String(line) + "   OutgingTo = " + String(Line[line].outgoingTo) + "   IncomingFrom = " + String(Line[line].incomingFrom));
      lineAction(Line[line].outgoingTo, line_disconnected);
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
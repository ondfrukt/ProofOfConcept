#include "SHKhandler.h"

void processSHKState() {
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
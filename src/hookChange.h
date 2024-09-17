#ifndef hookChange_h
#define hookCange_h

#include <Arduino.h>
#include "config.h"
#include "actions.h"

void hookChange(int line, HookState newHookState ){

  // -------------  Hook OFF -------------------
  if (newHookState == HOOK_OFF){

    // --- > Ready
    if (lineSystem.getCurrentLineStatus(line) == line_idle){
      lineSystem.setLineStatus(line, line_ready);
      Serial.print("Line "); Serial.print(line); Serial.println(" Ready");
      lineAction(line, line_ready);
      return;
    }

    // ---> connected
    if (lineSystem.getCurrentLineStatus(line) == line_incoming){
      lineSystem.setLineStatus(line, line_connected);
      Serial.print("Line "); Serial.print(line); Serial.println(" Connected");
      lineAction(line, line_connected);
      return;
    }
  }

  // -------------  Hook ON -------------------
  if (newHookState == HOOK_ON){
     statuses CurrentLineStatus = lineSystem.getCurrentLineStatus(line);

    // ---> idel
    if (
      CurrentLineStatus == line_connected || 
      CurrentLineStatus == line_disconnected || 
      CurrentLineStatus == line_abandoned || 
      CurrentLineStatus == line_busy ||
      CurrentLineStatus == line_fail ||
      CurrentLineStatus == line_timeout ){
      
      lineSystem.setLineStatus(line, line_idle);
      Serial.print("Line "); Serial.print(line); Serial.println(" Idle");
      lineAction(line, line_idle);
      return;
    }

    //  ---> disconnected
    if (CurrentLineStatus == line_ringing){
      lineSystem.setLineStatus(line, line_idle);
      Serial.print("Line "); Serial.print(line); Serial.println(" Idle");
      lineAction(line, line_disconnected);
      return;
    }
  }
};

#endif

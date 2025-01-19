#include "pulsHandler.h"

void pulsHandler(int line, bool newSHK){

  Line[line].gap = Line[line].edge ? millis() - Line[line].edge : 0;

  if(newSHK && Line[line].SHK && Line[line].gap <= pulseGapMax) return; // SHK high and unchanged, skip until long gap between digits
  if(!newSHK && !Line[line].SHK && Line[line].gap <= pulseGapMax) return; // SHK low and unchanged, skip always
  
  Line[line].SHK = newSHK;

  // Chek if hook on has been detected as pulsing
  if (Line[line].pulsing && Line[line].gap > gapTimeout) {
    Line[line].pulsing = false;
    Line[line].pulsCount = 0;
    Line[line].edge = 0;
    Line[line].pulsingFlag = false;
    return;
  }

  // Falling pulse edge
  if(!Line[line].SHK && !Line[line].pulsing) {
    Line[line].pulsing = true;
    Line[line].edge = millis();
    Line[line].pulsingFlag = true;
    return;
  }

    // Rising pulse edge
  if(Line[line].SHK && Line[line].pulsing){
    Line[line].pulsing = false;
    Line[line].pulsCount++;
    Line[line].edge = millis();
    if(Line[line].currentLineStatus != line_pulse_dialing) { 
      Line[line].currentLineStatus = line_pulse_dialing;
    }
    return;
  }

  // Summarize the pulses after a long positive edge gap
  if(Line[line].SHK && !Line[line].pulsing && Line[line].gap > pulseGapMax){
    char digit = String(Line[line].pulsCount - 1 % 10)[0]; // "-1" due to Sweden has "0" as first digit
    Line[line].pulsing = false;
    Line[line].pulsCount = 0;
    Line[line].edge = 0;
    Line[line].pulsingFlag = false;
    void newDigitReceived(char digit);
    Serial.println("Digit received: " + String(digit));
    return;
  }
}
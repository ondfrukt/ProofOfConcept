#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"

void ring(int line) {
  Serial.println("Ringing line " + String(line + 1));
}

// Function to determine the action to take based on the new line status
void lineAction(int line, lineStatuses newLineStatus) {
  
  switch (newLineStatus) {
    case line_idle:
      // Action
      break;

    case line_ready:
      // Action
      break;

    case line_tone_dialing:
      // Action
      lineSystem.startLineTimer(line, statusTimer_toneDialing);
      break;

    case line_pulse_dialing:
      // Action
      lineSystem.startLineTimer(line, statusTimer_pulsDialing);
      break;

    case line_connected:
      // Action
      break;

    case line_disconnected:
      // Action
      break;

    case line_incoming:
      // Action
      break;

    case line_ringing:
      lineSystem.startLineTimer(line, statusTimer_Ringing);
      dialedDigits = lineSystem.lineVector[line].diledDigits;
      for (int i = 0; i < activeLines; i++) {
        // Check if the diled digits match a number in the phonebook and the line is idle
        if (dialedDigits == lineSystem.phoneBook[i] && lineSystem.lineVector[i].currentLineStatus == line_idle) {
          ring(i);
        }
      }
      break;

    case line_timeout:
      // Action
      break;

    case line_abandoned:
      // Action
      break;

    case line_busy:
      lineSystem.lineVector[line].diledDigits = "";
      // Action
      break;

    case line_fail:
      lineSystem.lineVector[line].diledDigits = "";
      // Action
      break;

    case line_operator:
      // Action
      break;

    default:
      // Handle unexpected status
      break;
  }
}

// Function to determine the action to take based on the line status due to a timer expiration
void lineTimerExpired(int line) {
  lineSystem.stopLineTimer(line);
  uint32_t currentStatus = lineSystem.lineVector[line].currentLineStatus;

  switch (currentStatus) {
    case line_ready:
    case line_fail:
    case line_busy:
    case line_disconnected:
      lineSystem.setLineStatus(line, line_timeout);
      lineAction(line, line_timeout);
      break;

    case line_timeout:
      lineSystem.setLineStatus(line, line_abandoned);
      lineAction(line, line_abandoned);
      break;

    case line_incoming:
      lineSystem.setLineStatus(line, line_idle);
      lineAction(line, line_idle);
      break;

    case line_pulse_dialing:
    case line_tone_dialing:
      lineAction(line, line_ringing);
      break;

    default:
      // Handle unexpected status if necessary
      break;
  }
}


#endif
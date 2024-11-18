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
      Line[line].startLineTimer(statusTimer_toneDialing);
      break;

    case line_pulse_dialing:
      // Action
      Line[line].startLineTimer(statusTimer_pulsDialing);
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
      Line[line].startLineTimer(statusTimer_Ringing);
      dialedDigits = Line[line].dialedDigits;
      for (int i = 0; i < activeLines; i++) {
        // Check if the diled digits match a number in the phonebook and the line is idle
        if (dialedDigits == Line[i].phoneNumber && Line[i].currentLineStatus == line_idle) {
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
      Line[line].dialedDigits = '\0';
      // Action
      break;

    case line_fail:
      Line[line].dialedDigits = '\0';
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
  Line[line].stopLineTimer();
  uint32_t currentStatus = Line[line].currentLineStatus;

  switch (currentStatus) {
    case line_ready:
    case line_fail:
    case line_busy:
    case line_disconnected:
      Line[line].setLineStatus(line_timeout);
      lineAction(line, line_timeout);
      break;

    case line_timeout:
      Line[line].setLineStatus(line_abandoned);
      lineAction(line, line_abandoned);
      break;

    case line_incoming:
      Line[line].setLineStatus(line_idle);
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
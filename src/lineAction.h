#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"

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
      break;

    case line_pulse_dialing:
      // Action
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
      // Action
      break;

    case line_timeout:
      // Action
      break;

    case line_abandoned:
      // Action
      break;

    case line_busy:
      // Action
      break;

    case line_fail:
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

// Function to handle line timer expiration


// Function to determine the action to take based on the line status due to a timer expiration
void lineTimerExpired(int line) {
  lineSystem.stopLineTimer(line);
  uint32_t currentStatus = lineSystem.lineVector[line].currentStatus;

  switch (currentStatus) {
    case line_ready:
    case line_pulse_dialing:
    case line_tone_dialing:
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

    default:
      // Handle unexpected status if necessary
      break;
  }
}

void lineTimerCheck() {
  for (int i = 0; i < activeLines; i++) {
    if (bitRead(lineTimerFlags, i) == 1) {
      if (millis() - lineSystem.lineVector[i].lineTimerStart > lineSystem.lineVector[i].lineTimerLimit) {
        lineTimerExpired(i);
      }
    }
  }
}


#endif
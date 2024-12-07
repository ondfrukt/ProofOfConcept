#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"

void ring(int line) {
  Serial.println("Ringing line " + String(line));
}

// Function to determine the action to take based on the new line status
void lineAction(int line, uint8_t newLineStatus) {
  Serial.println("Line " + String(line) + " status: " + statusNames[newLineStatus]);
  switch (newLineStatus) {
    case line_idle:
      // Action
      mqttHandler.publishMQTT(line, line_idle);
      break;

    case line_ready:
      mqttHandler.publishMQTT(line, line_ready);
      break;

    case line_tone_dialing:
      // Action
      mqttHandler.publishMQTT(line, line_tone_dialing);
      break;

    case line_pulse_dialing:
      // Action
      mqttHandler.publishMQTT(line, line_pulse_dialing);
      break;

    case line_connected:
      mqttHandler.publishMQTT(line, line_connected);
      // Action
      break;

    case line_disconnected:
      mqttHandler.publishMQTT(line, line_disconnected);
      // Action
      break;

    case line_incoming:
      mqttHandler.publishMQTT(line, line_incoming);
      // Action
      break;

    case line_ringing:
      dialedDigits = Line[line].dialedDigits;
      for (int i = 0; i < activeLines; i++) {
        // Check if the diled digits match a number in the phonebook and the line is idle
        if (dialedDigits == Line[i].phoneNumber && Line[i].currentLineStatus == line_idle) {
          ring(i);
        }
      }
      mqttHandler.publishMQTT(line, line_ringing);
      break;

    case line_timeout:
      mqttHandler.publishMQTT(line, line_timeout);
      // Action
      break;

    case line_abandoned:
      mqttHandler.publishMQTT(line, line_abandoned);
      // Action
      break;

    case line_busy:
      Line[line].dialedDigits = "";
      mqttHandler.publishMQTT(line, line_busy);
      // Action
      break;

    case line_fail:
      Line[line].dialedDigits = "";
      mqttHandler.publishMQTT(line, line_fail);
      // Action
      break;

    case line_operator:
      mqttHandler.publishMQTT(line, line_operator);
      // Action
      break;

    default:
      // Handle unexpected status
      break;
  }
}

// Function to determine the action to take based on the line status due to a timer expiration
void lineTimerExpired(int line) {
  Serial.println("Line " + String(line + 1) + " timer expired");
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
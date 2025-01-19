#include "lineAction.h"


// Function to determine the action to take based on the new line status
void lineAction(int line, uint8_t newLineStatus) {
  switch (newLineStatus) {
    case line_idle:
      // Insert Action!
      Line[line].setLineStatus(line_idle);
      mqttHandler.publishMQTT(line, line_idle);
      break;

    case line_ready:
      // Insert Action!
      Line[line].setLineStatus(line_ready);
      mqttHandler.publishMQTT(line, line_ready);
      Line[line].startLineTimer(statusTimer_Ready);


      break;

    case line_tone_dialing:
      // Insert Action!
      Line[line].setLineStatus(line_tone_dialing);
      mqttHandler.publishMQTT(line, line_tone_dialing);
      break;

    case line_pulse_dialing:
      // Insert Action!
      Line[line].setLineStatus(line_pulse_dialing);
      mqttHandler.publishMQTT(line, line_pulse_dialing);
      break;

    case line_connected:
      Line[line].setLineStatus(line_connected);
      mqttHandler.publishMQTT(line, line_connected);

      Line[Line[line].incomingFrom].setLineStatus(line_connected);
      mqttHandler.publishMQTT(Line[line].incomingFrom, line_connected);

      break;

    case line_disconnected:
      Line[line].setLineStatus(line_disconnected);
      mqttHandler.publishMQTT(line, line_disconnected);

      Line[line].startLineTimer(statusTimer_disconnected);
      // Insert Action!
      break;

    case line_incoming:
      Line[line].setLineStatus(line_incoming);
      mqttHandler.publishMQTT(line, line_incoming);
      // Insert Action!
      break;

    case line_ringing:

      Serial.println("Line " + String(line) + " dialed digits: " + Line[line].dialedDigits);
      for (int i = 0; i < activeLines; i++) {

        // Check if the diled digits match a number in the phonebook, its not the same line and the called line is idle
        if (Line[line].dialedDigits == Line[i].phoneNumber && Line[i].currentLineStatus == line_idle && Line[i].lineNumber != line) {
          
          Line[line].setLineStatus(line_ringing);
          mqttHandler.publishMQTT(line, line_ringing);
          Line[line].outgoingTo = i;


          Line[i].setLineStatus(line_incoming);
          mqttHandler.publishMQTT(i, line_incoming);
          Line[i].incomingFrom = line;

          Line[line].resetDialedDigits();

          ringHandler.generateRingSignal(i);
          Line[line].startLineTimer(statusTimer_Ringing);
          return;
        }
      }
      // If no match is found in the loop, the line is set to fail
      Serial.println("Line " + String(line) + " failed to connect. Wrong number?");
      Line[line].resetDialedDigits();
      lineAction(line, line_fail);

      break;

    case line_timeout:
      mqttHandler.publishMQTT(line, line_timeout);
      // Insert Action!
      break;

    case line_abandoned:
      mqttHandler.publishMQTT(line, line_abandoned);
      // Insert Action!
      break;

    case line_busy:
      Line[line].dialedDigits = "";
      mqttHandler.publishMQTT(line, line_busy);
      // Insert Action!
      break;

    case line_fail:
      Line[line].setLineStatus(line_fail);
      mqttHandler.publishMQTT(line, line_fail);
      
      Line[line].resetDialedDigits();
      Line[line].startLineTimer(statusTimer_fail);
      
      // Insert Action!
      break;

    case line_operator:
      mqttHandler.publishMQTT(line, line_operator);
      // Insert Action!
      break;

    default:
      // Handle unexpected status
      break;
  }
}

// Function to determine the action to take based on the line status due to a timer expiration
void lineTimerExpired(int line) {
  Serial.println("Line " + String(line) + " timer expired");
  Line[line].stopLineTimer();
  uint32_t currentStatus = Line[line].currentLineStatus;

  switch (currentStatus) {
    case line_ready:
    case line_fail:
      lineAction(line, line_abandoned);
    case line_busy:
    case line_disconnected:
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
      lineAction(line, line_ringing);
      break;

    case line_tone_dialing:
      
      lineAction(line, line_ringing);
      break;



    case line_ringing:
      lineAction(line, line_fail);

      break;
    default:
      // Handle unexpected status if necessary
      break;
  }
}

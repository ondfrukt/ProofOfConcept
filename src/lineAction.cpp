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
      Line[line].setLineStatus(line_ready);
      mqttHandler.publishMQTT(line, line_ready);
      Line[line].startLineTimer(statusTimer_Ready);
      break;
    
    case line_pulse_dialing:
      Line[line].setLineStatus(line_pulse_dialing);
      mqttHandler.publishMQTT(line, line_pulse_dialing);
      Line[line].startLineTimer(statusTimer_pulsDialing);
      break;
    
    case line_tone_dialing:
      Line[line].setLineStatus(line_tone_dialing);
      mqttHandler.publishMQTT(line, line_tone_dialing);
      Line[line].startLineTimer(statusTimer_toneDialing);
      break;
    
    case line_busy:
      Line[line].setLineStatus(line_busy);
      mqttHandler.publishMQTT(line, line_busy);
      Line[line].startLineTimer(statusTimer_busy);
      break;

    case line_fail:
      Line[line].setLineStatus(line_fail);
      mqttHandler.publishMQTT(line, line_fail);
      
      Line[line].resetDialedDigits();
      Line[line].startLineTimer(statusTimer_fail);
      break;
    
    case line_ringing:

    Serial.println("Line " + String(line) + " dialed digits: " + Line[line].dialedDigits);
    for (int i = 0; i < activeLines; i++) {

      // Check if the diled digits match a number in the phonebook ant its not the same line as ringing
      if (Line[line].dialedDigits == Line[i].phoneNumber && Line[i].lineNumber != line) {
        
        // Checking if the line is idle
        if (Line[i].currentLineStatus != line_idle){
          lineAction(line, line_busy);
          return;
        } 

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
    // If no match is found in the loop, the line status is set to fail
    Serial.println("Line " + String(line) + " failed to connect. Wrong number?");
    Line[line].resetDialedDigits();
    lineAction(line, line_fail);
    break;
    
    case line_connected:
      Line[line].setLineStatus(line_connected);
      mqttHandler.publishMQTT(line, line_connected);


      // Setting for the calling line
      Line[Line[line].incomingFrom].setLineStatus(line_connected);
      Line[Line[line].incomingFrom].outgoingTo = line;

      // Setting for the called line
      Line[line].outgoingTo = Line[line].incomingFrom;

      // Publinshing the status for the calling line
      mqttHandler.publishMQTT(Line[line].incomingFrom, line_connected);

      //Connectiong 
      mt8816.connect(line, Line[line].incomingFrom );
      mt8816.connect(Line[line].incomingFrom, line);
      break;

    case line_disconnected:
      Line[line].setLineStatus(line_disconnected);
      mqttHandler.publishMQTT(line, line_disconnected);
      Line[line].startLineTimer(statusTimer_disconnected);
      
      // Oklart om detta fungerar?
      for (int i = 0; i < activeLines; i++){
        mt8816.disconnect(line, i);
        mt8816.disconnect(i, line);
      }
      break;
    
    case line_timeout:
      Line[line].setLineStatus(line_timeout);
      mqttHandler.publishMQTT(line, line_timeout);
      Line[line].startLineTimer(statusTimer_timeout);
      break;
    
    case line_abandoned:
    Line[line].setLineStatus(line_abandoned);
    mqttHandler.publishMQTT(line, line_abandoned);
    break;
    
    case line_incoming:
      Line[line].setLineStatus(line_incoming);
      mqttHandler.publishMQTT(line, line_incoming);
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
    case line_fail:
      lineAction(line, line_timeout);
      break;

    case line_busy:
      lineAction(line, line_timeout);
      break;

    case line_disconnected:
      lineAction(line, line_timeout);
      break;

    case line_timeout:
      lineAction(line, line_abandoned);
      break;

    case line_incoming:
      lineAction(line, line_idle);
      break;

    case line_pulse_dialing:
      lineAction(line, line_ringing);
      break;

    case line_tone_dialing:
      lineAction(line, line_ringing);
      break;

    case line_ringing:
      lineAction(line, line_disconnected);
      break;

    default:
      // Handle unexpected status if necessary
      break;
  }
}

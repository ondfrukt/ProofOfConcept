#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"

// Function to determine the action to take based on the new line status
void lineAction(int line, statuses newLineStatus){

  if (newLineStatus == line_idle){
    //Action

    return;
  }

  if (newLineStatus == line_ready){
    //Action
    return;
  }

  if (newLineStatus == line_tone_dialing){
    //Action
    return;
  }

  if (newLineStatus == line_pulse_dialing){
    //Action
    return;
  }

  if (newLineStatus == line_connected){
    //Action
    return;
  }

  if (newLineStatus == line_disconnected){
    //Action
    return;
  }

  if (newLineStatus == line_incoming){
    //Action
    return;
  }

  if (newLineStatus == line_ringing){
    //Action
    return;
  }

  if (newLineStatus == line_timeout){
    //Action
    return;
  }

  if (newLineStatus == line_abandoned){
    //Action
    return;
  }

  if (newLineStatus == line_busy){
    //Action
    return;
  }

  if (newLineStatus == line_fail){
    //Action
    return;
  }

  if (newLineStatus == line_operator){
    //Action
    return;
  }
}

// Function to determine the action to take based on the line status due to a timer expiration
void lineTimerExpired(int line){
  lineSystem.clearLineTimerFlag(line);
  statuses currentStatus = lineSystem.getCurrentLineStatus(line);
  if (
    currentStatus == line_ready ||
    currentStatus == line_pulse_dialing ||
    currentStatus == line_tone_dialing ||
    currentStatus == line_fail ||
    currentStatus == line_busy ||
    currentStatus == line_disconnected ){

    lineSystem.setLineStatus(line, line_timeout);
    lineAction(line, line_timeout);
    return;
  }

  if (currentStatus == line_timeout) {
    lineSystem.setLineStatus(line, line_abandoned);
    lineAction(line, line_abandoned);
    return;
  } 
  if (currentStatus == line_incoming) {
    lineSystem.setLineStatus(line, line_idle);
    lineAction(line, line_idle);
    return;
  }
}



#endif
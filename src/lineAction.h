#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"


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



void timerChecker(int line){
  statuses status = lineSystem.getCurrentLineStatus(line);
  unsigned long int timer = lineSystem.getLineTimer(line);
  

}

#endif
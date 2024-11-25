#include "PulsHandler.h"

pulseHandler::pulseHandler(unsigned pinSHKs[], int activeLines, Adafruit_MCP23X17 &mcp)
    : _mcp(mcp)
    , _activeLines(activeLines)
{
    // Allokera minne för alla arrayer
    PINs = new unsigned[activeLines];
    SHKs = new bool[activeLines];
    maybe = new bool[activeLines];
    dialing = new bool[activeLines];
    pulsing = new bool[activeLines];
    pulses = new int[activeLines];
    edge = new unsigned long[activeLines];
    lastDebounceTime = new unsigned long[activeLines];
    lastDebounceValue = new bool[activeLines];

    // Initiera värdena
    for (int i = 0; i < activeLines; i++) {
        PINs[i] = pinSHKs[i];
        SHKs[i] = false;
        maybe[i] = false;
        dialing[i] = false;
        pulsing[i] = false;
        pulses[i] = 0;
        edge[i] = 0;
        lastDebounceTime[i] = 0;
        lastDebounceValue[i] = false;
    }
}

// Destruktor för att frigöra minnet
pulseHandler::~pulseHandler() {
    delete[] PINs;
    delete[] SHKs;
    delete[] maybe;
    delete[] dialing;
    delete[] pulsing;
    delete[] pulses;
    delete[] edge;
    delete[] lastDebounceTime;
    delete[] lastDebounceValue;
}

void pulseHandler::setDigitCallback(void (*digitReceivedCallbacks[])(char)) {
  for (int i = 0; i < activeLines; i++) {
    digitReceivedCallbacks[i] = digitReceivedCallbacks[i];
  }
}

void pulseHandler::start(){
  for (int i = 0; i < activeLines; i++) {
    maybe[i] = false;
    dialing[i] = false;
    pulsing[i] = false;
    pulses[i] = 0;
    edge[i] = 0;
    lastDebounceTime[i] = millis();
  }
}

void pulseHandler::run(){
  for (int i = 0; i < activeLines; i++) {
    bool newSHK = mcp.digitalRead(PINs[i]);

    // debounce because SHK from SLIC is very noisy
    if(newSHK != lastDebounceValue[i]) {
      lastDebounceValue[i] = newSHK;
      lastDebounceTime[i] = millis();
      continue;
    }
    if((millis() - lastDebounceTime[i]) < pulseGapMin) continue;

    unsigned gap = edge[i] ? millis() - edge[i] : 0;
    if(newSHK && SHKs[i] && gap <= pulseGapMax) continue;
    if(!newSHK && !SHKs[i]) continue;
    SHKs[i] = newSHK;

    // falling edge
    if(!SHKs[i] && !pulsing[i]) {
      pulsing[i] = true;
      edge[i] = millis();
      continue;
    }

    // rising edge
    if(SHKs[i] && pulsing[i]){
      pulsing[i] = false;
      pulses[i]++;
      edge[i] = millis();
      if(!dialing[i]){
        dialing[i] = true;
        dialingStartedCallbacks[i](false);
      }
      continue;
    }

    // gap between digits
    if(SHKs[i] && !pulsing[i] && gap > pulseGapMax){
      char digit = String(pulses[i] % 10)[0];
      pulsing[i] = false;
      pulses[i] = 0;
      edge[i] = 0;
      digitReceivedCallbacks[i](digit);
    }
  }
}
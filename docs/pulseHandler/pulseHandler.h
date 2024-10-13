// By https://github.com/ktownsend-personal

#ifndef pulser_h
#define pulser_h
#include "Arduino.h"
#include <Adafruit_MCP23X17.h>

class pulseHandler {
  public:
    pulseHandler(unsigned pinSHK, void (*dialingStartedCallback)(bool));
    void setDigitCallback(void (*digitReceivedCallback)(char));
    void setMaybeCallback(void (*maybeDialingStartedCallback)());
    void start();
    void run();

  private:
    unsigned long lastPulseTime;
    unsigned long minPulseInterval;
    unsigned PIN;
    bool SHK;
    bool maybe;
    bool dialing;
    bool pulsing;
    byte pulses;
    bool lastDebounceValue;
    unsigned long lastDebounceTime;
    unsigned long edge;
    const unsigned pulseGapMin = 10;
    const unsigned pulseGapMax = 60;  //probably should be same as SHK debounceDelay in progressModes.h
    void (*maybeDialingStartedCallback)();
    void (*dialingStartedCallback)(bool);
    void (*digitReceivedCallback)(char);
};

#endif
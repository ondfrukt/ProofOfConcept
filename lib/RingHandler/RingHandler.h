#ifndef ringHandler_h
#define ringHandler_h

#include "Arduino.h"
#include <Adafruit_MCP23X17.h>

class ringHandler {
  public:
    void setPins(uint8_t RMPins, uint8_t FRPins);
    void init(unsigned pinRM, unsigned pinFR, unsigned channelFR);
    void start(int freq, int* cadence);
    void run();
    void stop();
    int ringCount = 0;

  private:
    unsigned RMpin;
    unsigned FRch;
    unsigned RING_FREQ;
    int* ringCadence; // expects cadence array, first element count of timings in the array, then the timings
    int cadenceCount = 0;
    int cadenceIndex = 0;
    unsigned long cadenceSince = 0;
    void on();
    void off();
    void (*counterCallback)(const int);
};

#endif
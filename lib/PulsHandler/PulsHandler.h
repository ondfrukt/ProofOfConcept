#ifndef PulsHandler_h
#define PulsHandler_h

#include "Arduino.h"
#include <Adafruit_MCP23X17.h>

class pulseHandler {
public:
    pulseHandler(unsigned pinSHKs[], int activeLines, Adafruit_MCP23X17 &mcp);
    ~pulseHandler(); // Destruktor för att frigöra minnet
    void setDigitCallback(void (*digitReceivedCallbacks[])(char));
    void start();
    void run();

private:
    unsigned* PINs;
    bool* SHKs;
    bool* maybe;
    bool* dialing;
    bool* pulsing;
    int* pulses;
    unsigned long* edge;
    unsigned long* lastDebounceTime;
    bool* lastDebounceValue;
    Adafruit_MCP23X17& _mcp;
    int _activeLines;
    unsigned long pulseGapMin = 50;
    unsigned long pulseGapMax = 80;
};

#endif

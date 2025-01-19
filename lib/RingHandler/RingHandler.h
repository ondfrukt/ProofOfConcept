#ifndef ringHandler_h
#define ringHandler_h

#include "Arduino.h"
#include <Adafruit_MCP23X17.h>
#include "LineHandler.h"

class RingHandler {
  public:
    RingHandler(Adafruit_MCP23X17& mcp, const uint8_t _activeLines, const uint8_t* _RMpins, const uint8_t* _FRpins, int _ringLength);
    void generateRingSignal(int line);

  private:
    Adafruit_MCP23X17& mcp_ks083f;  // MCP23017 object
    int activeLines;                // Number of active lines
    const uint8_t* RMPins;          // Array of ring mode pins
    const uint8_t* FRPins;          // Array of forward/reverse pins
    int ringLength;                 // Length of ring signal in milliseconds
    int iterations;                 // Number of iterations to generate 20 Hz ring signal
};

#endif
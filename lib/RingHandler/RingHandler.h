#ifndef ringHandler_h
#define ringHandler_h

#include "Arduino.h"
#include <Adafruit_MCP23X17.h>
#include "LineHandler.h"

class RingHandler {
  public:
    RingHandler(Adafruit_MCP23X17& mcp, const int _activeLines, const uint8_t* _RMpins, const uint8_t* _FRpins);
    void generateRingSignal(int line);

  private:
    Adafruit_MCP23X17& mcp_ks083f;
    int activeLines;
    const uint8_t* RMPins;
    const uint8_t* FRPins;
};

#endif
#ifndef MT8816_h
#define MT8816_h

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class MT8816 {
  public:
    MT8816(Adafruit_MCP23X17& mcp, const uint8_t* _ax_pins, const uint8_t* _ay_pins, uint8_t _strobe, uint8_t _data, uint8_t _reset, uint8_t _cs_pin);
    void begin();
    void reset();
    void connect(uint8_t x, uint8_t y);
    void disconnect(uint8_t x, uint8_t y);
    bool getConnection(int x, int y);
    void printConnections();
  private:
    Adafruit_MCP23X17& mcp_mt8816; 
    uint8_t ax_pins[4];            // AX0-AX3
    uint8_t ay_pins[3];            // AY0-AY2
    uint8_t strobe_pin;
    uint8_t data_pin;
    uint8_t reset_pin;
    uint8_t cs_pin;
    bool connections[8][8];
    void setAddress(uint8_t x, uint8_t y);
    void strobe();
};


#endif
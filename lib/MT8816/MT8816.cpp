#include <Arduino.h>
#include "MT8816.h"
#include <Wire.h>

MT8816::MT8816(Adafruit_MCP23X17& mcp, const uint8_t* _ax_pins, const uint8_t* _ay_pins, uint8_t _strobe, uint8_t _data, uint8_t _reset, uint8_t _cs_pin)
    : mcp_mt8816(mcp) {
    for (int i = 0; i < 4; i++) {
        ax_pins[i] = _ax_pins[i];
    }
    for (int i = 0; i < 3; i++) {
        ay_pins[i] = _ay_pins[i];
    }
    strobe_pin = _strobe;
    data_pin = _data;
    reset_pin = _reset;
    cs_pin = _cs_pin;

    // Initiera anslutningsmatris
    memset(connections, 0, sizeof(connections));
}

void MT8816::begin()
{
    // Reset MCP
    reset();
    Serial.println("MT8816 initialized successfully.");
}

void MT8816::reset()
{   
    // Pulse the reset pin to reset the IC
    mcp_mt8816.digitalWrite(reset_pin, LOW);
    delayMicroseconds(10);
    mcp_mt8816.digitalWrite(reset_pin, HIGH);
    delay(100);  
    mcp_mt8816.digitalWrite(reset_pin, LOW);
    Serial.println("MT8816 reset performed.");
}

void MT8816::connect(uint8_t x, uint8_t y)
{
    setAddress(x, y);
    mcp_mt8816.digitalWrite(data_pin, HIGH);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[x][y] = true;

    setAddress(y, x);
    mcp_mt8816.digitalWrite(data_pin, HIGH);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[y][x] = true;

}

void MT8816::disconnect(uint8_t x, uint8_t y)
{
    setAddress(x, y);
    mcp_mt8816.digitalWrite(data_pin, LOW);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[x][y] = false;

    setAddress(y, x);
    mcp_mt8816.digitalWrite(data_pin, LOW);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[y][x] = false;
}

bool MT8816::getConnection(int x, int y) {
    // Check if the coordinates are within the valid range
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        return connections[x][y];
    } else {
        // Handle invalid coordinates, e.g., by returning a default value or throwing an exception

        Serial.print("Error: Invalid coordinates (");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.println(").");
        return false;
    }
}

void MT8816::setAddress(uint8_t x, uint8_t y)
{
    for (int i = 0; i < 4; ++i) {
        bool bit = (x >> i) & 0x01;
        mcp_mt8816.digitalWrite(ax_pins[i], bit);
    }

    for (int i = 0; i < 3; ++i) {
        bool bit = (y >> i) & 0x01;
        mcp_mt8816.digitalWrite(ay_pins[i], bit);
    }
}

void MT8816::strobe()
{
    mcp_mt8816.digitalWrite(strobe_pin, HIGH);
    delayMicroseconds(5);
    mcp_mt8816.digitalWrite(strobe_pin, LOW);
    delayMicroseconds(5);
}
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

void MT8816::connect(uint8_t x, uint8_t y){
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

void MT8816::disconnect(uint8_t x, uint8_t y){
    setAddress(x, y);
    mcp_mt8816.digitalWrite(data_pin, LOW);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[x][7-y] = false;

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
    // Converting y value due to an connection misstake at the PCB
    y = 7 - y;
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

void MT8816::printConnections() {
  // Skriv ut kolumnrubrikerna
  Serial.print("     ");  // Något extra utrymme för radnummerkolumnen
  for (int j = 0; j < 8; j++) {
    Serial.print(j);
    Serial.print(" ");
  }
  Serial.println(); // Avsluta kolumnrubrikraden
  
  // Skriv ut varje rad med radnummer först
  for (int i = 0; i < 16; i++) {
    // Skriv ut radnumret
    if (i < 10) {
      // Lägg till ett extra mellanslag för enhetliga avstånd, om du har en- och tvåsiffriga tal
      Serial.print(" ");
    }
    Serial.print(i);
    Serial.print(" | ");  // Avgränsare mellan radnumret och själva matrisdata
    
    // Skriv ut värdena i raden
    for (int j = 0; j < 8; j++) {
      Serial.print((int)connections[i][j]);
      Serial.print(" ");
    }
    Serial.println(); // Ny rad efter varje rad i matrisen
  }
  Serial.println(); // Extra radbrytning för att skilja från annan output
}


// This function is temporary due to an misstake in the PCB design. Aout_x should be connected to an X-line not Y-line
void MT8816::connectAout_x() {
    uint8_t x_bridge = 15; // X-line as a connection bridge
    uint8_t Aout_x = 4; // Y-line to connect to
    setAddress(x_bridge, Aout_x);
    mcp_mt8816.digitalWrite(data_pin, HIGH);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[x_bridge][Aout_x] = true;
}

// This function is temporary due to an misstake in the PCB design. Aout_x should be connected to an X-line not Y-line
void MT8816::disconnectAout_x() {
    uint8_t x_bridge = 15; // X-line as a connection bridge
    uint8_t Aout_x = 4; // Y-line to connect to
    setAddress(x_bridge, Aout_x);
    mcp_mt8816.digitalWrite(data_pin, LOW);
    delay(10);  // Short delay to ensure data pin is stable
    strobe();
    connections[x_bridge][Aout_x] = false;
}
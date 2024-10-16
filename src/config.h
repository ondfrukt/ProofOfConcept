#ifndef config_h
#define config_h
#include <Arduino.h>
#include <MT8816.h>
#include <LineStatusHandler.h>
#include <Adafruit_MCP23X17.h>


// Number of lines
const int activeLines = 4;
String dialedDigits; // Declare the variable outside the switch statement

// Defining objects
LineSystem lineSystem(activeLines);
Adafruit_MCP23X17 mcp_ks083f;

// extern MT8816 matrix;

// Defining MCP pins
const int GPA0 = 0;
const int GPA1 = 1;
const int GPA2 = 2;
const int GPA3 = 3;
const int GPA4 = 4;
const int GPA5 = 5;
const int GPA6 = 6;
const int GPA7 = 7;
const int GPB0 = 8;
const int GPB1 = 9;
const int GPB2 = 10;
const int GPB3 = 11;
const int GPB4 = 12;
const int GPB5 = 13;
const int GPB6 = 14;
const int GPB7 = 15;

// MCP adresses
uint8_t mcp_mt8816_address = 0;
uint8_t mcp_ks083f_address = 1;

// KS083F pins setup
const uint8_t SHK_1 = GPA3;
const uint8_t RM_1 = GPA4;
const uint8_t FR_1 = GPA5;
const uint8_t SHK_2 = GPA0;
const uint8_t RM_2 = GPA1;
const uint8_t FR_2 = GPA2;
const uint8_t SHK_3 = GPB4;
const uint8_t RM_3 = GPB3;
const uint8_t FR_3 = GPB2;
const uint8_t SHK_4 = GPB7;
const uint8_t RM_4 = GPB6;
const uint8_t FR_4 = GPB5;


// --------------Timers--------------------------

unsigned long statusTimer_Ready = 30000; // 30 seconds
unsigned long statusTimer_Dialing = 5000; // 5 seconds
unsigned long statusTimer_Ringing = 5000; // 5 seconds
unsigned long statusTimer_pulsDialing = 5000; // 0 seconds
unsigned long statusTimer_toneDialing = 5000; // 0 seconds

// -----------------SHK--------------------------

// Array to store the pin numbers for each input
const uint8_t SHKPins[activeLines] = {SHK_1, SHK_2, SHK_3, SHK_4};

// Debouncing time in milliseconds
uint8_t SHKDebouncingTime = 150;


#endif

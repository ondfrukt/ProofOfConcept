#ifndef config_h
#define config_h
#include <Arduino.h>
#include <MT8816.h>
#include <LineStatusHandler.h>
#include <Adafruit_MCP23X17.h>

extern LineSystem lineSystem;
extern MT8816 matrix;
extern Adafruit_MCP23X17 mcp_ks083f;

// Number of lines
#define NUM_INPUTS 4

// Defining MCP pins
#define GPA0 0
#define GPA1 1
#define GPA2 2
#define GPA3 3
#define GPA4 4
#define GPA5 5
#define GPA6 6
#define GPA7 7
#define GPB0 8
#define GPB1 9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15

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


// -----------------SHK--------------------------

// Array to store the pin numbers for each input
const uint8_t SHKPins[NUM_INPUTS] = {SHK_1, SHK_2, SHK_3, SHK_4};

// Arrays to store debounce-related information for each input

volatile uint8_t SHKState = 0;           // Current SHK States
volatile bool stateChanged = false;      
unsigned int lastSHKDebounceTime = 0;    // Last time the input was toggled

uint8_t SHKDebouncing = 50;


enum HookState {
  HOOK_ON,
  HOOK_OFF
  };

#endif
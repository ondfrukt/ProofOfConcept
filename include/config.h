#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <MT8816.h>
#include "lineStatuses.h"
#include <LineHandler.h>
#include <MQTTHandler.h>
#include <RingHandler.h>


// ------------------ Globala konstanter ------------------

const uint8_t activeLines = 4; // Antal aktiva linjer

// LED-pinnar
extern const int hookLED;
extern const int mqttLED;
extern const int wifiLED;

// Testknapp
extern const int testButton1;

// MCP23017-pinnar
extern const uint8_t GPA0;
extern const uint8_t GPA1;
extern const uint8_t GPA2;
extern const uint8_t GPA3;
extern const uint8_t GPA4;
extern const uint8_t GPA5;
extern const uint8_t GPA6;
extern const uint8_t GPA7;
extern const uint8_t GPB0;
extern const uint8_t GPB1;
extern const uint8_t GPB2;
extern const uint8_t GPB3;
extern const uint8_t GPB4;
extern const uint8_t GPB5;
extern const uint8_t GPB6;
extern const uint8_t GPB7;

// KS083F-konfiguration
extern const uint8_t SHKPins[activeLines];
extern const uint8_t RMPins[activeLines];
extern const uint8_t FRPins[activeLines];

// MT8816-konfiguration

extern const uint8_t ax_pins[4];
extern const uint8_t ay_pins[3];

extern const uint8_t RESET;
extern const uint8_t DATA;
extern const uint8_t STROBE;
extern const uint8_t CS;

extern const uint8_t AX0;
extern const uint8_t AX1;
extern const uint8_t AX2;
extern const uint8_t AX3;
extern const uint8_t AY0;
extern const uint8_t AY1;
extern const uint8_t AY2;

extern bool Aout_x;


// MT8870 configuration
extern const int Q1;
extern const int Q2;
extern const int Q3;
extern const int Q4;
extern const int STD;

extern const char dtmf_map[16];

// MCP-adresser
extern const uint8_t mcp_mt8816_address;
extern const uint8_t mcp_ks083f_address;

// ------------------ Globala objekt ------------------

extern LineHandler Line[activeLines];
extern Adafruit_MCP23X17 mcp_ks083f;
extern Adafruit_MCP23X17 mcp_mt8816;
extern MQTTHandler mqttHandler;
extern MT8816 mt8816;
extern RingHandler ringHandler;

// ------------------ Timers ------------------

extern unsigned long statusTimer_Ready;
extern unsigned long statusTimer_Dialing;
extern unsigned long statusTimer_Ringing;
extern unsigned long statusTimer_pulsDialing;
extern unsigned long statusTimer_toneDialing;
extern unsigned long statusTimer_timeout;
extern unsigned long statusTimer_abandoned;
extern unsigned long statusTimer_incoming;
extern unsigned long statusTimer_operator;
extern unsigned long statusTimer_fail;
extern unsigned long statusTimer_busy;
extern unsigned long statusTimer_connected;
extern unsigned long statusTimer_disconnected;


extern uint8_t SHKDebouncingTime;
extern const unsigned pulseGapMax;
extern unsigned long edge;
extern const unsigned long gapTimeout;

// ---------------System variables ---------------
extern bool error; // Error flag
extern int lastLineReady; // Last line that was ready

// ------------------ Funktioner ------------------

void i2CScanner();
bool allLinesIdle();
void setupMCPPins();
void setupMT8870();
void timerHandler();

#endif

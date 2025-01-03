#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <MT8816.h>
#include <LineHandler.h>
#include <MQTTHandler.h>
#include <Adafruit_MCP23X17.h>
#include "lineStatuses.h"



// Number of lines
const int activeLines = 1;
// Defining line objects
LineHandler Line[1] = { 
    //LineHandler(0), 
    //LineHandler(1), 
    LineHandler(2), 
    //LineHandler(3) 
};

const int hookLED = 16;  // LED indicatior to se if a hook i lifted
const int mqttLED = 17;  // LED indicatior to se if MQTT is connected
const int wifiLED = 18;  // LED indicator to se if WiFi is connected

// Defining MQTTHandler object including wifi and mqtt settings
MQTTHandler mqttHandler(wifiLED, mqttLED);

// Defining MCP23017 objects
Adafruit_MCP23X17 mcp_ks083f;
// MT8816 matrix;

// Variable to store the dialed digits
String dialedDigits;

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
uint8_t mcp_mt8816_address = 0x23;
uint8_t mcp_ks083f_address = 0x26;

// KS083F pins setup
const uint8_t SHK_1 = GPA3;
const uint8_t RM_1 = GPA4;
const uint8_t FR_1 = GPA5;

const uint8_t SHK_2 = GPA0;
const uint8_t RM_2 = GPA1;
const uint8_t FR_2 = GPA2;

const uint8_t SHK_3 = GPB5;
const uint8_t RM_3 = GPB3;
const uint8_t FR_3 = GPB7;

const uint8_t SHK_4 = GPB2;
const uint8_t RM_4 = GPB6;
const uint8_t FR_4 = GPB4;

// --------------Timers--------------------------

unsigned long statusTimer_Ready = 30000; // 30 seconds
unsigned long statusTimer_Dialing = 5000; // 5 seconds
unsigned long statusTimer_Ringing = 5000; // 5 seconds
unsigned long statusTimer_pulsDialing = 5000; // 0 seconds
unsigned long statusTimer_toneDialing = 5000; // 0 seconds

// -----------------SHK--------------------------

// Array to store the pin numbers for each input
//const uint8_t SHKPins[activeLines] = {SHK_1, SHK_2, SHK_3, SHK_4};
const uint8_t SHKPins[activeLines] = { SHK_3};
// -----------------I2C Scanner------------------
void i2CScanner() {
  // Scan for I2C devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Scanning done\n");
  }
}

// Function to check if all lines are idle
bool allLinesIdle() {
  for (int line = 0; line < activeLines; line++) {
    if (Line[line].currentLineStatus != line_idle) {
      digitalWrite(hookLED, HIGH); // Turning LED indicator on
      return false;  // Retursn fals if any line is not idle

    }
  }
  digitalWrite(hookLED, LOW);
  return true;  // Om vi har kommit hit så är alla linjer idle
}


#endif

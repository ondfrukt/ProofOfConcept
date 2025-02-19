#include "config.h"
#include "lineAction.h"
#include "lineStatuses.h"

//------------------ ESP32 GPIO pins ------------------

// LED-pinnar
const int hookLED = 16;
const int mqttLED = 17;
const int wifiLED = 18;

// Testknapp
const int testButton1 = 19;

// Pin modes
void configLEDPins(){
  pinMode(hookLED, OUTPUT);
  digitalWrite(hookLED, LOW);
  pinMode(testButton1, INPUT_PULLUP);
}

//---------------- MCP23017 GPIO pins ----------------

// MCP23017 logic GPIO-pinns
const uint8_t GPA0 = 0;   //Pin 21 
const uint8_t GPA1 = 1;   //Pin 22
const uint8_t GPA2 = 2;   //Pin 23
const uint8_t GPA3 = 3;   //Pin 24
const uint8_t GPA4 = 4;   //Pin 25
const uint8_t GPA5 = 5;   //Pin 26
const uint8_t GPA6 = 6;   //Pin 27
const uint8_t GPA7 = 7;   //Pin 28
const uint8_t GPB0 = 8;   //Pin 1
const uint8_t GPB1 = 9;   //Pin 2
const uint8_t GPB2 = 10;  //Pin 3
const uint8_t GPB3 = 11;  //Pin 4
const uint8_t GPB4 = 12;  //Pin 5
const uint8_t GPB5 = 13;  //Pin 6
const uint8_t GPB6 = 14;  //Pin 7
const uint8_t GPB7 = 15;  //Pin 8

// KS083F constatns
const uint8_t SHKPins[activeLines] = {GPA3, GPA0, GPB5, GPB2};
const uint8_t RMPins[activeLines] = {GPA4, GPA1, GPB3, GPB6};
const uint8_t FRPins[activeLines] = {GPA5, GPA2, GPB7, GPB4};

// MT8816 constants
const uint8_t RESET = GPA0;
const uint8_t DATA = GPA1;
const uint8_t STROBE = GPA2;
const uint8_t CS = GPA3;

const uint8_t AX0 = GPB0;
const uint8_t AX1 = GPB1;
const uint8_t AX2 = GPB2;
const uint8_t AX3 = GPB3;
const uint8_t AY0 = GPB4;
const uint8_t AY1 = GPB5;
const uint8_t AY2 = GPB6;

const uint8_t ax_pins[4] = {AX0, AX1, AX2, AX3};
const uint8_t ay_pins[3] = {AY0, AY1, AY2};

bool Aout_x = false;

// MT8870 constants
const int Q1 = 26;  // Pin för Q1
const int Q2 = 23;  // Pin för Q2
const int Q3 = 4;   // Pin för Q3
const int Q4 = 5;   // Pin för Q4
const int STD = 33; // Signal detect (StD)

// DTMF-tabell för att mappa binärvärde till rätt tecken
const char dtmf_map[16] = {
  ' ', 
  '1', '2', '3',
  '4', '5', '6', 
  '7', '8', '9',
  '0', '*', '#',
};

// MCP-adresses
const uint8_t mcp_mt8816_address = 0x23;
const uint8_t mcp_ks083f_address = 0x26;

// ------------------ Timers ----------------------

unsigned long statusTimer_Ready = 240000;
unsigned long statusTimer_Dialing = 5000;
unsigned long statusTimer_Ringing = 10000;
unsigned long statusTimer_pulsDialing = 3000;
unsigned long statusTimer_toneDialing = 3000;
unsigned long statusTimer_fail = 30000;
unsigned long statusTimer_disconnected = 60000;
unsigned long statusTimer_timeout = 60000;
unsigned long statusTimer_busy = 1000;

int ringLength = 200; // Ring signal lenght in miliseconds

// ------------------ Globala objekt ------------------

LineHandler Line[activeLines] = {
    LineHandler(0),
    LineHandler(1),
    LineHandler(2),
    LineHandler(3)
};

Adafruit_MCP23X17 mcp_ks083f;
Adafruit_MCP23X17 mcp_mt8816;
MQTTHandler mqttHandler(wifiLED, mqttLED);
MT8816 mt8816(mcp_mt8816, (uint8_t[]){AX0, AX1, AX2, AX3}, (uint8_t[]){AY0, AY1, AY2}, STROBE, DATA, RESET, CS);
RingHandler ringHandler(mcp_ks083f, activeLines, RMPins, FRPins, ringLength);

// ---------------SHK variables--------------------

uint8_t SHKDebouncingTime = 10;       // Debounce time for SHK signals
const unsigned pulseGapMax = 80;      // Max time between digit pulses 
unsigned long edge = 0;               // Timestamp for the last puls edge
const unsigned long gapTimeout = 100; // Timeout for pulsing


// ---------------System variables ---------------

bool error = false;       // Error flag
int lastLineReady = -1;   // Last line that was ready. Used to guess witch line thats sends dtmf tones.

// ------------------ Funktions ------------------

void i2CScanner() {
    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    } else {
        Serial.println("Scanning done\n");
    }
}

// Function to setup MCP pins
void setupMCPPins() {

  //KS830F
  if (!mcp_ks083f.begin_I2C(mcp_ks083f_address)) {
    Serial.println("MCP for KSF083f initialization failed. Check connections and address.");
    error = true;
  } else {
  // Set all SHK pins as inputs with internal pull-up resistors
  for (int pinIndex = 0; pinIndex < activeLines; pinIndex++) {
    mcp_ks083f.pinMode(SHKPins[pinIndex], INPUT_PULLUP);
  }

  for (int pinIndex = 0; pinIndex < activeLines; pinIndex++) {
    mcp_ks083f.pinMode(RMPins[pinIndex], OUTPUT);
  }

  for (int pinIndex = 0; pinIndex < activeLines; pinIndex++) {
    mcp_ks083f.pinMode(FRPins[pinIndex], OUTPUT);
  }
  Serial.println("MCP for KSF083f initialized successfully.");
  }


  //MT8816
  if (!mcp_mt8816.begin_I2C(mcp_mt8816_address)) {
    Serial.println("MCP for MT8816 initialization failed. Check connections and address.");
    error = true;

  } else {
    // Configure address pins
    for (int i = 0; i < 4; ++i) {
        mcp_mt8816.pinMode(ax_pins[i], OUTPUT);}
    for (int i = 0; i < 3; ++i) {
        mcp_mt8816.pinMode(ay_pins[i], OUTPUT);}

    // Configure programming pins 
    mcp_mt8816.pinMode(STROBE, OUTPUT);
    mcp_mt8816.pinMode(DATA, OUTPUT);
    mcp_mt8816.pinMode(RESET, OUTPUT);
    mcp_mt8816.pinMode(CS, OUTPUT);
    
    // Set initial values
    mcp_mt8816.digitalWrite(STROBE, LOW);
    mcp_mt8816.digitalWrite(DATA, LOW);
    mcp_mt8816.digitalWrite(RESET, HIGH);
    mcp_mt8816.digitalWrite(CS, HIGH);

    Serial.println("MCP for MT8816 initialized successfully.");
  }
  
    // Om något gick fel, stoppa programmet
  if (error) {
    Serial.println("One or more I2C devices failed to initialize. Stopping execution.");
    while (true) {
      delay(1000);
    }
  }
}

// Function to check if all lines are idle
bool allLinesIdle() {
    for (int line = 0; line < activeLines; line++) {
        if (Line[line].currentLineStatus != line_idle) {
            digitalWrite(hookLED, HIGH);
            return false;
        }
    }
    digitalWrite(hookLED, LOW);
    return true;
}

void timerHandler() {

  // Line timer Check
  for (int line = 0; line < activeLines; line++) {
    // Check if the line status is not idle
    if (Line[line].currentLineStatus != line_idle) {
      // Check if the line timer is active
      if (Line[line].lineTimerActive) {
        // if the line timer has expired, trigger lineTimerExpired()
        if (millis() - Line[line].lineTimerStart > Line[line].lineTimerLimit) {
          lineTimerExpired(line);
          }
        }
      }
    }
  }

// MT8870 Setup
void setupMT8870() {
    pinMode(Q1, INPUT);
    pinMode(Q2, INPUT);
    pinMode(Q3, INPUT);
    pinMode(Q4, INPUT);
    pinMode(STD, INPUT);
}
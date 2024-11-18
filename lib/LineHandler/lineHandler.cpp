#include "LineStatusHandler.h"
#include <Arduino.h>
#include <string.h>

// Constructor
LineSystem::LineSystem(int line) {

    line_number = line;             // Identifier for the line (0-7)
    currentLineStatus = line_idle;  // Current status of the line
    previousLineStatus = line_idle; // Previous status for the line
    hookStatus = hook_on;           // Status of the hook (hook on/off)
    SHKState = 0;                   // Current state of the SHK pin (0 = hook on, 1 = hook off)
    lastSHKBounceTime = 0;      // Last time the SHK pin changed state
    pulsing = false;            // Flag to indicate if the line is currently pulsing
    pulsCount = 0;              // Count number of pulses
    diledDigits = "";           // String to store the dialed digits
    edgeTimestamp = 0;          // Timestamp for the last edge
    
    lineTimerLimit = 0;         // Current limit for the line timer
    lineTimerStart = 0;         // Start time for the line timer
    lineTimerActive = false     // Flag to indicate if the line timer is active
}

// Change line status
void LineSystem::setLineStatus(uint32_t new_status) {
    previousLineStatus = currentLineStatus;
    currentLineStatus = new_status;
    lineTimerLimit = 0;
    lineTimerActive = false;
}

// Start line timer
void LineSystem::startLineTimer(unsigned int limit) {
    lineTimerStart = millis();
    lineTimerLimit = limit;
    lineTimerActive = true;
}

// Stops and clears line timer
void LineSystem::stopLineTimer() {
    lineTimerActive = false;
    lineTimerStart = 0;
}

// Process a new digit for a specific line
void LineSystem::newDigitReceived(char digit) {
    diledDigits += digit;
    lineTimerStart = millis();
}
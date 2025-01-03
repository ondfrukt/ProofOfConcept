#include "LineHandler.h"
#include <Arduino.h>
#include <string.h>

// Constructor
LineHandler::LineHandler(int line) {

    line_number = line;             // Identifier for the line (0-7)
    phoneNumber = line;             // Phone number for the line
    currentLineStatus = line_idle;  // Current status of the line
    previousLineStatus = line_idle; // Previous status for the line
    hookStatus = hook_on;           // Status of the hook (hook on/off)
    SHK = 0;                        // Current state of the SHK pin (0 = hook on, 1 = hook off)
    previousSHKState = 0;           // previous state of the SHK pin (0 = hook on, 1 = hook off)
    lastDebounceTime = 0;           // Last time the SHK pin changed state
    gap = 0;                        // Time from last edge
    pulsing = false;                // Flag to indicate if the line is currently pulsing
    pulsCount = 0;                  // Count number of pulses
    dialedDigits = "";              // String to store the dialed digits
    edge = millis();                // Timestamp for the last edge
    lineTimerLimit = 0;             // Current limit for the line timer
    lineTimerStart = 0;             // Start time for the line timer
    lineTimerActive = false;        // Flag to indicate if the line timer is active
}

// Change line status
void LineHandler::setLineStatus(lineStatuses new_status) {
    previousLineStatus = currentLineStatus;
    currentLineStatus = new_status;
    lineTimerLimit = 0;
    lineTimerActive = false;
}

// Start line timer
void LineHandler::startLineTimer(unsigned int limit) {
    lineTimerStart = millis();
    lineTimerLimit = limit;
    lineTimerActive = true;
}

// Stops and clears line timer
void LineHandler::stopLineTimer() {
    lineTimerActive = false;
    lineTimerStart = 0;
}

// Process a new digit for a specific line
void LineHandler::newDigitReceived(char digit) {
    dialedDigits += digit;
    lineTimerStart = millis();
}
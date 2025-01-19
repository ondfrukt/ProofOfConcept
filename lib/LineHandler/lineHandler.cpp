#include "LineHandler.h"
#include <Arduino.h>
#include <string.h>

// Constructor
LineHandler::LineHandler(int line) {

    lineNumber = line;              // Identifier for the line (0-7)
    phoneNumber = String(line);     // Phone number for the line
    currentLineStatus = line_idle;  // Current status of the line
    previousLineStatus = line_idle; // Previous status for the line
    incomingFrom = 255;         // Incoming call from
    outgoingTo = 255;           // Outgoing call to
    hookStatus = hook_on;           // Status of the hook (hook on/off)
    SHK = 0;                        // Current state of the SHK pin (0 = hook on, 1 = hook off)
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
void LineHandler::setLineStatus(lineStatuses newStatus) {
    previousLineStatus = currentLineStatus;
    currentLineStatus = newStatus;
    if (newStatus == line_idle) lineIdle();
    Serial.println("Line " + String(lineNumber) + " status: " + statusNames[newStatus]);
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
    Serial.println("Digit received: " + String(digit));
    lineTimerStart = millis();
}
void LineHandler::resetDialedDigits() {
    dialedDigits = "";
} 
void LineHandler::lineIdle() {
    dialedDigits = "";
    stopLineTimer();
    pulsing = false;
    pulsCount = 0;                  
    dialedDigits = "";              
    edge = 0;                       
    lineTimerLimit = 0;             
    lineTimerStart = 0;             
    lineTimerActive = false;        
}

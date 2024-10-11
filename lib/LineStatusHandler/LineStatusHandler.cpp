#include "LineStatusHandler.h"
#include <Arduino.h>
#include <vector>

// Constructor
LineSystem::LineSystem(int activeLines) {
    lineVector.resize(activeLines);
    for (int i = 0; i < 8; ++i) {
        lineVector[i].line_number = i;
    }
}

// Set the status of a specific line
void LineSystem::setLineStatus(int lineNumber, uint32_t new_status) {
    if (lineNumber >= 0 && lineNumber < lineVector.size()) {
        lineVector[lineNumber].previousStatus = lineVector[lineNumber].currentStatus;
        lineVector[lineNumber].currentStatus = new_status;
        lineVector[lineNumber].lineTimerLimit = 0;
        lineVector[lineNumber].lineTimerActive = false;
    } else {
        Serial.println("Invalid line number!");
    }
}


// Start line timer
void LineSystem::startLineTimer(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < lineVector.size()) {
        lineVector[lineNumber].lineTimerStart = millis();
        lineVector[lineNumber].lineTimerActive = true;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Stops and celear line timer
void LineSystem::stopLineTimer(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < lineVector.size()) {
        lineVector[lineNumber].lineTimerActive = false;
        lineVector[lineNumber].lineTimerStart = 0;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Display the status of all lines
void LineSystem::displayAllLineStatuses() {
    for (int i = 0; i < lineVector.size(); ++i) {
        Serial.print("Line ");
        Serial.print(lineVector[i].line_number);
        Serial.print(": ");
        Serial.println(this->getStatusString(static_cast<lineStatuses>(lineVector[i].currentStatus)));
    }
}

// Helper function to convert status enum to string
const __FlashStringHelper* LineSystem::getStatusString(lineStatuses status) {
    // Convert enum to string representation
    switch(status) {
        case line_idle: return F("Idle");
        case line_ready: return F("Ready");
        case line_pulse_dialing: return F("Pulse Dialing");
        case line_tone_dialing: return F("Tone Dialing");
        case line_busy: return F("Busy");
        case line_fail: return F("Fail");
        case line_ringing: return F("Ringing");
        case line_connected: return F("Connected");
        case line_disconnected: return F("Disconnected");
        case line_timeout: return F("Timeout");
        case line_abandoned: return F("Abandoned");
        case line_incoming: return F("Incoming");
        case line_operator: return F("Operator");
        case system_config: return F("System Config");
        default: return F("Unknown");
    }
}
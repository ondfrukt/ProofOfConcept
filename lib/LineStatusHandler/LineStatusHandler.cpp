#include "LineStatusHandler.h"

// Constructor
LineSystem::LineSystem() {
    // Initialize all lines to idle state
    for (int i = 0; i < 8; ++i) {
        lineArray[i].line_number = i;
        lineArray[i].currentStatus = line_idle;
    }
}

// Set the status of a specific line
void LineSystem::setLineStatus(int lineNumber, statuses new_status) {
    if (lineNumber >= 0 && lineNumber < 8) {
        lineArray[lineNumber].previousStatus = lineArray[lineNumber].currentStatus;
        lineArray[lineNumber].currentStatus = new_status;
        lineArray[lineNumber].lineTimerLimit = 0;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Get the current status of a specific line
statuses LineSystem::getCurrentLineStatus(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].currentStatus;
    }
    Serial.println("Invalid line number!");
    return line_idle;  // Return default state for invalid line
}

// Display the status of all lines
statuses LineSystem::getPreviousLineStatus(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].previousStatus;
    }
    Serial.println("Invalid line number!");
    return line_idle;  // Return default state for invalid line
}

// Set the timer limit for a specific line
void LineSystem::setLineTimerLimit(int lineNumber, long unsigned int limit) {
    if (lineNumber >= 0 && lineNumber < 8) {
        lineArray[lineNumber].lineTimerLimit = limit;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Start line timer
void LineSystem::startLineTimer(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        lineArray[lineNumber].lineTimerStart = millis();
    } else {
        Serial.println("Invalid line number!");
    }
}

// Get timer start time for a specific line
long unsigned int LineSystem::getLineTimerStart(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].lineTimerStart;
    }
    Serial.println("Invalid line number!");
    return 0;
}

// Set the timer limit for a specific line
long unsigned int LineSystem::getLineTimerLimit(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].lineTimerLimit;
    }
    Serial.println("Invalid line number!");
    return 0;
}

// Display the status of all lines
void LineSystem::displayAllLineStatuses() {
    for (int i = 0; i < 8; ++i) {
        Serial.print("Line ");
        Serial.print(lineArray[i].line_number);
        Serial.print(": ");
        Serial.println(this->getStatusString(lineArray[i].currentStatus));
    }
}

// Marking a lines timer as active
void LineSystem::setLineTimerFlag(int line) {
    lineTimerFlags |= (1 << line);
}
// Clear line timer flag
void LineSystem::clearLineTimerFlag(int line) {
    lineTimerFlags &= ~(1 << line);
}

// Helper function to convert status enum to string
const __FlashStringHelper* LineSystem::getStatusString(statuses status) {
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
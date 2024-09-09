#include "LineStatusHandler.h"

LineSystem::LineSystem() {
    // Initialize all lines to idle state
    for (int i = 0; i < 8; ++i) {
        lineArray[i].line_number = i;
        lineArray[i].currentStatus = line_idle;
    }
}

void LineSystem::setLineStatus(int lineNumber, statuses new_status) {
    
    // Uppdating line status and store time when last changed
    if (lineNumber >= 0 && lineNumber < 8) {
        lineArray[lineNumber].previousStatus = lineArray[lineNumber].currentStatus;
        lineArray[lineNumber].currentStatus = new_status;
        lineArray[lineNumber].lastTimeChanged = millis();
    } else {
        Serial.println("Invalid line number!");
    }
}

statuses LineSystem::getCurrentLineStatus(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].currentStatus;
    }
    Serial.println("Invalid line number!");
    return line_idle;  // Return default state for invalid line
}

statuses LineSystem::getPreviousLineStatus(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].previousStatus;
    }
    Serial.println("Invalid line number!");
    return line_idle;  // Return default state for invalid line
}

long unsigned int LineSystem::getLastTimeChanged(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < 8) {
        return lineArray[lineNumber].lastTimeChanged;
    }
    Serial.println("Invalid line number!");
    return 0;
}

void LineSystem::displayAllLineStatuses() {
    for (int i = 0; i < 8; ++i) {
        Serial.print("Line ");
        Serial.print(lineArray[i].line_number);
        Serial.print(": ");
        Serial.println(this->getStatusString(lineArray[i].currentStatus));
    }
}

const __FlashStringHelper* LineSystem::getStatusString(statuses status) {
    // Convert enum to string representation
    switch(status) {
        case line_idle: return F("Idle");
        case line_ready: return F("Ready");
        case line_pulse_dialing: return F("Pulse Dialing");
        case line_tone_dialing: return F("Tone Dialing");
        case line_connecting: return F("Connecting");
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
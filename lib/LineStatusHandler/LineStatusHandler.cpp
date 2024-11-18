#include "LineStatusHandler.h"
#include <Arduino.h>
#include <string.h>

#define MAX_LINES 8 // Max antal linjer som stöds

// LineStatus struct
struct LineStatus {
    int line_number = 0;
    uint32_t currentLineStatus = 0;
    uint32_t previousLineStatus = 0;
    unsigned long lineTimerStart = 0;
    unsigned int lineTimerLimit = 0;
    bool lineTimerActive = false;
    String diledDigits;
};


// Constructor
LineSystem::LineSystem(int activeLines) {
    for (int i = 0; i < activeLines && i < MAX_LINES; ++i) {
        lineArray[i].line_number = i;
        phoneBook[i] = String(i + 1); // Första linjen får 1 som telefonnummer
        lineArray[i].currentLineStatus = line_idle;
        lineArray[i].previousLineStatus = line_idle;

    }
}

// Set the status of a specific line
void LineSystem::setLineStatus(int lineNumber, uint32_t new_status) {
    if (lineNumber >= 0 && lineNumber < MAX_LINES) {
        lineArray[lineNumber].previousLineStatus = lineArray[lineNumber].currentLineStatus;
        lineArray[lineNumber].currentLineStatus = new_status;
        lineArray[lineNumber].lineTimerLimit = 0;
        lineArray[lineNumber].lineTimerActive = false;
    } else {
        Serial.println("Invalid line number!");
    }

    // Printing new status
    Serial.print("Line ");
    Serial.print(lineNumber);
    Serial.print(" status changed to: ");
    Serial.println(this->getStatusString(static_cast<lineStatuses>(new_status)));

    // Kontrollera om alla linjer är inaktiva
    allLinesIdle = true;
    for (int i = 0; i < MAX_LINES; ++i) {
        if (lineArray[i].currentLineStatus != line_idle) {
            allLinesIdle = false;
            break;
        }
    }
}

// Change number to a specific line
void LineSystem::setNewPhoneNumber(int line, String newNumber) {
    if (line >= 0 && line < MAX_LINES) {
        phoneBook[line] = newNumber;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Start line timer
void LineSystem::startLineTimer(int lineNumber, unsigned int limit) {
    if (lineNumber >= 0 && lineNumber < MAX_LINES) {
        lineArray[lineNumber].lineTimerStart = millis();
        lineArray[lineNumber].lineTimerLimit = limit;
        lineArray[lineNumber].lineTimerActive = true;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Stops and clears line timer
void LineSystem::stopLineTimer(int lineNumber) {
    if (lineNumber >= 0 && lineNumber < MAX_LINES) {
        lineArray[lineNumber].lineTimerActive = false;
        lineArray[lineNumber].lineTimerStart = 0;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Display the status of all lines
void LineSystem::displayAllLineStatuses() {
    for (int i = 0; i < MAX_LINES; ++i) {
        Serial.print("Line ");
        Serial.print(lineArray[i].line_number);
        Serial.print(": ");
        Serial.println(this->getStatusString(static_cast<lineStatuses>(lineArray[i].currentLineStatus)));
    }
}

// Process a new digit for a specific line
void LineSystem::newDigitReceived(int line, char digit) {
    if (line >= 0 && line < MAX_LINES) {
        lineArray[line].diledDigits += digit;
        lineArray[line].lineTimerStart = 0;
    } else {
        Serial.println("Invalid line number!");
    }
}

// Helper function to convert status enum to string
const __FlashStringHelper* LineSystem::getStatusString(lineStatuses status) {
    switch (status) {
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

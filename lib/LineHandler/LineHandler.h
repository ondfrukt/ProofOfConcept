#ifndef LineHandler_h
#define LineHandler_h

#include <Arduino.h>
#include <string.h>
#include "../src/lineStatuses.h"

// Max antal linjer som stöds
#define MAX_LINES 8

enum lineStatuses;
enum hookStatuses;



// LineSystem-klass
class LineHandler {
public:
    int line_number;                    // Identifier for the line (0-7)
    String phoneNumber;
    lineStatuses currentLineStatus;     // Current status of the line
    lineStatuses previousLineStatus;    // Previous status for the line

    hookStatuses hookStatus;            // Status of the hook (hook on/off)
    bool SHK;                      // Current state of the SHK pin (0 = hook on, 1 = hook off)
    bool previousSHKState;              // Current state of the SHK pin (0 = hook on, 1 = hook off)
    unsigned long lastDebounceTime;    // Last time the SHK pin changed state
    bool lastDebounceValue;

    unsigned gap;                       // Time from last edge
    unsigned long edge;                 // Timestamp for the last edge
    bool pulsing;                       // Flag to indicate if the line is currently pulsing
    int pulsCount;                      // Count number of pulses
    bool pulsingFlag;                   // Flag to indicate if pulsing are active
    String dialedDigits;                // char to store the dialed digits

    
    unsigned int lineTimerLimit;        // Current limit for the line timer
    unsigned long lineTimerStart;       // Start time for the line timer
    bool lineTimerActive;               // Flag to indicate if the line timer is active

    LineHandler(int line);
    void setLineStatus(lineStatuses new_status);
    void startLineTimer(unsigned int limit);
    void stopLineTimer();
    void newDigitReceived(char digit);
    void setNewPhoneNumber(String newNumber);
};

#endif
#ifndef LineStatusHandler_h
#define LineStatusHandler_h

#include <Arduino.h>

// Enum representing all possible statuses of a line 
enum statuses {
    line_idle,           // Line is not in use
    line_ready,          // Dial tone is playing, waiting for input
    line_pulse_dialing,  // Old-style rotary dialing in progress
    line_tone_dialing,   // Modern touch-tone dialing in progress
    line_busy,           // Receiving busy signal
    line_fail,           // Line failed to connect
    line_ringing,        // Line is ringing (outgoing call)
    line_connected,      // Call is active
    line_disconnected,   // Call has ended, but line not yet idle
    line_timeout,        // Line timed out
    line_abandoned,      // Line was abandoned
    line_incoming,       // Incoming call
    line_operator,       // Connected to operator
    system_config        // Line is in configuration mode
};

// Structure representing a single line
struct Line {
    int line_number;                        // Identifier for the line (0-7)
    statuses currentStatus;                 // Current status of the line
    statuses previousStatus;                // Previus status for the line
    unsigned int lineTimerLimit = 0;        // Current limit for the line timer
    unsigned long int lineTimerStart = 0;   // Start time for the line
};

class LineSystem {
public:
    // Binary 8-bit to store if a timer is active due to each line
    byte lineTimerFlags;
    // Constructor
    LineSystem();
    // Set the status of a specific line
    void setLineStatus(int line, statuses new_status);
    // Get the current status of a specific line
    statuses getCurrentLineStatus(int line);
    // Display the status of all lines
    statuses getPreviousLineStatus(int line);

    // Display the status of all lines
    void displayAllLineStatuses();
    // Marking a lines timer as active
    void setLineTimerFlag (int line);
    // Clear line timer flag
    void clearLineTimerFlag (int line);

    // Set the timer limit for a specific line
    void setLineTimerLimit(int line, long unsigned int limit);
    // Get the timer limit for a specific line
    long unsigned int getLineTimerLimit(int line);
    // Start line timer
    void startLineTimer(int line);
    // Get timer start time for a specific line
    long unsigned int getLineTimerStart(int line);

private:
    Line lineArray[8];  // Array to hold 8 lines
    // Helper function to convert status enum to string
    const __FlashStringHelper* getStatusString(statuses status);
};

#endif // LineStatusHandler_h
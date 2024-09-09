#ifndef LineStatusHandler_h
#define LineStatusHandler_h

#include <Arduino.h>

// Enum representing all possible statuses of a line
enum statuses {
    line_idle,           // Line is not in use
    line_ready,          // Dial tone is playing, waiting for input
    line_pulse_dialing,  // Old-style rotary dialing in progress
    line_tone_dialing,   // Modern touch-tone dialing in progress
    line_connecting,     // Attempting to establish a connection
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
    int line_number;                    // Identifier for the line (0-7)
    statuses currentStatus;             // Current status of the line
    statuses previousStatus;             // Previus status for the line
    unsigned long int lastTimeChanged = 0;  // Timestampe for last change
};

class LineSystem {
public:
    LineSystem();
    // Set the status of a specific line
    void setLineStatus(int line, statuses new_status);
    // Get the current status of a specific line
    statuses getCurrentLineStatus(int line);
    // Display the status of all lines
    statuses getPreviousLineStatus(int line);
    // Display the status of all lines
    long unsigned int getLastTimeChanged(int line);
    // Display the status of all lines
    void displayAllLineStatuses();
    
private:
    Line lineArray[8];  // Array to hold 8 lines
    // Helper function to convert status enum to string
    const __FlashStringHelper* getStatusString(statuses status);
};

#endif // LineStatusHandler_h
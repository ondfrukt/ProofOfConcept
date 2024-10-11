#ifndef LineStatusHandler_h
#define LineStatusHandler_h

#include <Arduino.h>
#include <vector>

// Enum representing all possible statuses of a line 
enum lineStatuses {
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
struct lineStruct {
    int line_number;                        // Identifier for the line (0-7)
    uint32_t currentStatus;                 // Current status of the line
    uint32_t previousStatus;                // Previus status for the line
    unsigned int lineTimerLimit = 0;        // Current limit for the line timer
    unsigned long int lineTimerStart = 0;   // Start time for the line
    bool lineTimerActive = false;               // Flag to indicate if the timer is active
};

class LineSystem {
public:
    // Constructor
    LineSystem(int activeLines);

    // Vector to store line information
    std::vector<lineStruct> lineVector;
    void setLineStatus(int line, uint32_t new_status);
    void displayAllLineStatuses();
    void startLineTimer(int line);
    void stopLineTimer(int line);

private:
    // Helper function to convert status enum to string
    const __FlashStringHelper* getStatusString(lineStatuses status);
};

#endif // LineStatusHandler_h
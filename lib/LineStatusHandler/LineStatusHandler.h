#ifndef LineStatusHandler_h
#define LineStatusHandler_h

#include <Arduino.h>
#include <vector>

// Enum representing all possible statuses of the hook
enum hookStatuses {
    hook_off,
    hook_on
};

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
    hookStatuses hookStatus;                      // Flag to indicate the status of the hook. false = on, true = off
    unsigned int long lastSHKBounceTime;    // Last time the SHK pin changed state
    uint8_t currentSHKState;                // Current state of the SHK pin (0 = hook on, 1 = hook off)

    uint32_t currentLineStatus;             // Current status of the line
    uint32_t previousLineStatus;            // Previus status for the line
    unsigned int lineTimerLimit = 0;        // Current limit for the line timer
    unsigned long int lineTimerStart = 0;   // Start time for the line
    bool lineTimerActive = false;           // Flag to indicate if the line timer is active
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
    bool allLinesIdle;

private:
    // Helper function to convert status enum to string
    const __FlashStringHelper* getStatusString(lineStatuses status);
};

#endif // LineStatusHandler_h
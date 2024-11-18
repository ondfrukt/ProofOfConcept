#ifndef LineHandler_h
#define LineHandler_h

#include <Arduino.h>
#include <string.h>

// Max antal linjer som stöds
#define MAX_LINES 8

// Enum representing all possible statuses of the hook
enum hookStatuses {
    hook_off,
    hook_on
    disconnected
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


// LineSystem-klass
class LineHandler {
public:
    int line_number;                    // Identifier for the line (0-7)
    uint32_t currentLineStatus;         // Current status of the line
    uint32_t previousLineStatus;        // Previous status for the line

    hookStatuses hookStatus;            // Status of the hook (hook on/off)
    uint8_t SHKState;                   // Current state of the SHK pin (0 = hook on, 1 = hook off)
    unsigned long lastSHKBounceTime;    // Last time the SHK pin changed state

    bool pulsing                        // Flag to indicate if the line is currently pulsing
    int int pulsCount                   // Count number of pulses
    String diledDigits                  // String to store the dialed digits
    unsigned long edgeTimestamp;        // Timestamp for the last edge
    
    unsigned int lineTimerLimit         // Current limit for the line timer
    unsigned long lineTimerStart        // Start time for the line timer
    bool lineTimerActive                // Flag to indicate if the line timer is active

    void setLineStatus(uint32_t new_status);
    void startLineTimer(unsigned int limit);
    void stopLineTimer();
    void newDigitReceived(char digit);
    void setNewPhoneNumber(String newNumber);
};

#endif
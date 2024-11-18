#ifndef LineStatusHandler_h
#define LineStatusHandler_h

#include <Arduino.h>
#include <string.h>

// Max antal linjer som stöds
#define MAX_LINES 8

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
    uint32_t currentLineStatus;             // Current status of the line
    uint32_t previousLineStatus;            // Previous status for the line

    hookStatuses hookStatus = hook_on;      // Status of the hook (hook on/off)
    uint8_t SHKState = 0;                   // Current state of the SHK pin (0 = hook on, 1 = hook off)
    uint8_t lastSHKState = 0;               // Last state of the SHK pin                  
    unsigned long lastSHKBounceTime = 0;    // Last time the SHK pin changed state

    bool pulsing = false;                   // Flag to indicate if the line is currently pulsing
    int pulsCount = 0;                      // Count number of pulses
    String diledDigits = "";                // String to store the dialed digits
    unsigned long edgeTimestamp = 0;        // Timestamp for the last edge
    
    unsigned int lineTimerLimit = 0;        // Current limit for the line timer
    unsigned long lineTimerStart = 0;       // Start time for the line timer
    bool lineTimerActive = false;           // Flag to indicate if the line timer is active
};

// LineSystem-klass
class LineSystem {
public:
    // Constructor
    LineSystem(int activeLines);

    // Telefonbok för linjer
    String phoneBook[MAX_LINES];

    // Array för att lagra linjeinformation
    lineStruct lineArray[MAX_LINES];

    void setLineStatus(int line, uint32_t new_status);
    void displayAllLineStatuses();
    void startLineTimer(int line, unsigned int limit);
    void stopLineTimer(int line);
    void newDigitReceived(int line, char digit);
    void setNewPhoneNumber(int line, String newNumber);

    bool allLinesIdle;

private:
    // Hjälpfunktion för att konvertera status enum till sträng
    const __FlashStringHelper* getStatusString(lineStatuses status);
};

#endif // LineStatusHandler_h

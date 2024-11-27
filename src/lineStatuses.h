#include <Arduino.h>

#ifndef lineStatuses_h
#define lineStatuses_h

// Enum representing all possible statuses of a line
enum lineStatuses : uint8_t {
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

const String statusNames[14] = { 
    "line_idle",           // Line is not in use
    "line_ready",          // Dial tone is playing, waiting for input
    "line_pulse_dialing",  // Old-style rotary dialing in progress
    "line_tone_dialing",   // Modern touch-tone dialing in progress
    "line_busy",           // Receiving busy signal
    "line_fail",           // Line failed to connect
    "line_ringing",        // Line is ringing (outgoing call)
    "line_connected",      // Call is active
    "line_disconnected",   // Call has ended, but line not yet idle
    "line_timeout",        // Line timed out
    "line_abandoned",      // Line was abandoned
    "line_incoming",       // Incoming call
    "line_operator",       // Connected to operator
    "system_config"        // Line is in configuration mode
};

// Enum representing all possible statuses of the hook
enum hookStatuses {
    hook_off,
    hook_on,
    disconnected
};


#endif
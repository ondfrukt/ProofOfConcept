#ifndef lineAction_h
#define lineAction_h

#include <Arduino.h>
#include "config.h"

void lineAction(int line, uint8_t newLineStatus);
void lineTimerExpired(int line);

#endif
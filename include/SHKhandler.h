#ifndef SHKHandler_h
#define SHKHandler_h

#include <Arduino.h>
#include "config.h"
#include "pulsHandler.h"

void SHKHandler();

void hookChange(int line, hookStatuses newHookState);
void setupHookChecker();

#endif
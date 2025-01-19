#ifndef hookChange_h
#define hookChange_h

#include <Arduino.h>
#include "config.h"
#include "lineAction.h"

void hookChange(int line, hookStatuses newHookState);
void setupHookChecker();

#endif

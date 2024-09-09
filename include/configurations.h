#include <Arduino.h>

// Hook stauts inputs
const int numberOfLines = 4;
const int SHKPins[numberOfLines] = {2, 3, 4, 5};
const unsigned long hookDebounceDelay = 200;
int hookStates[numberOfLines] = {LOW, LOW, LOW, LOW};
unsigned long lastSHKDebounceTime[numberOfLines] = {0, 0, 0, 0};




#ifndef SHK_sense_h
#define SHK_sense_h
#include <Arduino.h>

class SHK_sense {
    public:
        SHK_sense(int SHK1, int SHK2, int SHK3, int SHK4, int numberOfLines, unsigned long SHKDebounceTime);
        void begin();
        bool readSHK(int lineIndex);
    private:
        int _pins;
        bool _lastPinState;
        unsigned long _lastDebounceTime;
        unsigned long _debounceDelay;
};


#endif
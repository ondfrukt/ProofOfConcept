#ifndef TONEGEN_H
#define TONEGEN_H

#include <Arduino.h>

#define TWO_PI 6.28318530718
#define MODE_NORMAL 0
#define MODE_UNOBTAINABLE 1

class toneGen {
public:
    toneGen(int dacPin);
    void begin(uint32_t sampleRate);
    void dialTone();
    void ringTone();
    void busyTone();
    void unobtainableTone();
    void stopTone();
    void update();

private:
    int _dacPin;
    uint32_t _sampleRate;
    float _f1, _f2;
    float _phase1, _phase2;
    float _phaseInc1, _phaseInc2;
    bool _toneActive;
    bool _usePulse;
    uint32_t _onTime, _offTime;
    uint32_t _lastToggleTime;
    int _mode;
    int _unobtainableIndex;

    static void _toneTask(void *pvParameters);
    void _updateDAC();
};

#endif

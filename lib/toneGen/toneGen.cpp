#include "toneGen.h"
#include <math.h>
#include "Arduino.h"

// Konstruktor
toneGen::toneGen(int dacPin)
  : _dacPin(dacPin),
    _sampleRate(50000),
    _f1(0), _f2(0),
    _phase1(0), _phase2(0),
    _toneActive(false),
    _usePulse(false),
    _onTime(0),
    _offTime(0),
    _lastToggleTime(0),
    _mode(MODE_NORMAL),
    _unobtainableIndex(0),
    _taskHandle(NULL) {}

void toneGen::begin(uint32_t sampleRate) {
    _sampleRate = sampleRate;

    // Skapa FreeRTOS-task som kör på kärna 0
    xTaskCreatePinnedToCore(
        _toneTask,
        "ToneTask",
        10000,
        this,
        1,
        &_taskHandle,
        0
    );
}

void toneGen::_toneTask(void *pvParameters) {
    toneGen *instance = static_cast<toneGen *>(pvParameters);
    while (true) {
        instance->update();
        instance->_updateDAC();
        vTaskDelay(1);
    }
}

void toneGen::dialTone() {
    _mode = MODE_NORMAL;
    _f1 = 425.0;
    _f2 = 425.0;
    _usePulse = false;
    _toneActive = true;
    _phase1 = 0;
    _phase2 = 0;
    _phaseInc1 = TWO_PI * _f1 / _sampleRate;
    _phaseInc2 = TWO_PI * _f2 / _sampleRate;
}

void toneGen::ringTone() {
    _mode = MODE_NORMAL;
    _f1 = 425.0;
    _f2 = 425.0;
    _usePulse = true;
    _onTime = 1000;
    _offTime = 5000;
    _lastToggleTime = millis();
    _toneActive = true;
    _phase1 = 0;
    _phase2 = 0;
    _phaseInc1 = TWO_PI * _f1 / _sampleRate;
    _phaseInc2 = TWO_PI * _f2 / _sampleRate;
}

void toneGen::busyTone() {
    _mode = MODE_NORMAL;
    _f1 = 425.0;
    _f2 = 425.0;
    _usePulse = true;
    _onTime = 250;
    _offTime = 250;
    _lastToggleTime = millis();
    _toneActive = true;
    _phase1 = 0;
    _phase2 = 0;
    _phaseInc1 = TWO_PI * _f1 / _sampleRate;
    _phaseInc2 = TWO_PI * _f2 / _sampleRate;
}

void toneGen::unobtainableTone() {
    _mode = MODE_UNOBTAINABLE;
    _unobtainableIndex = 0;
    _f1 = 950.0;
    _f2 = 950.0;
    _phase1 = 0;
    _phase2 = 0;
    _phaseInc1 = TWO_PI * _f1 / _sampleRate;
    _phaseInc2 = TWO_PI * _f2 / _sampleRate;
    _toneActive = true;
    _lastToggleTime = millis();
}

void toneGen::stopTone() {
    _toneActive = false;
    dacWrite(_dacPin, 128);
}

void toneGen::update() {
    uint32_t currentMillis = millis();
    if (_mode == MODE_NORMAL && _usePulse) {
        if (_toneActive && (currentMillis - _lastToggleTime >= _onTime)) {
            _toneActive = false;
            _lastToggleTime = currentMillis;
        } else if (!_toneActive && (currentMillis - _lastToggleTime >= _offTime)) {
            _toneActive = true;
            _lastToggleTime = currentMillis;
        }
    }
}

void toneGen::_updateDAC() {
    if (_toneActive) {
        float s1 = sin(_phase1);
        float s2 = sin(_phase2);
        float sample = (s1 + s2) / 2.0;
        uint8_t dacVal = (uint8_t)((sample + 1.0) * 127.5);
        dacWrite(_dacPin, dacVal);
        _phase1 += _phaseInc1;
        if (_phase1 >= TWO_PI) _phase1 -= TWO_PI;
        _phase2 += _phaseInc2;
        if (_phase2 >= TWO_PI) _phase2 -= TWO_PI;
    } else {
        dacWrite(_dacPin, 128);
    }
}

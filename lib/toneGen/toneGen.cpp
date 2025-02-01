#include "toneGen.h"
#include <math.h>

// Initiera statisk instanspekare
toneGen* toneGen::_instance = nullptr;

toneGen::toneGen(int dacPin)
  : _dacPin(dacPin),
    _sampleRate(50000),
    _f1(0), _f2(0),
    _phase1(0), _phase2(0),
    _toneActive(false),
    _timer(nullptr),
    _timerMux(portMUX_INITIALIZER_UNLOCKED),
    _usePulse(false),
    _onTime(0),
    _offTime(0),
    _lastToggleTime(0),
    _mode(MODE_NORMAL),
    _unobtainableIndex(0)
{
}

void toneGen::begin(uint32_t sampleRate) {
  _sampleRate = sampleRate;
  _instance = this;
  _timer = timerBegin(0, 80, true); // 80 MHz/80 = 1 MHz bas
  timerAttachInterrupt(_timer, toneGen::_timerCallback, true);
  timerAlarmWrite(_timer, 1000000 / _sampleRate, true);
  timerAlarmEnable(_timer);
}

void toneGen::dialTone() {
  // DialTone: 425 Hz, kontinuerlig
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
  // RingTone: 425 Hz, pulserande med 1 sekund på och 5 sekunder av
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
  // BusyTone: 425 Hz, pulserande med 0,25 s på och 0,25 s av
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
  // UnobtainableTone: en sekvens med 3 olika frekvenser
  // Sekvensen: 950 Hz, 1400 Hz, 1800 Hz, vardera 0,33 s på följt av 1 s paus.
  _mode = MODE_UNOBTAINABLE;
  _unobtainableIndex = 0;
  // Starta med den första frekvensen:
  _f1 = 950.0;
  _f2 = 950.0;
  _phase1 = 0;
  _phase2 = 0;
  _phaseInc1 = TWO_PI * _f1 / _sampleRate;
  _phaseInc2 = TWO_PI * _f2 / _sampleRate;
  _toneActive = true; // "On" i början
  _lastToggleTime = millis();
}

void toneGen::stopTone() {
  portENTER_CRITICAL(&_timerMux);
  _toneActive = false;
  dacWrite(_dacPin, 128); // Sätt DAC till mittnivå (tyst)
  portEXIT_CRITICAL(&_timerMux);
}

void toneGen::update() {
  uint32_t currentMillis = millis();

  if (_mode == MODE_NORMAL && _usePulse) {
    // Hantera enkel pulsering
    if (_toneActive && (currentMillis - _lastToggleTime >= _onTime)) {
      portENTER_CRITICAL(&_timerMux);
      _toneActive = false;
      _lastToggleTime = currentMillis;
      portEXIT_CRITICAL(&_timerMux);
    }
    else if (!_toneActive && (currentMillis - _lastToggleTime >= _offTime)) {
      portENTER_CRITICAL(&_timerMux);
      _toneActive = true;
      _lastToggleTime = currentMillis;
      portEXIT_CRITICAL(&_timerMux);
    }
  }
  else if (_mode == MODE_UNOBTAINABLE) {
    // Hantera sekvensläge:
    // Varje ton spelas i 330 ms ("on") och sedan följer 1 s paus innan nästa ton i sekvensen startar.
    if (_toneActive && (currentMillis - _lastToggleTime >= 330)) {
      // Slå av tonen (paus)
      portENTER_CRITICAL(&_timerMux);
      _toneActive = false;
      _lastToggleTime = currentMillis;
      portEXIT_CRITICAL(&_timerMux);
    }
    else if (!_toneActive && (currentMillis - _lastToggleTime >= 1000)) {
      // Gå vidare till nästa frekvens i sekvensen
      _unobtainableIndex = (_unobtainableIndex + 1) % 3;
      // Sätt ny frekvens utifrån index:
      if (_unobtainableIndex == 0) {
        _f1 = 950.0;
        _f2 = 950.0;
      } else if (_unobtainableIndex == 1) {
        _f1 = 1400.0;
        _f2 = 1400.0;
      } else if (_unobtainableIndex == 2) {
        _f1 = 1800.0;
        _f2 = 1800.0;
      }
      // Uppdatera faser och fasinkrement
      _phase1 = 0;
      _phase2 = 0;
      _phaseInc1 = TWO_PI * _f1 / _sampleRate;
      _phaseInc2 = TWO_PI * _f2 / _sampleRate;
      portENTER_CRITICAL(&_timerMux);
      _toneActive = true;
      _lastToggleTime = currentMillis;
      portEXIT_CRITICAL(&_timerMux);
    }
  }
}

// Intern timer-ISR: uppdaterar DAC utifrån _toneActive och genererade sinusvärden
void toneGen::_updateDAC() {
  portENTER_CRITICAL_ISR(&_timerMux);
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
  portEXIT_CRITICAL_ISR(&_timerMux);
}

// Statisk callback för timern
void IRAM_ATTR toneGen::_timerCallback() {
  if (_instance != nullptr) {
    _instance->_updateDAC();
  }
}

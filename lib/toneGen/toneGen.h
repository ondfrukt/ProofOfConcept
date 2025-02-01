#ifndef TONEGEN_H
#define TONEGEN_H

#include <Arduino.h>

#ifndef TWO_PI
  #define TWO_PI (2.0 * PI)
#endif

// Klassdefinition för toneGen
class toneGen {
  public:
    // Konstruktor: anger vilken DAC-utgång (t.ex. GPIO25) som ska användas.
    toneGen(int dacPin);

    // Initierar biblioteket med en önskad sampleRate (t.ex. 50000)
    void begin(uint32_t sampleRate = 50000);

    // Funktioner för de olika tonerna:
    void dialTone();         // 425 Hz, kontinuerlig
    void ringTone();         // 425 Hz, 1s på – 5s av
    void busyTone();         // 425 Hz, 0,25s på – 0,25s av
    void unobtainableTone(); // 950/1400/1800 Hz, 0,33 s per ton, sedan 1 s paus

    // Stoppar aktuell ton
    void stopTone();

    // Denna funktion ska anropas i loop() för att hantera pulsering eller state-machine
    void update();

    // (Valfritt) En getter för att se om tonen är aktiv
    bool isToneActive() const { return _toneActive; }

  private:
    int _dacPin;
    uint32_t _sampleRate;
    
    float _f1, _f2;             // De två frekvenserna som spelas (om de är lika blir det en ren ton)
    float _phase1, _phase2;       // Aktuella faser för sinusvågorna
    float _phaseInc1, _phaseInc2; // Fasinkrement per sample

    volatile bool _toneActive;  // Om tonen för närvarande är "på"
    hw_timer_t* _timer;         // Hårdvarutimer för att uppdatera DAC
    portMUX_TYPE _timerMux;     // Mutex för säker avbrottshantering

    // Pulseringsvariabler (för de toner som ska pulseras)
    bool _usePulse;             // Om vi använder en enkel pulsering (on/off)
    uint32_t _onTime;           // On-tid (ms)
    uint32_t _offTime;          // Off-tid (ms)
    uint32_t _lastToggleTime;   // Tidpunkt för senaste toggling

    // För unobtainableTone:
    enum ToneGenMode { MODE_NORMAL, MODE_UNOBTAINABLE };
    ToneGenMode _mode;
    uint8_t _unobtainableIndex;  // Index i frekvenssekvensen för unobtainableTone

    // Statisk callback för timer-ISR
    static void IRAM_ATTR _timerCallback();
    // Statisk pekare till instansen (för att använda i callbacken)
    static toneGen* _instance;

    // Intern metod för att uppdatera DAC:en (anropas från timer-ISR)
    void _updateDAC();
};

#endif

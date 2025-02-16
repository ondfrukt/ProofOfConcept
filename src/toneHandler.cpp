#include "toneHandler.h"

#define DEBOUNCE_TIME 50  // Tid i ms för att ignorera glitchar
unsigned long lastDebounceTime = 0;  
bool lastSTDState = LOW;
bool buttonPressed = false;  // Håller koll på om en knapp redan är registrerad

void toneHandler() {
  bool currentSTDState = digitalRead(STD);

  // Kontrollera om knappen ändrats och debounce-tid har passerat
  if (currentSTDState != lastSTDState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    if (currentSTDState == HIGH && !buttonPressed) {  
      // Endast registrera knapp om den precis tryckts ner och inte redan är registrerad
      buttonPressed = true;

      // Ställ in linjestatus för den senaste linjen

      if (Line[lastLineReady].currentLineStatus != line_tone_dialing){
        Line[lastLineReady].setLineStatus(line_tone_dialing);
      }

      // Läs DTMF-knappar
      int q1 = digitalRead(Q1);
      int q2 = digitalRead(Q2);
      int q3 = digitalRead(Q3);
      int q4 = digitalRead(Q4);

      int dtmf_index = (q1 << 0) | (q2 << 1) | (q3 << 2) | (q4 << 3) & 0x0F;

      // Hämta rätt tecken från tabellen
      char dtmf_value = dtmf_map[dtmf_index];

      // Skicka siffran till telefonlinjen
      Line[lastLineReady].newDigitReceived(dtmf_value);
      Line[lastLineReady].startLineTimer(statusTimer_toneDialing);
    } 
    else if (currentSTDState == LOW) {
      // Om knappen släpps, återställ `buttonPressed` så att nästa tryck kan registreras
      buttonPressed = false;
    }
  }
  lastSTDState = currentSTDState; // Uppdatera senaste status
}

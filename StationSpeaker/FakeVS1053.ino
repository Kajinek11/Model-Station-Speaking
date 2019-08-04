/**
 * Toto je "zpraseny" kod slouzici k odladeni maxima pro VS1053 prehravac, ktery pri vyvoji nemam :)
 * Namisto pouziti skutecneho API atd definujeme prazdne metody jen vypisujici hlasky.
 */
#include "Config.h"
#include "VS1053Config.h"
#include <SD.h>

extern File curFile;

#ifdef VS1053_FAKE
// Umozni pripojit ctecku SD kartet "jako" by byla pripojena
// ctecka, co je soucasti VS1053 prehravace. Pri vyvoji jsem nemel 
// desku, tento "ukrok" mi umoznil aspon odladit hledani souboru
const int SHIELD_CS = A4;
const int SHIELD_DCS = A5;
const int SHIELD_CARDCS = A3;
const int SHIELD_DREQ = 3;

#endif

// rucni signal o konci prehravani
const int FAKE_PLAY_FINISHED = 2;

 void vsDoPlaySound(const char* filename) {
  _DPRINTLN2("Fake playing: ", filename);
}

void _vsfakeSetup() {
  // inicializovano primo jako globalni promenna.
  pinMode(2, INPUT_PULLUP);
  pinMode(SHIELD_CARDCS, OUTPUT);
  if (!SD.begin(SHIELD_CARDCS)) {
    Serial.println(F("Could not initialize card"));
  }
}


#ifdef VS1053_FAKE
void vsDoLoop() {
  // Spojenim pin 2 (FAKE_PLAY_FINISHED) k zemi
  // se signalizuje konec skladby
  static long lastActive = -1;
  if (digitalRead(FAKE_PLAY_FINISHED) == 0) {
    if (millis() >= lastActive + 1000) {
      lastActive = millis();
      playerFinishedCallback();
    }
  }
}

void vsStopSound() {
  _DPRINTLN("Stopping sound");
}

void vsDoSetVolume(int vol) {
}
#endif

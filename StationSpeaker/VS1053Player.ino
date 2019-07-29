#include "Config.h"
#include "VS1053Config.h"

/**
 * Pokusi se na SD karte najit soubor, ktery zacina danym cislem,
 * doplnenym zepredu nulami na MAX_TRACK_DIGITS. Zbytek jmena je zcela
 * libovolny s slouzi jako popisek - kdyby prehravac nekdy dostal display.
 */
File vsFindTrackNumber(int n) {
  File dir = SD.open("/");
  File entry;
  static char selectNumber[MAX_TRACK_DIGITS + 1];
  String format = String("%0") + MAX_TRACK_DIGITS + "d";
  sprintf(selectNumber, format.c_str(), n);

  while (entry = dir.openNextFile()) {
    int l = strlen(selectNumber);
    if (strncmp(selectNumber, entry.name(), l) == 0) {
      // nasel...
      return entry;
    }
    entry.close();
  }
  // vracim prazdny soubor.
  return File();
}

void vsPlaySelectedSound(int track) {
}

void vsStopSound() {
  
}


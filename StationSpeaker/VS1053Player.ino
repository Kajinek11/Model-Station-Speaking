#include "Config.h"
#include "VS1053Config.h"

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
  return File();
}


void vsPlaySelectedSound(int track) {
}

void vsStopSound() {
  
}


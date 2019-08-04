#include "Config.h"

#ifdef VS1053
#include "VS1053Config.h"
#include <SPI.h>
#include <SD.h>


#ifndef VS1053_FAKE
#include <Adafruit_VS1053.h>

Adafruit_VS1053_FilePlayer vsPlayer = Adafruit_VS1053_FilePlayer(
    SHIELD_RESET,
    SHIELD_CS,
    SHIELD_DCS,
    SHIELD_DREQ,
    SHIELD_CARDCS);
#endif

File curFile;

#define STR(x) #x
#define TOK(x) STR(x)

#define FORMAT "%0" TOK(MAX_TRACK_DIGITS) "d"

File sdRoot;

/**
 * Pokusi se na SD karte najit soubor, ktery zacina danym cislem,
 * doplnenym zepredu nulami na MAX_TRACK_DIGITS. Zbytek jmena je zcela
 * libovolny s slouzi jako popisek - kdyby prehravac nekdy dostal display.
 */
boolean vsFindTrackNumber(int n) {
  if (sdRoot) {
    sdRoot.close();
  }
  sdRoot = SD.open("/");
  if (!sdRoot) {
    Serial.println(F("Could not read SD card !"));
    return true;
  }
  sdRoot.rewindDirectory();
  static char selectNumber[MAX_TRACK_DIGITS + 1];
  sprintf(selectNumber, FORMAT, n);
  _DPRINTLN2("File prefix: ", selectNumber);

  while (true) {
    File entry = sdRoot.openNextFile();
    if (!entry) {
      break;
    }
    _DPRINTLN2("Trying: ", entry.name());
    int l = strlen(selectNumber);
    if (strncmp(selectNumber, entry.name(), l) == 0) {
      // nasel...
      curFile = entry;
      _DPRINTLN("Success");
      sdRoot.rewindDirectory();
      return false;
    }
    entry.close();
  }
  // vracim prazdny soubor.
  _DPRINTLN("NOT FOUND");
  sdRoot.rewindDirectory();
  return true;
}

void vsPlaySound(int trackNo) {
  if (vsFindTrackNumber(trackNo) || !curFile) {
    _DPRINTLN2("Track not found: ", trackNo);
    playerMissingTrackCallback();
    return;
  } else {
    _DPRINTLN2("Playing track: ", trackNo);
    _DPRINTLN2("Filename: ", curFile.name());
  }
  vsDoPlaySound(curFile.name());
  curFile.close();
}

#ifndef VS1053_FAKE

void vsDoPlaySound(const char* filename) {
  vsPlayer.startPlayingFile(curFile.name());
}

void vsSetup() {
  if (!vsPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    Serial.println(F("Warning: DREQ pin is not interrupt one"));
  }
}

void vsDoLoop() {
  if (!vsPlayer.playingMusic) {
    playerFinishedCallback();
  }
}

void vsDoSetVolume(int vol) {
  vsPlayer.setVolume(vol, vol);
}


void vsStopSound() {
  _DPRINTLN("Stopping sound");
  vsPlayer.stopPlaying();
}

void _vsRealSetVolume(int vol) {
  vsPlayer.setVolume(vol, vol);
}

#endif

void vsLoop() {
  if (curFile) {
    curFile.close();
  }
  if (playing) {
    vsDoLoop();
  }
}

/**
 * VsPlayer ma max hlasitost pri nastaveni 0x00, ticho je 0xfe.
 */
void vsSetVolume(int volume) {
  int v = volume * 8;
  if (v < 0) {
    v = 0;
  } else if (v >= 256) {
    v = 256;
  }
  int vol = 256 - v;
  if (vol >= 0xf8) {
    vol = 0xfe;
  }
  _DPRINTLN2("Volume set to: ", vol);
  vsDoSetVolume(vol);
}

boolean vsCheckTrackMissing(int track) {
  _DPRINTLN("Checking track");
  boolean ret = vsFindTrackNumber(track);
  if (ret) {
    ret = vsFindTrackNumber(track);
  }
  return ret;
}

#endif // VS1053

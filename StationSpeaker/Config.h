#ifndef __config_h__
#define __config_h__

/**
 * Pocet radku a sloupcu klavesnice
 */
const int KEYPAD_ROWS = 4;
const int KEYPAD_COLS = 4;

/**
 * LEDka, ktera slouzi pro oznamovani stavu a chyb.
 */
const int LED_FEEDBACK = 13;

const int MAX_QUEUE_LEN = 7;
const int MAX_QUEUE_COUNT = 9;

/**
 * Piny radku klavesnice
 */
const byte keyRowPins[] = { 9, 8, 7, 6 };
/**
 * Piny sloupcu klavesnice
 */
const byte keyColPins[] = { 5, 4, 3, 2 };

/**
 * Minimalni delka "dlouheho" stisknuti klavesy
 */
const int LONG_KEYPRESS_MIN = 500;

// Datova struktura, ktera se bude zapisovat do EEPROM jako nastaveni
struct Config {
  static const byte currentVersion = 1;
  byte version;
  byte volume;

  Config() : version(currentVersion), volume(16) {}
};

#define DEBUG

#undef VS1053
#define DFPlayer

#ifdef DFPlayer
#define _playSound  dfPlaySound
#define _stopSound dfpStopSound
#define _loop dfpLoop
#define _setup dfSetup
#define _setVolume dfpSetVolume
#define _checkTrackMissing dfpCheckTrackMissing
#endif

const boolean debug = false;

#ifdef DEBUG
#define _DPRINT(x)  Serial.print(F(x));
#define _DPRINTLN(x)  Serial.println(F(x));
#define _DPRINT2(x, y)  Serial.print(F(x)); Serial.print(y);
#define _DPRINTLN2(x, y)  Serial.print(F(x)); Serial.println(y);
#define _DIFPRINT(x, y) if (x) _DPRINT(y);
#define _DIFPRINTLN(x, y) if (x) _DPRINTLN(y);
#define _DIFPRINT2(x, y, z) if (x) _DPRINT2(y, z);
#define _DIFPRINTLN2(x, y, z) if (x) _DPRINTLN2(y, z);
#else 
#define _DPRINT(x)
#define _DPRINTLN(x)
#define _DPRINT2(x, y)
#define _DPRINTLN2(x, y)

#define _IFDPRINT(x)
#define _IFDPRINTLN(x)
#define _IFDPRINT2(x, y)
#define _IFDPRINTLN2(x, y)
#endif

#endif



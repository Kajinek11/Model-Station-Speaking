#ifndef __config_h__
#define __config_h__

/**
 * Pokud je definovano, pise se na seriovou konzoli opravdu HODNE hlasek o prubehu
 */
#undef DEBUG

// Zmenit na #define pro prehravani pres VS1053
#undef VS1053        
// zmenit na #define pro prehravani DFPlayerem
#define DFPlayer

// ladeni bez shieldu, jen s SD kartou
#define VS1053_FAKE

/**
 * Maximalni cislo, ktere jde zadat. Pokud se zada vice cifer, 
 * vyhlasi se chyba a zadava se opet od zacatku. 999 = trimistna cisla, pri 
 * zadani 4. mista dojde k chybe.
 * Jako makro kvuli retezeni stringu
 */
#define MAX_TRACK_DIGITS 3

/**
 * Minimalni pocet cislic
 */
const int minTrackDigits = 2;

/**
 * Nejmensi pocet cislic k oznaceni jedine skladby. Pri zacdani mensiho poctu cislic
 * se bude vstup povazovat za sekvenci a doplni se "0".
 */
const int minTrackDigitsSingle = 3;


/**
 * Pocet radku a sloupcu klavesnice
 */
const int KEYPAD_ROWS = 4;
const int KEYPAD_COLS = 4;

/**
 * Piny radku klavesnice
 */
const byte keyRowPins[] = { 10, 9, 8, 7 };
/**
 * Piny sloupcu klavesnice
 */
const byte keyColPins[] = { 6, 5, 4, 3 };

/**
 * Timeout pro zadavani cisel, v milisekundach. Nasledujici cislice se musi stisknout do
 * behem nastaveneho casu, jinak se vstup vymaze.
 */
const int DIGIT_TIMEOUT = 1000;

/**
 * Prodleva mezi jednotlivymi prehravanymi zvuky
 */
const long MASTER_QUEUE_PROCESS_DELAY = 3000;

/**
 * Ochranna lhuta pri pokynu "dalsi z fronty", kdy lze skladbu preskocit
 */
const long NEXT_FROM_QUEUE_DELAY = 2000;

/**
 * LEDka, ktera slouzi pro oznamovani stavu a chyb.
 * Pozor - pri pouziti VS1053 se potluce se SCK !
 */
#if defined(VS1053)

// V pripade VS1053 nelze pouzit vestavenou LED, na D13 je hodinovy signal SCK
const int LED_FEEDBACK = A4;

#elif defined (DFPLAYER) 

// V pripade DFPlayeru klidne monzo pouzit
const int LED_FEEDBACK = 13;

#else

const int LED_FEEDBACK = 13;

#endif

/**
 * Maximalni pocet polozek ve fronte; netyka se sekvenci, ty nezabiraji "zadne" misto
 */
const int MAX_QUEUE_LEN = 6;

/**
 * Maximalni pocet front (1-x)
 */
const int MAX_QUEUE_COUNT = 9;


/**
 * Minimalni delka "dlouheho" stisknuti klavesy
 */
const int LONG_KEYPRESS_MIN = 500;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prehravac VS1053
#ifdef VS1053
#define _playSound  vsPlaySound
#define _stopSound vsStopSound
#define _setVolume vsSetVolume
#define _checkTrackMissing vsCheckTrackMissing
#define _loop vsLoop

# ifdef VS1053_FAKE 
#   define _setup _vsfakeSetup
# else 
#   define _setup vsSetup
# endif

const int defaultVolume = 16;

#endif

#ifdef DFPlayer
#define _playSound  dfPlaySound
#define _stopSound dfpStopSound
#define _loop dfpLoop
#define _setup dfSetup
#define _setVolume dfpSetVolume
#define _checkTrackMissing dfpCheckTrackMissing

const int defaultVolume = 16;
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

void vsDoPlaysound(const char* filename);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Datova struktura, ktera se bude zapisovat do EEPROM jako nastaveni
struct Config {
  static const byte currentVersion = 1;
  byte version;
  byte volume;

  Config() : version(currentVersion), volume(defaultVolume) {}
};

#if defined(DFPLAYER) && defined (VS1053) 
# error "Must define just one of: DFPLAYER, VS1053
#endif


#endif

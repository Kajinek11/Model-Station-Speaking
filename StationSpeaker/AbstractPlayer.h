#ifndef __abstract_player_h__
#define __abstract_player_h__

/**
 * Prehraje soubor zadaneho cisla
 */
void _playSound(int trackId);

/**
 * Zastavi prehravani
 */
void _stopSound();

/**
 * Inicializuje prehravac, nakonfiguruje I/O piny.
 */
void _playerSetup();

/**
 * Nastavi hlasitost prehravani.
 */
static void setVolume(int level);
/**
 * Vola se periodicky z loop(), ma za ukol
 * sledovat stav prehravace a pripadne volat callbacky
 */
void _playerLoop();

/**
 * Vraci true, jestlize skladba jiste neexistuje.
 * Pokud nelze zjistit, vraci false.
 */
boolean _checkTrackMissing(int trackId);

//////////////////////////////////////////////////////////////////////
// Callbacky z prehravace do 'planovace'

/**
 * Prehravac musi zavolat tuto funkci pote,
 * co skonci prehravani skladby.
 */
void playerFinishedCallback();

/**
 * Musi se zavolat, pokud je dan pokyn
 * k prehrani neexistujici skladby.
 */
void playerMissingTrackCallback();


#endif

#include <DFMiniMp3.h>
#include <SoftwareSerial.h>
#include "Config.h"

/**
 * Signal, kterym DFPlayer rika zda prehrava ci ne.
 */
const int DFPLAYER_BUSY = A0;

const int DFPLAYER_RX = A2;
const int DFPLAYER_TX = A1;


int lastFinished = -1;

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode) {
    _DPRINTLN2("DfPlayer: onerror: ", errorCode);
    if (errorCode == DfMp3_Error_FileMismatch) {
      playerMissingTrackCallback();  
    }
  }
  static void OnPlayFinished(uint16_t globalTrack) { 
    if (lastFinished == globalTrack) {
      return;
    }
    lastFinished = globalTrack;
    _DPRINTLN2("DfPlayer: playFinished: ", globalTrack);
    playerFinishedCallback(); 
  }
  static void OnCardOnline(uint16_t code) {}
  static void OnUsbOnline(uint16_t code) {}
  static void OnCardInserted(uint16_t code) {
    _DPRINTLN2("DfPlayer: cardInserted: ", code);
  }
  static void OnUsbInserted(uint16_t code) {}
  static void OnCardRemoved(uint16_t code) {
    _DPRINTLN2("DfPlayer: cardRemoved: ", code);
  }
  static void OnUsbRemoved(uint16_t code) {}
};

/**
 * Seriove rozhrani pro ovladani DF Playeru
 */
SoftwareSerial dfpSerial(DFPLAYER_RX, DFPLAYER_TX);

DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(dfpSerial);

void dfpLoop() {
  mp3.loop();
}

void dfpSetVolume(int vol) {
  mp3.setVolume(vol);
}

/**
 * DFPlayer umi jenom 99 adresaru a v kazdem z nich
 * max 999 skladeb. Provedeme to tak, ze prvni dve cifry skladby MUSI
 * odpovidat adresari. Dalsi 1-2 cifry musi odpovidat cislu skladby v adresari
 */
void dfPlaySound(int trackNo) {
  int folderN = trackNo / 100;
  int trackN = trackNo % 100;

  if (folderN > 99 || trackN > 999) {
    errorAndClear();
    return;
  }
  _DPRINT2("DFPlayer: play folder: ", folderN);
  _DPRINTLN2(", track: ", trackN);
  mp3.playFolderTrack16(folderN, trackN);
  lastFinished = -1;
}

/**
 * Neumi zjistit zda skladba chybi.
 */
boolean dfpCheckTrackMissing(int trackId) {
  return false;
}

void dfpStopSound() {
  mp3.stop();
}

void dfSetup() {
  dfpSerial.begin (9600);
  mp3.setVolume(config.volume);
}

#include <DFMiniMp3.h>
#include <SoftwareSerial.h>
#include "Config.h"

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

SoftwareSerial dfpSerial(10, 11);

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
  mp3.playFolderTrack(folderN, trackN);
  lastFinished = -1;
}

void dfpStopSound() {
  mp3.stop();
}

void dfSetup() {
  dfpSerial.begin (9600);
  mp3.setVolume(config.volume);
}


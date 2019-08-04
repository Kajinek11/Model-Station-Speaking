/**
 * Jednoducha implementace FIFO fronty.
 */

#ifndef __queue_h__
#define __queue_h__

#include "Config.h"

class Queue {
  /**
   * Vlastni pole prvku
   */
  int tracks[MAX_QUEUE_LEN];
  /**
   * Pocet prvku v poli, vc. jiz vyjmutych
   */
  byte count;
  /**
   * Index dosud nepouziteho vlozeneho prvku. Pokud
   * je head >= count, je fronta prazdna.
   */
  byte head;
  /**
   * Automaticky inkrement na dalsi skladbu
   */
  long  playAt = 0;

  int   autoIncrement;
public:
  Queue();
  void clear();
  boolean add(int no); // prida skladbu na konec fronty. Pri auto rezimu na zacatek fronty
  int poll(); // vrati cislo dalsi skladby
  boolean isEmpty();  // true, pokud ve fronte nic neni
  boolean isClear();  // true, pokud je fronta zcela nepouzita.
  boolean isAutoMode(); // true, pokud obsahuje sekvenci
  void autoPlay(int start); // zacne hrat automaticky sekvenci pocinaje cislem
  void resetSchedule() { playAt = -1; } // zrusi pozdrzeni
  void schedule(long timeAt) { playAt = max(playAt, timeAt); }
  long getSchedule() { return playAt; }
  void retract();     // prida zpet prvek naposledy vytazeny z fronty (poll);
  boolean discardLast(); // odstrani posledni ve fronte
};

#endif

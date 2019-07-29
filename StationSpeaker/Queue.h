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
  boolean add(int no);
  int poll();
  void finished();
  boolean isEmpty();
  boolean isAutoMode();
  void autoPlay(int start);
  void resetSchedule() { playAt = -1; }
  void schedule(long timeAt) { playAt = max(playAt, timeAt); }
  long getSchedule() { return playAt; }
  void retract();
};

#endif


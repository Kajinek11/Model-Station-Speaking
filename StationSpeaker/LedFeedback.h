/**
 * Jednoducha odezva pomoci blikani jedne (vice) LED. LED umi blikat podle zadaneho vzoru v poli int[],
 * kde se stridaji intervaly v ms: sviti-nesviti. Sekvence je ukoncena 0. Druhy rezim blikani je urcity
 * zadany pocet bliknuti (napr. potvrzeni poctu).
 */

#ifndef __led_feedback_h__
#define __led_feedback_h__

class LedFeedback {
private:
  /**
   * Vystupni noha
   */
  const int ledNumber;

  /**
   * Ukazatel na "blikaci vzor", NULL pokud se neblika.
   */
  int*  blinkPtr;

  /**
   * Absolutni cas v ms, kdy se LED preklopi do noveho stavu
   */
  unsigned long  nextSignal;

  /**
   * true/false sviti/nesviti
   */
  boolean state;

  /**
   * Pocet bliknuti, 0 pokud nesignalizuje poctem.
   */
  byte blinkCounter;
  
  void clear();
public:
  LedFeedback(int aN) : ledNumber(aN), blinkPtr(NULL), state(false), nextSignal(-1), blinkCounter(0) {}
  void signal(int* seq);
  void run();
  void blink(int count);
};

#endif


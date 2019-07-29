#include "Queue.h"

Queue::Queue() : count(0), head(0), autoIncrement(0), playAt(0)  {}

boolean Queue::isEmpty() {
  return !isAutoMode() && (head == count);
}

boolean Queue::isClear() {
  return !isAutoMode() && count == 0;
}

boolean Queue::isAutoMode() {
  return autoIncrement != 0;
}

void Queue::clear() {
  count = 0;
  head = 0;
  autoIncrement = 0;
  playAt = 0;
}

void Queue::autoPlay(int start) {
  autoIncrement = start;
}

boolean Queue::add(int no) {
  if (count >= MAX_QUEUE_LEN) {
    if (head > 0) {
      _DPRINTLN2("Compacting from ", head);
      int pos = 0;
      for (int i = head; i < count; i++) {
        tracks[pos++] = tracks[head];
      }
      head = 0;
      count = pos;
    } else {
      return false;
    }
  }
  _DPRINTLN2("Add qitem ", count);
  tracks[count++] = no;
  return true;
}

/**
 * Pokud je neco ve fronte, vezme se to prednostne.
 * 
 */
int Queue::poll() {
  if (head < count) {
    _DPRINTLN2("Fetch qitem ", head);
    return tracks[head++];
  }
  if (isAutoMode()) {
    head = count = 0; // aby pristi retract vratil zpet autoincrement a discardLast dekrementoval citac.
    return autoIncrement++;
  }
  return 0;
}

void Queue::retract() {
  if (isAutoMode() && autoIncrement > 1) {
    autoIncrement--;
    return;
  }
  if (head == 0) {
    return;
  }
  head--;
}

boolean Queue::discardLast() {
  if (count > 0 && count > head) {
    count--;
  } else if (autoIncrement > 0) {
    autoIncrement--;
  } else {
    return false;
  }
  return true;
}


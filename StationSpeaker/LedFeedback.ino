#include "Config.h"
#include "LedFeedback.h"

int blinkData[] = { 200, 300, 0 };

void LedFeedback::signal(int* seq) {
  blinkPtr = seq;
  state = true;
  digitalWrite(ledNumber, HIGH);
  nextSignal = millis() + *blinkPtr;
}

void LedFeedback::blink(int count) {
  blinkCounter = count;
  signal(&blinkData[0]);
}

void LedFeedback::clear() {
  blinkPtr = NULL;
  state = false;
  nextSignal = -1;
  blinkCounter = 0;
  digitalWrite(ledNumber, LOW);
}

void LedFeedback::run() {
  unsigned long m = millis();
  if (nextSignal > m) {
    return;
  }
  if (blinkPtr != NULL && ((*blinkPtr) > 0)) {
    blinkPtr++;
  }
  int n = blinkPtr == NULL ? 0 : (*blinkPtr);
  if (n <= 0) {
    if (blinkCounter > 0) {
      if (--blinkCounter > 0) {
        signal(&blinkData[0]);
        return;
      }
    }
    clear();
    return;
  }
  state != state;
  nextSignal  = m + n;
  digitalWrite(ledNumber, state ? HIGH : LOW);
}


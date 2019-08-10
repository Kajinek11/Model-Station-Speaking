#include <Key.h>
#include <Keypad.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

#include "Config.h"

#include "AbstractPlayer.h"
#include "LedFeedback.h"
#include "Queue.h"

const int LONG_NUMBER = 'K' - '0';

const int maxTrackDigits = MAX_TRACK_DIGITS;

const char STAR = '*';    // Potvrdi zadani cisla, prehraje zvuj AZ soucasny skonci
const char HASH = '#';    // Potvrdi zadani cisla, prehraje zvuk

const char LONG_STAR = '^';    // Potvrdi zadani cisla, prehraje zvuj AZ soucasny skonci
const char LONG_HASH = '$';    

enum CommandState {
    initial = 0,
    trackSelect,    // vstup cisla zvuku
    queueSelect,    // vstup cisla fronty
    sequenceQueueSelect, // vstup cisla fronty pro sekvenci
};

CommandState speakerState;

Config config;

/**
 * Rozlozeni klavesnice, po radcich a sloupcich jak
 * je na skutecne klavesnici co vidi uzivatel
 */
char  keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
      {   '1',      '2',      '3',    'a' },
      {   '4',      '5',      '6',    'b' },
      {   '7',      '8',      '9',    'c' },
      {  STAR,      '0',     HASH,    'd' },
};

Keypad keypad(makeKeymap(keymap), keyRowPins, keyColPins, KEYPAD_ROWS, KEYPAD_COLS);

LedFeedback ledFeedback(LED_FEEDBACK);

/**
 * Jednotlive fronty zvuku.
 */
Queue queues[MAX_QUEUE_COUNT];

/**
 * Master queue obsahuje budto cisla jednotlivych skladeb (> 0),
 * nebo FRONT ze kterych se skladby berou, s opacnym znamenkem +1,
 * tzn. fronte c.0 odpovida hodnota -1.
 */
Queue& masterQueue = queues[0];


/**
 * Cilov fronta pro pridavani zvuku. -1, pokud
 * cilova fronta neni urcena
 */
char currentQueue = -1;

/**
 * Fronta, ze ktere se prehrava soucasny zvuk. -1, pokud
 * se zvuk neprehrava, nebo se prehrava zvuk mimo frontu.
 */
char playingFromQueue = -1;

/**
 * Blikani pri neplatnem zadani: 3x 200ms sviti-nesviti
 */
const short invalidSelection[] = {
  //  on   off  on   off  on   off  konec
      200, 200, 200, 200, 200, 200, 0
};

/**
 * Potvrzovaci kratke bliknuti po zadani cislice.
 */
const int numberAck[] = {
  //  on   konec
      100, 0
};

/**
 * Dlouhe bliknuti - prehraje dalsi skladbu
 */
const int selectionAck[] = {
  //  on   konec
      500, 0
};

/**
 * 2x Dlouhe bliknuti - prehraje dalsi skladbu
 */
const int cancelAck[] = {
  //  on   off  on   konec
      500, 200, 500, 0
};

/**
 * Chybove prebliknuti
 */
const int errorCancel[] = {
  //  on   off  on   off  on   konec
      100, 200, 100, 200, 100, 0
};

int inputNumber = -1;

/**
 * Cislo vybrane skladby. Pokud neni zadna vybrana, je zde -1.
 */
int trackSelection = -1;

/**
 * True, pokud prehravac prave prehrava
 */
boolean playing = false;

/**
 * Pocet vlozenych cislic.
 */
int digits = 0;

/**
 * Pokud neni -1, bude se dalsi zvuk z master queue prehravat
 * az uplyne stanoveny cas. Resetuje se pri zacatku prehravani zvuku.
 */
long masterQueuePlayTime = -1;

/**
 * Nastavi pocatecni (zakladni) stav pro ovladaci logiku.
 */
void restart() {
  speakerState = initial;
  clearSelection();
}

/**
 * Vymaze nastaveni skladby
 */
void clearSelection() {
  trackSelection = -1;
  inputNumber = -1;
  digits = 0;
}

/**
 * Zablika chybu a prejde do zakladniho stavu.
 */
void errorAndClear() {
  ledFeedback.signal(&errorCancel[0]);
  restart();
}

/**
 * Smaze vsechny fronty. Prejde do zakladniho stavu.
 */
void resetAndClearAll() {
  _DPRINTLN("Resetting all...");
  for (int i = 0; i < MAX_QUEUE_COUNT; i++) {
    queues[i].clear();
  }
  currentQueue = -1;
  restart();  
}

/**
 * Zapise jednu cislici; pokud je cislic vice nez 
 * max povolena hodnota, vyhlasi chybu
 */
void inputDigit(int d) {
  if (d < '0' || d > '9') {
    return;
  }
  d = d - '0';
  if (d >= 10) {
    return;
  }
  if (inputNumber < 0) {
    inputNumber = 0;
  }
  inputNumber = (inputNumber * 10) + d;
  if (digits > maxTrackDigits) {
    _DPRINTLN("Too many digits.");
    errorAndClear();
    return;
  }
  digits++;
  _DPRINTLN2("input: ", inputNumber);
  _DPRINTLN2("digits: ", digits);
  ledFeedback.signal(&numberAck[0]);
}

/**
 * Ukonci okamzite prehravani skladby.
 */
void stopCurrentSound() {
  _DPRINTLN("Stopping sound");
  ledFeedback.signal(&cancelAck[0]);
  playing = false;
  if (!masterQueue.isEmpty()) {
    scheduleMaster(millis() + MASTER_QUEUE_PROCESS_DELAY);
    _DPRINTLN2("Master not empty, suspended until ", masterQueuePlayTime);
    
  }
  _stopSound();
}

/**
 * Zahraje skladbu vybranou cislem - vlozi ji do 'master'
 * fronty
 */
void playSelectedSound(boolean stopCurrent) {
  int trackNo = inputNumber;
  if (trackNo <= 0) {
    ledFeedback.signal(&invalidSelection[0]);
    _DPRINTLN2("Invalid track: ", trackNo);
    return;
  }
  if (_checkTrackMissing(trackNo)) {
    ledFeedback.signal(&invalidSelection[0]);
    _DPRINTLN2("Missing track: ", trackNo);
    return;
  }
  _DPRINTLN2("Queuing track: ", trackNo);
  if (stopCurrent) {
    // zastavime produkci, vycistime master frontu, aby se zvuk zacal hned prehravat.
    stopAndclearMasterQueue();
  }

  clearSelection();
  // bez prodlevy
  masterQueuePlayTime = -1;  
  if (!masterQueue.add(trackNo)) {
    errorAndClear();
  }
}

/**
 * Prijme jeden znak ze vstupu; klavesnice nebo terminal
 */
void interpretChar(char c) {
  _DPRINTLN2("Interpret: ", c);
  // kdykoliv se zmackne "#", dojde k reset-u
  if (c == HASH) {
    errorAndClear();
    return;
  }
  switch (speakerState) {
    case initial: processInitial(c); break;
    case trackSelect: processTrackSelect(c); break;
    case sequenceQueueSelect: case queueSelect: processQueueSelect(c); break;
    default:
      Serial.print("Invalid state: "); Serial.println(speakerState);
      restart();
      break;
  }
}

void playNextInQueue() {
  if (currentQueue == -1 || currentQueue >= MAX_QUEUE_COUNT) {
    errorAndClear();
    return;
  }
  Queue& q = queues[currentQueue];
  // jestlize se predtim zmacklo 'b', muze jeste probihat ochranna lhuta pro prehravani:
  long sch = q.getSchedule();
  if (sch > millis()) {
    int c = q.poll();
    // zrusime prehravani skladby, prejdeme na dalsi
    _DPRINTLN2("Skipping track: ", c);
  }
  if (q.isEmpty()) {
    q.clear();
    return;
  }
  addQueueToMaster(currentQueue);
}

void playNextQueue() {
  int c = currentQueue;
  if (c == -1) {
    c = 0;
  }
  for (c++; c < MAX_QUEUE_COUNT; c++) {
    Queue& q = queues[c];
    if (!q.isEmpty()) {
      // Pokusi se zkontrolovat skladbu:
      int tn = q.poll();
      q.retract();
      if (tn > 0 && !_checkTrackMissing(tn)) {
        currentQueue = c;
        playNextInQueue();
        return;
      }
    }
  }
  if (currentQueue <= 0) {
    return;
  }
  for (c = 1; c <= currentQueue; c++) {
    Queue& q = queues[c];
    if (!q.isEmpty()) {
      // Pokusi se zkontrolovat skladbu:
      int tn = q.poll();
      q.retract();
      if (tn > 0 && !_checkTrackMissing(tn)) {
        currentQueue = c;
        playNextInQueue();
        return;
      }
    }
  }
}

void scheduleMaster(long newTime) {
  if (masterQueuePlayTime >= 0) {
    if (masterQueuePlayTime < newTime) {
      masterQueuePlayTime = newTime;
    } 
  } else {
    masterQueuePlayTime = newTime;
  }
  _DPRINTLN2("MQPT = ", masterQueuePlayTime);
}

void addQueueToMaster(int currentQueue) {
  if (currentQueue <= 0 || currentQueue >= MAX_QUEUE_COUNT) {
    return;
  }
  _DPRINTLN2("Adding queue to master: ", currentQueue);
  Queue& q = queues[currentQueue];
  q.schedule(millis() + NEXT_FROM_QUEUE_DELAY);
  _DPRINTLN2("Queue scheduled to: ", q.getSchedule());
  masterQueue.add(- currentQueue);
}

void insertInActiveQueue() {
  insertInActiveQueue(true);
}

void insertInActiveQueue(boolean startPlaying) {
  _DPRINTLN2("Queuing track ", trackSelection);
  if (currentQueue == -1) {
    _DPRINTLN("No queue");
    errorAndClear();
    return;
  }
  if (trackSelection <= 0 || _checkTrackMissing(trackSelection)) {
    errorAndClear();
    return;
  }
  _DPRINTLN2("Queuing into ", (int)currentQueue);
  Queue& q = queues[currentQueue];
  boolean wasEmpty = q.isEmpty();
  boolean wasClear = q.isClear();
  if (speakerState == sequenceQueueSelect) {
    if (!wasEmpty) {
      _DPRINTLN("Queue not empty");
      errorAndClear();
      return;
    }
    q.clear();
    _DPRINTLN2("Autoplay from: ", trackSelection);
    q.autoPlay(trackSelection);
  } else {
    if (wasEmpty) {
      q.clear();
    }
    if (!q.add(trackSelection)) {
      errorAndClear();
      return;
    }
  }
  // Neprehravalo se z ni; pokud s z ni prave prehrava, jeste
  // nema status 'clear'
  if (startPlaying && wasClear) {
    _DPRINTLN("Play empty queue");
    addQueueToMaster(currentQueue);
  }
  restart();
}

int findFreeQueue() {
  if (currentQueue == -1) {
    return 1;
  }
  for (int i = currentQueue; i < MAX_QUEUE_COUNT; i++) {
    Queue& q = queues[i];
    if (q.isEmpty()) {
      return i;
    }
  }
  for (int i = 1; i < currentQueue; i++) {
    Queue& q = queues[i];
    if (q.isEmpty()) {
      return i;
    }
  }
  return -1;
}

void insertInNewQueue() {
  int fq = findFreeQueue();
  if (fq == -1) {
    errorAndClear();
    return;
  }
  _DPRINTLN2("Found free slot: ", fq);
  ledFeedback.blink(fq);
  currentQueue = fq;
  insertInActiveQueue();
}

/**
 * Ukonci prehravani a vycisti master frontu, ceka
 * se na pokyny z klavesnice
 */
void stopAndclearMasterQueue() {
  _DPRINTLN("Stop + clear queue");
  _stopSound();
  masterQueue.clear();
  masterQueuePlayTime = -1;
  playingFromQueue = -1;
  playing = false;
}

/**
 * Zpracovani vychoziho / zakladniho stavu
 * dlouhe # -> vsechno smazat
 * * -> zrusit aktualni skladbu, bude se hrat nasledujici
 * dlouhe * -> vsechno prerusit, vyprazdnit frontu: prestane se ihned vsechno hrat
 * B -> prehrani nasledujici skladby soucasne fronty
 * dlouhe B -> prehrani skladby DALSI neprazdne fronty (cykli se dokola)
 */
void processInitial(char c) {
  _DPRINTLN2("init-", c);
  switch (c) {
    case LONG_HASH:
      resetAndClearAll();
      return;
    case STAR:
      stopCurrentSound();
      restart();
      break;
    case LONG_STAR:
      stopAndclearMasterQueue();
      restart();
      break;
    case 'b':
      playNextInQueue();
      restart();
      break;
    case 'B':
      playNextQueue();
      restart();
      break;
    case 'c':
      _DPRINTLN2("Clear last from: ", (int)currentQueue);
      clearQueueItem(currentQueue, false);
      restart();
      break;
    case 'C':
      _DPRINTLN2("Clear all queue: ", (int)currentQueue);
      clearQueueItem(currentQueue, true);
      restart();
      break;
    case 'K':
      saveEEPROM();
      ledFeedback.signal(&numberAck[0]);
      restart();
      break;
    case 'L':
      adjustVolume(false);
      restart();
      break;
    case 'M':
      adjustVolume(true);
      restart();
      break;
  }
  if (c >= '0' && c <= '9') {
    speakerState = trackSelect;
    processTrackSelect(c);
    return;
  }
}

/**
 * Prehraje skladbu z urcene fronty
 */
void takeFromQueue(int qn) {
  if (qn < 1 || qn > 9) {
    errorAndClear();
    return;
  }
  if (queues[qn].isEmpty()) {
    errorAndClear();
    return;
  }
  ledFeedback.blink(qn);
  addQueueToMaster(qn);
}

/**
 * Smaze posledni prvek zadane fronty; je-li
 * full true, smaze celou frontu
 */
void clearQueueItem(int queueNo, boolean full) {
  if (queueNo <= 0 || queueNo >= MAX_QUEUE_COUNT) {
    errorAndClear();
    return;
  }
  Queue& q = queues[queueNo];
  if (full) {
    q.clear();
  } else {
    if (!q.discardLast()) {
      _DPRINTLN("Could not remove last item");
      errorAndClear();
    } else {
      ledFeedback.signal(&numberAck[0]);
    }
  }
}


/**
 * Rezim vyberu cisla skladby.
 * Cislice -> pridani do celkoveho cisla, omezeni na pocet cislic
 * Hvezdicka -> vlozeni do hlavni fronty (prehrani v poradi)
 * Dlouha hvezdicka -> okamzite prehrani
 * A -> vlozeni do fronty + prehrani
 * Dlouhe A -> vlozeni sekvence do fronty
 */
void processTrackSelect(char c) {
  _DPRINTLN2("track-select-", c);
  switch (c) {
    case STAR:  // okamzite prehrani
      trackSelection = inputNumber;
      playSelectedSound(false);
      restart();
      return;
    case LONG_STAR:
      trackSelection = inputNumber;
      playSelectedSound(true);
      restart();
      return;
    case 'a':   // zaradit do aktualni fronty
      speakerState = queueSelect;
      trackSelection = inputNumber;
      _DPRINTLN2("Entering queue select: ", inputNumber);
      ledFeedback.signal(&numberAck[0]);
      return;
    case 'A':   // vytvorit frontu
     _DPRINTLN2("Entering sequenced queue select", inputNumber);
      speakerState = sequenceQueueSelect;
      trackSelection = inputNumber;
      ledFeedback.signal(&numberAck[0]);
      return;
    case 'b':   // prehrat z fronty X
      _DPRINTLN2("Play selected queue ", inputNumber);
      takeFromQueue(inputNumber);
      restart();
      break;

    case 'c':   // vymazat posledni ze zadane fronty
      _DPRINTLN2("Clear last from ", (int)inputNumber);
      clearQueueItem(inputNumber, false);
      break;
    case 'C':   // vymazat celou zadanou frontu
      _DPRINTLN2("Clear all queue ", (int)inputNumber);
      clearQueueItem(inputNumber, true);
      break;
  }
  if (c >= '0' && c <= '9') {
      inputDigit(c);
      return;
  }
}

/**
 * Zpracovani v rezimu "vyber fronty".
 * Cislice -> cilova fronta
 * Dlouha hvezdicka -> vyhledani nepouzite fronty
 * Hvezdicka -> pridani do soucasne fronty
 */
void processQueueSelect(char c) {
  _DPRINTLN2("queue-select-", c);
  if (trackSelection <= 0) {
    errorAndClear();
    return;
  }
  boolean playImmediately = false;
  switch (c) {
    case LONG_STAR:
      insertInActiveQueue(true);
      return;
    case STAR:
      insertInActiveQueue(false);
      return;
  }
  if (c < '1' || c > '9') {
    if (c >= '1' + LONG_NUMBER && c < '1' + LONG_NUMBER + 10) {
      c = c - LONG_NUMBER;
      playImmediately = true;
    } else {
      return;
    }
  }
  int qn = c - '0';
  if (qn >= MAX_QUEUE_COUNT) {
    errorAndClear();
    return;
  }
  currentQueue = qn;
  insertInActiveQueue(playImmediately);
}

/**
 * Nastavi se na true, pokud je klavesa pridrzena; pak se nebude interpretovat
 * jen jako pouhy stisk.
 */
boolean keyWasHeld = false;

/**
 * Reakce na klavesey. Klavesa se neinterpretuje hned po stisknuti, ale
 * po uvolneni; tim je mozne odlisit kratky a dlouhy stisk - podle eventu.
 * Dlouhe stisky maji prisouzene posunute znaky:
 * a..d -> A..D
 * 0..9 -> K..T
 * HASH(#) -> LONG_HASH($)
 * STAR(*) -> LONG_STAR(^)
 * V dalsim zpracovani se uz nemusime ohlizet na delku stisku
 */
void keypadHandler(KeypadEvent key) {
  char kchar = 0;
  switch (keypad.getState()) {
    case PRESSED:
      _DPRINTLN2("Pressed: ", key);
      keyWasHeld = false;
      return;
    case HOLD:
      _DPRINTLN2("Hold: ", key);
      kchar = key;
      if (isDigit(kchar)) {
        // "velka cisla" ;)
        kchar = LONG_NUMBER + kchar;
      } else if (kchar >= 'a' && kchar <= 'd') {
        kchar = 'A' + (kchar - 'a');
      } else {
        switch (kchar) {
          case HASH: kchar = LONG_HASH; break;
          case STAR: kchar = LONG_STAR; break;
          default: return;
        }
      }
      keyWasHeld = true;
      break;
    case RELEASED:
      _DPRINTLN2("Released: ", key);
      if (keyWasHeld) {
        keyWasHeld = false;
        return;
      }
      kchar = key;
      break;
    default:
      return;
  }
  interpretChar(kchar);
}

void adjustVolume(boolean up) {
  if (up) {
    config.volume++;
  } else {
    config.volume--;
  }
  _DPRINTLN2("Set volume to: ", config.volume);
  _setVolume(config.volume);
  ledFeedback.signal(&numberAck[0]);
}


/**
 * Periodicke zpracovani 'master' fronty. Pokud se neprehrava zvuk,
 * vezme dalsi skladbu nebo odkaz na frontu skladeb.
 */
void processMasterQueue() {
  if (playing) {
    // kdyz se prehrava, nedelame nic
    return;
  }
  if (masterQueuePlayTime >= 0) {
    if (millis() < masterQueuePlayTime) {
      // pokud je vynucena pauza, nedela se nic, dokud pauza mezi skladbami neskonci
      return;
    }
  }
  masterQueuePlayTime = -1;
  int no = masterQueue.poll();
  if (no == 0) {
    return;
  }
  _DPRINTLN2("Starting time", millis());
  _DPRINTLN2("Master queue item: ", no);
  if (no < 0) {
    // odkaz na frontu skladeb; frontu c.0 nemame.
    no = -no;
    if (no < MAX_QUEUE_COUNT) {
      Queue& q = queues[no];
      _DPRINTLN2("Trying from queue: ", no);
      long sch = q.getSchedule();
      if (sch > millis()) {
        // ma se prehravat fronta, ale ta ma urcene prehravani az za chvili. Pockame tedy.
        _DPRINTLN("Queue suspended, waiting");
        masterQueue.retract();
        scheduleMaster(sch);
        _DPRINTLN2("Master follows quque, suspended until ", masterQueuePlayTime);
        return;
      }
      _DPRINTLN2("Fetching from queue: ", no);
      int track = q.poll();
      if (track > 0) {
        _DPRINTLN2("Queued track: ", track);
        playingFromQueue = no;
        playing = true;
        _playSound(track);
        return;
      }
    }
  } else {
    // Prime cislo skladby
    _DPRINTLN("Playing direct sound");
    playingFromQueue = 0;
    playing = true;
    _playSound(no);
    return;
  }
  // nic neprehravame.
  _DPRINTLN("Queue empty");
  playing = false;
}

/**
 * Vola se kdyz prehravac skoncil prehravani.
 */
void playerFinishedCallback() {
  if (!playing) {
    return;
  }
  if (playingFromQueue > 0) {
    _DPRINTLN2("Finished playing queue ", playingFromQueue);
    Queue& q = queues[playingFromQueue];
    if (q.isEmpty()) {
      // to ze je prazdna vubec neznamena, ze je cista :)
      q.clear();
    }
  }
  if (!masterQueue.isEmpty()) {
    // dalsi zvuk se nebude prehravat hned, ale az za chvili
    scheduleMaster(millis() + MASTER_QUEUE_PROCESS_DELAY);
    _DPRINTLN2("Master not empty, suspended until ", masterQueuePlayTime);
  } else {
    masterQueue.clear();
  }
  _DPRINTLN("Finished.");
  playing = false;
}

/**
 * Vola se, pokud urcena skladba neexistuje; ukonci
 * prehravani skladeb z 'auto' fronty
 */
void playerMissingTrackCallback() {
  _DPRINTLN("Missing track called");
  if (playingFromQueue > 0) {
    _DPRINTLN2("Clearing queue: ", playingFromQueue);
    if (playingFromQueue < MAX_QUEUE_COUNT) {
      queues[playingFromQueue].clear();
    }
    playingFromQueue = -1;  
  }
  playing = false;
}

void readEEPROM() {
  _DPRINTLN("Reading config from EEPROM");
  EEPROM.get(0, config);
  if (config.version != Config::currentVersion) {
    _DPRINTLN2("Incorrect version, clearing eeprom: ", config.version);
    config = Config();
    saveEEPROM();
  }
}

void saveEEPROM() {
  _DPRINTLN("Save config to EEPROM");
  EEPROM.put(0, config);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Station speaker v1.0");

  // indikacni LED - vystup
  pinMode(LED_FEEDBACK, OUTPUT);
  keypad.setHoldTime(LONG_KEYPRESS_MIN);
  keypad.addEventListener(keypadHandler);
  readEEPROM();
  _setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    int x = Serial.read();
    interpretChar(x);
  }
  keypad.getKey();
  ledFeedback.run();
  _loop();
  processMasterQueue();
}

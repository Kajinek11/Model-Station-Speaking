# Jednoduchy prehravac hlaseni ve stanici

Cilem je naprogramovat Arduino pro prehravani stanicnich hlaseni a/nebo dalsich zvuku. Arduino se ovlada z **maticove klavesnice** 4x4 (viz napr. z [Aliexpressu](https://www.aliexpress.com/item/32656556339.html)), ze ktere se zadavaji cisla skladeb. Skladby se ctou z **SD karty** a prehravaji pomoci prehravace.

V soucasne dobe se podporuji 2 typy prehravacu:
* [DFPlayer](https://www.aliexpress.com/item/32677381035.htm)
* [VS1053 Arduino Shield](https://www.aliexpress.com/item/2037926167.html)

Skladby se po zadani radi do fronty, ze ktere se postupne prehravaji. Je mozne prehravani prerusit (a pak se prehraje dalsi skladba z fronty), nebo prehravani zcela zrusit. 

Prehravac umoznuje vytvorit celou **sekvenci** po sobe (ciselne) nasledujich skladeb s tim, ze se potom tlacitkem prehrava vzdy dalsi skladba - takovych sekvenci muze byt az 9 (tlacitek na klavesnici). Zamysleny ucel je sekvence hlaseni pro vlak ve stanici:
* vlak prijizdi
* vlak prijel
* prestupujte xxx
* vlak je pripraven k odjezdu

Jakmile se sekvence vytvori, je mozne stiskem tlacitka prehrat vzdy dalsi hlaseni v sekvenci. Delsim stiskem se aktivuje a prehraje dalsi sekvence, takze v pripade pritomnosti vice vlaku se mohou hlaseni bez namahy stridat:
1. Vlak A prijizdi
2. Vlak B prijizdi
3. Vlak A prijel
4. Vlak B prijel
5. Ukoncete nastup do A
6. Ukoncete nastup do B

Je mozne zvolit prehrani dalsi skladby s pozadovane sekvence. 

Planovane / implementovane funkce:
* Prehrani zvolene skladby.
* Preruseni prehravani, vymaz vsech skladeb z fronty
* Vytvoreni sekvence ze skladeb, ktere po sobe ciselne nasleduji
* Vytvoreni sekvence z rucne vybranych hlaseni, vymaz posledni zadane, vymaz celku
* Prehrani dalsi skladby v sekvenci
* Prehrani dalsi skladby v dalsi sekvenci - opakovanym stiskem se prepinaji a postupne prehravaji definovane sekvence
* Zvyseni, snizeni hlasitosti; ulozeni hlasitosti do FLASH pameti

Mozna dalsi rozsireni:
* ulozeni sekvence (cisel skladeb) do FLASH, prehrani jednim tlacitkem
* Spousteni zvuku digitalnim vstupem

# Konfigurace
Konfigurace je mozna ruznymi nastavenimi v souboru `Config.h`. Pomoci `#define` a `#undef` se zvoli vystupni zarizeni - smi byt definovane jen jedine. `#define VS1053_FAKE` slouzi pro ladeni, nebot VS1053 nemam :) jen jsem se s kolegou Kajinkem dohodl ze napisu zaklad.

Dale je zde konfigurace maticove klavesnice - predpoklada se 4x4 (viz `KEYPAD_ROWS`, `KEYPAD_COLS`). Jednotlive piny se pak definuji v `keyRowPins`, `keyColPins`. Pozor - zatimco "radkove" (row) piny jsou vstupni, "column" piny jsou **vystupni**. V pripade nutnosti jde upravit knihovnu Keypad tak, aby jako radkove piny mohly slouzit **A6** a **A7** (ktere jsou urcene jen pro analogovy vstup)

`LED_FEEDBACK` urcuje pin ovladajici LED, ktera blikanim signalizuje co se deje (viz ovladani). LED se vypina nastavenim pinu na **-1**.
* Kratky blik - prijaty stisk tlacitka nebo prikaz
* Dlouhy blik - prijeti skladby k prehrani
* 2x dlouy blik - storno
* 3x rychly blik - chyba, prechod do zakladniho stavu

Pokud je zapnuto `#define DEBUG`, pise se do seriove konzole hrozna spousta ladicich hlasek.

Zbyla nastaveni jsou popsana v textu a nejsou az tak podstatna

# Konfigurace pro DFPlayer
Konfigurace pinu je v `DFPlayer.ino`. Komunikuje se pomoci serioveho rozhrani, zapojeni viz popis DFPlayeru. **POZOR** - `TX` DFPlayeru musi prijit na pin uvedeny jako `DFPLAYER_RX`, `RX` DFPlayeru pak na pin `DFPLAYER_TX`. Pouziva se softwarova emulace serioveho rozhrani, **neni nutne** volit piny s HW podporou UARTu.

# Konfigurace pro VS1053
TBD - Kajinek11 dopise.


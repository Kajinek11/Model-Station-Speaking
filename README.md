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


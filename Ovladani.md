# Užiatelský manuál

**StationSpeaker** slouží pro **ruční** přehrávání nádražních hlášení nahraných na připojené SD kartě. Jednotlivé zvuky (nahrávky) se ukládají do souborů s číselnými názvy. Přehrávač

## Organizace souborů na SD kartě

### DFPlayer
DFPlayer dokáže přerávat jen soubory s číselnými názvy 0-255 řazenými do podadresářů 0-99. Tedy skladba může mít číslo v rozsahu **0000-9999**. Pozor - název souboru **musí mít 3 čislice***, tedy jednotlivé skladby jsou pojmenované 
* 00/000.mp3 - zvuk 0000
* 00/001.mp3 - zvuk 0001
* 02/003.mp3 - zvuk 0203

a tak dále.

### VS1053
VS1053 je chytřejší, umí libovolně pojmenované soubory. StationSpeaker vyhledává jméno souboru, které **začíná 4-ciferným číslem** zvuku; zbytek názvu souboru se ignoruje, a slouží pro orientaci uživatele v obsahu SD karty. Pokud více různách souborů začíná stejným 4-ciferným číslem (a zbytek názvu se různí), vybere se první takový nalezený soubor. Všechny soubory se ukládájí do **kořenu SD karty**. Příklad pojmenování souborů:
* 0100 - Bechyně, přijíždí.mp3
* 0101 - Bechyně, přijel.mp3
* 0102 - Bechyně, nastupujte.mp3

## Přehled ovládání

* písmeno, znak - nečíselné znaky na klávesnici
* ^ písmeno, znak, číslice - **dlouhý** stisk znaku, delší než 0.5 sekundy
* {nnnn} - 1-4 číslice - číslo skladby
* {f} - jediná číslice (0-9), číslo fronty

Po každém přijatém stisku klávesy krátce problikne indikační LED.

| Klávesy | Funkce| 
| :-- | :--- |
| ^ # | **Reset, výmaz všech front, ukončení přehrávání**                       |
| #   | Vymaže zadávaný přikaz či skladbu                                       |
| *   | Ukončí přehrávání zvuku, bude se přehrávat další připravený|
| ^ * | Ukončí přehrávání zvuku, a zruší se všechny připravené     |
| \{nnnn\} *****  | Přehraje skladbu **nnnn**.                                  |
| \{nnnn\} ^ *****| Okamžitě ukončí přehrávání a přehraje skladbu **nnnn**.     |
| \{nnnn} A \{f}  | Zařadí zvuk **nnnn** do fronty **f** a také jej přehraje    |
| \{nnnn} ^ A \{f}| Vytvoří **sekvenci** zvuků počínaje **nnnn** ve frontě **f**|
| C               | Umaže **poslední vložený** zvuk z fronty                    |
| ^ C             | Smaže úplně frontu                                          |
| \{f} ^ C        | Smaže určenou frontu **f**                                  |
| B   | přehraje **další zvuk** ve frontě                                       |
| \{f}| přehraje **další zvuk** ze zvolené fronty **f**                         |
| ^ B | piřehraje **další zvuk** z **další fronty **                            |
| ^ 1| sníží hlasitost o stupeň, celkem 32 stupňů                               |
| ^ 2| zvýší hlasitost o stupeň                                                 |
| ^ 0| uloží nastavení do EEPROM                                                |

## Přehrávání zvuků

Po pokynu z klávesníce se zvolený zvuk **připraví** do výstupní "fronty". Jestliže je fronta prázdná (nic se nepřehrává), začne se ihned přehrávat zvolená skladba. Jinak se připravené zvuky přehrávají jeden po druhém, jak byly připraveny. Mezi přehrávávním jednotlivých zvuků je **vždy prodleva cca 3 sekundy**, tato prodleva nejde zrušit.

Sady zvuků se dají **připravit předem** do **front** (maximálně 9) a poté se dají přehrát stiskem jednoho nebo dvou tlačítek - funkce je zamýšlená k přehrávání posloupnosti staničních hlášení k jednotlivým spojům ve stanici. I takové zvuky se při pokynu k přehrání pouze zařadí do "výstupní" fronty a přehrají postupně. 


### Přehrávání jednotlivých zvuků
Nejjednodušší řežim použití. Kdykoliv se dá zadat číslo skladby (až 4 číslice) a potvrdit hvězdičkou (****). Zvolená skladba se přehraje - pokud se UŽ přehrává nějaká jiná skladba, zařadí se tato nově zadaná jako další v pořadí. Je možné **přidržet** hvězdičku - vymaže se celá fronta připravených zvuků a začne se přehrávat nově zvolený.

### Ruční příprava sady zvuků

Sada zvuků se začne tvořit stiskem **{nnnn} A {f}**, tedy například
    101 A 1
vytvoří **frontu 1**, a do ní vloží zvuk 101. Další zvuky se přidají opětovným stiskem "A" po čísle sklaby. Namísto čísla fronty jde používat hvězdičku - přidání do aktivní fronty. Stiskem **C** se vymaže poslední vložený zvuk z fronty. Jakmile se založí nová fronta, stane se tato nová "aktivní" a pracuje se s ní.

> Příklad:
>
> -   101 A 1
> -   103 A *
> *   211 A *
> *   102 A 5
> *   522 A 5
> *   C
> *   222 A 5
> *   301 A 3
> *   322 A 3

Vytvoří celkem 3 sady zvuků:
* **sadu 1**, ve které jsou zvuky č. 101, 103 a 211
* **sadu 5**, ve které jsou zvuky č. 102 a 222. Všimněte si smazání původně zadaného zvuku 522
* **sadu 3**, se zvuky 301 a 302


### Sada zvuků z posloupnosti

Jestliže jsou na SD kartě zvuky očíslované tak, že jejich čísla po sobě následují, můžete z nich udělat sadu jediným zadáním: Klávesu "A" stikněte **dlouze** (min 0.5 sec). Všechy zvuky počínaje zadaným číslem, které tvoří nepřerušenou vzestupnou číselnou řadu vytvoří sadu.

> Příklad: 
> 
> Na SD kartě DFPlayeru jsou tyto soubory:
>
> * 01/011.mp3
> * 01/012.mp3
> * 01/013.mp3
> * 01/014.mp3
> * 01/031.mp3
> * 01/032.mp3  
> * 01/033.mp3
> * 02/001.mp3
> * 02/002.mp3
>
> Nyní založíme jednotlivé sady:

> * 111 ^ A 2
> * 131 ^ A 4
> * 201 ^ A 9

Vytvoří se celkem 3 sady:
* **sadu 2**, ve které jsou zvuky č. 111-114. Zvuk 105 neexistuje, takže přehrávání končí na 104
* **sadu 4**, ve které jsou zvuky č. 131-133.
* **sadu 9**, se zvuky 201 a 202

## Přehrávání připravených zvuků
Vždy je **aktivní jedna sada zvuků**, ta do které se naposledy přidávalo, nebo která se vytvořila, nebo ze které se naposledy přehrávalo. Stisk "**B**" přehraje **další zvuk** z aktivní sady. Chceme-li změnit sadu, stiskneme **B dlouze** - najde se první *další fronta*, ve které ještě něco je, a z ní se přehraje zvuk.

Je možné změnit frontu na nějakou konkrétní - **před stiskem** "B" se zapíše číslo fronty.

> Příklad - obsah sad viz příklady výše:
>
> * 1 B
> * ^ B
> * ^ B
> * B
> * ^ B
> * ^ B
> * ^ B
> * 9 B
> * B
> * ^ B
> * ^ B
> * 1 B
> * ^ B
> * ^ B
> * 5 B
> * ^ B
> * ^ B

Provede toto:

1. Přehraje zvuk z výslovně určené sady 1, první zvuk je 101.
2. Přehraje zvuk z **další sady**, to jest 2. První zvuk je 111.
3. Přehraje zvuk z **další sady**, to jest 3. První zvuk je 301.
4. Přehraje *další zvuk z téže sady*, tedy 302
5. Přehraje zvuk z **další sady**, to jest 4. První zvuk je 131.
6. Přehraje zvuk z **další sady**, to jest 5. První zvuk je 102.
7. Přehraje zvuk z **další sady**, to jest 9. Všimněte si, že sady 6-8 nejsou definované - jsou prázdné. První zvuk je 201
9. Přehraje zvuk z výslovně určené sady 9, ńa řadě je 202
10. Zkusí přehrát další zvuk ze **současné** sady (9), tam nic není. Nestane se nic.
11. Přehraje zvuk z **další sady**, to jest 1. Všimněte si "zacyklení" - po 9 následuje 1. Na řadě je 103
12. Přehraje zvuk z **další sady**, to jest 2. Na řadě je 112.
13. Opět přehraje zvuk z určené sady 1, poslední zde je 211.
13. Opět přehraje zvuk z určené sady 2. Na řadě je 113.
14. Přehraje zvuk z **další sady**, to jest 3. Na řadě je poslední zvuk, 302.
15. Přehraje zvuk z výslovně určené sady 5, ńa řadě je 222, poslední zvuk.

V tomto místě chci zopakovat obsah jednotlivých front:
* 1: zvuk 211
* 2: zvuk 114
* 3: prázdná
* 4: zvuky 132, 133
* 5: prázdná
* 9: pokus o 203

Stav "pokus o 203" je důležitý u sad vzniklých z číselných posloupností zvuků. DFPlayer **neumí listovat skladbami na SD kartě**, pouze hlásí chybu, když dojde k přehrání skladby, která neexistuje. Až ve chvíli přehrání skladby se tedy zjistí, že je sada vlastně prázdná. A až v tu chvíli se sada vyprázdní - uvolní. Dojde tedy k **marnému stisku dlouhého B**, které nařídí přehrávání z (již prázdné) fronty.

Když tedy nyní stiskneme

* 4 B
* ^ B
* ^ B

tak se:

* Ze sady 4 přehraje zvuk 132 
* sada 5 je prázdná, přeskakuje se. Přehraje se sada 9. Až **při přehrání** se zjistí konec, sada se uvolní. **nestane se nic**
* Ze sady 1 se přehraje zvuk 114.



## Přehrávání hlášení o vlacích
Sady zvuků jsou zamýšlené k přehrávání posloupnosti staničních hlášení tak, jak postupně následují při přijetí, odbavení a vypravení vlaku.

| Soubor    | Spoj            | Obsah                 |
|-----------|-----------------|-----------------------|
| 01/101.mp | Rakovník - Praha| Přijede na kolej 4    |
| 01/102.mp3|                 | Přijíždí na kolej 4   |
| 01/103.mp3|                 | Stanice Kladno        |
| 01/104.mp3| -               | Přestupujte na Kralupy|
| 01/105.mp3| Rakovník - Praha| Ukončete nástup       |
| 02/201.mp3| Praha - Kralupy | Přijede na kolej 3    |
| 02/202.mp3|                 | Přijíždí na kolej 3   |
| 02/203.mp3|                 | Přijel na kolej 3     |
| 02/204.mp3| -               | Přestupujte na Prahu  |
| 02/211.mp3| K.Dubí - Kladno | Přijede na kolem 5    |
| 02/212.mp3|                 | Přijel, končí         |
| 02/213.mp3|                 | Stanice Kladno, končí |

*Poznámka: ano, vím, že trať Praha - Kralupy a Kladno - Dubí je stejná, a tudíž asi těžko přijedou 2 vlaky po sobě ... ale došly mi nápady. Vymyslete a naformátujte lepší příklad.* 

Předpokládejme, že soupravy se začnou sjíždět v pořadí "Praha - Kralupy", "Dubí - Kladno", "Praha - Rakovník". Provedeme:

* 201 ^ A ^ 1
* 211 ^ A 2
* 101 ^ A 3

Protože sada 1 se založí **dlouhým stiskem**, začne se ihned přehrávat první hlášení "prijede na kolej ...". Potom, podle nálady můžeme stisknout "B" (Praha - Kralupy přijíždí), nebo "dlouhé B" - provede se ohlášení dalšího vlaku. Použití doporučují takové, že ve chvíli prvního ohlášení vlaku se vytvoří sada **a zároveň se přehraje** první zvuk. Poté se **dlouhým B** přehraje vždy další hlášení dalšího vlaku až do zblbnutí a vyčerpání zásob.


## Nastavení
V současné verzi obsahuje nastavení **pouze úroveň hlasitosti**. Dlouhým stiskem **0** se současná úroveň hlasitosti přehrávání uloží a použije se po dalším zapnutí.

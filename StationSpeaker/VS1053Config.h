#ifndef __vs1053_config_h__
#define __vs1053_config_h__

/**
 * RESET, u shieldu nepouzito
 */
const int SHIELD_RESET = -1;

#ifndef VS1053_FAKE
////////////////////////////////////////////////////////
// Nastaveni pinu VS1053 -- podle dokumentace
// a Adafruit shieldu:

/**
 * Signal pro ridici prikazy
 */
const int SHIELD_CS = 7;

/**
 * Signal pro datove prenosy
 */
const int SHIELD_DCS = 6;

/**
 * Linka pro komunikaci se SD kartou
 */
const int SHIELD_CARDCS = 10;

/**
 * Signal indikujici pripravenost prijmout data.
 * Mel by byt na interrupt vstupu, pin 2 nebo 3.
 */
const int SHIELD_DREQ = 3;

#endif

/**
 * Nasledujici by MELY byt na MISO/MOSI/SCK pinech Arduina
 * Upravit podle konretni desky
 */
const int SHIELD_SCK  = 13;
const int SHIELD_MOSI = 11;
const int SHIELD_MISO = 12;

#endif

#ifndef __hwconfig_h__
#define __hwconfig_h__

/**
 * RESET, u shieldu nepouzito
 */
const int SHIELD_RESET = -1;

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
const int SHIELD_SDS = 4;

/**
 * Signal indikujici pripravenost prijmout data.
 * Mel by byt na interrupt vstupu, pin 2 nebo 3.
 */
const int SHIELD_DREQ = 3;

#endif


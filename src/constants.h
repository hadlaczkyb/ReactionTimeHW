/* @Leírás:
 * 	A használt konstansokat tartalmazó .h file
 */

#ifndef EMLIB_CONSTANTS_H_
#define EMLIB_CONSTANTS_H_

/* az indítashoz és a random karakter generálásához szükséges nyomógomb
 * megfelelõ port / láb kiosztásnak definiálása
 *
 * erre azért van szükség, hogy a random generátor tényleg
 * véletlen karaktert adjon ki, mivel a rand() függvény
 * önmagában a kontrolleren mindig ugyanazokat a karaktereket adná
 * indítás után
 */
#define BUTTON_PORT     gpioPortB
#define BUTTON_PIN      9

/* az idõméréshez szükséges konstansok
 *
 * mivel a Timer csak 4.79s-ig tud elszámolni, így az idõmérés elve a következõ:
 * a Timer-t csak 1s-ig engejük elszámolni, ekkor kérünk egy IT-t és elõröl kezdjük a
 * számolást, a végén természetesen beleszámítjuk, hogy hányszor volt IT, azaz hány
 * másodperc telt el, ehhez megfelelõ osztásarány szükséges
 */
#define DEBUG_BREAK                                   __asm__("BKPT #0"); // debugolást segítõ asm utasítás, így könnyebb észrevenni ha hiba van
#define ONE_MILLISECOND_BASE_VALUE_COUNT             1000 // 1000ms-ot hozzá kell adni a mért idõhöz IT esetén, mivel eltelt 1s
#define ONE_SECOND_TIMER_COUNT                        13672 // ekkor telt el 1s
#define MILLISECOND_DIVISOR                           13.672 // ennyivel osztjuk a Timer értékét a ms-os felbontáshoz

#endif /* EMLIB_CONSTANTS_H_ */

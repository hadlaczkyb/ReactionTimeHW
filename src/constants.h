/* @Le�r�s:
 * 	A haszn�lt konstansokat tartalmaz� .h file
 */

#ifndef EMLIB_CONSTANTS_H_
#define EMLIB_CONSTANTS_H_

/* az ind�tashoz �s a random karakter gener�l�s�hoz sz�ks�ges nyom�gomb
 * megfelel� port / l�b kioszt�snak defini�l�sa
 *
 * erre az�rt van sz�ks�g, hogy a random gener�tor t�nyleg
 * v�letlen karaktert adjon ki, mivel a rand() f�ggv�ny
 * �nmag�ban a kontrolleren mindig ugyanazokat a karaktereket adn�
 * ind�t�s ut�n
 */
#define BUTTON_PORT     gpioPortB
#define BUTTON_PIN      9

/* az id�m�r�shez sz�ks�ges konstansok
 *
 * mivel a Timer csak 4.79s-ig tud elsz�molni, �gy az id�m�r�s elve a k�vetkez�:
 * a Timer-t csak 1s-ig engej�k elsz�molni, ekkor k�r�nk egy IT-t �s el�r�l kezdj�k a
 * sz�mol�st, a v�g�n term�szetesen belesz�m�tjuk, hogy h�nyszor volt IT, azaz h�ny
 * m�sodperc telt el, ehhez megfelel� oszt�sar�ny sz�ks�ges
 */
#define DEBUG_BREAK                                   __asm__("BKPT #0"); // debugol�st seg�t� asm utas�t�s, �gy k�nnyebb �szrevenni ha hiba van
#define ONE_MILLISECOND_BASE_VALUE_COUNT             1000 // 1000ms-ot hozz� kell adni a m�rt id�h�z IT eset�n, mivel eltelt 1s
#define ONE_SECOND_TIMER_COUNT                        13672 // ekkor telt el 1s
#define MILLISECOND_DIVISOR                           13.672 // ennyivel osztjuk a Timer �rt�k�t a ms-os felbont�shoz

#endif /* EMLIB_CONSTANTS_H_ */

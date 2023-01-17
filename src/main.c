/********************************************************************************************************//**
 * @Le�r�s:
 * 	A j�t�k az alfanumerikus kijelz� valamelyik poz�ci�j�ra �rjon ki egy karaktert (sz�m vagy kisbet�).
 * 	A j�t�kosnak min�l hamarabb vissza kell k�ldenie a kijelzett karaktert a soros porton kereszt�l.
 *
 * @K�sz�tette:
 * 	Szab� Bence - C4W6E3
 * 	Hadlaczky Bence - IC0IKR
 *
 * @Kelt:
 * 	2019.11.02.
 ***********************************************************************************************************/

// felhaszn�lt default .h file-ok
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_timer.h"
#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"
#include "segmentlcd.h"

// saj�t .h file-ok
#include "constants.h"
#include "reac.h"
#include "perif.h"

/**************************************************************************//**
* A f�program
*****************************************************************************/
int main(void)
{

  // chip inicializ�ci�
	CHIP_Init();

  // a PB0, az UART �s a Timer perif�ri�k inicializ�l�sa
	button_Init();
	uart_Init();
	timer_Init();

  // LCD kijelz� inicaliz�l�sa
	SegmentLCD_Init(false);

	int err = 0; // hibasz�m v�ltoz�ja

  // nyit�sz�veg ki�r�sa
	greet_Write();

  // v�rjuk, hogy a felhaszn�l� lenyomja a PB0-�t �s m�rj�k a k�zben eltel id�t
	buttonActiveCount();

  // vigy�zz, k�sz, rajt ki�r�sa
	ready_Write();


/**************************************************************************//**
* A f�program ciklusa
*****************************************************************************/

  while (1)
  {
	  char p; // v�letlen karakter v�ltoz�ja
	  char in; // bemeneti v�ltoz�



	// p-re megh�vjuk a random gener�tort �s �talak�tjuk string-g�
	  p = randomChar();
	  char pstring[2];
	  pstring[0] = p;
	  pstring[1] = '\0';

	// ki�rjuk p-t az LCD-re (stringet v�r, ez�rt hozunk l�tre egyet feljebb)
	  SegmentLCD_Write(pstring);


	  int64_t start_time = get_time_in_ms(); // id�m�r�s kezdete

	  in = USART_Rx(UART0); // bemenet olvas�sa

	  int32_t end_time = elapsed_ms(start_time); // id�m�r�s v�ge

	// karakter �sszehasonl�t�s
	  if(p == in)
	  {
		  err = err; // ha azonosak, akkor nem n� a hibasz�m
	  }
	  else
	  {
		  err = err + 1; // ha k�l�nb�znek, akkor 1-gyel n�velj�k a hibasz�mot
	  }

	// az elv�rt karakter, a hibasz�m �s a reakci�id� ki�r�sa soros portra
	  info_Print(p,end_time,err);

	// 1s-os k�sleltet�s
	  delay_ms(1000);
  }
}

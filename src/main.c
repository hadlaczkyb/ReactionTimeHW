/********************************************************************************************************//**
 * @Leírás:
 * 	A játék az alfanumerikus kijelzõ valamelyik pozíciójára írjon ki egy karaktert (szám vagy kisbetû).
 * 	A játékosnak minél hamarabb vissza kell küldenie a kijelzett karaktert a soros porton keresztül.
 *
 * @Készítette:
 * 	Szabó Bence - C4W6E3
 * 	Hadlaczky Bence - IC0IKR
 *
 * @Kelt:
 * 	2019.11.02.
 ***********************************************************************************************************/

// felhasznált default .h file-ok
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

// saját .h file-ok
#include "constants.h"
#include "reac.h"
#include "perif.h"

/**************************************************************************//**
* A fõprogram
*****************************************************************************/
int main(void)
{

  // chip inicializáció
	CHIP_Init();

  // a PB0, az UART és a Timer perifériák inicializálása
	button_Init();
	uart_Init();
	timer_Init();

  // LCD kijelzõ inicalizálása
	SegmentLCD_Init(false);

	int err = 0; // hibaszám változója

  // nyitószöveg kiírása
	greet_Write();

  // várjuk, hogy a felhasználó lenyomja a PB0-át és mérjük a közben eltel idõt
	buttonActiveCount();

  // vigyázz, kész, rajt kiírása
	ready_Write();


/**************************************************************************//**
* A fõprogram ciklusa
*****************************************************************************/

  while (1)
  {
	  char p; // véletlen karakter változója
	  char in; // bemeneti változó



	// p-re meghívjuk a random generátort és átalakítjuk string-gé
	  p = randomChar();
	  char pstring[2];
	  pstring[0] = p;
	  pstring[1] = '\0';

	// kiírjuk p-t az LCD-re (stringet vár, ezért hozunk létre egyet feljebb)
	  SegmentLCD_Write(pstring);


	  int64_t start_time = get_time_in_ms(); // idõmérés kezdete

	  in = USART_Rx(UART0); // bemenet olvasása

	  int32_t end_time = elapsed_ms(start_time); // idõmérés vége

	// karakter összehasonlítás
	  if(p == in)
	  {
		  err = err; // ha azonosak, akkor nem nõ a hibaszám
	  }
	  else
	  {
		  err = err + 1; // ha különböznek, akkor 1-gyel növeljük a hibaszámot
	  }

	// az elvárt karakter, a hibaszám és a reakcióidõ kiírása soros portra
	  info_Print(p,end_time,err);

	// 1s-os késleltetés
	  delay_ms(1000);
  }
}

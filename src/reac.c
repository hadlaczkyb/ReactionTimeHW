/* @Leírás:
 * 	A program által használt függvények definícióit
 * 	tartalmazó .c file (a periféria inicializációt végzõ
 * 	függvényeken kívül)
 */

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

// az idõméréshez használt változók deklarációja
volatile uint64_t base_value = 0;
volatile bool timer1_overflow = false;
volatile uint32_t msTicks = 0;

/* a random karaktergenerátor inicializálásahoz használt
* és a nyomógomb kezeléséhez szükséges változók
*/
volatile int64_t buttom_start;
volatile int32_t button_end;
volatile bool live_button_state;

const char alphanum[] = "abcdefghijklmnopqrstuvwxyz0123456789"; //alfanumerikus tömb a random karakter generálásához

// SysTick kezelõ függvény az idõméréshez
void SysTick_Handler(void)
{
  msTicks++;
}

// a Timer1 túlcsordulását kezelõ IT függvény
void TIMER1_IRQHandler(void)
{
      timer1_overflow = true;
      base_value += ONE_MILLISECOND_BASE_VALUE_COUNT;
      TIMER_IntClear(TIMER1, TIMER_IF_OF);
}

// a mért idõ kifejezése ms-ban
uint64_t get_time_in_ms()
{
    // a túlcsordulásjelzõ törlése
      timer1_overflow = false;

    // az aktuális számlálóérték
      uint16_t count = TIMER1->CNT;

    // a bázis érték másolása
      uint64_t copy_of_base_value = base_value;

    // ha közben túlcsordulás történt, akkor újra felvesszük az értékeket
      if (timer1_overflow)
      {
            count = TIMER1->CNT;
            copy_of_base_value = base_value;
      }

    // milliszekundumok számítása
      return copy_of_base_value + count / MILLISECOND_DIVISOR;
}

// eltelet ms-ok
int64_t elapsed_ms(int64_t start_ms)
{
      return get_time_in_ms() - start_ms;
}

// késleltetés
void delay_ms(uint64_t milliseconds)
{
      uint64_t trigger_time = get_time_in_ms() + milliseconds;
      while (get_time_in_ms() < trigger_time);
}

// a nyomógomb által biztosított késleltetést vegzõ függvény
void  buttonActiveCount()
{
  // idõmeres kezdete
	buttom_start = get_time_in_ms();

  // a gomb állapotának leolvasása: Lenyomva 0, Elengedve 1
	live_button_state = GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN);

	while (live_button_state==1)
	{
  // a gomb állapotának leolvasása: Lenyomva 0, Elengedve 1
		live_button_state = GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN);
	}
		button_end = elapsed_ms(buttom_start); // idõmérés vége
}

/* a random alfanumerikus karaktert generáló függvények
 *
 * elõször a rand() függvény által adott szám és a gomb lenyomásáig eltelt idõt
 * maradékos osztjuk az alfanumerikus karakterek számával
 *
 * ezután kiválasztjuk az elõbb kapott számmmal megegyezõ sorszámú karaktert a tömbbõl
 */
int intN(int n)
{
	return (rand() + button_end ) %n;
}
char randomChar(void)
{
	char rchr = alphanum[intN(strlen(alphanum))];
	return rchr;
}

// üdvözlõ szöveg
void greet_Write (void)
{
	delay_ms(2000);
	SegmentLCD_Write("React.");
	delay_ms(1000);
	SegmentLCD_Write("Time");
	delay_ms(1000);
	SegmentLCD_Write("Game");
	delay_ms(1000);

  // nyomd meg a PB0-át
	SegmentLCD_Write("Push");
	delay_ms(1000);
	SegmentLCD_Write("BTN0!");
}

// vigyázz, kész, rajt kiírása
void ready_Write (void)
{
	SegmentLCD_Write("Ready...");
	delay_ms(1000);
	SegmentLCD_Write("Set...");
	delay_ms(1000);
	SegmentLCD_Write("Go!");
	delay_ms(2000);
}

// az elvárt karakter, a hibaszám és a reakcióidõ kiírása soros portra
void info_Print(char p, int32_t end_time, int err)
{
	char* req = "REQ: ";
	char* error = " ERROR: ";
	char* reactime = " RT: ";
	char* crlf = "\r\n";

  // az elvárt karakter kiírása UART-on
	for(int i=0; i < strlen(req); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,req[i]);
	}
	USART_Tx(UART0, p);

  /* a hibák számának kiírása
   *
   * mivel a UART ASCII-t használ, így a hibszámot át kell váltani
   * ASCII-ba, itt éltünk azzal a feltételezéssel, hogy 99-nél
   * nagyobb hibaszám elérése nem reális, hiszen ahhoz nagyon hosszan
   * kéne Reset nélkül játszani, vagy célzottan arra kellene törekedni,
   * hogy még véletlenül se az elvárt karaktert írjuk be
   */
	for(int i=0; i < strlen(error); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,error[i]);
	}

  // a hibaszám kiírása ASCII-ban (max. 99)
	int err_10; // a tízes helyiérték számjegye
	int err_1; // az egyes helyiérték számjegye
	if(err < 100)
	{
		err_10 = err / 10;
		err_1 = err - err_10*10;
	}
	else
	{
		err_10 = 9;
		err_1 = 9;
	}
	USART_Tx(UART0, 48 + err_10);
	USART_Tx(UART0, 48 + err_1);

  /* a reakcióidõ kiírása
   *
   * mivel a UART ASCII-t használ, így a hibszámot át kell váltani
   * ASCII-ba, itt azzal az egyszerûsítéssel éltünk, hogy az
   * STK32GG990F1024 csak 9999-et tud kiírni maximum, így a UART-ra
   * is max ekkora értéket küldünk ki
   */
	for(int i=0; i < strlen(reactime); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,reactime[i]);
	}
  // reakcióidõ kiírása ASCII-ban (max. 9999)
	int et_1000; // az ezres helyiérték számjegye
	int et_100;  // a százas helyiérték számjegye
	int et_10;  // a tízes helyiérték számjegye
	int et_1;  // a egyes helyiérték számjegye
	if(end_time < 10000)
	{
		et_1000 = end_time / 1000;
		et_100 = (end_time - et_1000*1000) / 100;
		et_10 = (end_time - et_1000*1000 - et_100*100) / 10;
		et_1 = (end_time - et_1000*1000 - et_100*100 - et_10*10);
	}
	else
	{
		et_1000 = 9;
		et_100 = 9;
		et_10 = 9;
		et_1 = 9;
	}
	USART_Tx(UART0,48 + et_1000);
	USART_Tx(UART0,48 + et_100);
	USART_Tx(UART0,48 + et_10);
	USART_Tx(UART0,48 + et_1);
	SegmentLCD_Number(end_time);

  // CR-LF kiírása
	for(int i = 0; i < strlen(crlf); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,crlf[i]);
	}
}


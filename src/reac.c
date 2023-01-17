/* @Le�r�s:
 * 	A program �ltal haszn�lt f�ggv�nyek defin�ci�it
 * 	tartalmaz� .c file (a perif�ria inicializ�ci�t v�gz�
 * 	f�ggv�nyeken k�v�l)
 */

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

// az id�m�r�shez haszn�lt v�ltoz�k deklar�ci�ja
volatile uint64_t base_value = 0;
volatile bool timer1_overflow = false;
volatile uint32_t msTicks = 0;

/* a random karaktergener�tor inicializ�l�sahoz haszn�lt
* �s a nyom�gomb kezel�s�hez sz�ks�ges v�ltoz�k
*/
volatile int64_t buttom_start;
volatile int32_t button_end;
volatile bool live_button_state;

const char alphanum[] = "abcdefghijklmnopqrstuvwxyz0123456789"; //alfanumerikus t�mb a random karakter gener�l�s�hoz

// SysTick kezel� f�ggv�ny az id�m�r�shez
void SysTick_Handler(void)
{
  msTicks++;
}

// a Timer1 t�lcsordul�s�t kezel� IT f�ggv�ny
void TIMER1_IRQHandler(void)
{
      timer1_overflow = true;
      base_value += ONE_MILLISECOND_BASE_VALUE_COUNT;
      TIMER_IntClear(TIMER1, TIMER_IF_OF);
}

// a m�rt id� kifejez�se ms-ban
uint64_t get_time_in_ms()
{
    // a t�lcsordul�sjelz� t�rl�se
      timer1_overflow = false;

    // az aktu�lis sz�ml�l��rt�k
      uint16_t count = TIMER1->CNT;

    // a b�zis �rt�k m�sol�sa
      uint64_t copy_of_base_value = base_value;

    // ha k�zben t�lcsordul�s t�rt�nt, akkor �jra felvessz�k az �rt�keket
      if (timer1_overflow)
      {
            count = TIMER1->CNT;
            copy_of_base_value = base_value;
      }

    // milliszekundumok sz�m�t�sa
      return copy_of_base_value + count / MILLISECOND_DIVISOR;
}

// eltelet ms-ok
int64_t elapsed_ms(int64_t start_ms)
{
      return get_time_in_ms() - start_ms;
}

// k�sleltet�s
void delay_ms(uint64_t milliseconds)
{
      uint64_t trigger_time = get_time_in_ms() + milliseconds;
      while (get_time_in_ms() < trigger_time);
}

// a nyom�gomb �ltal biztos�tott k�sleltet�st vegz� f�ggv�ny
void  buttonActiveCount()
{
  // id�meres kezdete
	buttom_start = get_time_in_ms();

  // a gomb �llapot�nak leolvas�sa: Lenyomva 0, Elengedve 1
	live_button_state = GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN);

	while (live_button_state==1)
	{
  // a gomb �llapot�nak leolvas�sa: Lenyomva 0, Elengedve 1
		live_button_state = GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN);
	}
		button_end = elapsed_ms(buttom_start); // id�m�r�s v�ge
}

/* a random alfanumerikus karaktert gener�l� f�ggv�nyek
 *
 * el�sz�r a rand() f�ggv�ny �ltal adott sz�m �s a gomb lenyom�s�ig eltelt id�t
 * marad�kos osztjuk az alfanumerikus karakterek sz�m�val
 *
 * ezut�n kiv�lasztjuk az el�bb kapott sz�mmmal megegyez� sorsz�m� karaktert a t�mbb�l
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

// �dv�zl� sz�veg
void greet_Write (void)
{
	delay_ms(2000);
	SegmentLCD_Write("React.");
	delay_ms(1000);
	SegmentLCD_Write("Time");
	delay_ms(1000);
	SegmentLCD_Write("Game");
	delay_ms(1000);

  // nyomd meg a PB0-�t
	SegmentLCD_Write("Push");
	delay_ms(1000);
	SegmentLCD_Write("BTN0!");
}

// vigy�zz, k�sz, rajt ki�r�sa
void ready_Write (void)
{
	SegmentLCD_Write("Ready...");
	delay_ms(1000);
	SegmentLCD_Write("Set...");
	delay_ms(1000);
	SegmentLCD_Write("Go!");
	delay_ms(2000);
}

// az elv�rt karakter, a hibasz�m �s a reakci�id� ki�r�sa soros portra
void info_Print(char p, int32_t end_time, int err)
{
	char* req = "REQ: ";
	char* error = " ERROR: ";
	char* reactime = " RT: ";
	char* crlf = "\r\n";

  // az elv�rt karakter ki�r�sa UART-on
	for(int i=0; i < strlen(req); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,req[i]);
	}
	USART_Tx(UART0, p);

  /* a hib�k sz�m�nak ki�r�sa
   *
   * mivel a UART ASCII-t haszn�l, �gy a hibsz�mot �t kell v�ltani
   * ASCII-ba, itt �lt�nk azzal a felt�telez�ssel, hogy 99-n�l
   * nagyobb hibasz�m el�r�se nem re�lis, hiszen ahhoz nagyon hosszan
   * k�ne Reset n�lk�l j�tszani, vagy c�lzottan arra kellene t�rekedni,
   * hogy m�g v�letlen�l se az elv�rt karaktert �rjuk be
   */
	for(int i=0; i < strlen(error); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,error[i]);
	}

  // a hibasz�m ki�r�sa ASCII-ban (max. 99)
	int err_10; // a t�zes helyi�rt�k sz�mjegye
	int err_1; // az egyes helyi�rt�k sz�mjegye
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

  /* a reakci�id� ki�r�sa
   *
   * mivel a UART ASCII-t haszn�l, �gy a hibsz�mot �t kell v�ltani
   * ASCII-ba, itt azzal az egyszer�s�t�ssel �lt�nk, hogy az
   * STK32GG990F1024 csak 9999-et tud ki�rni maximum, �gy a UART-ra
   * is max ekkora �rt�ket k�ld�nk ki
   */
	for(int i=0; i < strlen(reactime); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,reactime[i]);
	}
  // reakci�id� ki�r�sa ASCII-ban (max. 9999)
	int et_1000; // az ezres helyi�rt�k sz�mjegye
	int et_100;  // a sz�zas helyi�rt�k sz�mjegye
	int et_10;  // a t�zes helyi�rt�k sz�mjegye
	int et_1;  // a egyes helyi�rt�k sz�mjegye
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

  // CR-LF ki�r�sa
	for(int i = 0; i < strlen(crlf); i++)
	{
		while( !(UART0->STATUS & (1 << 6)) );
		USART_Tx(UART0,crlf[i]);
	}
}


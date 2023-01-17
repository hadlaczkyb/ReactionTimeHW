/* @Le�r�s:
 * 	A program �ltal haszn�lt perif�ri�k inicaliz�ci�j�t
 * 	v�gz� f�ggv�nyek defin�ci�i
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

// PB0 inicializ�ci�ja
void button_Init(void)
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_PIN, gpioModeInput, 0);
}

// UART0 inicializ�ci�ja
void uart_Init(void)
{
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
  // PF7 kimenetbe �ll�t�sa (push-pull)
	GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE7_PUSHPULL;
  // PF7 "magasba" �ll�t�sa
	GPIO->P[5].DOUTSET = 1 << 7;

	CMU_ClockEnable(cmuClock_UART0, true);

  // UART0 115200 Baud, 8N1 keret form�tummal
	USART_InitAsync_TypeDef uart0_init = USART_INITASYNC_DEFAULT;
	uart0_init.baudrate = 115200;
	uart0_init.refFreq = 0;
	uart0_init.databits = usartDatabits8;
	uart0_init.parity = usartNoParity;
	uart0_init.stopbits = usartStopbits1;
	uart0_init.mvdis = false;
	uart0_init.oversampling = usartOVS16;
	uart0_init.prsRxEnable = false;
	uart0_init.prsRxCh = 0;
	uart0_init.enable = usartEnable;
	USART_InitAsync(UART0, &uart0_init);

  // PE0 (TX) push-pull kimenet
	GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);
  // PE1 (RX) bemenetbe �ll�t�sa
	GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);

  // Location 1-et haszn�lja UART0
	UART0->ROUTE |= USART_ROUTE_TXPEN | USART_ROUTE_RXPEN;
	UART0->ROUTE |= USART_ROUTE_LOCATION_LOC1;
}

// TIMER1 inicializ�ci�ja
void timer_Init(void)
{
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000))
	{
		DEBUG_BREAK;
	}

	CMU_ClockEnable(cmuClock_TIMER1, true);

	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	timerInit.prescale = timerPrescale1024;

	TIMER_IntEnable(TIMER1, TIMER_IF_OF);

 // TIMER1 IT enged�lyez�se NVIC-ben
	NVIC_EnableIRQ(TIMER1_IRQn);

 // Timer top �rt�k be�ll�t�sa
	TIMER_TopSet(TIMER1, ONE_SECOND_TIMER_COUNT);

	TIMER_Init(TIMER1, &timerInit);

 // V�rakoz�s Timer "indul�s�ra"
	while (TIMER1->CNT == 0);
}



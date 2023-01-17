/* @Leírás:
 * 	A program által használt függvények deklarációit
 * 	tartalmazó .h file (a periféria inicializációt végzõ
 * 	függvényeken kívül)
 */

#ifndef INC_REAC_H_
#define INC_REAC_H_

void SysTick_Handler(void);
void TIMER1_IRQHandler(void);
uint64_t get_time_in_ms();
int64_t elapsed_ms(int64_t start_ms);
void delay_ms(uint64_t milliseconds);
void  buttonActiveCount();
int intN(int n);
char randomChar(void);
void greet_Write (void);
void ready_Write (void);
void info_Print(char p, int32_t end_time, int err);

#endif /* INC_REAC_H_ */

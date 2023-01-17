/* @Le�r�s:
 * 	A program �ltal haszn�lt f�ggv�nyek deklar�ci�it
 * 	tartalmaz� .h file (a perif�ria inicializ�ci�t v�gz�
 * 	f�ggv�nyeken k�v�l)
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

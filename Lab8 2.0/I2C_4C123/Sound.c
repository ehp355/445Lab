/*Sound.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
*/
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#define PD3       (*((volatile uint32_t *)0x40007020))

void Timer0A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x01;    // 0) activate TIMER0
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = 181818-1;	    // 4) reload value for 440 Hz
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x60000000;// 8) priority 3
  NVIC_EN0_R |= 1<<19;           // 9) enable IRQ 19 in NVIC
}

void Timer0A_Handler(void){ 
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	PD3 ^= 0x8;
}
	
void soundAlarm(void){
  TIMER0_CTL_R = 0x00000001; // 10) enable TIMER1A
}

void endAlarm(void){
	TIMER0_CTL_R &= ~0x1;	
}
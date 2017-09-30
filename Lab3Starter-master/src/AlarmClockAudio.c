/*AlarmClockAudio.c
**Enrique Perez-Osborne
**Juliana Pulido
**Created: 9/20/17
**Audio interface for Alarm Clock
**Lab3
**TA: Cody Horton
**Last Modified: 9/28/17
*/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "../inc/tm4c123gh6pm.h"

#define PC4 (*((volatile uint32_t *)0x40006040))
	
extern int8_t B1;
extern int8_t B2;
extern int8_t B3;
extern int8_t B4;
int count =0;	
int alarm_On =0;

void PortC_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000004; 	// (a) activate clock for port C
	while((SYSCTL_PRGPIO_R&0x004) == 0){};// port C ready?
  GPIO_PORTC_AMSEL_R &= ~0x10;     // 3) disable analog for PC4
  GPIO_PORTC_PCTL_R &= ~0x000F0000;// 4) configure as GPIO
  GPIO_PORTC_DIR_R |= 0x10;        // 5) PC4 is output
  GPIO_PORTC_AFSEL_R &= ~0x10;     // 6) normal function
  GPIO_PORTC_DEN_R |= 0x10;        // 7) digital I/O on PF0
}

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x02;    // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = 182000;	    // 4) reload value for 440 Hz
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x0000A000; // 8) priority 5
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
}

void Timer1A_Handler(void){
	TIMER1_IMR_R &= ~0x1;	 
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	PC4 ^= 0x10;
	count++;
	TIMER1_IMR_R |= 0x1;
	if(count > 800){
		TIMER1_CTL_R &= ~0x1;						//disable TIMER1A
	}
	
}
	
void soundAlarm(void){
	TIMER1_IMR_R = 0x00000001;		//arm timer 
	TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}



/*Timer1A.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/

#include <stdio.h>
#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "Music.h"

#define PF3       (*((volatile uint32_t *)0x40025020))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void Timer1A_Init(void){
	long sr;
	sr =StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000001;   // 3) configure for one-shot mode, default down-count settings
  TIMER1_TAILR_R = 50000;				// 4) 10 ms
	TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	//TIMER1_CTL_R = 0x00000001;    // Timer 1A enabled in Play()
  EndCritical(sr);
}


void Timer1A_Handler(void){
	PF3 ^= 0x08;
	PF3 ^= 0x08;
	TIMER1_ICR_R = 0x01;	//acknowledge flag
	TIMER0_CTL_R = 0x00000000;    
	TIMER0_TAILR_R = getSong().periodicReload;	//load periodic reload value
	TIMER1_TAILR_R = getSong().oneShotReload;		//load oneshot reload value
	TIMER0_CTL_R = 0x00000001;   
	TIMER1_CTL_R = 0x00000001;
	nextNote();
	PF3 ^= 0x08;
}
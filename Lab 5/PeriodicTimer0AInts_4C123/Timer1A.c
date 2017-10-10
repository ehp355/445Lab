#include <stdio.h>
#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "Music.h"
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
  TIMER1_TAILR_R = 50000000;		
	TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00006000; // 8) priority 3
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	//TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
  EndCritical(sr);
}


void Timer1A_Handler(void){
	TIMER1_ICR_R = 0x01;	//acknowledge flag
	TIMER0_CTL_R = 0x00000000;    // 10) disable TIMER0A
	TIMER0_TAILR_R = getSong().periodicReload;
	TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	TIMER1_TAILR_R = getSong().oneShotReload;
	TIMER1_CTL_R = 0x01;
	nextNote();
}
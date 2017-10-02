#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"


uint32_t start_Time;
uint32_t end_Time;
uint32_t time_Ms =0;

void Timer3A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate timer3
  TIMER3_CTL_R = 0x00000000;    // 1) disable timer3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000000;   // 3) configure for periodic mode
  TIMER3_TAILR_R = 80000;    	  // 4) reload value (1ms)
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // 8) priority 3
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN1_R |= 0x8;           	// 9) enable IRQ 35 in NVIC
	TIMER3_CTL_R = 0x01;					// enable timer 3
}

void Timer3A_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	time_Ms++;
}

void set_Start_Time(void){
	start_Time = time_Ms;
}

void set_End_Time(void){
	end_Time = time_Ms;
}

uint32_t time_Diff(void){
	return end_Time - start_Time;
}
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void Timer0A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x1;	//Activate timer 0
	TIMER0_CTL_R = 0x00000000;	//Disable the timer
	TIMER0_CFG_R = 0x00000000;	//32bit configuration
	TIMER0_TAMR_R = 0x00000001;	//one shot mode
	TIMER0_TAILR_R = 800000;		//10ms
	TIMER0_TAPR_R = 0;            // 5) bus clock resolution
	TIMER0_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
	TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
	}

void Timer0A_Handler(void){
		TIMER0_IMR_R &= ~0x1;						//disarm timout
		TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
		GPIO_PORTB_IM_R  |= 0x1E;

}
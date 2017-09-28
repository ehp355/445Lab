#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x2;	//Activate timer 0
	TIMER1_CTL_R = 0x00000000;	//Disable the timer
	TIMER1_CFG_R = 0x00000000;	//32bit configuration
	TIMER1_TAMR_R = 0x00000001;	//one shot mode
	TIMER1_TAILR_R = 800000;		//10ms
	TIMER1_TAPR_R = 0;            // 5) bus clock resolution
	TIMER1_ICR_R = 0x00000001;    // 6) clear timer1A timeout flag
	TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
	NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	}

void Timer1A_Handler(void){
		TIMER1_IMR_R &= ~0x1;						//disarm timout
		TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
		GPIO_PORTB_IM_R  |= 0x1E;					//arm buttons

}
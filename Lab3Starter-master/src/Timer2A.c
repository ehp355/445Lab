#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"


	void Timer2A_Init(void){
		TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
		TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
		TIMER2_TAMR_R = 0x00000001;   // 3) configure for one shot mode
		TIMER2_TAILR_R = 800000;			//10ms
		TIMER2_TAPR_R = 0;            // 5) bus clock resolution
		TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
		TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
		NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
		NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
	}
	
	void Timer2A_Handler(void){
		TIMER2_IMR_R &= ~0x1;						//disarm timout
		TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
		GPIO_PORTB_IM_R  |= 0x1E;
		//set imr for port b 			//arm buttons
	}
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
int32_t button;
int8_t B1;
int8_t B2;
int8_t B3;
int8_t B4;



void Edge_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;	//ACTIVATE CLOCK FOR PORT B
	GPIO_PORTB_DIR_R &= ~0x1E;	//ENABLE DIGITAL I/O FOR PB1-4
	GPIO_PORTB_DEN_R |= 0x1E;
	GPIO_PORTB_AFSEL_R &= ~0x1E; //disable special functions PB1-4
	GPIO_PORTB_IS_R &= ~0x1E;		//edge sensitive at PB4-1
	GPIO_PORTB_IBE_R &= ~0x1E;	//enable rising edge trigger
	GPIO_PORTB_IEV_R |= 0x1E; 	//rising edge for PB1-4

	GPIO_PORTB_ICR_R = 0x1E;
	GPIO_PORTB_IM_R |= 0x1E;		//arm trigger flags
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)| 0x00004000; //set to priority 2
	NVIC_EN0_R  = 0x02;
	
	SYSCTL_RCGCTIMER_R |= 0x02;		//activate TIMER2
	//EnableInterrupts();
}



void GPIOPortB_Handler(void){
	GPIO_PORTB_ICR_R |= 0x1E;		//acknowledge flag
	GPIO_PORTB_IM_R  &= ~0x1E;			//ignore interrupts for portb
	//disable int
	//write 0 to imr for portbhandler
	//GPIO_PORTB_ICR_R &= ~0x1E;			//disarm buttons
	button = (GPIO_PORTB_DATA_R & 0x1E);		//CHECK button pressed
	
	switch(button){
		case 2:
			B1 = 1;
			B2 = 0;
			B3 = 0;
			B4 = 0;
			break;
		case 4: 
			B1 = 0;
			B2 = 1;
			B3 = 0;
			B4 = 0;
			break;
		case 8:
			B1 = 0;
			B2 = 0;
			B3 = 1;
			B4 = 0;
			break;
		case 0x10:
			B1 = 0;
			B2 = 0;
			B3 = 0;
			B4 = 1;
			break;
	//	default:
		//	do nothing
	}
	
	
	
	//start one shot timer
	
	TIMER2_IMR_R = 1;							//arm timer 1
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A		
	
	}



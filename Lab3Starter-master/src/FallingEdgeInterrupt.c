/*FallingEdgeInterrupt.c
**Enrique Perez-Osborne
**Juliana Pulido
**Created: 9/20/17
**EdgeInterrupt Service Routine
**for when buttons are pressed
**Lab3
**TA: Cody Horton
**Last Modified: 9/28/17
*/

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
int32_t button;
//B1 = Button 1, etc.
int8_t B1;
int8_t B2;
int8_t B3;
int8_t B4;
int8_t pressed = 0;


void Edge_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;	//ACTIVATE CLOCK FOR PORT B
	GPIO_PORTB_DIR_R &= ~0x01;	//ENABLE DIGITAL I/O FOR PB0
	GPIO_PORTB_DEN_R |= 0x01;
	GPIO_PORTB_AFSEL_R &= ~0x01; //disable special functions PB1-4
	GPIO_PORTB_IS_R &= ~0x01;		//edge sensitive at PB4-1
	GPIO_PORTB_IBE_R &= ~0x01;	//enable rising edge trigger
	GPIO_PORTB_IEV_R |= 0x01; 	//rising edge for PB1-4

	GPIO_PORTB_ICR_R = 0x01;
	GPIO_PORTB_IM_R |= 0x01;		//arm trigger flags
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)| 0x00004000; //set to priority 2
	NVIC_EN0_R  = 0x02;
}



void GPIOPortB_Handler(void){
	GPIO_PORTB_ICR_R |= 0x1E;		//acknowledge flag
	GPIO_PORTB_IM_R  &= ~0x1E;			//ignore interrupts for portb
	button = (GPIO_PORTB_DATA_R & 0x1E);		//CHECK button pressed
	pressed = 1;
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
		default:
			pressed = 0;
	}

	//start one shot timer
	if(pressed){
		TIMER0_IMR_R = 1;							//arm timer 0
		TIMER0_CTL_R = 0x00000001;    //10) enable timer0A		
	}
}



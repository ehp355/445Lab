/*SwitchControl.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
 
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

uint8_t button;

void PortC_Init(void){
	
	SYSCTL_RCGCGPIO_R |= 0x04;		//activate clock for port c
	GPIO_PORTC_DIR_R &= ~0x30;		//(c) make PC4-5 button inputs, PC6 input
	GPIO_PORTC_DEN_R |= 0x30;     //enable digital I/O on PC4-6
	GPIO_PORTC_AFSEL_R &= ~ 0x30;	//disable alternate function for PC4-5
	GPIO_PORTC_IS_R &= ~0x30;     //(d) PC4-6 are edge-sensitive 
	GPIO_PORTC_IBE_R &= ~0x30;    //PC4-5 is not both edges
	GPIO_PORTC_IEV_R |= 0x30;     //PC4-5 rising edge event
	GPIO_PORTC_ICR_R = 0x30;      //(e) clear flags
	GPIO_PORTC_IM_R |= 0x30;      //(f) arm interrupt on PC4-5
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0xA0000000; // (g) priority 5
	NVIC_EN0_R |= 1<<3;           // (h) enable interrupt 3 in NVIC
	
}

void GPIOPortC_Handler(void){
	GPIO_PORTC_IM_R &= ~0x7;		//disarm buttons
	TIMER1_CTL_R = 0x1;
	TIMER1_IMR_R = 0x1;					//arm timer
	button = (GPIO_PORTC_RIS_R & 0x30);
	GPIO_PORTC_ICR_R= button;	//acknowledge flag	
	
}

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x2;   // 0) activate TIMER2
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000001;   // 3) configure for one-shot mode, default down-count settings
  TIMER1_TAILR_R = 799999;				// 4) 10 ms
	TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x1;   				// 6) clear TIMER2A timeout flag
 // TIMER2_IMR_R = 0x1;    				// 7) arm timeout interrupt
	TIMER1_CTL_R = 0x1;						//enable timer
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x8000;					//set priority to 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R |= 1<<21;           // 9) enable IRQ 23 in NVIC
	//Timer 1A enabled in GPIOD handler
}

void Timer1A_Handler(void){
	GPIO_PORTC_IM_R |= 0x7;
	TIMER1_IMR_R=0;		//disarm timer
	TIMER1_ICR_R = 0x1;		//acknowledge the flag
}
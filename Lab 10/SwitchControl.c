/*SwitchControl.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
 
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

uint8_t button;

void PortE_Init(void){
	
	SYSCTL_RCGCGPIO_R |= 0x10;		//activate clock for port D
	GPIO_PORTE_DIR_R &= ~0xC;		//(c) make PE2-3 button inputs
	GPIO_PORTE_DEN_R |= 0xC;     //enable digital I/O on PE2-3
	GPIO_PORTE_AFSEL_R &= ~0xC;	//disable alternate function for PE2-3
	GPIO_PORTE_IS_R &= ~0xC;     //(d) PE2-3 are edge-sensitive 
	GPIO_PORTE_IBE_R &= ~0xC;    //PE2-3 is not both edges
	GPIO_PORTE_IEV_R |= 0xC;     //PE2-3 rising edge event
	GPIO_PORTE_ICR_R = 0xC;      //(e) clear flags
	GPIO_PORTE_IM_R |= 0xC;      //(f) arm interrupt on PE2-3
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x000000A0; // (g) priority 5
	NVIC_EN0_R |= 1<<4;           // (h) enable interrupt 3 in NVIC
	
}

void GPIOPortE_Handler(void){
	GPIO_PORTE_IM_R &= ~0xC;		//disarm buttons
	TIMER1_CTL_R = 0x1;
	TIMER1_IMR_R = 0x1;					//arm timer
}

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x2;   // 0) activate TIMER1
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
	button = (GPIO_PORTE_RIS_R & 0xC);
	GPIO_PORTE_ICR_R= button;	//acknowledge flag	
	GPIO_PORTE_IM_R |= 0x0C;
	TIMER1_IMR_R=0;		//disarm timer
	TIMER1_ICR_R = 0x1;		//acknowledge the flag
}

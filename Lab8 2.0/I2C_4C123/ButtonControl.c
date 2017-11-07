/*ButtonControl.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
*/
//Button 1 -> PD0
//Button 2 -> PD1
//Button 3 -> PD2
//PD4 is non intrusive flag for handler
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#define PD4       (*((volatile uint32_t *)0x40007040))
	
uint8_t button;

void PortD_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x08;		//activate clock for port d
	GPIO_PORTD_DIR_R &= ~0x07;		//(c) make PD0-2 button inputs
	GPIO_PORTD_DIR_R |= 0x18;			//PD3-4 are outputs
	GPIO_PORTD_DEN_R |= 0x1F;     //enable digital I/O on PD0-2
	GPIO_PORTD_AFSEL_R &= ~ 0x1F;	//disable alternate function
	GPIO_PORTD_IS_R &= ~0x07;     //(d) PD0-2 are edge-sensitive 
	GPIO_PORTD_IBE_R &= ~0x07;    //PD0-2 is not both edges
	GPIO_PORTD_IEV_R |= 0x07;     //PD0-2 falling edge event
	GPIO_PORTD_ICR_R = 0x07;      //(e) clear flags
	GPIO_PORTD_IM_R |= 0x07;      //(f) arm interrupt on PD0-2
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0xE0000000; // (g) priority 7
	NVIC_EN0_R |= 1<<3;            // (h) enable interrupt 3 in NVIC
}

void Timer2A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x4;   // 0) activate TIMER2
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000001;   // 3) configure for one-shot mode, default down-count settings
  TIMER2_TAILR_R = 799999;				// 4) 10 ms
	TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x1;   				// 6) clear TIMER2A timeout flag
 // TIMER2_IMR_R = 0x1;    				// 7) arm timeout interrupt
	TIMER2_CTL_R = 0x1;						//enable timer
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // 8) priority 1
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R |= 1<<23;           // 9) enable IRQ 23 in NVIC
	//Timer 1A enabled in GPIOD handler
}

void GPIOPortD_Handler(void){
	GPIO_PORTD_IM_R &= ~0x7;		//disarm buttons
	TIMER2_CTL_R = 0x1;
	TIMER2_IMR_R = 0x1;					//arm timer
	button = (GPIO_PORTD_RIS_R & 0x07);
	GPIO_PORTD_ICR_R= button;	//acknowledge flag	
	GPIO_PORTD_IM_R |= 0x7;
}

void Timer2A_Handler(void){
	TIMER2_IMR_R = 0;				//diasrm timer
	TIMER2_ICR_R = 0x1;	//acknowledge flag
}
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
	GPIO_PORTD_DEN_R |= 0x07;     //enable digital I/O on PD0-2
	GPIO_PORTD_AFSEL_R &= ~ 0x07;	//disable alternate function
	GPIO_PORTD_IS_R &= ~0x07;     //(d) PD0-2 are edge-sensitive 
	GPIO_PORTD_IBE_R &= ~0x07;    //PD0-2 is not both edges
	GPIO_PORTD_IEV_R |= 0x07;     //PD0-2 falling edge event
	GPIO_PORTD_ICR_R = 0x07;      //(e) clear flags
	GPIO_PORTD_IM_R |= 0x07;      //(f) arm interrupt on PD0-2
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x40000000; // (g) priority 2
	NVIC_EN0_R = 1<<3;           // (h) enable interrupt 3 in NVIC
}

void Timer1A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000001;   // 3) configure for one-shot mode, default down-count settings
  TIMER1_TAILR_R = 50000;				// 4) 10 ms
	TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = TIMER_IMR_TATOIM;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00002000; // 8) priority 1
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	//Timer 1A enabled in GPIOD handler
}


void GPIOPortD_Handler(void){
	PD4 ^= 0x10;
	PD4 ^= 0x10;
	GPIO_PORTD_ICR_R=0x07;	//acknowledge flag
	button = (GPIO_PORTD_DATA_R & 0x07);
	GPIO_PORTD_IM_R &= ~0x07;
	TIMER1_IMR_R = 0x00000001;
	TIMER1_CTL_R = 0x00000001;
	GPIO_PORTD_IM_R |= 0x07;
	PD4 ^= 0x10;
}

void Timer1A_Handler(void){
	TIMER1_ICR_R = 0x01;	//acknowledge flag
}
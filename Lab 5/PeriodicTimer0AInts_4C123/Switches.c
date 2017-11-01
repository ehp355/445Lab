/*Switches.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/

#include <stdio.h>
#include <stdint.h>
#include "Music.h"
#include "..//inc//tm4c123gh6pm.h"

#define PF1       (*((volatile uint32_t *)0x40025008))

int button;
extern struct Song Ode_To_Joy;


void EdgeTriggered_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04;			//activate clock for port c
	//GPIO_PORTC_LOCK_R = 0x4C4F434B; // unlock GPIO Port C
  GPIO_PORTC_DIR_R &= ~0x70;    // (c) make PC4-6 button inputs 
  GPIO_PORTC_DEN_R |= 0x70;     //     enable digital I/O on PC4-6
  GPIO_PORTC_AFSEL_R &= ~ 0x70;	// disable alternate function
	GPIO_PORTC_IS_R &= ~0x70;     // (d) PC4-6 are edge-sensitive 
  GPIO_PORTC_IBE_R &= ~0x70;    //     PC4-6 is not both edges 
  GPIO_PORTC_IEV_R |= 0x70;     //     PC4-6 falling edge event 
  GPIO_PORTC_ICR_R = 0x70;      // (e) clear flags
  GPIO_PORTC_IM_R |= 0x70;      // (f) arm interrupt on PC4-6
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R |= 0x04;              // (h) enable interrupt 2 in NVIC
}

void GPIOPortC_Handler(void){
	PF1 ^= 0x02; 
	PF1 ^= 0x02; 
	GPIO_PORTC_ICR_R=0x70;	//acknowledge flag
	button = (GPIO_PORTC_DATA_R & 0xF0);	//if = 0x40, PC6, if = 0x20, PC5, if = 0x10, PC4
	GPIO_PORTC_IM_R &= ~0x70; //disarm PC4-6
	TIMER2_CTL_R = 0x00000001;    //enable TIMER2A (one-shot for debounce)
	GPIO_PORTC_IM_R |= 0x70;
	PF1 ^= 0x02; 
	}
	


void Play(void){
	TIMER1_CTL_R = 0x00000001;    // enable TIMER1A to start the periodic
																// and one shot timers
}

void Stop(void){
	TIMER1_CTL_R = 0;							//stop music
	TIMER0_CTL_R = 0;
 }

void Rewind(void){
	TIMER1_CTL_R = 0;							//stop music	
	TIMER0_CTL_R = 0;
	reset_Index();
 }

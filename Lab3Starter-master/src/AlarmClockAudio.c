/*AlarmClockAudio.c
**Enrique Perez-Osborne
**Juliana Pulido
**Created: 9/20/17
**Audio interface for Alarm Clock
**Lab3
**TA: Cody Horton
**Last Modified: 9/20/17
*/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "../inc/tm4c123gh6pm.h"

#define PF0 (*((volatile uint32_t *)0x40025004))
#define PF4 (*((volatile uint32_t *)0x40025040))
	
extern int8_t B1;
extern int8_t B2;
extern int8_t B3;
extern int8_t B4;
	
int alarm_On =0;

void PortF_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4 and PF0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  //GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R |= 0x10;     // (d) PF4 is lvl-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R |= 0x10;    //     PF4 high lvl event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  //EnableInterrupts();           // (i) Clears the I bit
}

//void portEInit(void){
//	SYSCTL_RCGCGPIO_R |= 0x10;	//enable clock for port E
//	GPIO_PORTE_DIR_R  |= 0x03;		//make PE0 an input
//	//GPIO_PORTE_DIR_R  &= ~0x02;		//make PE1 an output
//	GPIO_PORTE_AFSEL_R &= ~0x03; //disable alt. funct. for PE0-1
//	GPIO_PORTE_DEN_R |= 0x03;
//}

//void soundIntInit(void){
//	SYSCTL_RCGCGPIO_R |= 0x10;	//enable clock for port E
//	GPIO_PORTE_IM_R &= ~0x02;		//disarm port E1 int.
//	GPIO_PORTE_IBE_R = 0;				
//	GPIO_PORTE_IEV_R = 1;
//	GPIO_PORTE_RIS_R = 0;				//
//	GPIO_PORTE_IS_R	|= 0x02;		//detect PE1 level
//	GPIO_PORTE_IM_R |= 0x02;		//arm PE1	
//	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x000000A0; // (g) priority 5
//  NVIC_EN0_R = 0x00100000;      // (h) enable interrupt 20 in NVIC
//  
//}
	
void GPIOPortF_Handler (void){

	while(!B1 && !B2 && !B3 && !B4){
	PF0 |= 0x01;
	}
	PF0 &= 0;
}

void soundAlarm(void){
	PF4 |= 1;
}


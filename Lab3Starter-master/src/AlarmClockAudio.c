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

#define PE0 (*((volatile uint32_t *)0x4005C004))

void portEInit(void){
	SYSCTL_RCGCGPIO_R |= 0x10;	//enable clock for port E
	GPIO_PORTE_DIR_R  |= 0x01;		//make PE0 and output
	GPIO_PORTE_AFSEL_R &= ~0x01; //disable alt. funct. for PE0
	GPIO_PORTE_DEN_R |= 0x01;
}

void soundAlarm(void){
	PE0 ^= 0x01;
}

void stopAlarm(void){
	PE0 = 0;
}
/*DAC.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/
#include <stdint.h>
#include "DAC.h"
#include "Music.h"
#include "..//inc//tm4c123gh6pm.h"


void DAC_Init(void){
	SYSCTL_RCGCSSI_R |= 0x08;		//activate SS3
	SYSCTL_RCGCGPIO_R |= 0x08;	//acgtivate port D
	while((SYSCTL_PRGPIO_R&0x08) ==0){
	};
	GPIO_PORTD_AFSEL_R = 0x0B; //enable alt funct on PD 0,1,3
	GPIO_PORTD_DEN_R |= 0x0B;	 //configure PD0,1,3 as SSI
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0F00)+0x00001011; //set PD0,1,3 as SSI
	GPIO_PORTD_AMSEL_R = 0;		//disable analog functionality on PD
	SSI3_CR1_R = 0x0;					//disable SSI, master mode
	SSI3_CPSR_R = 0x02;				//set clock to 8MHz
	SSI3_CR0_R &= ~(0x0000FFB0); //SCR = 0, SPH = 0, SPO = 1 Freescale (FRF = 0)
	SSI3_CR0_R |= 0x0F;
	SSI3_CR1_R |= 0x02;				//enable SSI
}

void DAC_Out(unsigned short value){
		SSI3_DR_R = value;
}
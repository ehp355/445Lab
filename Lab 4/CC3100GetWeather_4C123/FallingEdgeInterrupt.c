#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "adcSamplerTool.h"
#include "ADCSWTrigger.h"

int32_t button;
int8_t B0=0;



void Edge_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;	//ACTIVATE CLOCK FOR PORT B
	GPIO_PORTB_DIR_R &= ~0x1;	//ENABLE DIGITAL I/O FOR PB1-4
	GPIO_PORTB_DEN_R |= 0x1;
	GPIO_PORTB_AFSEL_R &= ~0x1; //disable special functions PB1-4
	GPIO_PORTB_IS_R &= ~0x1;		//edge sensitive at PB4-1
	GPIO_PORTB_IBE_R &= ~0x1;	//enable rising edge trigger
	GPIO_PORTB_IEV_R |= 0x1; 	//rising edge for PB1-4

	GPIO_PORTB_ICR_R = 0x1;
	GPIO_PORTB_IM_R |= 0x1;		//arm trigger flags
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)| 0x00004000; //set to priority 2
	NVIC_EN0_R  = 0x02;
	//EnableInterrupts();
}

void Timer2A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000001;   // 3) configure for one-shot mode
  TIMER2_TAILR_R = 800000;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  //TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
	TIMER2_IMR_R &= ~0x1;						//disarm timout
	ADCvalue = ADC0_InSeq3();
	B0 = 1;
	//ADCtoVolt();
	GPIO_PORTB_IM_R  |= 0x1;			//arm B0;
}



void GPIOPortB_Handler(void){
	GPIO_PORTB_ICR_R |= 0x1;		//acknowledge flag
	GPIO_PORTB_IM_R  &= ~0x1;			//ignore interrupts for portb
	TIMER2_IMR_R = 1;							//arm timer 1
	TIMER2_CTL_R = 0x00000001;    //10) enable timer1A		
	}



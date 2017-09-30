#include <stdio.h>
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADCSWTrigger.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"

	
extern int8_t B0;
	
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


volatile uint32_t ADCvalue;
// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.


void adcSampler_Init(void){
  //PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch0();         // allow time to finish activating

	//0x02 = x4 sampling
	//0x04 =  x16 sampling
	//0x06 = x64 sampling
	ADC0_SAC_R = 0x06;
	
	ST7735_InitR(INITR_REDTAB);		//Initialization for screen
	
  EnableInterrupts();
}

void ADCtoVolt(void){
	uint32_t volt = (ADCvalue*330)/4095;
	uint32_t Digit3 = volt%10;
	volt = volt/10;
	uint32_t Digit2 = volt%10;
	uint32_t Digit1 = volt/10;
	
	char title[] = "Voltage~";
	ST7735_SetCursor(0,5);
	ST7735_OutString(title);
	ST7735_OutChar(Digit1+'0');
	ST7735_OutChar('.');
	ST7735_OutChar(Digit2+'0');
	ST7735_OutChar(Digit3+'0');
	ST7735_OutChar('V');

}

int32_t button;
int8_t B0=0;




void Edge_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04;			//activate clock for port c
	GPIO_PORTC_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTC_DIR_R &= ~0x10;    // (c) make PC4 in 
  GPIO_PORTC_DEN_R |= 0x10;     //     enable digital I/O on PC4
  GPIO_PORTC_IS_R &= ~0x10;     // (d) PC4 is edge-sensitive 
  GPIO_PORTC_IBE_R &= ~0x10;    //     PC4 is not both edges 
  GPIO_PORTC_IEV_R &= ~0x10;    //     PC4 falling edge event 
  GPIO_PORTC_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTC_IM_R |= 0x10;      // (f) arm interrupt on PC4
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R |= 4;              // (h) enable interrupt 2 in NVIC

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
  //Timer2A enabled in GPIOPortC_Hander
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
	TIMER2_IMR_R &= ~0x1;						//disarm timout
	ADCvalue = ADC0_InSeq3();
	B0 = 1;
	GPIO_PORTC_IM_R  |= 0x10;			//arm B0;
}





void GPIOPortC_Handler(void){
	GPIO_PORTC_ICR_R |= 0x10;		//acknowledge flag
	GPIO_PORTC_IM_R  &= ~0x10;			//ignore interrupts for portb
	TIMER2_IMR_R = 1;							//arm timer 2
	TIMER2_CTL_R = 0x00000001;    //10) enable timer1A		
	}



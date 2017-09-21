// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"
#include "Timer2.h"
#include "Timer3.h"

#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int32_t adc_Time[1000];
int32_t adc_Value[1000];
int32_t adc_Index = 0;
int32_t max_Time_Period = 0;
int32_t min_Time_Period = 0xFFFFFFFF;
int32_t adc_Time_Jitter;
int32_t hist[4096];
int32_t y_Pos;
int32_t x_Pos;
int32_t radius_Spread = 100;

volatile uint32_t ADCvalue;
// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}

/*void timeJitter(void)
**input: none
**output: none
**calculates the difference of the greates and smallest 
**time difference between two adjacent sample points
*/
void timeJitter(void){
	for(int32_t i = 0; i < 999; i++){
		if(min_Time_Period>adc_Time[i+1]-adc_Time[i]){
			min_Time_Period = adc_Time[i+1]-adc_Time[i];
		}
		
		if(max_Time_Period<adc_Time[i+1]-adc_Time[i]){
			max_Time_Period = adc_Time[i+1]-adc_Time[i];
		}
	}
	adc_Time_Jitter = max_Time_Period-min_Time_Period;
}
/* void processADC(void)
**input: none
**output: none
**fills the histogram with the collected adc values
*/
void processADC(void){
	for(int32_t i = 0; i < 4096; i++){
		hist[adc_Value[i]]++;
	}
}


/*void calculateHist(void)
**input: none
**output: none
**finds the adc value that occured the most in sampling
**and its index in the histogram
*/
void calculateHist(void){
	int32_t maxIndex;
	int32_t max = 0;
	for(int32_t i = 1; i < 4096;i++){
		if(hist[i]>max){
			max=hist[i];
			maxIndex = i;
		}
	}
	x_Pos= maxIndex;
	y_Pos= max;
}

/*void displayHist(void)
**input: none
**output: none
**draws the histogram onto the lcd screen
*/
void displayHist(void){
	ST7735_PlotClear(0, 159);
	
	calculateHist();
	ST7735_XYplotInit("PMF", x_Pos-radius_Spread, x_Pos+radius_Spread, 0, y_Pos+10);

	ST7735_XYplot((radius_Spread*2)+1, x_Pos-radius_Spread, adc_Value, hist);
	
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
  ADCvalue = ADC0_InSeq3();
  PF2 ^= 0x04;                   // profile
	
	if(adc_Index <1000){
		adc_Value[adc_Index] = ADCvalue;
		adc_Time[adc_Index] = TIMER1_TAR_R;
		adc_Index= adc_Index+1;
	}else if (adc_Index == 1000){
		timeJitter();
		processADC();
		displayHist();
		adc_Index ++;
	}
}


int main(void){
  PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
  Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
  GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                      // turn off LED
	
	//0x02 = x4 sampling
	//0x04 =  x16 sampling
	//0x06 = x64 sampling
	//ADC0_SAC_R = 0x06;
	
	ST7735_InitR(INITR_REDTAB);		//Initialization for screen
	Timer1_Init(Timer0A_Handler,0);
	//Timer2_Init(9259);	
	//Timer3_Init(9259);	
	
  EnableInterrupts();
	
  while(1){
		//PF1 = (PF1*12345678)/1234567+0x02;  // this line causes jitter
    PF1 ^= 0x02;  // toggles when running in main
		//GPIO_PORTF_DATA_R ^= 0x02;  // toggles when running in main, and causes a critical section
	}
	
}




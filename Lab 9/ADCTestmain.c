// ADCTestmain.c
// Runs on LM4F120/TM4C123
// Provide a function that initializes Timer0A to trigger ADC
// SS3 conversions and request an interrupt when the conversion
// is complete.
// Daniel Valvano
// May 3, 2015

/*
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/14/17
 */

/*
	Ways to initialize converison
	ADC0_EMUX_R:
	0x0		Software start
	0x1		Analog Comparator 0
	0x2		Analog Comparator 1
	0x3		Analog Comparator 2		
	0x4		External(GPIO PB4)
	0x5		Timer
	0x6		PWM0
	0x7		PWM1
	0x8		PWM2
	0xF		ALways(continuously sample)
 */
 
/*
When do you know adc conversion is complete:
Bit 3 in ADC0_RIS_R is set and triggers and interrupt
 */

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
// top of X-ohm potentiometer connected to +3.3V through X/10-ohm ohm resistor
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADCT0ATrigger.h"
#include "PLL.h"
#include "UART.h"
#include "FIFO.h"
#include "Visual.h"
#include "ST7735.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//debug code
//
// This program periodically samples ADC0 channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.

extern uint32_t ADCvalue;
extern uint8_t flag;

uint32_t values[100];
uint8_t counter = 0;
txDataType data;
uint8_t temperature[2];
int main(void){
  PLL_Init(Bus80MHz);                      // 80 MHz system clock
	
	UART_Init();
	TxFifo_Init();
	ST7735_InitR(INITR_REDTAB);
	
  SYSCTL_RCGCGPIO_R |= 0x00000020;         // activate port F
  ADC0_InitTimer0ATriggerSeq3(0, 80000); // ADC channel 0, 1000 Hz sampling
  GPIO_PORTF_DIR_R |= 0x04;                // make PF2 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;             // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;                // enable digital I/O on PF2
                                           // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;                  // disable analog functionality on PF
  GPIO_PORTF_DATA_R &= ~0x04;              // turn off LED
  EnableInterrupts();
	
	while(1){
		
		if(TxFifo_Size()>0){
			TxFifo_Get((&data));
			temperature[0] = fixedPoint(data);
			temperature[1] = fixedPointDecimal(data);
			updateScreen(temperature[0],temperature[1],data);
			
		}
	}
	
	
  while(1){
    //WaitForInterrupt();
    //GPIO_PORTF_DATA_R ^= 0x04;             // toggle LED
		if(flag==1){
			flag =0;
			values[counter]=ADCvalue;
			counter++;
		}
		if(counter ==100){
			break;
		}
	}
	
	for(uint8_t i = 0; i <counter; i++){
		UART_OutString("\n\rADC data =");
    UART_OutUDec(values[i]);
	}
	
}


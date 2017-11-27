// PWMtest.c
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 6.8, section 6.3.2

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2015
   Program 8.4, Section 8.3

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
#include <stdint.h>
 #include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "PWM.h"
#include "SwitchControl.h"
#include "Motor.h"
#include "Tacho.h"
#include "ST7735.h"
#include "Visual.h"
uint32_t displaySpeed;
uint32_t fs = 100;
uint32_t N = 16;
uint32_t DesiredRPS;
uint32_t ActualRPS;


extern uint8_t button;				//check if works properly
extern uint32_t desiredSpeed;
extern uint32_t Speed;

void WaitForInterrupt(void);  // low power mode
void DisableInterrupts(void);
void EnableInterrupts(void);

int main(void){
	uint32_t j =0;
	DisableInterrupts();
  PLL_Init(Bus80MHz);               // bus clock at 80 MHz
	ST7735_InitR(INITR_REDTAB);
	//sweeping graph init
	//plotRPSInit();
	PeriodMeasure_Init();
  PWM0B_Init(40000, 20000);         // initialize PWM0, 1000 Hz, 50% duty

	
	Timer2A_Init(800000);
	Timer1A_Init();
	PortE_Init();
	EnableInterrupts();
	
//  PWM0B_Init(40000, 10000);         // initialize PWM0, 1000 Hz, 25% duty
//  PWM0_Duty(4000);    // 10%
//  PWM0_Duty(10000);   // 25%
//  PWM0_Duty(30000);   // 75%

//  PWM0_Init(4000, 2000);         // initialize PWM0, 10000 Hz, 50% duty
//  PWM0_Init(1000, 900);          // initialize PWM0, 40000 Hz, 90% duty
//  PWM0_Init(1000, 100);          // initialize PWM0, 40000 Hz, 10% duty
//  PWM0_Init(40, 20);             // initialize PWM0, 1 MHz, 50% duty
		



  while(1){
		//button pd2 makes motor go faster
		
		if(button==0x04){
			Speed_Up();
		//button pd1 makes motor go slower	
		}else if(button==0x02){
			Speed_Down();
		}
		button = 0;
		//sweeping graph
		
		plotRPS(j,N,fs, displaySpeed, desiredSpeed, Speed);
		j++;
	}
}

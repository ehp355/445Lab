/*PWMtest.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/29/17
 *the "main.c" of PWM project
 */

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

//variables used for sweeping graph visual provided by valvano
uint32_t fs = 100;
uint32_t N = 16;

//global variables that are updated by 
//various handlers from Tacho.c
extern uint8_t button;
extern uint32_t desiredSpeed;
extern uint32_t Speed;
extern uint32_t Period;

void WaitForInterrupt(void);  // low power mode
void DisableInterrupts(void);
void EnableInterrupts(void);

int main(void){
	uint32_t j =0;
	DisableInterrupts();
	
  PLL_Init(Bus80MHz);               // bus clock at 80 MHz
	ST7735_InitR(INITR_REDTAB);
	//sweeping graph init
  plotRPSInit();
  TimerCapture_Init();
	PWM0B_Init(40000, 20000);         // initialize PWM0, 1000 Hz, 50% duty
	Timer2A_Init(800000);
	Timer1A_Init();
	
	//PortE_Init();
	EnableInterrupts();

  while(1){
		//button PE3 makes motor go faster
		if(button==0x8){
			Speed_Up();
		//button PE2 makes motor go slower	
		}else if(button==0x4){
			Speed_Down();
		}
		button = 0;
		//sweeping graph
		plotRPS(j,N,fs, desiredSpeed, Speed, Period);
		j++;
	}
}

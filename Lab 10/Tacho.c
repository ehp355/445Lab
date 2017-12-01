/*Tacho.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/29/17
 */
 #include <stdint.h>
 #include "../inc/tm4c123gh6pm.h"
 #include "PWM.h"
 
#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable
#define PF2                     (*((volatile uint32_t *)0x40025010))
#define PF4											(*((volatile uint32_t *)0x40025040))
#define TIMER_TAMR_TACMR        0x00000004  // GPTM TimerA Capture Mode
#define TIMER_TAMR_TAMR_CAP     0x00000003  // Capture mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_CTL_TAEVENT_POS   0x00000000  // Positive edge
#define TIMER_CTL_TAEVENT_NEG   0x00000004  // Negative edge
#define TIMER_CTL_TAEVENT_BOTH  0x0000000C  // Both edges
#define TIMER_IMR_CAEIM         0x00000004  // GPTM CaptureA Event Interrupt
                                            // Mask
#define TIMER_ICR_CAECINT       0x00000004  // GPTM CaptureA Event Interrupt
                                            // Clear
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load

void (*PeriodicTask)(void);   // user function
 
uint32_t PW;					//hex value of pulse width

uint32_t desiredSpeed;
uint32_t Speed;
uint32_t Period;
//error used for slowly approach desired speed
int32_t E; 
//period passed through PWM0B_Duty() to slowly change
//duty cyle
int32_t U;


//array used to capture the first 5 errors
//for report purposes
uint32_t errArr[5];
uint32_t errArrIndex;

uint32_t First;               // Timer0A first edge
int32_t Done;                 // set each rising
// max period is (2^24-1)*12.5ns = 209.7151ms
// min period determined by time to run ISR, which is about 1us

uint8_t flag;

// InputCapture.c
// Runs on LM4F120/TM4C123
// Use Timer0A in edge time mode to request interrupts on the rising
// edge of PB0 (CCP0), and count the pulses.
// Daniel Valvano
// May 3, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Example 6.1, Program 6.1

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

// external signal connected to PB6 (T0CCP0) (trigger on rising edge)
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable

#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TACMR        0x00000004  // GPTM TimerA Capture Mode
#define TIMER_TAMR_TAMR_CAP     0x00000003  // Capture mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_CTL_TAEVENT_POS   0x00000000  // Positive edge
#define TIMER_IMR_CAEIM         0x00000004  // GPTM CaptureA Event Interrupt
                                            // Mask
#define TIMER_ICR_CAECINT       0x00000004  // GPTM CaptureA Event Interrupt
                                            // Clear
#define TIMER_TAILR_M           0xFFFFFFFF  // GPTM Timer A Interval Load
                                            // Register
#define PE0		(*((volatile uint32_t *)0x40024004))
#define PE1		(*((volatile uint32_t *)0x40024008))
#define PE4		(*((volatile uint32_t *)0x40024040))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function


uint32_t PeriodFlag;

void TimerCapture_Init(void){long sr;
	flag = 0;
	Period = 4000000;
	desiredSpeed = 800000000/4000000;
	errArrIndex=0;
	U = 20000;
	First = 0;                       // first will be wrong
  Done = 0;                        // set on subsequent
  sr = StartCritical();
  SYSCTL_RCGCTIMER_R |= 0x08;// activate timer3
  SYSCTL_RCGCGPIO_R |= 0x02; // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
  GPIO_PORTB_DIR_R &= ~0x4;       // make PB2 in
  GPIO_PORTB_AFSEL_R |= 0x4;      // enable alt funct on PB2
  GPIO_PORTB_DEN_R |= 0x4;        // enable digital I/O on PB2
                                   // configure PB2 as T3CCP0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFF0FF)+0x00000700;
  GPIO_PORTB_AMSEL_R &= ~0x4;     // disable analog functionality on PB6
  TIMER3_CTL_R &= ~0x00000001; // disable timer0A during setup
  TIMER3_CFG_R = 0x00000004; // configure for 16-bit timer mode
                                   // configure for capture mode, default down-count settings
  TIMER3_TAMR_R = 0x00000007;		// configure for rising edge event
  TIMER3_CTL_R &= ~(0xC);
  TIMER3_TAILR_R = 0x0000FFFF;  // max start value
	TIMER3_TAPR_R = 0xFF;
  TIMER3_IMR_R |= 0x00000004; // enable capture match interrupt
  TIMER3_ICR_R = 0x00000004;// clear timer0A capture match flag
  TIMER3_CTL_R |= 0x00000001;  // enable timer0A 16-b, +edge timing, interrupts
                                   // Timer0A=priority 2
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x00000000; // top 3 bits, priority = 2
  NVIC_EN1_R = 0x8;     // enable interrupt 35 in NVIC
  EndCritical(sr);
}

void Timer3A_Handler(void){
	//PE0 ^= 0x1;
	//PE0 ^= 0x1;
	
	TIMER3_ICR_R = 0x00000004;// acknowledge timer0A capture match
	PeriodFlag=(First - TIMER3_TAR_R)&0x00FFFFFF;
	Period = (First - TIMER3_TAR_R)&0x00FFFFFF;// 24 bits, 12.5ns resolution
	First = TIMER3_TAR_R;            // setup for next
	Done = 1;	
	//PE0
	//PE0 ^=0x01;
}


//periodic timer: handler calls speed control
void Timer2A_Init(unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2

  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // 8) priority 1
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

//helper function to capture first 5 errors of desiredSpeed and Speed
//and averages them
void debugErrArr(uint32_t E){
	if(errArrIndex<5){
		errArr[errArrIndex]=E;
		errArrIndex++;
	}else{
		uint32_t errAvg;
		for(uint8_t i = 0; i < 5; i++){
			errAvg += errArr[i];
		}
		errAvg = errAvg/5;
	}
}

//periodically called by Timer2AHanlder to gradually
//change Speed to desiredSpeed
void SpeedControl(void){
	Speed = 200000000/Period;
	E = desiredSpeed-Speed;
	//debugErrArr(E);							//
	U = U+(3*E)/48;								//the values 3 and 48 can be changed to makethe approach to desiredSpeed more/less gradual
	if(U<10000) U=10000;					
	if(U>40000) U=40000;					//max duty input is 40000 due to the PWM0B initialization values in main

  PWM0B_Duty(U);
}

//peridic handler
void Timer2A_Handler(void){
	//PE1
	//PE1 ^= 0x02;
	//PE1 ^= 0x02;
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
	SpeedControl();// execute user task
	
	//PE1
	//PE1 ^= 0x02;
}

void Speed_Up(void){
	if(desiredSpeed >350){
		desiredSpeed = 400;
	}else{
		desiredSpeed = desiredSpeed+50;
	}
}

void Speed_Down(void){
	if(desiredSpeed < 50){
		desiredSpeed = 0;
	}else{
		desiredSpeed = desiredSpeed - 50;
	}
}

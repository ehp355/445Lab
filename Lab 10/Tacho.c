/*Tacho.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
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
uint32_t E; 
int32_t U;

uint32_t errArr[5];
uint32_t errArrIndex;

uint32_t First;               // Timer0A first edge
int32_t Done;                 // set each rising
// max period is (2^24-1)*12.5ns = 209.7151ms
// min period determined by time to run ISR, which is about 1us



//Need to switch from Timer0 to timer3
//if switch does not work
//do a manual rising edge trigger a global timer
void PeriodMeasure_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x08;// activate timer3    
  SYSCTL_RCGCGPIO_R |= 0x22;       // activate port B and port F
                                   // allow time to finish activating
  First = 0;                       // first will be wrong
  Done = 0;                        // set on subsequent
  GPIO_PORTB_DIR_R &= ~0x4;       // make PB2 in
  GPIO_PORTB_AFSEL_R |= 0x4;      // enable alt funct on PB2/T3CCP0
  GPIO_PORTB_DEN_R |= 0x4;        // enable digital I/O on PB2
                                   // configure PB2 as T3CCP0
  GPIO_PORTB_PCTL_R |= (GPIO_PORTB_PCTL_R&0xFFFFF0FF)|0x00000700;
  GPIO_PORTB_AMSEL_R &= ~0x4;     // disable analog functionality on PB6
  GPIO_PORTF_DIR_R |= 0x14;        // make PF2&PF4 out (PF2 built-in blue LED)
  
	GPIO_PORTF_AFSEL_R &= ~0x14;     // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x14;        // enable digital I/O on PF2
                                   // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFF0F0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  TIMER3_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER3_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
                                   // configure for 24-bit capture mode
  TIMER3_TAMR_R = (TIMER_TAMR_TACMR|TIMER_TAMR_TAMR_CAP);
                                   // configure for rising edge event
  TIMER3_CTL_R &= ~(TIMER_CTL_TAEVENT_POS|0xC);
  TIMER3_TAILR_R = TIMER_TAILR_TAILRL_M;// start value
  TIMER3_TAPR_R = 0xFF;            // activate prescale, creating 24-bit
  TIMER3_IMR_R |= TIMER_IMR_CAEIM; // enable capture match interrupt
  TIMER3_ICR_R = TIMER_ICR_CAECINT;// clear timer3A capture match flag
  TIMER3_CTL_R |= TIMER_CTL_TAEN;  // enable timer3A 16-b, +edge timing, interrupts
                                   // Timer0A=priority 2
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN1_R = 0x23;     // enable interrupt 19 in NVIC
}
void Timer0A_Handler(void){
  PF2 = PF2^0x04;  // toggle PF2
  PF2 = PF2^0x04;  // toggle PF2
  TIMER0_ICR_R = TIMER_ICR_CAECINT;// acknowledge timer0A capture match
  Period = (First - TIMER0_TAR_R)&0xFFFFFF;// 24 bits, 12.5ns resolution
  First = TIMER0_TAR_R;            // setup for next
  Done = 1;
  PF2 = PF2^0x04;  // toggle PF2
}


//periodic timer
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



void SpeedControl(void){
	Speed = 800000000/Period;
	E = desiredSpeed-Speed;				//change 250 to desired speed(250= 25 rps)
	debugErrArr(E);
	U = U+(3*E)/64;								//may have to change 64 to something more or less depending on how quickly motor speeds up/slowsdownd
	if(U<0) U=0;
	if(U>4000) U=4000;
	PWM0B_Duty(U);
}


void Timer2A_Handler(void){
	PF4 = PF4^0x10;  // toggle PF4
  PF4 = PF4^0x10;  // toggle PF4
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
	SpeedControl();// execute user task
	PF4 = PF4^0x10;
}


void Speed_Up(void){
	Speed = 800000000/Period;
	if(Speed >350){
		desiredSpeed = 400;
	}else{
		desiredSpeed = Speed+50;
	}
}

void Speed_Down(void){
	Speed = 800000000/Period;
	if(Speed < 50){
		desiredSpeed = 0;
	}else{
		desiredSpeed = Speed - 50;
	}
}

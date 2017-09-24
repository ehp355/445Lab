#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "SysTick.h"
#include "AlarmClockVisual.h"
#include "AlarmClockAudio.h"
#include "AlarmClockSwitchControl.h"
//#include "Timer2.h"
#include "Timer2A.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


/*void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
	
	
}*/

int main(){
	ST7735_InitR(INITR_REDTAB);		//Initialization for screen
	
	SysTick_Init();

	
	startClock();
	
}


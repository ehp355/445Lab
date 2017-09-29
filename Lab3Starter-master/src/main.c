#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "SysTick.h"
#include "AlarmClockVisual.h"
#include "AlarmClockAudio.h"
#include "AlarmClockSwitchControl.h"
#include "RisingEdgeInterrupt.h"
#include "Timer0A.h"



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


int main(){
	PLL_Init(Bus80MHz);

	ST7735_InitR(INITR_REDTAB);		//Initialization for screen
	
	SysTick_Init();
	
	Edge_Init();
	
	Timer0A_Init();
	
	Timer1A_Init();
	
	PortC_Init();
	
	startClock();
	
}


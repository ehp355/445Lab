#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "SysTick.h"
#include "AlarmClockVisual.h"
#include "AlarmClockAudio.h"
#include "AlarmClockSwitchControl.h"

int32_t current_Hour;
int32_t current_Min;
uint32_t current_Time=0;
uint32_t alarm_Time=0;

int32_t Button_1 = 0;
int32_t Button_2 = 0;
int32_t Button_3 = 0;

//0 = alarm off, 1 = alarm on
int32_t alarm_Enabled = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void runClockFace(void);
void runDigitalClock(void);
void checkAlarm(void);

/*void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
	
	
}*/

int main(){
	ST7735_InitR(INITR_REDTAB);		//Initialization for screen
	
	SysTick_Init();
	
	//Setup Screen
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetTextColor(ST7735_CYAN);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Juliana&Enrique");
	ST7735_SetCursor(0, 1);
	ST7735_OutString("Lab3");
	ST7735_SetCursor(0, 2);

	paintClockFace();
	runClockFace();
	
}

void runClockFace(void){
	while(Button_1 != 1 || Button_2 != 1 || Button_3 != 1){
		
		SysTick_Wait10ms(1);	//wait 10ms
		current_Time++;
		
		//occurs every minute
		if(current_Time%60000==0){
			paintClockFace();
			drawClockHand(current_Time/360000, current_Time/6000);
			
			if(alarm_Enabled == 1){
				checkAlarm();
			}
		
		}
		
	}
	//user is switching to Digital display
	if(Button_1 == 1){
		Button_1 = 0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(current_Time/360000, current_Time/6000);
		runDigitalClock();
	
	//user is enabling/disbaling alarm	
	}else if(Button_2 ==1){
		Button_2 = 0;
		alarm_Enabled ^= 1;
		runClockFace();
	//user wants to set alarm
	}else if(Button_3 ==1){
		Button_3 =0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		alarm_Time = setAlarm(alarm_Time);
	}
}

void runDigitalClock(void){
	while(Button_1 != 1 || Button_2 != 1 || Button_3 != 1){
		SysTick_Wait10ms(1);
		current_Time++;
		
		//occurs every minute
		if(current_Time%60000==0){
			ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
			paintDigitalClock(current_Time/360000, current_Time/60000);
			
			if(alarm_Enabled == 1){
				checkAlarm();
			}
		}
		
	}
	
	//user is switching to Clock display
	if(Button_1 == 1){
		Button_1 = 0;
		paintClockFace();
		drawClockHand(current_Time/360000, current_Time/6000);
		runClockFace();
	
	//user is enabling/disabling alarm
	}else if(Button_2 ==1){
		Button_2 = 0;
		alarm_Enabled ^= 1;
		runDigitalClock();
	}else if(Button_3 ==1){
		Button_3=0;
	
	}
	
}

void checkAlarm(void){
	uint32_t min = alarm_Time%60;
	uint32_t hour = alarm_Time/60;
	
	if(min == current_Time/60000 && hour == current_Time/360000){
		soundAlarm();
	}
}


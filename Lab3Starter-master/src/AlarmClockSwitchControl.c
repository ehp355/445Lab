/*AlarmClockSwitchControl.c
**Enrique Perez-Osborne
**Juliana Pulido
**Created: 9/20/17
**Audio interface for Alarm Clock
**Lab3
**TA: Cody Horton
**Last Modified: 9/20/17
*/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "SysTick.h"
#include "AlarmClockVisual.h"
#include "AlarmClockAudio.h"
int32_t current_Hour;
int32_t current_Min;
//keeping track of time in seconds

uint32_t alarm_Time=0;
uint32_t time=0;

int32_t Button_1 = 0;
int32_t Button_2 = 0;
int32_t Button_3 = 0;
int32_t Button_4 = 0;

//0 = alarm off, 1 = alarm on
int32_t alarm_Enabled = 0;

void runClockFace(void);
void runDigitalClock(void);
void checkAlarm(void);
void settingTime(void);

uint32_t setAlarm(uint32_t currentAlarm){
	return currentAlarm;
}

//Clock Face State
void runClockFace(void){
	while(Button_1 != 1 || Button_2 != 1 || Button_3 != 1 || Button_4 != 1){
		
		
		//occurs every minute
		if(getTime()%60==0){
			//paintClockFace();
			drawClockHand(getTime()/3600, getTime()/60);
			//if alarm is enable, checks to see if the current time matches alarm time
			if(alarm_Enabled == 1){
				checkAlarm();
			}
		
		}
		
	}
	//user is switching to Digital display
	if(Button_1 == 1){
		Button_1 = 0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60);
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
	//user wants to set time
	}else if(Button_4 == 1){
		Button_4 =0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60);
		settingTime();
		
	}
	
}

//Digital Display state
void runDigitalClock(void){
	while(Button_1 != 1 || Button_2 != 1 || Button_3 != 1 || Button_4 != 1){
		
		//occurs every minute
		if(getTime()%60==0){
			ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
			paintDigitalClock(getTime()/3600, getTime()/60);
			//if alarm is enable, checks to see if the current time matches alarm time
			if(alarm_Enabled == 1){
				checkAlarm();
			}
		}
		
	}
	
	//user is switching to Clock display
	if(Button_1 == 1){
		Button_1 = 0;
		paintClockFace();
		drawClockHand(getTime()/3600, getTime()/60);
		runClockFace();
	
	//user is enabling/disabling alarm
	}else if(Button_2 ==1){
		Button_2 = 0;
		alarm_Enabled ^= 1;
		runDigitalClock();
		
	//user wants to set alarm
	}else if(Button_3 ==1){
		Button_3=0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		alarm_Time = setAlarm(alarm_Time);
	
	//user wants to set time
	}else if(Button_4 == 1){
		Button_4 = 0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60);
		settingTime();
	}
	
}

//set Time state
void settingTime(void){
	int32_t hour = 0;
	int32_t min = 0;
	
	// 0 = setting hour, 1 = setting min
	int32_t placeHolder = 0;
	while(Button_1 != 1 || Button_2 != 1 || Button_3 != 1 || Button_4 != 1){
		//consider flashing hour or min to let user know
		//which they are currently set to change
	}

	//user wants to increase hour or minute
	if(Button_1 == 1){
		Button_1 = 0;
		switch(placeHolder){
			case 0:
				if(hour == 11){
					hour = 0;
				}else{
					hour++;
				}
				break;
				
			case 1:
				if(min == 59){
					min = 0;
				}else{
					min++;
				}
		}
		//updates digital display for user
		setDigitalClock(hour,min);
		
	//user wants to decrease hour or minute	
	}else if(Button_2 == 1){
		Button_2 = 0;
		switch(placeHolder){
			case 0:
				if(hour == 0){
					hour = 11;
				}else{
					hour--;
				}
				break;
				
			case 1:
				if(min == 0){
					min = 59;
				}else{
					min--;
				}
		}
		//updates digital display for user
		setDigitalClock(hour,min);
		
	//user is switching between hour or minute
	}else if(Button_3 == 1){
		Button_3 = 0;
		switch(placeHolder){
			case 0:
				placeHolder =1;
				break;
				
			case 1:
				placeHolder =0;
				break;
		}
		//updates digital display for user
		setDigitalClock(hour,min);
	
	//user has set clock and wants to go back to clock
	}else if(Button_4 == 1){
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		setTime((3600*hour)+(min*60));
		paintClockFace();
		runClockFace();
	}
	
}

void checkAlarm(void){
	uint32_t min = alarm_Time%60;
	uint32_t hour = alarm_Time/60;
	
	if(min == getTime()/60 && hour == getTime()/3600){
		soundAlarm();
	}
}


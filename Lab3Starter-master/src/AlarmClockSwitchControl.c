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
uint32_t set_Time = 0;
uint32_t time=0;
int32_t placeHolder = 0;

extern int8_t B1;
extern int8_t B2;
extern int8_t B3;
extern int8_t B4;

int8_t for_Alarm;

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
//This function runs the clock in analog mode
void runClockFace(void){
	if(alarm_Enabled == 1){
		alarmEnabled(alarm_Time/60, alarm_Time%60);
	}
	
	while(B1 != 1 && B2 != 1 && B3 != 1 && B4 != 1){
			
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
	if(B1 == 1){
		B1 = 0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60,ST7735_BLUE,ST7735_BLUE);
		runDigitalClock();
	
	//user is enabling/disbaling alarm	
	}else if(B2 ==1){
		B2 = 0;
		alarm_Enabled ^= 1;
		if(alarm_Enabled == 1){
			alarmEnabled(alarm_Time/60, alarm_Time%60);
		}
		else{
			ST7735_FillRect(0, 130, 127, 29, ST7735_BLACK);
		}
		runClockFace();
		
	//user wants to set alarm
		}else if(B3 ==1){
			B3 =0;
			ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
			for_Alarm = 1;
			settingTime();
	
	//user wants to set time
	}else if(B4 == 1){
		B4 =0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60, ST7735_BLUE,ST7735_BLUE);
		for_Alarm = 0;
		settingTime();
		
	}
	
}

//Digital Display state
void runDigitalClock(void){
	//alarm_Enabled =1;	//TEST
	//alarm_Time =1;		//TESTNG
	if(alarm_Enabled == 1){
		alarmEnabled(alarm_Time/60, alarm_Time%60);
		}
	while(B1 != 1 && B2 != 1 && B3 != 1 && B4 != 1){
		
		//occurs every minute
		if(getTime()%60==0){
	
			//ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
			paintDigitalClock(getTime()/3600, getTime()/60, ST7735_BLUE,ST7735_BLUE);
			//if alarm is enable, checks to see if the current time matches alarm time
			if(alarm_Enabled == 1){
				
				checkAlarm();
			}
		
		}
		
	}
	
	//user is switching to Clock display
	if(B1 == 1){
		B1 = 0;
		paintClockFace();
		drawClockHand(getTime()/3600, getTime()/60);
		runClockFace();
	
	//user is enabling/disabling alarm
}else if(B2 ==1){
		B2 = 0;
		alarm_Enabled ^= 1;
		if(alarm_Enabled == 1){
			alarmEnabled(alarm_Time/60, alarm_Time%60);
		}
		else{
			ST7735_FillRect(0, 130, 127, 29, ST7735_BLACK);
		}
		runDigitalClock();
		
	//user wants to set alarm
	}else if(B3 ==1){
		B3 = 0;		
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		for_Alarm = 1;
		settingTime();
	
	//user wants to set time
	}else if(B4 ==1){
		B4 = 0;
		ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		paintDigitalClock(getTime()/3600, getTime()/60, ST7735_BLUE,ST7735_BLUE);
		for_Alarm = 0;
		settingTime();
	}
	
}

//set Time state
void settingTime(void){
	
	// 0 = setting hour, 1 = setting min
//	int32_t placeHolder = 0;
	while(B4 != 1){
			while(B1 != 1 && B2 != 1 && B3 != 1 &&  B4 != 1){
		//consider flashing hour or min to let user know
		//which they are currently set to change
				if(placeHolder==0){
					paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_WHITE,ST7735_BLUE);
				}else{
					paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_BLUE,ST7735_WHITE);
				}
			}

	//user wants to increase hour or minute
			if(B1 == 1){
			B1 = 0;
			switch(placeHolder){
				case 0:
					set_Time = set_Time+60;
					paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_WHITE, ST7735_BLUE);
				break;
				
				case 1:
					set_Time = set_Time+1;
					paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_BLUE, ST7735_WHITE);
			}
			//settingTime();
	//user wants to decrease hour or minute	
			}else if(B3 ==1){
				B3 = 0;
				switch(placeHolder){
					case 0:
						if(set_Time<=59){
							set_Time = set_Time+ 660;
						}else{
							set_Time = set_Time - 60;
						}
						paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_WHITE, ST7735_BLUE);
						break;
				
					case 1:
						if(set_Time == 0){
							set_Time = set_Time + 59;
						}else{
							set_Time--;
						}
						paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_BLUE, ST7735_WHITE);
				}
	
				//settingTime();
	//user is switching between hour or minute
			}else if(B2 == 1){
				B2 = 0;
				switch(placeHolder){
					case 0:
						placeHolder =1;
						paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_WHITE, ST7735_BLUE);
					break;
				
					case 1:
						placeHolder =0;
						paintDigitalClock((set_Time/60),(set_Time % 60),ST7735_BLUE, ST7735_WHITE);
					break;
				}
				//settingTime();
	//user has set clock and wants to go back to clock 
//	}else if(Button_4 == 1){
			}else if(B4 == 1){
				B4 = 0;
				ST7735_FillRect(0, 20, 127, 159, ST7735_BLACK);
		//set_Time =0;
				if(for_Alarm ==0){
					setTime((set_Time*60));
					set_Time =0;
					paintClockFace();
					runClockFace();
				}else{
					alarm_Time = set_Time;
					set_Time = 0;
					paintClockFace();
					runClockFace();
				}
		}
	}
	
	
}

void checkAlarm(void){
	uint32_t min = alarm_Time%60;
	uint32_t hour = alarm_Time/60;
	
	if(min == getTime()/60 && hour == getTime()/3600){
		soundAlarm();
	}
}


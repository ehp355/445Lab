/*AlarmClockVisual.c
**Enrique Perez-Osborne
**Juliana Pulido
**Created: 9/20/17
**LCD interface for Alarm Clock
**Lab3
**TA: Cody Horton
**Last Modified: 9/20/17
*/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "AlarmClockSwitchControl.h"


void paintClockFace(void){
	
	ST7735_FillRect(0, 20, 127, 159, ST7735_WHITE);
	//To Draw image of clock with no arms
	//ST7735_DrawBitmap(0, 159, const uint16_t *image, 127, 139);
	
}

void drawClockHand(uint32_t hour, uint32_t min){
	min = min % 60;
	hour = hour % 12;
	
	switch(min){}
	switch(hour){}
	
}

void paintDigitalClock(uint32_t hour, uint32_t min){
	min = min% 60;
	hour = hour % 12;
	
	switch(min){}
	switch(hour){}
}

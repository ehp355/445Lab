/*Logger.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 12/3/17
 *TA: Cody Horton
 *Last Edited: 12/3/17
*/

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Visual.h"
#include "ST7735.h"


#define	MAX			100
uint8_t logger[MAX];
int8_t logIndex;
int8_t topIndex;
uint8_t blinkTopToggle;
uint8_t blinkBotToggle;
uint8_t topBound;

void blinkTopScroller(void);
void blinkBottomScroller(void);

void Log_Init(void){
	logIndex =0;
	topIndex =0;
	blinkTopToggle =0;
	blinkBotToggle=0;
}

void logToArray(uint16_t value){
	logger[logIndex] = value;
	logIndex++;
}

//diplay Log state to LCD
void displayLog(void){
	uint8_t x_pos=2;
	uint8_t y_pos=5;

	
	if(logIndex<10){
		topBound = logIndex;
	}else{
		topBound = topIndex+10;
	}
	
	 
	for(uint8_t i =topIndex; i< topBound;i++){
		char logCharacters[3] ={logger[i]/100+'0',((logger[i]%100)/10)+'0',logger[i]%10+'0'};
		ST7735_DrawString(x_pos,y_pos,logCharacters,ST7735_WHITE);
		y_pos++;
	}
	
}


void resetTopIndex(void){
	topIndex=0;
}

void scrollUp(void){
	if(topIndex!=0){
		topIndex--;
		displayLog();
	}
}

void scrollDown(void){
	if(topIndex<logIndex-10){
		topIndex++;
		displayLog();
	}
}

void blinkTopScroller(void){
	uint8_t x_pos=0;
	uint8_t y_pos=50;
	if(topIndex >0){
		if(blinkTopToggle){
			ST7735_DrawChar(x_pos,y_pos,'^',ST7735_RED,ST7735_BLACK,1);
			blinkTopToggle=0;
		}else{
			ST7735_DrawChar(x_pos,y_pos,'^',ST7735_BLACK,ST7735_BLACK,1);
			blinkTopToggle=1;
		}
		Delay1ms(1000);
	}else{
		ST7735_DrawChar(x_pos,y_pos,'^',ST7735_BLACK,ST7735_BLACK,1);
	}
}

void blinkBottomScroller(void){
	uint8_t x_pos=0;
	uint8_t y_pos=140;
	//need to find asci value for down arrow
	if(logIndex>topBound){
		if(blinkBotToggle){
			ST7735_DrawChar(x_pos,y_pos,'V',ST7735_RED,ST7735_BLACK,1);
			ST7735_FillRect(0,139,5,5,ST7735_BLACK);
			blinkBotToggle=0;
		}else{
			ST7735_DrawChar(x_pos,y_pos,'V',ST7735_BLACK,ST7735_BLACK,1);
			blinkBotToggle=1;
		}
		Delay1ms(1000);
	}else{
		ST7735_DrawChar(x_pos,y_pos,'V',ST7735_BLACK,ST7735_BLACK,1);
	}
}

void killScrollers(void){
	uint8_t x_pos=0;
	uint8_t y_pos=140;
	
	ST7735_DrawChar(x_pos,y_pos,'V',ST7735_BLACK,ST7735_BLACK,1);
	
	x_pos=0;
	y_pos=45;
	
	ST7735_DrawChar(x_pos,y_pos,'^',ST7735_BLACK,ST7735_BLACK,1);
}

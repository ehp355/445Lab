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

void Log_Init(void){
	logIndex =0;
	topIndex =0;
}

void logToArray(uint16_t value){
	logger[logIndex] = value;
	logIndex++;
}

//diplay Log state to LCD
void displayLog(void){
	uint8_t x_pos=0;
	uint8_t y_pos=5;
	uint8_t topBound;
	
	if(logIndex<10){
		topBound = logIndex;
	}else{
		topBound = topIndex+10;
	}
	
	 
	for(uint8_t i =topIndex; i< topBound;i++){
		char logCharacters[3] ={logger[i]/100+'0',((logger[i]%100)/10)+'0',logger[i]%10+'0'};
		ST7735_DrawString(x_pos,y_pos,logCharacters,ST7735_CYAN);
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

/*Visual.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
 */
#include <stdint.h>
#include "ST7735.h"

void displayWord(){
	ST7735_DrawString(40, 90, "Hello World?", ST7735_BLUE);
}

//visual module for demo
void display2DigitNumber(uint32_t num){
	char zero = '0';
	char Char1= ((char)(num%10))+zero;
	char Char2= ((char)(num/10))+zero;
	ST7735_DrawCharS(40, 80, Char2, ST7735_BLUE, ST7735_BLACK, 3);
	ST7735_DrawCharS(50, 80, Char1, ST7735_BLUE, ST7735_BLACK, 3);
}

 //displays heartbeat state to LCD
 void displayHB(uint32_t heartBeat){
	 char hbString[3];
	 hbString[0]=heartBeat/100;
	 heartBeat = heartBeat%100;
	 hbString[1] = heartBeat/10;
	 hbString[2] = heartBeat%10;

	 ST7735_DrawString(40, 90, hbString, ST7735_BLUE);
 }
 
 //displays Alert state to LCD
 void displayAlert(void){
 
 }
 
 //diplay Log state to LCD
 void displayLog(void){
 
 }
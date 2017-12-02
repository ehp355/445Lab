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
	ST7735_DrawCharS( 0, 0, 'a', ST7735_BLUE, ST7735_BLACK, 3); 
}

//visual module for demo
void display2DigitNumber(uint32_t num){
	char zero = '0';
	char Char1= ((char)(num%10))+zero;
	char Char2= ((char)(num/10))+zero;
	ST7735_DrawCharS(40, 80, Char2, ST7735_BLUE, ST7735_BLACK, 3);
	ST7735_DrawCharS(60, 80, Char1, ST7735_BLUE, ST7735_BLACK, 3);
}

void erase2DigitNumber(uint32_t num){
	char zero = '0';
	char Char1= ((char)(num%10))+zero;
	char Char2= ((char)(num/10))+zero;
	ST7735_DrawCharS(40, 80, Char2, ST7735_BLACK, ST7735_BLACK, 3);
	ST7735_DrawCharS(60, 80, Char1, ST7735_BLACK, ST7735_BLACK, 3);
}

 //displays heartbeat state to LCD
 void displayHB(uint32_t heartBeat){
	 char hbString[3];
	 hbString[0]=heartBeat/100;
	 heartBeat = heartBeat%100;
	 hbString[1] = heartBeat/10;
	 hbString[2] = heartBeat%10;

	 ST7735_DrawString(5, 5, hbString, ST7735_BLUE);
 }
 
 //displays Alert state to LCD
 void displayAlert(void){
 
 }
 
 //diplay Log state to LCD
 void displayLog(void){
 
 }
 
 
 //dummy functions to test software modules
 void displayStateOne(void){
	 ST7735_DrawString(0, 0, "State One", ST7735_BLUE);
 }
 
 
 void displayStateTwo(void){
	 ST7735_DrawString(0, 0, "State Two", ST7735_RED);
 }
 
 void displayStateThree(void){
	 ST7735_DrawString(0, 0, "State Three", ST7735_GREEN);
 }
 
 void displayStart(void){
	 ST7735_DrawString(0,0, "START", ST7735_YELLOW);
 }

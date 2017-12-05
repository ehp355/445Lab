/*Visual.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
 */
#include <stdint.h>
#include "ST7735.h"
#include "Logger.h"
#include "../inc/tm4c123gh6pm.h"
extern int8_t logIndex;

const unsigned short heart[] = {
 0x0000, 0x0000, 0x0000, 0x001F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x0000, 0x001F, 0x001F, 0x0000, 0x001F, 0x001F, 0x0002,

};

const unsigned short biggerHeart[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000E, 0x001F, 0x001F, 0x001F, 0x0014, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000D, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x000D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x000E, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0002, 0x000E, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000E, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x000A, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x000A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x000B, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002,
 0x000B, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x000B, 0x0000, 0x0000, 0x0002, 0x000A, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000A, 0x0000, 0x0012, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000B, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x000F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000D, 0x000B, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x000F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000E, 0x0000, 0x000C, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x000C, 0x0000, 0x0010, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0016, 0x0000,
 0x0000, 0x0000, 0x000E, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x000D, 0x0000, 0x0000, 0x0003, 0x000E, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x000E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0016, 0x001F, 0x001F, 0x001F, 0x000B, 0x0003, 0x0001, 0x0000,
 0x0001, 0x0008, 0x000D, 0x001F, 0x001F, 0x001F, 0x000C, 0x0000, 0x0000, 0x0000,

};



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
uint8_t toggleAlert =0; 
void displayAlert(void){

	if(toggleAlert == 0){
		ST7735_FillScreen(ST7735_BLACK);
		toggleAlert=1;
		Delay1ms(1000);
	}else{
		ST7735_FillScreen(ST7735_RED);
		toggleAlert=0;
		Delay1ms(1000);
	}
}

void displayStartScreen(void){
	//127
	//159
	uint8_t x_pos =13;
	uint8_t y_pos =15;
	char pocket[6]="Pocket";
	char doc[3]="Doc";
	
	char heartFilled[24]="Have a heart filled day!";
	//draws the word 'pocket'
	for(uint8_t i =0; i < 6; i++){
		if(i==1){
			//ST7735_DrawChar(x_pos,y_pos,pocket[i],ST7735_RED,ST7735_BLACK,3);
			ST7735_DrawBitmap(x_pos-3,y_pos+22,biggerHeart,21,18);
		}else{
			ST7735_DrawChar(x_pos,y_pos,pocket[i],ST7735_WHITE,ST7735_BLACK,3);
		}
		x_pos+=19;
	}
	x_pos =40;
	y_pos+=30;
	//draws the word 'doc'
	for(uint8_t i=0;i<3;i++){
		ST7735_DrawChar(x_pos,y_pos,doc[i],ST7735_WHITE,ST7735_BLACK,3);
		x_pos+=18;
	}
	
	x_pos=15;
	y_pos+=45;
	//draws"Have a heart filled day!"
	for(uint8_t i=0;i<24;i++){
		if(i==13){
			y_pos+=15;
			x_pos=43;
		}
		ST7735_DrawChar(x_pos,y_pos,heartFilled[i],ST7735_WHITE,ST7735_BLACK,1);
		x_pos+=6;
	}
	
	ST7735_DrawBitmap(95,47,heart,7,6);
	
}
//info for buttons at start menu
void displayButtonControlState0(void){
	ST7735_DrawString(3, 13, "Press Button 3", ST7735_RED);
	ST7735_DrawString(5,14,"to begin",ST7735_RED);
}

//infor for buttons at hb monitoring state
void displayButtonControlState1(void){
	//Button1->read HB
	//Button2->switch to logger
	//Button3->log displayed HB
	char button1[16] = "1.Log Heart Beat";
	char button2[5] = "2.Log";
	char button3[19] = "3.Update Heart Beat";
	
	uint8_t x_pos=0;
	uint8_t y_pos=0;
	
	//draws instructions for button1 at state 1
	for(uint8_t i = 0; i<16; i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button1[i],ST7735_YELLOW,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button1[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
	y_pos +=10;
	x_pos =0;
	//draws instructions for button2 at state 1
	for(uint8_t i = 0; i<5; i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button2[i],ST7735_MAGENTA,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button2[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
	y_pos +=10;
	x_pos=0;
	for(uint8_t i =0; i<19;i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button3[i],ST7735_GREEN,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button3[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
}

void displayHeartState1(void){
	ST7735_DrawBitmap(80,80,biggerHeart,21,18);
}


//info for buttons at logger view state
void displayButtonControlState2(void){
	//Button1->scroll up
	//Button2->switch to hb montioring
	//Button3->scroll down	
	char button1[13] = "1.Scroll Down";
	char button2[20] = "2.Heart Beat Monitor";
	char button3[19] = "3.Scroll Up";
	
	uint8_t x_pos=0;
	uint8_t y_pos=0;
	//draws button 1 instructions on stae 2
	for(uint8_t i = 0; i<13; i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button1[i],ST7735_YELLOW,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button1[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
	y_pos +=10;
	x_pos =0;
	//draws button 2 instructions on stae 2
	for(uint8_t i = 0; i<20; i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button2[i],ST7735_MAGENTA,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button2[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
	y_pos +=10;
	x_pos=0;
	//draws button 3 instructions on stae 2
	for(uint8_t i =0; i<19;i++){
		if(i==0||i==1){
			ST7735_DrawChar(x_pos,y_pos,button3[i],ST7735_GREEN,ST7735_BLACK,1);
		}else{
			ST7735_DrawChar(x_pos,y_pos,button3[i],ST7735_WHITE,ST7735_BLACK,1);
		}
		x_pos+=6;
	}
}

//info for buttons at alert state
void displayButtonControlState3(void){
	//AllButtons->return to HB monitoring
	ST7735_DrawString(3,14,"Press any button to continue",ST7735_WHITE);
}
 

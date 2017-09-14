//fixed.c
//Enrique Perez-Osborne, Juliana Pulido Plazas
//Sept. 6, 2017
//List of functions to calculate fixed point
//and plot to screen
//Lab 1
//Cody Horton
//Sept. 12, 2017
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO)
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

int32_t x_Total;
int32_t y_Total;

int32_t offset_X;
int32_t offset_Y;

void ST7735_sDecOut3(uint32_t n){
	int32_t m = n;
		
	if (m >= 10000 || m <= -10000){
			fputc(' ',0);
			fputc('*',0);
			fputc('.',0);
			for(int32_t i =0; i < 3; i++){
				fputc('*',0);
			}
	}else if(m>=0 && m <10000){
		//for even numbers
		
		char numList[3];
		
		for(int16_t i = 0; i <3; i ++){
			numList[i] = (m%10)+48;
			m = m/10;
		}
			
		fputc(' ',0);
		fputc((m+48),0);
		fputc('.',0);
		for(int32_t k = 2; k >=0; k--){
			fputc(numList[k],0);
		}
			
	}else{
		//for negative numbers
		m = -1* m;
		char numList[3];
		
		for(uint16_t i = 0; i <3; i ++){
			numList[i] = (m%10)+48;
			m = m/10;
		}
			
		//first number
		fputc('-',0);
		fputc((m+48),0);
		fputc('.',0);
		for(int32_t k = 2; k >=0; k--){
			fputc(numList[k],0);
		}
	}
}

void ST7735_uBinOut8(uint32_t n){
	
	int32_t m = n;
	if(m >= 256000){
		fputc(' ',0);
		fputc('*',0);
		fputc('*',0);
		fputc('*',0);
		fputc('.',0);
		for(int32_t i =0; i < 2; i++){
			fputc('*',0);
		}
	
	}else if(m<0){
		fputc(' ',0);
		fputc('*',0);
		fputc('*',0);
		fputc('*',0);
		fputc('.',0);
		for(int32_t i =0; i < 2; i++){
			fputc('*',0);
		}

	}else if(m<2560){
		//need to make sure this rounds to the 100th decimal place
		char numList[5];
		for(int32_t i = 0; i<5; i++){
			
			if(m/256 >=10){
				numList[i] = (((m/256) %10)+48);
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m *10;
		}
		
	
		fputc(' ',0);
		fputc(' ',0);
		fputc(' ',0);
		fputc(numList[0],0);
		fputc('.',0);
		
		for(int32_t i =1; i <3; i++){
			fputc(numList[i],0);
		}
		
	}else if(m<25600){
		
		int32_t floater = m % 10;
		m = m / 10;
		char numList[5];
		for(int32_t i = 0; i<5; i++){
			
			if(m/256 >=10){
				numList[i] = (((m/256) %10)+48);
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m *10;
			m = m+ floater;
			floater = 0;
		}
		
		fputc(' ',0);
		fputc(' ' ,0);
		fputc(numList[0],0);
		fputc(numList[1],0);
		fputc('.', 0);
		for(int32_t i =2; i <4; i++){
			fputc(numList[i],0);
		}
		
		
	}else if(m<256000){
		int32_t floater = m % 10;
		m = m / 10;
		int32_t floater2= m % 10;
		m = m /10;
		char numList[5];
		for(int32_t i = 0; i<5; i++){
			
			if(m/256 >=10){
				numList[i] = (((m/256) %10)+48);
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m *10;
			
			if( i == 0){
				m = m + floater2;
				floater2 = 0;
			}
			if(i == 1){
				m = m + floater;
				floater=0;
			}
		}
		fputc(' ',0);
		fputc(numList[0],0);
		fputc(numList[1],0);
		fputc(numList[2],0);
		fputc('.', 0);
		
		for(int32_t i =3; i <5; i++){
			fputc(numList[i],0);
		}
		
	}
}


void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
	
	ST7735_FillScreen(ST7735_BLACK); 
	ST7735_SetCursor(0,0);
	uint32_t i = 0;
	//Title
	while(title[i]!= 0){
		fputc(title[i],0);
		i = i + 1;
	}
	//White plot area
	ST7735_FillRect(0, 32, 127, 159, ST7735_WHITE);
	
	x_Total = maxX-minX;		
	y_Total = maxY-minY;
	
	offset_X=abs(minX);
	offset_Y=abs(minY);
	
//	int16_t mx = 0;
//	int16_t my = 10;
//	int16_t w = (maxX*(128/xTot));
//	int16_t h = (maxY*(150/yTot));

}

//Circle: -2500, 2500
//Diff: 5000

//Star:  -450, 150
//Diff: 600

//Add abs(minX) to each value
//then scale it: 

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
	int32_t x;
	int32_t y;

	for(int32_t i = 0; i < num; i++){
		x = (((bufX[i]+offset_X)*128)/x_Total);
		y = (128-(((bufY[i]+offset_Y)*128)/y_Total))+32;

		
		ST7735_DrawPixel(x,y,ST7735_BLUE);
	
	}
	
}
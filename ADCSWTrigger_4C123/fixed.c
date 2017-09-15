#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

int32_t xTot;
int32_t yTot;

int32_t mxX;
int32_t mxY;

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
	
	xTot = maxX-minX;		
	yTot = maxY-minY;
	
	mxX=minX;
	mxY=abs(minY);
	
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

void ST7735_XYplot(int32_t num, int32_t histIndex, int32_t bufX[], int32_t bufY[]){
	int32_t x;
	int32_t y;

	for(int32_t i = histIndex; i < num+histIndex; i++){
		x = (((i-mxX)*128)/xTot);
		//y has been changed for the histogram in Lab2
		y = (128-(((bufY[i]+mxY)*128)/yTot))+32;

		
		ST7735_DrawPixel(x,y,ST7735_BLUE);
	
	}
	
}
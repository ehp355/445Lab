/*fixed.c
**Enrique Perez-Osborne
**Juliana Pulido
**9/13/17
**List of functions to help with Fixed point calculations and to plot to screen
**Lab2
**TA: Cody Horton
**9/20/17
*/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

int32_t x_Total;
int32_t y_Total;

int32_t max_X;
int32_t max_Y;

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
	
	max_X=minX;
	max_Y=abs(minY);
	
}




void ST7735_Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color){
	
	uint16_t offSet;
	uint16_t flag;

	if(y1<y2){
		if(x1<x2){
		//x1<x2 and y1<y2
			if(y2-y1 > x2-x1){
		
				offSet = (y2-y1)/(x2-x1);
				flag = 1;
		
			}else{

				offSet = (x2-x1)/(y2-y1);
				flag = 0;

			}
			
			if(offSet == 0){
				if(flag == 1){

					for(uint16_t i = x1; i <=  x2; i++){
						for(uint16_t j = y1; j < y2; j++){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1+offSet;
					}

				}else{

					for(uint16_t i = y1; i <=  y2; i++){
						for(uint16_t j = x1; j < x2; j++){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1+offSet;
					}

				}
				
			}else{
				if(flag == 1){

					for(uint16_t i = x1; i <=  x2; i++){
						for(uint16_t j = y1; j < y1 + offSet; j++){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1+offSet;
					}

				}else{

					for(uint16_t i = y1; i <=  y2; i++){
						for(uint16_t j = x1; j < x1 + offSet; j++){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1+offSet;
					}

				}
			}
		}else{
			//else x1>x2 and y1<y2
			if(y2-y1 > x1-x2){
		
				offSet = (y2-y1)/(x1-x2);
				flag = 1;
		
			}else{

				offSet = (x1-x2)/(y2-y1);
				flag = 0;

			}
			if(offSet==0){
				if(flag == 1){

					for(int16_t i = x1; i >=  x2; i--){
						for(int16_t j = y1; j < y2; j++){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1+offSet;
					}

				}else{

					for(int16_t i = y1; i <=  y2; i++){
						for(int16_t j = x1; j > x2; j--){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1-offSet;
					}

				}
			}else{
				if(flag == 1){

					for(int16_t i = x1; i >=  x2; i--){
						for(int16_t j = y1; j < y1 + offSet; j++){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1+offSet;
					}

				}else{

					for(int16_t i = y1; i <=  y2; i++){
						for(int16_t j = x1; j > x1 - offSet; j--){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1-offSet;
					}

				}
			}
		}
	}else{
		//else y1>y2
		if(x1<x2){
			if(y1-y2 > x2-x1){
		
				offSet = (y1-y2)/(x2-x1);
				flag = 1;
		
			}else{

				offSet = (x2-x1)/(y1-y2);
				flag = 0;

			}
			if(offSet == 0){
			
				if(flag == 1){

					for(int16_t i = x1; i <=  x2; i++){
						for(int16_t j = y1; j > y2; j--){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1-offSet;
					}

				}else{

					for(int16_t i = y1; i >=  y2; i--){
						for(int16_t j = x1; j < x2; j++){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1+offSet;
					}

				}
			
			}else{
				if(flag == 1){

				for(int16_t i = x1; i <=  x2; i++){
					for(int16_t j = y1; j > y1 - offSet; j--){
						ST7735_DrawPixel(i,j,color);
					}
					y1 = y1-offSet;
				}

				}else{

					for(int16_t i = y1; i >=  y2; i--){
						for(int16_t j = x1; j < x1 + offSet; j++){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1+offSet;
					}

				}
		
			}
		}	/**/else{
			//x1>x2 and y1>y2
			if(y1-y2 > x1-x2){
		
				offSet = (y1-y2)/(x1-x2);
				flag = 1;
		
			}else{

				offSet = (x1-x2)/(y1-y2);
				flag = 0;

			}
			if(offSet == 0){
			
				if(flag == 1){

					for(int16_t i = x1; i >=  x2; i--){
						for(int16_t j = y1; j > y2; j--){
							ST7735_DrawPixel(i,j,color);
						}
						y1 = y1-offSet;
					}

				}else{

					for(int16_t i = y1; i >=  y2; i--){
						for(int16_t j = x1; j > x2; j--){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1-offSet;
					}

				}
			
			}else{
				//offset != 0 and x1>x2 and y1>y2
				if(flag == 1){

				for(int16_t i = x1; i >=  x2; i--){
					for(int16_t j = y1; j > y1 - offSet; j--){
						ST7735_DrawPixel(i,j,color);
					}
					y1 = y1-offSet;
				}

				}else{

					for(int16_t i = y1; i >=  y2; i--){
						for(int16_t j = x1; j > x1 - offSet; j--){
							ST7735_DrawPixel(j,i,color);
						}
						x1 = x1-offSet;
					}

				}
		
			}
			
		}
	}	
	
}

void ST7735_XYplot(int32_t num, int32_t histIndex, int32_t bufX[], int32_t bufY[]){
	int32_t x;
	int32_t y;

	for(int32_t i = histIndex; i < num+histIndex; i++){
		x = (((i-max_X)*128)/x_Total);
		//y has been changed for the histogram in Lab2
		y = (128-(((bufY[i]+max_Y)*128)/y_Total))+32;

		ST7735_Line(x,159,x,y,ST7735_BLUE);
	
	}
	
}

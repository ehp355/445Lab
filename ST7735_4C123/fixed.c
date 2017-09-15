/******************fixed.c******************
 Enrique Perez-Osborne, Juliana Pulido Plazas
 Sept. 6, 2017
 List of functions to calculate fixed point and plot to LCD
 Lab 1
 TA: Cody Horton
 Sept. 12, 2017
 Backlight (pin 10) connected to +3.3 V
 MISO (pin 9) unconnected 
 SCK (pin 8) connected to PA2 (SSI0Clk)
 MOSI (pin 7) connected to PA5 (SSI0Tx)
 TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 CARD_CS (pin 5) unconnected
 Data/Command (pin 4) connected to PA6 (GPIO)
 RESET (pin 3) connected to PA7 (GPIO)
 VCC (pin 2) connected to +3.3 V
 Gnd (pin 1) connected to ground
*/
                                                                                
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

int32_t x_Total;								
int32_t y_Total;

int32_t offset_X;
int32_t offset_Y;

void ST7735_sDecOut3(uint32_t n){
	int32_t m = n;												//define the value n as a signed int m
		
	if (m >= 10000 || m <= -10000){				//for numbers outside the range:
			fputc(' ',0);											//print "*.***" to LCD
			fputc('*',0);
			fputc('.',0);
			for(int32_t i =0; i < 3; i++){
				fputc('*',0);
			}
	}else if(m>=0 && m <10000){						//for positive numbers within the range:
																				
		char numList[3];
		
		for(int16_t i = 0; i <3; i ++){			
			numList[i] = (m%10)+48;						//extract each digit and store it in
			m = m/10;													//array numList
		}
			
		fputc(' ',0);												//print out numbers in numList array 
		fputc((m+48),0);										//to LCD
		fputc('.',0);
		for(int32_t k = 2; k >=0; k--){
			fputc(numList[k],0);
		}
			
	}else{																//for negative numbers within the range
		m = -1* m;													//get positive value of number
		char numList[3];
		
		for(uint16_t i = 0; i <3; i ++){		//extract each digit and store it in 
			numList[i] = (m%10)+48;						//array numList
			m = m/10;
		}
			
		fputc('-',0);												//print out '-' and then print out
		fputc((m+48),0);										//numbers in numList array to LCD
		fputc('.',0);
		for(int32_t k = 2; k >=0; k--){
			fputc(numList[k],0);
		}
	}
}

void ST7735_uBinOut8(uint32_t n){
	
int32_t m = n;												//define value n as a signed int m
	if(m >= 256000 || m<0){							//if value outside of range,  
		fputc(' ',0);											//print "***.**" on LCD
    fputc('*',0);
		fputc('*',0);
		fputc('*',0);
		fputc('.',0);
		for(int32_t i =0; i < 2; i++){
		  fputc('*',0);
		}
//	if(m >= 256000){
//		fputc(' ',0);
//		fputc('*',0);
//		fputc('*',0);
//		fputc('*',0);
//		fputc('.',0);
//		for(int32_t i =0; i < 2; i++){
//			fputc('*',0);
//		}
//	
//	}else if(m<0){
//		fputc(' ',0);
//		fputc('*',0);
//		fputc('*',0);
//		fputc('*',0);
//		fputc('.',0);
//		for(int32_t i =0; i < 2; i++){
//			fputc('*',0);
//		}

		}else if(m<2560){											//for numbers under 2560:
		char numList[5];
		for(int32_t i = 0; i<5; i++){					
			if(m/256 >=10){											//extract last digit of m
				numList[i] = (((m/256) %10)+48); 	//store last digit of m in array
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m*10;														//shift m left by one decimal place
		}																			//in order to extract next digit

		fputc(' ',0);													//print 3 spaces 
		fputc(' ',0);
		fputc(' ',0);
		fputc(numList[0],0);									//print first number
		fputc('.',0);
		
		for(int32_t i =1; i <3; i++){					//print last two numbers
			fputc(numList[i],0);
		}
		
		}else if(m<25600){										//for numbers b/t 2560 and 25600:
		
		int32_t floater = m % 10;							//floater holds the last digit of m
		m = m / 10;														//shift m right by one decimal place
																					//in order to extract first digit
		char numList[5];
		for(int32_t i = 0; i<5; i++){
			
			if(m/256 >=10){											//extract last digit of m
				numList[i] = (((m/256) %10)+48);	//store it in array
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m*10;														//shift m left by one decimal place
			m = m+floater;											//add last digit of m back to m										
			floater = 0;												//clear floater
		}
		
		fputc(' ',0);													//print two spaces
		fputc(' ' ,0);
		fputc(numList[0],0);									//print first two numbers
		fputc(numList[1],0);
		fputc('.', 0);
		for(int32_t i =2; i <4; i++){					//print last two numbers
			fputc(numList[i],0);
		}
		
		
	}else if(m<256000){											//for numbers b/t 25600 and 256000:
		int32_t floater = m % 10;							//floater holds last digit of m
		m = m / 10;														//shift m right by one decimal place
		int32_t floater2= m % 10;							//floater2 holds next digit of m
		m = m /10;														//shift m right by one decimal place
		char numList[5];
		for(int32_t i = 0; i<5; i++){
			
			if(m/256 >=10){											//extract last digit of m
				numList[i] = (((m/256) %10)+48);	//store in array
			}else{
				numList[i]= ((m/256)+48);
			}
			m = m *10;													//shift m left by one decimal place
			
			if( i == 0){												//add floater2 back to m
				m = m + floater2;									//after first left shift to m
				floater2 = 0;											//clear floater2
			}
			if(i == 1){													//add floater back to m 
				m = m + floater;									//after second left shift to m
				floater=0;												//clear floarer 
			}
		}
		fputc(' ',0);													//print one space
		fputc(numList[0],0);									//print first three numbers in array
		fputc(numList[1],0);
		fputc(numList[2],0);
		fputc('.', 0);												//print decimal point
		
		for(int32_t i =3; i <5; i++){
			fputc(numList[i],0);								//print last two numbers
		}
	}
}


void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
	
	ST7735_FillScreen(ST7735_BLACK); 				//clear LCD
	ST7735_SetCursor(0,0);									//set cursor 
	uint32_t i = 0;													//i is the title index

	while(title[i]!= 0){										//print title 
		fputc(title[i],0);
		i = i + 1;
	}
	
	ST7735_FillRect(0, 32, 127, 159, ST7735_WHITE);	//fill white 128x128px square
																									//on the LCD as XY plot			
	
	x_Total = maxX-minX;										//width of data pts to scale 	
	y_Total = maxY-minY;										//length of data pts to scale
	
	offset_X=abs(minX);											//offsets to add to x and y data pts
	offset_Y=abs(minY);											//before scaling
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
	int32_t x;																					//x and y points to plot
	int32_t y;

	for(int32_t i = 0; i < num; i++){										//scale data points
		x = (((bufX[i]+offset_X)*128)/x_Total);						//(pt)*plotWidth/dataWitdh
		y = (128-(((bufY[i]+offset_Y)*128)/y_Total))+32;
		
		ST7735_DrawPixel(x,y,ST7735_BLUE);								//plot scaled data points
	}
}
/*Visual.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/29/17
 */
 #include <stdint.h>
 #include "../inc/tm4c123gh6pm.h"
 
 #include "ST7735.h"
 

void plotRPSInit(void){
	ST7735_SetCursor(0,0);
	ST7735_PlotClear(0,500);
}


void displayPeriod(uint32_t period){
	char pz = period/100000000 +'0';
	char p0 = (period%100000000)/10000000 + '0';
	char p1 = (period%10000000)/1000000 + '0';
	char p2 = (period%1000000)/100000 + '0';
	char p3 = (period%100000)/10000 + '0';
	char p4 = (period%10000)/1000 + '0';
	char p5 = (period%1000)/100 + '0';
	char p6 = (period%100)/10 + '0';
	char p7 = (period%10) + '0';
  char rpsString[16]={'P','e','r','i','o','d',':' ,pz,p0,p1,p2,p3,p4,p5,p6,p7};
	ST7735_DrawString(0,3 , rpsString, ST7735_BLUE);
}


void displayDesiredRPS(uint32_t drps){
	char rpsString[13]={'D','e','s','i','r','e','d',':' ,(drps/100)+'0',((drps%100)/10)+'0','.',(drps%10)+'0', 0};
	ST7735_DrawString(0,1 , rpsString, ST7735_BLUE);
}

void displayActualRPS(uint32_t arps){
	char rpsString[12]={'A','c','t','u','a','l',':',(arps/100)+'0', ((arps%100)/10)+'0','.',(arps%10)+'0', 0};
	ST7735_DrawString(0,2 , rpsString, ST7735_BLUE);
}

void plotRPS(uint32_t j,uint32_t N, uint32_t fs,uint32_t DesiredRPS,uint32_t ActualRPS, uint32_t period){
		//sweeping graph
		ST7735_PlotPoint(ActualRPS);  
		ST7735_PlotPoint(DesiredRPS);
		if((j&(N-1))==0){
			ST7735_PlotNextErase();
		}
		if((j%fs)==0){
			displayDesiredRPS(DesiredRPS);
			displayActualRPS(ActualRPS);
			//displayPeriod(period);
		}
}

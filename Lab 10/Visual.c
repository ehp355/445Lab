/*Visual.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
 #include <stdint.h>
 #include "../inc/tm4c123gh6pm.h"
 
 #include "ST7735.h"
 

void plotRPSInit(void){
	ST7735_SetCursor(0,0);
	ST7735_PlotClear(0,90);
}





void displayDesiredRPS(uint32_t drps){
	char rpsString[10]={'D','e','s','i','r','e','d',':' ,(drps/10)+'0',(drps%10)+'0'};

	 ST7735_DrawString(0,1 , rpsString, ST7735_BLUE);
}

void displayActualRPS(uint32_t arps){
	char rpsString[9]={'A','c','t','u','a','l',':', (arps/10)+'0',(arps%10)+'0'};

	 ST7735_DrawString(0,2 , rpsString, ST7735_BLUE);
}

void plotRPS(uint32_t j,uint32_t N, uint32_t fs,uint32_t displaySpeed,uint32_t DesiredRPS,uint32_t ActualRPS){
	//sweeping graph
		ST7735_PlotPoint(displaySpeed);  
		if((j&(N-1))==0){
			ST7735_PlotNextErase();
		}
		if((j%fs)==0){
			displayDesiredRPS(DesiredRPS);
			displayActualRPS(ActualRPS);
		}
}

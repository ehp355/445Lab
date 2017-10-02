#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"

uint32_t start_Time;
uint32_t end_Time;
uint32_t time_Ms =0;

void Timer3A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate timer3
  TIMER3_CTL_R = 0x00000000;    // 1) disable timer3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000000;   // 3) configure for periodic mode
  TIMER3_TAILR_R = 80000;    	  // 4) reload value (1ms)
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // 8) priority 3
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN1_R |= 0x8;           	// 9) enable IRQ 35 in NVIC
	TIMER3_CTL_R = 0x01;					// enable timer 3
}

void Timer3A_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	time_Ms++;
}

void set_Start_Time(void){
	start_Time = time_Ms;
}

void set_End_Time(void){
	end_Time = time_Ms;
}

uint32_t time_Diff(void){
	return end_Time - start_Time;
}

uint16_t find_Min(uint16_t *arr){
	uint16_t min = arr[0];
	for(int i=1; i<10; i++){
		if(min > arr[i]){
			min = arr[i];
		}
	}	
	return min;
}

uint16_t find_Max(uint16_t *arr){
	uint16_t max = arr[0];
	for(int i=1; i<10; i++){
		if(max < arr[i]){
			max = arr[i];
		}
	}	
	return max;
}

uint16_t find_Avg(uint16_t *arr){
	uint32_t sum = arr[0]; 
	uint16_t avg;
	for(int i=1; i<10; i++){
		sum = sum + arr[i];
	}
	avg = sum/10;
	return avg;
}

void printData(int16_t min, int16_t max, int16_t avg, int32_t y){
	int8_t Digit3;
	int8_t Digit2;
	int8_t Digit1;
	char minTitle[] = "min = ";
	char maxTitle[] = "max = "; 
	char avgTitle[] = "avg = ";

	Digit3 = min%10;
	min = min/10;
	Digit2 = min%10;
	min = min/10;
	Digit1 = min;
	
	ST7735_SetCursor(0,y);
	ST7735_OutString(minTitle);
	ST7735_OutChar(Digit1+'0');
	ST7735_OutChar(Digit2+'0');
	ST7735_OutChar(Digit3+'0');
	ST7735_OutString("ms");
	
	Digit3 = max%10;
	max = max/10;
	Digit2 = max%10;
	max = max/10;
	Digit1 = max;

	
	ST7735_SetCursor(0,y+1);
	ST7735_OutString(maxTitle);
	ST7735_OutChar(Digit1+'0');
	ST7735_OutChar(Digit2+'0');
	ST7735_OutChar(Digit3+'0');
	ST7735_OutString("ms");
	
	Digit3 = avg%10;
	min = min/10;
	Digit2 = min%10;
	min = min/10;
	Digit1 = min;
	
	ST7735_SetCursor(0,y+2);
	ST7735_OutString(avgTitle);
	ST7735_OutChar(Digit1+'0');
	ST7735_OutChar(Digit2+'0');
	ST7735_OutChar(Digit3+'0');
	ST7735_OutString("ms");
	
	
	
}


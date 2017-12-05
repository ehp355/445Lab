// I2CTestMain.c
// Runs on LM4F120/TM4C123
// Test the functions provided in I2C0.c by periodically sampling
// the temperature, parsing the result, and sending it to the UART.
// Daniel Valvano
// May 3, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Section 8.6.4 Programs 8.5, 8.6 and 8.7

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
// I2C0SCL connected to PB2 and to pin 4 of HMC6352 compass or pin 3 of TMP102 thermometer
// I2C0SDA connected to PB3 and to pin 3 of HMC6352 compass or pin 2 of TMP102 thermometer
// SCL and SDA lines pulled to +3.3 V with 10 k resistors (part of breakout module)
// ADD0 pin of TMP102 thermometer connected to GND
#include <stdint.h>
#include "I2C0.h"
#include "PLL.h"
#include "UART.h"
#include "ST7735.h"
#include "Visual.h"
#include "Sound.h"
#include "ButtonControl.h"
#include "Logger.h"

//git test

// For debug purposes, this program may peek at the I2C0 Master
// Control/Status Register to try to provide a more meaningful
// diagnostic message in the event of an error.  The rest of the
// interface with the I2C hardware occurs through the functions
// in I2C0.c.
#define I2C0_MASTER_MCS_R       (*((volatile unsigned long *)0x40020004))

// DEBUGPRINTS==0 configures for no test prints, other value prints test text
// This tests the math used to convert the raw temperature value
// from the thermometer to a string that is displayed.  Verify
// that the left and right columns are the same.
#define DEBUGPRINTS 0
// DEBUGWAIT is time between test prints as a parameter for the Delay() function
// DEBUGWAIT==16,666,666 delays for 1 second between lines
// This is useful if the computer terminal program has limited
// screen or log space to prevent the text from scrolling too
// fast.
// Definition has no effect if test prints are off.
#define DEBUGWAIT   16666666

// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
  //Code Composer Studio Code
  void Delay(unsigned long ulCount){
  __asm (  "    subs    r0, #1\n"
      "    bne     Delay\n"
      "    bx      lr\n");
}

#else
  //Keil uVision Code
  __asm void
  Delay(unsigned long ulCount)
  {
    subs    r0, #1
    bne     Delay
    bx      lr
  }

#endif
// function parses raw 16-bit number from temperature sensor in form:
// rawdata[0] = 0
// rawdata[15:8] 8-bit signed integer temperature
// rawdata[7:4] 4-bit unsigned fractional temperature (units 1/16 degree C)
//  or
// rawdata[0] = 1
// rawdata[15:7] 9-bit signed integer temperature
// rawdata[6:3] 4-bit unsigned fractional temperature (units 1/16 degree C)

//volatile uint16_t heading = 0;
//volatile uint8_t controlReg = 0;


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
	
extern uint8_t button;
/*State = 0 -> start screen
 *State = 1 -> HeartBeat Display
 *State = 2 -> Log display
 *State = 3 -> Alarm state
 */
uint8_t State;


int main(void){
	State =0;
  PLL_Init(Bus80MHz);
	long sr;
	sr =StartCritical();

	uint16_t dummyHBS[17] = {124,99,86,45,1,300,289,201,180,155,0,123,60,78,133,23,205};
	uint8_t dummyCounter =0;
	Log_Init();
  I2C_Init();
	PortD_Init();
	Timer0A_Init();
	Timer2A_Init();
	ST7735_InitR(INITR_REDTAB);


	EndCritical(sr);
	//clearFifo();
	setMode(0x3);	
	//clearFifo();
  setLEDCurrent(0x1F,0x1F);	
	//setSamplingRate(0);
	//setSamplingAverage(0x2);
	//setADCRangeControl(0x1);
	//setLEDPulseWidth(0);						
	
	enableSlots();
	//enableProxThresh();
	
	//registerDebugger();
	
	//clearFifo();
	//uint8_t hb = getHeartBeat();
	/*//code for lab7 checkout
	soundAlarm();
	for(uint32_t i=0; i<900000; i++){
		int a = 0;
		int b = 1;
	}
	endAlarm();
	*/
	uint16_t dummy;
	while(1){
		
		if(State == 0 && button==0x1){
			ST7735_FillRect(0,0,127,159,ST7735_BLACK);
			State = 1;
			button = 0;
		}else if(State == 1 && button==0x1){
			
			State =1;
			//dummy function to display raw ir data to screen
			//rawIRList();
		 //call function to update hb monitor
			button =0;
		
		}else if(State ==1 && button == 0x2){
		
			erase2DigitNumber(60);
			ST7735_FillRect(0,0,127,159,ST7735_BLACK);
			State = 2;
			displayLog();
			button = 0;
 
		}else if(State == 1 && button==0x4){

			dummy = dummyHBS[dummyCounter];
			logToArray(dummy);
			if(dummyCounter<16)dummyCounter++;
			button = 0;
	
		}else if(State == 2 && button == 0x1){

			scrollUp();
			button = 0;

		}else if(State == 2 && button == 0x2){
			
			State = 1;
			resetTopIndex();
			killScrollers();
			ST7735_FillRect(0,0,128,160,ST7735_BLACK);
			button = 0;

		}else if(State == 2 && button == 0x4){

			scrollDown();
			button = 0;

		}else if(State == 3 && button > 0){

			State = 1;
			ST7735_FillRect(0,0,127,159,ST7735_BLACK);
			endAlarm();
			button = 0;

		}
		
		
		//start screen
		if(State ==0){
			displayStartScreen();
			displayButtonControlState0();
			
		//HB display
		}else if(State == 1){
			displayButtonControlState1();
			displayHeartState1();
			//place hb in  in display2DigitNumber();
			//display2DigitNumber();
			
		//Log display
		}else if(State ==2){
			displayButtonControlState2();
			blinkTopScroller();
			blinkBottomScroller();
			
		//Alarm display and sound
		}else if(State ==3){
			displayButtonControlState3();
			displayAlert();
			soundAlarm();
		}
	}
}

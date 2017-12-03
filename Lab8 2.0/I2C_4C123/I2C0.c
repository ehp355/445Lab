// I2C0.c
// Runs on LM4F120/TM4C123
// Provide a function that initializes, sends, and receives the I2C0 module
// interfaced with an HMC6352 compass or TMP102 thermometer.
// Daniel Valvano
// May 3, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Section 8.6.4 Programs 8.5, 8.6 and 8.7

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// I2C0SCL connected to PB2 and to pin 4 of HMC6352 compass or pin 3 of TMP102 thermometer
// I2C0SDA connected to PB3 and to pin 3 of HMC6352 compass or pin 2 of TMP102 thermometer
// SCL and SDA lines pulled to +3.3 V with 10 k resistors (part of breakout module)
// ADD0 pin of TMP102 thermometer connected to GND
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"


#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable

#define STATUS									0x00
#define ENABLE 									0x02
#define FIFO_WRITE_PTR 					0x04
#define FIFO_OVERFLOW_CTR				0x05
#define FIFO_READ_PTR						0x06
#define FIFO_DATA								0x07
#define	MODE_CONF								0x09
#define SPO2_CONF								0x0A
#define SAMP_AVG_CONF						0x08
#define LED1PA									0x0C
#define LED2PA									0x0D
#define LED_CONF_1							0x11
#define LED_CONF_2							0x12
#define PROX_INT_THRESH					0x30

#define MAXRETRIES              5           // number of receive attempts before giving up

#include "ST7735.h"


uint8_t temp;
uint8_t flag;
uint32_t rawData[2];
uint32_t samples[32][2];
float dcFilter_t [2];

//meanDiffFilter_t
float values[32];
uint8_t indexMDF=0;
float sumMDF=0;
uint8_t countMDF =0;



void I2C_Init(void){
  SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
  SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?

  GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
  GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only
  GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
                                        // 6) configure PB2,3 as I2C
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
  GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
  I2C0_MCR_R = I2C_MCR_MFE;      // 9) master function enable
  I2C0_MTPR_R = 24;              // 8) configure for 100 kbps clock
  // 20*(TPR+1)*20ns = 10us, with TPR=24
}

// receives one byte from specified slave
// Note for HMC6352 compass only:
// Used with 'r' and 'g' commands
// Note for TMP102 thermometer only:
// Used to read the top byte of the contents of the pointer register
//  This will work but is probably not what you want to do.
uint8_t I2C_Recv(int8_t slave){
  int retryCounter = 1;
  do{
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R |= 0x01;              // MSA[0] is 1 for receive
    I2C0_MCS_R = (0
                        // & ~I2C_MCS_ACK     // negative data ack (last byte)
                         | I2C_MCS_STOP     // generate stop
                         | I2C_MCS_START    // generate start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    retryCounter = retryCounter + 1;        // increment retry counter
  }                                         // repeat if error
  while(((I2C0_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0) && (retryCounter <= MAXRETRIES));
  return (I2C0_MDR_R&0xFF);          // usually returns 0xFF on error
}
// receives two bytes from specified slave
// Note for HMC6352 compass only:
// Used with 'A' commands
// Note for TMP102 thermometer only:
// Used to read the contents of the pointer register
uint16_t I2C_Recv2(int8_t slave){
  uint8_t data1,data2;
  int retryCounter = 1;
  do{
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R |= 0x01;              // MSA[0] is 1 for receive
    I2C0_MCS_R = (0
                         | I2C_MCS_ACK      // positive data ack
                       //  & ~I2C_MCS_STOP    // no stop
                         | I2C_MCS_START    // generate start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    data1 = (I2C0_MDR_R&0xFF);       // MSB data sent first
    I2C0_MCS_R = (0
                       //  & ~I2C_MCS_ACK     // negative data ack (last byte)
                         | I2C_MCS_STOP     // generate stop
                       //  & ~I2C_MCS_START   // no start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    data2 = (I2C0_MDR_R&0xFF);       // LSB data sent last
    retryCounter = retryCounter + 1;        // increment retry counter
  }                                         // repeat if error
  while(((I2C0_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0) && (retryCounter <= MAXRETRIES));
  return (data1<<8)+data2;                  // usually returns 0xFFFF on error
}

// sends one byte to specified slave
// Note for HMC6352 compass only:
// Used with 'S', 'W', 'O', 'C', 'E', 'L', and 'A' commands
//  For 'A' commands, I2C_Recv2() should also be called
// Note for TMP102 thermometer only:
// Used to change the pointer register
// Returns 0 if successful, nonzero if error
uint32_t I2C_Send1(int8_t slave, uint8_t data1){
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                    //   & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}
// sends two bytes to specified slave
// Note for HMC6352 compass only:
// Used with 'r' and 'g' commands
//  For 'r' and 'g' commands, I2C_Recv() should also be called
// Note for TMP102 thermometer only:
// Used to change the top byte of the contents of the pointer register
//  This will work but is probably not what you want to do.
// Returns 0 if successful, nonzero if error
uint32_t I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2){
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C0_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN    // master disable
                        );   
                                          // return error bits if nonzero
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C0_MDR_R = data2&0xFF;         // prepare second byte
  I2C0_MCS_R = (0
                      // & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                      // & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}
// sends three bytes to specified slave
// Note for HMC6352 compass only:
// Used with 'w' and 'G' commands
// Note for TMP102 thermometer only:
// Used to change the contents of the pointer register
// Returns 0 if successful, nonzero if error
uint32_t I2C_Send3(int8_t slave, uint8_t data1, uint8_t data2, uint8_t data3){
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                     //  & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C0_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN   // master disable
                       );   
                                          // return error bits if nonzero
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C0_MDR_R = data2&0xFF;         // prepare second byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                     //  & ~I2C_MCS_STOP    // no stop
                     //  & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C0_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN   // master disable
                        );
                                          // return error bits if nonzero
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C0_MDR_R = data3&0xFF;         // prepare third byte
  I2C0_MCS_R = (0
                    //   & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                    //   & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}

//below are helper functions to be implemented to control the component
//the addresss and data values are dummy values and need to be exchanged
//to the specific values

void setMode(uint8_t mode){
	temp = I2C_Send1(0x57,MODE_CONF);
	int8_t currentMode = I2C_Recv(0x57);
	currentMode = (currentMode & 0xF8)| mode;
	flag = I2C_Send2(0x57, MODE_CONF,currentMode);
	int i = 8 +8;
}

void setSamplingRate (uint8_t rate){
	temp = I2C_Send1(0x57,SPO2_CONF);
	int8_t Spo2Conf = I2C_Recv(0x57);
	Spo2Conf = (Spo2Conf &0xE3) | (rate <<2);

	flag = I2C_Send2(0x57,SPO2_CONF, Spo2Conf);
	
	int i = 8 +8;
}

void setLEDPulseWidth (uint8_t LEDpw){
	temp = I2C_Send1(0x57,SPO2_CONF);
	int8_t Spo2Conf = I2C_Recv(0x57);
	Spo2Conf = ((Spo2Conf &0xFC) | LEDpw);
	flag = I2C_Send2(0x57,SPO2_CONF, Spo2Conf);
	temp = I2C_Send1(0x57,SPO2_CONF);
	Spo2Conf = I2C_Recv(0x57);
	int i = 8 +8;
}

void setSamplingAverage(uint8_t sampAVG){
	temp = I2C_Send1(0x57,SAMP_AVG_CONF);
	uint8_t sAVG = I2C_Recv(0x57);
	sampAVG = (sAVG &0x18) | (sampAVG<<5);
	sampAVG |= (1<<4);
	flag = I2C_Send2(0x57,SAMP_AVG_CONF, sampAVG);
}

void setADCRangeControl(uint8_t rangeContr){
	temp = I2C_Send1(0x57,SPO2_CONF);
	int8_t Spo2Conf = I2C_Recv(0x57);
	Spo2Conf = (Spo2Conf &0x9F) | (rangeContr <<5);
	flag = I2C_Send2(0x57,SPO2_CONF, Spo2Conf);
	int i = 8 +8;
}


void setLEDCurrent(uint8_t redCurrent, uint8_t IRCurrent){
	int8_t flag1 = I2C_Send2(0x57,LED1PA, redCurrent);
	int8_t flag2 = I2C_Send2(0x57,LED2PA, IRCurrent);
	flag= I2C_Send2(0x57,0x10,redCurrent);
	int i = 8 +8;
}

void enableSlots(void){

	temp = I2C_Send1(0x57,LED_CONF_1);
	uint8_t LedCon= I2C_Recv(0x57);
	LedCon = ((LedCon & 0xF8)|0x1);
	flag = I2C_Send2(0x57,LED_CONF_1,LedCon);
	
	temp = I2C_Send1(0x57,LED_CONF_1);
	LedCon = I2C_Recv(0x57);
	LedCon = ((LedCon & 0x8F) | 0x2<<4);
	flag = I2C_Send2(0x57,LED_CONF_1,LedCon);
}

void enableProxThresh(void){
	flag = I2C_Send2(0x57,PROX_INT_THRESH,0x1);
}

void registerDebugger(void){
	temp = I2C_Send1(0x57,MODE_CONF);
	uint8_t currentMode = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,SPO2_CONF);
	uint8_t Spo2Conf = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,SAMP_AVG_CONF);
	uint8_t sAVG = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,LED1PA);
	uint8_t LED1current = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,LED2PA);
	uint8_t LED2current = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,0x10);
	uint8_t proxLEDAmp =I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,PROX_INT_THRESH);
	uint8_t proxThresh = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,FIFO_DATA);
	uint8_t fidata = I2C_Recv(0x57);
	uint8_t i = 8+8;
	

}



int8_t filterData(int8_t data){
//edit raw data in any fashion here
	return 0;
}

uint32_t I2C_Send1_RepStart(int8_t slave, uint8_t data1){
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                    //   & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}

void clearFifo(void){
	flag = I2C_Send2(0x57,FIFO_READ_PTR, 0);
	flag = I2C_Send2(0x57,FIFO_WRITE_PTR,0);
	flag = I2C_Send2(0x57,FIFO_OVERFLOW_CTR,0);
}

//reads from the read ptr an returns value
uint8_t readFromRD_PTR(void){
	int8_t temp = I2C_Send1(0x57,FIFO_READ_PTR);
	uint8_t byte = I2C_Recv(0x57);
	
	temp = I2C_Send1(0x57,byte);
	byte =  I2C_Recv(0x57);
	
	return byte;
}
//returns 1 byte from input regAddr of MAX30105
uint8_t readFromR(uint16_t regAddr){
	int8_t temp = I2C_Send1(0x57,regAddr);
	uint8_t byte = I2C_Recv(0x57);
	return byte;
}

uint8_t diffPTR(uint8_t wPTR, uint8_t rPTR){
	//wrap around has not occured
	if(wPTR > rPTR){
		return wPTR - rPTR;
	//wrap around has occured
	}else if(wPTR ==0 && rPTR==0){
		return 0;
	}else{
		return (32-rPTR)+wPTR;
	}
}

void readFromFifo(void){
	//clearFifo();															//good
	
	temp = I2C_Send1(0x57,FIFO_WRITE_PTR);
	uint8_t wPTR = I2C_Recv(0x57);
	temp = I2C_Send1(0x57,FIFO_READ_PTR);
	uint8_t rPTR = I2C_Recv(0x57);
	
	
	uint8_t numSamples= diffPTR(wPTR,rPTR);
	ST7735_DrawChar(80,80,wPTR/10+'0',ST7735_GREEN,ST7735_BLACK,3);
	ST7735_DrawChar(95,80,wPTR%10+'0',ST7735_GREEN,ST7735_BLACK,3);
	
	ST7735_DrawChar(80,100,rPTR/10+'0',ST7735_YELLOW,ST7735_BLACK,3);
	ST7735_DrawChar(95,100,rPTR%10+'0',ST7735_YELLOW,ST7735_BLACK,3);
	
	ST7735_DrawChar(80,120,numSamples/10+'0',ST7735_RED,ST7735_BLACK,3);
	ST7735_DrawChar(95,120,numSamples%10+'0',ST7735_RED,ST7735_BLACK,3);
	
	int8_t temp = I2C_Send1(0x57,FIFO_DATA);

	
	uint8_t buff[3];
	uint8_t data1;
	uint8_t slave = 0x57;
	uint8_t counter = 0;
	for(uint8_t j =0; j < numSamples; j++){
		for(uint8_t i=0; i < 2; i++){
			for(uint8_t h=0; h<3; h++){
					//temp = I2C_Send1(0x57,FIFO_DATA);
				if(counter < (numSamples*6-1)){
					while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R |= 0x01;              // MSA[0] is 1 for receive
    I2C0_MCS_R = (0
                         | I2C_MCS_ACK      // positive data ack
                       //  & ~I2C_MCS_STOP    // no stop
                         | I2C_MCS_START    // generate start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    buff[h] = (I2C0_MDR_R&0xFF);       // MSB data sent first
		
		}else{
			I2C0_MCS_R = (0
                       //  & ~I2C_MCS_ACK     // negative data ack (last byte)
                         | I2C_MCS_STOP     // generate stop
                       //  & ~I2C_MCS_START   // no start/restart
                         | I2C_MCS_RUN);    // master enable
			 while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
       buff[h] = (I2C0_MDR_R&0xFF);       // LSB data sent last
		}
	
			counter++;
			
			
			}
			samples[j][i]= (buff[0]<<13) | (buff[1]<<5) | (buff[2]>>3);
		}
	}
	
	

}

int8_t getHeartBeat(void){
	readFromFifo();
	//filteredIR = filterData(rawData[0],filteredIR[0],0 );
	//data = filterData(data);
	return 0;
}


void displayIRList(void){
	//50,000
	uint32_t value = samples[0][0];
	char c1 = value / 10000+'0';
	value = value % 10000;
	char c2 = value/1000+'0';
	value = value % 1000;
	char c3 = value/100+'0';
	value = value % 100;
	char c4 = value /10+'0';
	char c5 = value % 10+'0';
	
	char IRstring[5] = {c1,c2,c3,c4,c5};
	ST7735_DrawString(2,2,IRstring,ST7735_GREEN);
	
	value = samples[0][1];
	IRstring[0] = value / 10000+'0';
	value = value % 10000;
	IRstring[1] = value/1000+'0';
	value = value % 1000;
	IRstring[2] = value/100+'0';
	value = value % 100;
	IRstring[3] = value /10+'0';
	IRstring[4] = value % 10+'0';
	ST7735_DrawString(2,3,IRstring,ST7735_YELLOW);
	
	
	value = samples[1][0];
	IRstring[0] = value / 10000+'0';
	value = value % 10000;
	IRstring[1] = value/1000+'0';
	value = value % 1000;
	IRstring[2] = value/100+'0';
	value = value % 100;
	IRstring[3] = value /10+'0';
	IRstring[4] = value % 10+'0';
	ST7735_DrawString(2,4,IRstring,ST7735_GREEN);
	
	value = samples[1][1];
	IRstring[0] = value / 10000+'0';
	value = value % 10000;
	IRstring[1] = value/1000+'0';
	value = value % 1000;
	IRstring[2] = value/100+'0';
	value = value % 100;
	IRstring[3] = value /10+'0';
	IRstring[4] = value % 10+'0';
	ST7735_DrawString(2,5,IRstring,ST7735_YELLOW);
	
}

//dummy function to display raw ir list
void rawIRList(void){
	readFromFifo();
	displayIRList();
}



//alpha should be around 0.95
void dcRemoval(float x, float prev_w, float alpha){
	dcFilter_t[0] = x + alpha * prev_w;
	dcFilter_t[1]=dcFilter_t[0]-prev_w;
}

void meanDiff(float M){
	float avg =0;
	sumMDF -= values[indexMDF];
	values[indexMDF]=M;
	sumMDF += values[indexMDF];
	
	indexMDF++;
	indexMDF = indexMDF % 32;			//index = index % sixe of values array
	if(countMDF < 32){
		countMDF++;
	}
	
	avg = sumMDF/countMDF;
	//return avg - M;
}

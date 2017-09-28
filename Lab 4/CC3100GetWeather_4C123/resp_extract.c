/*
 *	resp_extract.c
 *	Juliana Pulido and Enrique Perez-Osborne
 *	September 27, 2017
 *
 */

#include "resp_extract.h"
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

/*********************** extract *******************************
 *	This function extracts information from buffer and creates a
 *	fixed length string to display information
 *
 */
 
 #define NUM 7			//length of the temp array
 #define LENGTH 15	//length of data saved
 

 void extract(char string[]){
	 char my_text[] = "\"temp\":";
 
	int i = 0;		//index for mytext
	int j = 0;		//index for buffer
	int k = 8; 	//index to start writing info
				
	 
	while (i!= NUM){
		if( my_text[i] == string[j]){
			i++;
			j++;			
		}
		else{
			i = 0;	//reset my_text index
			j++;	
		}
	}
	
	j++;		//enters the first digit of the information value
	char info[LENGTH] = "Temp = ";
	k=8;
	while(string[j] != ','){
		info[k] = string[j];
		k++;
		j++;		
	}
		
	//need to set cursor before calling extract function
	ST7735_OutString(info);
 }
 
 


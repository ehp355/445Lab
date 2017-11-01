/*Music.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/

#include <stdint.h>
#include <stdio.h>
#include "Music.h"


#define QUARTER	25000000*2
#define EIGHTH	12500000*2
#define SIXTEENTH 6250000*2
#define DOTQUARTER	37500000*2
#define DOTSIXTEENTH	9375000*2

uint16_t songIndex =0;	//index for nArr

//array with values for each note's frequency (periodic reload) 
//and length (oneshot reload)
struct Note nArr[] ={{3551, DOTSIXTEENTH},{3163,SIXTEENTH},{3551, SIXTEENTH},{4223,DOTQUARTER},
														{3551, DOTSIXTEENTH},{3163,SIXTEENTH},{3551, SIXTEENTH},{4223,DOTQUARTER},
														{2367, QUARTER},{2367,EIGHTH},{2820,DOTQUARTER}, //end of first bar
														{2657, QUARTER},{2657,EIGHTH},{3551,DOTQUARTER},
														{3163,QUARTER},{3163,EIGHTH},{2657,DOTSIXTEENTH},{2820,SIXTEENTH},{3163,SIXTEENTH},
														{3551, DOTSIXTEENTH},{3163,SIXTEENTH},{3551, SIXTEENTH},{4223,DOTQUARTER}, //end of second bar
														{3163,QUARTER},{3163,EIGHTH},{2657,DOTSIXTEENTH},{2820,SIXTEENTH},{3163,SIXTEENTH},
														{3551, DOTSIXTEENTH},{3163,SIXTEENTH},{3551, SIXTEENTH},{4223,DOTQUARTER},
														{2367,QUARTER},{2367,EIGHTH},{1993,DOTSIXTEENTH},{2367,SIXTEENTH},{2820,SIXTEENTH}, //end of 3rd bar
														{2657,DOTQUARTER},{2111,DOTQUARTER},
														{2657,SIXTEENTH},{3551, SIXTEENTH},{4223,SIXTEENTH},{ 3551, DOTSIXTEENTH},{3986,SIXTEENTH},{4734,SIXTEENTH},
														{5314,43750000}};

struct Note getSong(void){
	return nArr[songIndex];
}

void nextNote(void){
	if(songIndex == 46){
		songIndex = 0;
	}
	else{
	songIndex++;
	}
}

void reset_Index(void){
	songIndex=0;
}

/*Music.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/
#include <stdint.h>
#include <stdio.h>



//Note struct contains reload values for a note's frequency
//and length to be played
struct Note {
	uint32_t periodicReload;
	uint32_t oneShotReload;
};

//Song struct contains array of all the different note structs
//for Silent Night
struct Song{
	struct Note noteArr[46]; 
	
};

//Returns Note struct values for a certain note
struct Note getSong(void);
void nextNote(void);


void reset_Index(void);
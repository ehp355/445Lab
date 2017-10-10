/*Music.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/3/17
*/
#include <stdint.h>
#include <stdio.h>




struct Note {
	uint32_t periodicReload;
	uint32_t oneShotReload;
};

struct Song{
	struct Note noteArr[46]; 
	
};

struct Note getSong(void);
void nextNote(void);


void reset_Index(void);
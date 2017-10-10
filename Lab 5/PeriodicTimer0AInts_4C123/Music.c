/*Music.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/3/17
*/

#include <stdint.h>
#include <stdio.h>
#include "Music.h"


#define QUARTER	25000000*2
#define EIGHTH	12500000*2
#define SIXTEENTH 6250000*2
#define DOTQUARTER	37500000*2
#define DOTSIXTEENTH	9375000*2




const struct Note A_Dot_16th = { 3550, DOTSIXTEENTH};

//A_Dot_16th.periodicReload=3550;
//A_Dot_16th.oneShotReload=DOTSIXTEENTH;

struct Note B_16th= {3150,SIXTEENTH};
//B_16th.periodicReload=3150;
//B_16th.oneShotReload=SIXTEENTH;

struct Note A_16th={3550, SIXTEENTH};
//A_16th.periodicReload=3550;
//A_16th.oneShotReload=SIXTEENTH;

struct Note F_Dot_Quarter={4475,DOTQUARTER};
//F_Dot_Quarter.periodicReload=4475;
//F_Dot_Quarter.oneShotReload=DOTQUARTER;

struct Note E_Sharp_Quarter={2237, QUARTER};
//E_Sharp_Quarter.periodicReload=2237;
//E_Sharp_Quarter.oneShotReload = QUARTER;

struct Note E_Sharp_8th={2237,EIGHTH};
//E_Sharp_8th.periodicReload = 2237;
//E_Shapr_8th.oneShotReload = EIGHTH;

struct Note C_Dot_Quarter={5965,DOTQUARTER};
//C_Dot_Quarter.periodicReload=5965;
//C_Dot_Quarter.oneShotReload=DOTQUARTER;
//End of First Bar

//Beg of second bar
struct Note D_Sharp_Quarter={5022, QUARTER};
//D_Sharp_Quarter.periodicReload = 5022;
//D_Sharp_Quarter.oneShotReload = QUARTER;

struct Note D_Sharp_Eighth={5022,EIGHTH};
//D_Sharp_Eighth.periodicReload = 5022;
//D_Sharp_Eighth.oneShotReload = EIGHTH;

struct Note A_Dot_Quarter={3550,DOTQUARTER};
//A_Dot_Quarter.periodicReload = 3550;
//A_Dot_Quarter.oneShotReload = DOTQUARTER;

struct Note B_Quarter={3150,QUARTER};
//B_Quarter.periodicReload = 3150;
//B_Quarter.oneShotReload = QUARTER;

struct Note B_Eighth={3150,EIGHTH};
//B_Eighth.periodicReload = 3150;
//B_Eighth.oneShotReload = EIGHTH;

struct Note D_Sharp_Dot_16th={5022,DOTSIXTEENTH};
//D_Sharp_Dot_16th.periodicReload = 5022;
//D_Sharp_Dot_16th.oneShotReload = DOTSIXTEENTH;

struct Note C_16th={5965,SIXTEENTH};
//C_16th.periodicReload=5965;
//C_16th.oneShotReload=SIXTEENTH;

//3rd bar, 3rd column
struct Note G_Sharp_Dot_16th={3762,DOTSIXTEENTH};
//G_Sharp_Dot_16th.periodicReload=3762;
//G_Sharp_Dot_16th.oneShotReload=DOTSIXTEENTH;

struct Note E_Sharp_16th={2237,SIXTEENTH};
//E_Sharp_16th.periodicReload = 2237;
//E_Sharp_16th.oneShotReload = SIXTEENTH;

struct Note D_Sharp_Dot_Quarter={5022,DOTQUARTER};
//D_Sharp_Dot_Quarter.periodicReload=5022;
//D_Sharp_Dot_Quarter.oneShotReload=DOTQUARTER;

struct Note F_Sharp_Dot_Quarter={4223,DOTQUARTER};
//F_Sharp_Dot_Quarter.periodicReload=4223;
//F_Sharp_Dot_Quarter.oneShotReload=DOTQUARTER;

struct Note D_Sharp_16th={5022,SIXTEENTH};
//D_Sharp_16th.periodicReload=5022;
//D_Sharp_16th.oneShotTimeReload=SIXTEENTH;

struct Note F_16th={4475,SIXTEENTH};
//F_16th.periodicReload=4475;
//F_16th.oneShotReload=SIXTEENTH;

struct Note G_16th={3985,SIXTEENTH};
//G_16th.periodicReload=3985;
//G_16th.oneShoteReload=SIXTEENTH;

struct Note E_16th={4740,SIXTEENTH};
//E_16th.periodicReload=4740;
//E_16th.oneShotReload=SIXTEENTH;

struct Note D_Dot_LONG={5315,43750000};
//D_Dot_LONG.periodicReload=5315;
//D_Dot_LONG.oneShotReload=43750000;

uint16_t songIndex =0;



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

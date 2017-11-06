/*Sound.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
*/

//Initialize timer for sound
void Timer0A_Init(void);


//sounds alarm when hb enters Alert state
void soundAlarm(void);

//Stops alarm when any button is pressed
void endAlarm(void);
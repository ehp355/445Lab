/*Switches.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 10/3/17
 *TA: Cody Horton
 *Last Edited: 10/10/17
*/

void EdgeTriggered_Init(void);
void EdgeTrigger_Handler(void);

//Play functin plays music starting from the
//song array's index.
void Play(void);

//Stop function stops the music but maintains
//the song array's index.
void Stop(void);

//Rewind function stops the music and resets the
//song array's index to play from the start next
//time Play is pressed.
void Rewind(void);
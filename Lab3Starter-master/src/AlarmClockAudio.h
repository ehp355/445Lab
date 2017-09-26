/* ******** AlarmClockAudio.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** LCD interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/20/17
*/

/****************soundAlarm***************
When called, sounds alarm to speaker connected
to microcontroller
inputs: none
outputs: none
*/ 

void PortC_Init(void);
void Timer1A_Init(void);
void soundAlarm(void);
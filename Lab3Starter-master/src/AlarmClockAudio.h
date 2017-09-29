/* ******** AlarmClockAudio.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** LCD interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/28/17
*/

/****************soundAlarm***************
When called, sets TIMER1_CTL_R so that 
the alarm sound ISR is called 
inputs: none
outputs: none
*/ 
void soundAlarm(void);

/****************PortC_Init***************
Initializes portC so that PC4 is output
inputs: none
outputs: none
*/
void PortC_Init(void);

/****************Timer1A_Init***************
Initializes Timer1A which will be used for the
square signal output to PC4 for the speaker(alarm sound)
inputs: none
outputs: none
*/
void Timer1A_Init(void);

/****************Timer1A_Handler***************
outputs the flickering signal to PC4 to sound
the alarm 
inputs: none
outputs: none
*/
void Timer1A_Handler(void);

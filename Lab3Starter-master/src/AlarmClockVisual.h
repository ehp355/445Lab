/* ******** AlarmClockVisual.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** LCD interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/20/17
*/

/****************paintClockFace***************
Fills screen with bmp of clock face
inputs: none
outputs: none
 */ 
void paintClockFace(void);

/****************drawClockHand***************
Updates screen by redrawing paintClockFace and
drawing minute hand and hour hand based on input
inputs: hour time and minute time(int32_t)
outputs: none
*/ 
 void drawClockHand(uint32_t hour, uint32_t minute);
 
/****************paintDigitalClock***************
Displays the digital version of the clock based
on the inputs.
inputs: hour time and minute time(int32_t)
outputs: none
*/ 
void paintDigitalClock(uint32_t hour, uint32_t minute);


 
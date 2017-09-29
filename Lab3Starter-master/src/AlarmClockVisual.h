/* ******** AlarmClockVisual.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** LCD interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/28/17
*/

/****************startClock***************
Sets up screen for boot up, and starts
with the clock face at 12:00
inputs: none
outputs: none
 */ 
void startClock(void);

/****************paintClockFace***************
Fills screen with bmp of clock face
inputs: none
outputs: none
 */ 
void paintClockFace(void);

/****************drawClockHand***************
Updates screen by drawing over previous hand positions
and drawing new minute hand and hour hand based on 
input
inputs: hour time and minute time(int32_t)
outputs: none
NOTE: Due to the crude line function used, some minute hand
and hour hand positions are shared by adjacent time periods
Example: the line drawn for to show the minute hand at 6 minutes
is the same as the line drawn to show 8 minutes
*/ 
 void drawClockHand(uint32_t hour, uint32_t minute);
 
/****************paintDigitalClock***************
Displays the digital version of the clock based
on the inputs.
inputs: hour time and minute time(int32_t)
outputs: none
*/ 
void paintDigitalClock(uint32_t hour, uint32_t minute, uint32_t hourColor, uint32_t minColor);

/****************alarmEnabled***************
Draws the alarm time on the screen in paranthesis
in the color red to notify the user that the alarm
is enabled.
inputs: hour time and minute time(int32_t)
outputs: none
*/ 
void alarmEnabled(uint32_t hour,uint32_t min);
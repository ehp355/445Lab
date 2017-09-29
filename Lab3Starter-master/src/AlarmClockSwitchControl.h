/* ******** AlarmClockSwitchControl.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** Switch interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/28/17
*/

/****************switchDisplay***************
Toggles ClockFace and Digital Clock
inputs: none
outputs: none
*/ 
void switchDisplay(void);

/****************runClockFace***************
Places the main thread of code in a state so 
that it us contiuously updating the analog face 
of the clock until one of the four buttons is pressed
inputs: none
outputs: none
*/
void runClockFace(void);

/****************runDigital***************
Places the main thread of code in a state so 
that it us contiuously updating the digital 
clock until one of the four buttons is pressed
inputs: none
outputs: none
*/
void runDigitalClock(void);

/****************settingTime***************
Places the main thread of code in a state so 
that the user may set the time of the alarm
or of the clock. This is decided by the global
variable for_Alarm that is set based on which
option the user chose by using the buttons
inputs: none
outputs: none
*/
void settingTime(void);

/****************checkAlarm***************
Helper function to check if the current time
is equivalent to the alarm time
inputs: none
outputs: none
*/
void checkAlarm(void);

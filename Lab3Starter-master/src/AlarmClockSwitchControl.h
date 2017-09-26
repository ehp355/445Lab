/* ******** AlarmClockSwitchControl.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
** Switch interface for Alarm Clock
** Lab3
** TA: Cody Horton
** Last Modified: 9/20/17
*/



/****************setAlarm***************
Sets clock so that the alarm goes off at
the given time
inputs: hour time and minute time(int32_t)
outputs: none
*/ 
uint32_t setAlarm(uint32_t currentAlarm);

/****************turnOffAlarm***************
Turns off the alarm after it begins going
off
inputs: none
outputs: none
*/ 
void turnOffAlarm(void);

/****************snoozeAlarm***************
Snoozes alarm after it begins going off
inputs: none
outputs: none
*/ 
void snoozeAlarm(void);

/****************enableAlarm***************
Enables for the alarm to go off, based on
the time the alarm was set
inputs: none
outputs: none
*/ 
void enableAlarm(void);

/****************disableAlarm***************
Disables for the alarm to go off, based on
the time the alarm was set
inputs: none
outputs: none
*/ 
void disbleAlarm(void);

/****************switchDisplay***************
Toggles ClockFace and Digital Clock
inputs: none
outputs: none
*/ 
void switchDisplay(void);

void runClockFace(void);
void runDigitalClock(void);
void settingTime(uint16_t hour, uint16_t min, uint8_t forAlarm);
void checkAlarm(void);

/*Tacho.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
//Periodic Timer Init to periodically call SpeedControl()
void Timer2A_Init(long period);
//------------TimerCapture_Init------------
// Initialize Timer0A in edge time mode to request interrupts on
// the rising edge of PB0 (CCP0).  The interrupt service routine
// acknowledges the interrupt and calls a user function.
// Input: task is a pointer to a user function
// Output: none
void TimerCapture_Init(void);
//called to increase desired speed by 5.0 rps
void Speed_Down(void);
//called to decrease desired speed by 5.0 rps
void Speed_Up(void);
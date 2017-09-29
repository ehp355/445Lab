
/* ******** FalloingEdgeInterrupt.h ************** 
** Enrique Perez-Osborne ehp355
** Juliana Pulido 
** Created: 9/20/17
**EdgeInterrupt Service Routine
**for when buttons are pressed
** Lab3
** TA: Cody Horton
** Last Modified: 9/28/17
*/


/****************Timer0A_Init***************
Initializes Timer0A as one-shot timer for 
debouncing buttons
inputs: none
outputs: none
*/
void Timer0A_Init(void);

/****************Timer0A_Handler***************
Handler for Timer0A, re-arms buttons
inputs: none
outputs: none
*/
void Timer0A_Handler(void);
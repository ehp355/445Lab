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

/****************Edge_Init***************
Initializes portB for rising edge interrupt
for when PB1-PB4
inputs: none
outputs: none
*/ 
void Edge_Init(void);

/****************GPIOPortB_Handler***************
Called when any PB1-PB4 experience a rising edge
inputs: none
outputs: none
*/
void GPIOPortB_Handler(void);
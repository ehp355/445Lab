/*Visual.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/29/17
 */
void displayDesiredRPS(uint32_t drps);
void displayActualRPS(uint32_t arps);
//Initializes LCD screen for sweping graph visual provided by Valvano
void plotRPSInit(void);
//called continuously in main to update sweeping graph on LCD
void plotRPS(uint32_t j,uint32_t N, uint32_t fs,uint32_t DesiredRPS,uint32_t ActualRPS, uint32_t period);
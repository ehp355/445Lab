/*Visual.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/14/17
 */
 
 //input:tmeperature value, decimal temperature value, ADC value
 //output: none
 //redraws screen with latest read temp from FIFO
void updateScreen(uint8_t tempWhole, uint8_t tempDec,uint16_t rawData);

//input: ADC value
//output: whole temperature value
uint8_t fixedPoint(uint16_t rawData);

//input: ADC value
//output: decimal temperature value
uint8_t fixedPointDecimal(uint16_t rawData);
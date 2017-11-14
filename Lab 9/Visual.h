/*Visual.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/14/17
 */
void updateScreen(uint8_t tempWhole, uint8_t tempDec,uint16_t rawData);

uint8_t fixedPoint(uint16_t rawData);

uint8_t fixedPointDecimal(uint16_t rawData);
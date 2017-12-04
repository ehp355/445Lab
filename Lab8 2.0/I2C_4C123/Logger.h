/*Logger.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 12/3/17
 *TA: Cody Horton
 *Last Edited: 12/3/17
*/

void Log_Init(void);
void scrollUp(void);
void scrollDown(void);
void logToArray(uint16_t value);
void resetTopIndex(void);
void blinkTopScroller(void);
void blinkBottomScroller(void);
void killScrollers(void);
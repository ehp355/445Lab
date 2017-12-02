/*Visual.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *Creation Date: 11/1/17
 *TA: Cody Horton
 *Last Edited: 11/11/17
 */
 //displays heartbeat state to LCD
 void displayHB(uint32_t heartBeat);
 
 void display2DigitNumber(uint32_t num);
 void erase2DigitNumber(uint32_t num);
 
 //displays Alert state to LCD
 void displayAlert(void);
 
 //diplay Log state to LCD
 void displayLog(void);
 
 void displayWord(void);
 
 //dummy functions for testing software modules
 void displayStateOne(void);
 void displayStateTwo(void);
 void displayStateThree(void);
 void displayStart(void);

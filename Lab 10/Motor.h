/*Motor.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
void InitMotor(uint32_t var);
//cuts motor off
void stopMotor(void);
//sets Motor to specific speed and direction
//based on input
void setMotor(uint32_t power);
//set Motor to full speed
void fullPWRMotor(void);

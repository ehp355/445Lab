/*Tacho.h
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/16/17
 */
//Initialize Tachometer
void InitTachometer(uint32_t var);
//return tachometer reading
uint32_t readTacho(void);
void SpeedControl(void);
void PeriodMeasure_Init(void);
void Timer2A_Init(long period);

void Speed_Down(void);
void Speed_Up(void);
/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the Timer1 driver
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"
typedef enum
{
	NO_CLK,NO_PRESCALER,PRESCALER_8,PRESCALER_64,PRESCALER_256,PRESCALER_1024,EXTERNAL_CLK_FALLING,EXTERNAL_CLK_RISING
}Timer1_Prescaler;

typedef enum
{
	NORMAL, CTC=4
}Timer1_Mode;


typedef struct {
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
} Timer1_ConfigType;





void Timer1_init(const Timer1_ConfigType *Config_Ptr);

void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));



#endif /* TIMER1_H_ */

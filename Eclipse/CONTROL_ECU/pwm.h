/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the PWM driver
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"

/*start pwm mode for timer0*/
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */

/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dcmotor.c
 *
 * Description: Source file for the DC Motor driver
 *
 *******************************************************************************/

#include "dcmotor.h"
#include "pwm.h"
#include "gpio.h"


void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_PORT1_ID,DC_MOTOR_PIN1_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT2_ID,DC_MOTOR_PIN2_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(ENABLE_PORT, ENABLE_PIN, PIN_OUTPUT);

	/* Motor is stopped at the beginning */
	GPIO_writePin(DC_MOTOR_PORT1_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT2_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
	GPIO_writePin(ENABLE_PORT, ENABLE_PIN, LOGIC_HIGH);
}


void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	if(state == DC_MOTOR_CW)
	{
		/* Rotates the Motor CW */
		GPIO_writePin(DC_MOTOR_PORT1_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID,DC_MOTOR_PIN2_ID,LOGIC_HIGH);
		PWM_Timer0_Start(speed);
	}
	else if(state == DC_MOTOR_ACW)
	{
		/* Rotates the Motor A-CW */
		GPIO_writePin(DC_MOTOR_PORT1_ID,DC_MOTOR_PIN1_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT2_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
		PWM_Timer0_Start(speed);
	}
	else if(state == DC_MOTOR_STOP)
	{
		/* Stop the Motor */
		GPIO_writePin(DC_MOTOR_PORT1_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
		PWM_Timer0_Start(speed);
	}

}

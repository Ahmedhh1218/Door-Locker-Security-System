/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dcmotor.h
 *
 * Description: Header file for the DC Motor driver
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* The Ports and Pins IDs for the DC-Motor two pins */
#define DC_MOTOR_PORT1_ID   PORTD_ID
#define DC_MOTOR_PORT2_ID   PORTD_ID
#define ENABLE_PORT         PORTD_ID

#define DC_MOTOR_PIN1_ID    PIN2_ID
#define DC_MOTOR_PIN2_ID    PIN3_ID
#define ENABLE_PIN          PIN4_ID
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	DC_MOTOR_STOP,DC_MOTOR_CW,DC_MOTOR_ACW
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */

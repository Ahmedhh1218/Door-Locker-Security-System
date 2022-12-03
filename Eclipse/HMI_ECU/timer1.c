/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the Timer1 driver
 *
 *******************************************************************************/
#include "timer1.h"
#include "common_macros.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
static volatile void(*g_callback_ptr) (void)=NULL_PTR;

ISR(TIMER1_COMPA_vect)
{
	if(g_callback_ptr != NULL_PTR)
	{
		(*g_callback_ptr)();
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_callback_ptr != NULL_PTR)
	{
		(*g_callback_ptr)();
	}
}
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT1=Config_Ptr->initial_value;
	OCR1A=Config_Ptr->compare_value;
	TCCR1A= (1<<FOC1A)|(1<<FOC1B);
	TCCR1B= (0xFF & Config_Ptr->prescaler);
	if(Config_Ptr->mode==NORMAL)
	{
		TCCR1A|= (Config_Ptr->mode);
		TIMSK|= (1<<TOIE1);
	}
	else if(Config_Ptr->mode==CTC)
	{
		TCCR1B|=(1<<WGM12);
		TIMSK|= (1<<OCIE1A);
	}
}

void Timer1_deInit(void)
{
	TCCR1B= NO_CLK;
	TCNT1=0;
	OCR1A=0;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callback_ptr=a_ptr;
}

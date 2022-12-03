/******************************************************************************
 *
 * Module: APPLICATION
 *
 * File Name: HMI.c
 *
 * Description: Source file for the Application layer of the HMI ECU
 *
 *******************************************************************************/
#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <util/delay.h>

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/

#define M1_READY			0x10
#define M2_READY 			0x20
#define Error_pass 			0x00
#define INITIALIZED_FLAG	0x01
#define SAVE 				0x02
#define END_SAVING 			0x03
#define CHECK_PASS 			0x04
#define UNLOCK_DOOR 		0x06

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint16 g_tick = 0;
uint8 g_calc_sec_flag=0;
uint8 counter_error =0;
uint8 return_flag=0;

void Calculate_time(void)
{
	g_tick ++;

/*
	if(g_tick ==1)
	{
		g_calc_sec_flag ++;
		g_tick =0;
	}*/
}



void New_Pass()
{

	uint8 key = 0,pass=0,count_pass;

	sint32 new_pass = 0, re_pass = 0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Enter Pass:");

	for(count_pass=0; count_pass<5; count_pass++)
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(50);

		while(!((key >= 0) && (key <= 9)))
		{
			key = KEYPAD_getPressedKey();
			_delay_ms(50);
		}
		LCD_displayStringRowColumn(1,count_pass,"*");
		_delay_ms(70);
		new_pass= new_pass*10 + key;
		_delay_ms(250);
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Press =");
	while(key!='=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
	}


	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Re-enter The");
	LCD_displayStringRowColumn(1,0,"Same Pass:");

	for(count_pass=0; count_pass<5; count_pass++)
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(50);

		while(!((key >= 0) && (key <= 9)))
		{
			key = KEYPAD_getPressedKey();
			_delay_ms(50);
		}
		LCD_displayStringRowColumn(1,count_pass+10,"*");
		_delay_ms(70);
		re_pass= re_pass*10 + key;
		_delay_ms(250);
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Press =");
	while(key!='=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
	}


	if(re_pass == new_pass)
	{
		UART_sendByte(SAVE);
		for(count_pass=0;count_pass<5;count_pass++)
		{
			pass = re_pass % 10;
			while(UART_recieveByte()!= M2_READY);
			UART_sendByte(pass);
			re_pass /= 10;
		}
		while(UART_recieveByte()!= END_SAVING);
	}
	else
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"NOT MATCHING");
		_delay_ms(500);
		New_Pass();
	}
}

void Check_Pass(const Timer1_ConfigType * Config_Ptr)
{
	sint32 pass=0;

	uint8 count_pass,key=0,pass_digit=0,error_pass=0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Enter pass:");

	for(count_pass=0; count_pass<5; count_pass++)
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(50);

		while(!((key >= 0) && (key <= 9)))
		{
			key = KEYPAD_getPressedKey();
			_delay_ms(50);
		}
		LCD_displayStringRowColumn(1,count_pass,"*");
		_delay_ms(70);
		pass= pass*10 + key;
		_delay_ms(250);
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Press =");
	while(key!='=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
	}


	UART_sendByte(CHECK_PASS);

	for(count_pass=0; count_pass<5; count_pass++)
	{
		pass_digit = pass % 10;
		UART_sendByte(pass_digit);

		if(UART_recieveByte() == Error_pass)
		{
			error_pass ++;
		}
		pass /= 10;
	}

	if(error_pass != 0)
	{
		counter_error ++;
		UART_sendByte(counter_error);

		if(counter_error < 3)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"INCORRECT PASS");
			_delay_ms(500);
			Check_Pass(Config_Ptr);
		}
		else
		{
			LCD_clearScreen();


			Timer1_init(Config_Ptr);
			g_tick =0;
			g_calc_sec_flag =0;
			while (g_tick != 60)
			{
				LCD_displayStringRowColumn(0,0,"Wait 60 Sec");
				LCD_displayStringRowColumn(1,0,"el7a2 7aramy");
			}

			counter_error = 0;
			return_flag =1;
		}
	}
	else
	{
		UART_sendByte(4);
		counter_error = 0;
	}
}
void Open_Door(const Timer1_ConfigType * Config_Ptr)
{
	UART_sendByte(UNLOCK_DOOR);

	LCD_clearScreen();
	Timer1_init(Config_Ptr);
	g_tick =0;
	g_calc_sec_flag =0;

	while(g_tick != 15)
	{
		LCD_displayStringRowColumn(0,0,"Opening Door..");
	}

	LCD_clearScreen();
	g_tick =0;
	g_calc_sec_flag =0;

	while(g_tick != 3)
	{
		LCD_displayStringRowColumn(0,0,"Door Opened");
	}

	LCD_clearScreen();
	g_tick =0;
	g_calc_sec_flag =0;

	while(g_tick!= 15)
	{
		LCD_displayStringRowColumn(0,0,"Closing Door..");
	}
}

void Main_Menu(const Timer1_ConfigType * Config_Ptr)
{
	uint8 key=0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	while((key != '+') && (key != '-'))
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(50);
	}

	Check_Pass(Config_Ptr);
	UART_sendByte(return_flag);


	if(return_flag == 0)
	{
		UART_sendByte(key);
		if (key == '-')
		{
			New_Pass();

		}
		else
		{
			Open_Door(Config_Ptr);
		}
	}
	else
	{
		return_flag=0;
	}

}
int main(void)
{



	SREG|=(1<<7);
	LCD_init();

	Timer1_ConfigType Timer_Config = {PRESCALER_1024,CTC,0,6000};
		Timer1_setCallBack(Calculate_time);

	UART_ConfigType UART_Config={BIT_DATA_8,DISABLED,ONE_BIT,9600};
	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&UART_Config);

	UART_sendByte(M1_READY);
	New_Pass();


	while(1)
	{




			Main_Menu(& Timer_Config);

	}
	return 0;
}

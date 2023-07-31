/*
* OCT23.c
*
* Created: 3/17/2023 3:45:07 PM
* Author : user
*/

#include <avr/io.h>
#include <stdlib.h>
#include "STD_Types.h"
#include "BitMath.h"
#include "Clock.h"
#include "DIO.h"
#include "LCD.h"
#include "Keypad.h"

#define MAX_EXPRESSION_LENGTH 20
int current_length = 0;

u8 expression[MAX_EXPRESSION_LENGTH] = "";

void processKey(u8 key);
void solve();

int main()
{
	DIO_Init();
	LCD_Init();
	Keypad_Init();
	while (1)
	{
		u8 key = GetKey();
		processKey(key);
	}
}

void processKey(u8 key)
{
	//Checking valid keys
	if(key >= '0' && key <='9')
	{
		//add digit to the expression
		if(current_length < MAX_EXPRESSION_LENGTH-1)
		{
			expression[current_length] = key;
			current_length++;
			expression[current_length] = '\0'; //Terminate string
			LCD_Char(key); //show the digit on LCD
		}
	}
	else if(key == '+' || key == '-' || key == '*' || key == '/')
	{
		//add the operators to the expression
		if(current_length < MAX_EXPRESSION_LENGTH-1)
		{
			expression[current_length] = key;
			current_length++;
			expression[current_length] = '\0';
			LCD_Char(key);
		}
	}
	else if(key == 'C')
	{
		expression[0] = '\0'; //clear expression
		current_length = 0; //reset counter
		LCD_Clear(); //clear LCD display
	}
	else if(key == '=')
	{
		//Evaluate the Equation
		solve();
	}
}

void solve()
{
	//Check if the expression is not empty first
	if(current_length > 0)
	{
		//parse expression
		int operands[MAX_EXPRESSION_LENGTH/2];
		u8 operators[MAX_EXPRESSION_LENGTH/2];
		int op_count = 0;
		int i = 0;
		
		//Extract operands and operators
		while (i < current_length)
		{
			int operand = 0;
			while (expression[i] >= '0' && expression[i] <= '9')
			{
				operand = operand * 10 + (expression[i] - '0');
				i++;
			}
			operands[op_count] = operand;
			op_count++;

			if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
			{
				operators[op_count - 1] = expression[i];
				i++;
			}
		}

		// Perform calculations
		int result = operands[0];
		for (int j = 0; j < op_count - 1; j++)
		{
			switch (operators[j])
			{
				case '+':
				result += operands[j + 1];
				break;
				case '-':
				result -= operands[j + 1];
				break;
				case '*':
				result *= operands[j + 1];
				break;
				case '/':
				result /= operands[j + 1];
				break;
			}
		}

		LCD_Clear();
		LCD_Int(result);
		current_length = 0;
		expression[0] = '\0';
		_delay_ms(1000);
		LCD_Clear();
	}
}
/*
 *@file		:	LCD_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program for LCD
 */

#include <stdint.h>
#include "../../LIBRARY/ErrTypes.h"
#include "../../LIBRARY/BitMath.h"
#include "../Inc/LCD_Interface.h"

#include "../../MCAL/Inc/RCC_Interface.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
#include "../../MCAL/Inc/SYSTICK_Interface.h"

static GPIO_REG_t* LCD_PORT = GPIOB;


/***********************************
 * @function 		:	LCD_voidSendCommand
 * @brief			:	Send Command to LCD
 * @parameter[in]	:	uint8_t Command
 * @retval			:	void
 */
void LCD_voidSendCommand(uint8_t Command)
{
	uint8_t Command_Reversed =0;      /*To hold reversed value of the command*/
	uint8_t Counter =0;
	uint32_t ODR_Value=0;             /*To hold ODR Value to assign it in one instruction*/
	uint8_t Bit=0;                    /*to Check on BIT*/

	/*Reversing the counter BITS to make it sustainable to blue bill */

	for (Counter=0;Counter<8;Counter++)
	{
		Bit = GET_BIT(Command,Counter);
		if(Bit==1)
			SET_BIT(Command_Reversed,(7-Counter));
		else if (Bit==0)
			CLR_BIT(Command_Reversed,(7-Counter));
	}

	/*SET RS to 0 to send Command*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_RS, LOW_V);

	/*Send Highest 4 bits of Command First*/
	LCD_PORT->ODR &= ~ ((LCD_MASK)<<12);

	ODR_Value = LCD_PORT->ODR | ((Command_Reversed&COMMAND_MASK)<<12);

	LCD_PORT->ODR =ODR_Value;

	/*Enable LCD To receive data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, HIGH_V);

	/*Waiting till data Sent*/
	SYSTICK_u8Delay_ms(2);

	/*Disable LCD After receiving data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, LOW_V);

	/*Send Lowest 4 bits of Command Secondly*/
	LCD_PORT->ODR &= ~ ((LCD_MASK)<<12);
	ODR_Value = LCD_PORT->ODR | (((Command_Reversed>>4)&COMMAND_MASK)<<12);

	LCD_PORT->ODR =ODR_Value;

	/*Enable LCD To receive data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, HIGH_V);

	/*Waiting till data Sent*/
	SYSTICK_u8Delay_ms(2);

	/*Disable LCD After receiving data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, LOW_V);

}


/***********************************
 * @function 		:	LCD_voidSendData
 * @brief			:	Send Data to LCD
 * @parameter[in]	:	uint8_t Data
 * @retval			:	void
 */
void LCD_voidSendData(uint8_t Data)
{
	uint8_t Command_Reversed =0;	/*To hold reversed value of the command*/
	uint8_t Counter =0;
	uint32_t ODR_Value=0;			/*To hold ODR Value to assign it in one instruction*/
	uint8_t Bit=0;					/*to Check on BIT*/

	/*Reversing the counter to make it sustainable to blue bill */
	for (Counter=0;Counter<8;Counter++)
	{
		Bit = GET_BIT(Data,Counter);
		if(Bit==1)
			SET_BIT(Command_Reversed,(7-Counter));
		else if (Bit==0)
			CLR_BIT(Command_Reversed,(7-Counter));
	}

	/*SET RS to 1 to send Data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_RS, HIGH_V);

	/*Send Highest 4 bits of Command First*/
	LCD_PORT->ODR &= ~ ((LCD_MASK)<<12);
	ODR_Value = LCD_PORT->ODR | ((Command_Reversed&COMMAND_MASK)<<12);
	LCD_PORT->ODR =ODR_Value;

	/*Enable LCD To receive data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, HIGH_V);

	/*Waiting till data Sent*/
	SYSTICK_u8Delay_ms(2);

	/*Disable LCD After receiving data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, LOW_V);

	/*Send Lowest 4 bits of Command Secondly*/
	LCD_PORT->ODR &= ~ ((LCD_MASK)<<12);
	ODR_Value = LCD_PORT->ODR | (((Command_Reversed>>4)&COMMAND_MASK)<<12);

	LCD_PORT->ODR =ODR_Value;

	/*Enable LCD To receive data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, HIGH_V);

	/*Waiting till data Sent*/
	SYSTICK_u8Delay_ms(2);

	/*Disable LCD After receiving data*/
	GPIO_u8SetPinStatus(LCD_CTRL_PORT, LCD_PIN_EN, LOW_V);

}


/***********************************
 * @function 		:	LCD_voidInit
 * @brief			:	Initialization for LCD
 * @parameter[in]	:	void
 * @retval			:	void
 * @warning 		:	- This function will work properly with BLUEBILL Kit.
 *						- This Function initializes LCD at 4BIT mode
 */
void LCD_voidInit(void)
{
	/*Enable System tick and configure it*/
	SYSTICK_Configs_t Sysconfig={
				.INT_State = SYSTICK_INT_Disabled,
				.SYSTICK_Clock = AHB_DIRECT,
				.SYSTICK_State = SYSTICK_ENABLED
		};
		SYSTICK_u8SetConfigs(&Sysconfig);


	/*Enable GPIO Clock*/
	RCC_Set_APB2Peripheral_CLK(APB2_IOPA, ENABLED);
	RCC_Set_APB2Peripheral_CLK(APB2_IOPB, ENABLED);


	/*Initialize LCD pins Array of structures with each pin configurations*/
	PinConfig_t LCD_Pins[6]={

			{ .Port_Num=LCD_CTRL_PORT, .Pin_Num=LCD_PIN_EN 	  , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}
			,	{ .Port_Num=LCD_CTRL_PORT, .Pin_Num=LCD_PIN_RS    , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}
			,	{ .Port_Num=LCD_DATA_PORT, .Pin_Num=LCD_DATA_PIN4 , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}
			,	{ .Port_Num=LCD_DATA_PORT, .Pin_Num=LCD_DATA_PIN5 , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}
			,	{ .Port_Num=LCD_DATA_PORT, .Pin_Num=LCD_DATA_PIN6 , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}
			,	{ .Port_Num=LCD_DATA_PORT, .Pin_Num=LCD_DATA_PIN7 , .Mode= OUTPUT_MSPEED,.Output_Type=GP_PUSH_PULL}

	};
	uint8_t Counter=0;
	/*Initialize LCD Pins*/
	for(Counter=0;Counter<6;Counter++)
	{
		GPIO_u8PinInit(&LCD_Pins[Counter]);
	}
	/*Apply initializing sequence on LCD from data sheet*/

	SYSTICK_u8Delay_ms(35);
	LCD_voidSendCommand(0x33);
	LCD_voidSendCommand(0x32);	/* Send for 4 bit initialization of LCD  */
	LCD_voidSendCommand(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	LCD_voidSendCommand(0x0f);	/* Display on cursor Blinking */
	LCD_voidSendCommand(0x06);	/* Increment cursor (shift cursor to right) */
	LCD_voidSendCommand(0x01);	/* Clear display screen */
}

/***********************************
 * @function 		:	LCD_u8SendString
 * @brief			:	Send String to LCD
 * @parameter[in]	:	const char * String
 * @retval			:	uint8_t ErrorState
 */
uint8_t LCD_u8SendString(const char * String)
{
	uint8_t Local_u8ErrorState = OK;
	uint8_t	Counter=0;
	/*Check The string if available*/
	if (String != NULL)
	{
		/*Loop on String until it finishes*/
		while (String[Counter] != '\0')
		{
			/*If first line ended Skip the hidden blocks until u reach the second line blocks*/
			if(Counter==16)
			{
				for(int i =0;i<24;i++)
				{
					LCD_voidSendData(' ');
				}
			}
			/*Display the current letter of the string*/
			LCD_voidSendData(String[Counter]);
			/*Move to the next letter*/
			Counter++;
		}

	}
	/*If String isn't available or not passed*/
	else {
		/*define a Null Pointer Error*/
		Local_u8ErrorState = Null_Pointer;
	}
	return Local_u8ErrorState;
}


/***********************************
 * @function 		:	LCD_u8SendNumber
 * @brief			:	Send Number to LCD
 * @parameter[in]	:	uint32_t Number
 * @retval			:	uint8_t ErrorState
 */
uint8_t LCD_u8SendNumber(uint32_t Number)
{
	uint8_t Local_u8ErrorState = OK;
	uint8_t	Counter=0;
	uint8_t NumberOfDigits=0;
	uint8_t NumberDigits[10]={0};
	/*Loop on Number Digits until it finishes*/
	while (Number != 0)
	{
		NumberDigits[Counter] = (Number%10)+48;
		Number = Number/10;
		Counter++;
		NumberOfDigits++;
	}
	/*Display digits of the number*/
	for (Counter=0;Counter<NumberOfDigits;Counter++)
	{
		LCD_voidSendData(NumberDigits[NumberOfDigits-Counter-1]);
	}
	return Local_u8ErrorState;
}

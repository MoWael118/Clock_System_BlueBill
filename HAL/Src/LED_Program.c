/*
 *@file		:	LED_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program body for LED Module
 */

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../../Inc/ErrTypes.h"
#include "../../../MCAL/GPIO/Inc/GPIO_Interface.h"
#include "../Inc/LED_Interface.h"
/*******************************************************/

/*
 * @function 		:	LED_ON
 * @brief			:	Set LED Pin to HIGH
 * @param			:	LED Configurations
 * @retval			:	Error State
 */
void LED_ON(PinConfig_t * LED_Configs)
{
	if (NULL != LED_Configs)
	{
		GPIO_u8PinInit(&LED_Configs);
		GPIO_u8SetPinStatus(LED_Configs->Port_Num, LED_Configs->Pin_Num, HIGH_V);
	}
	else
	{
		/*Error => Do Nothing*/
	}
}

/*
 * @function 		:	LED_OFF
 * @brief			:	Set LED Pin to LOW
 * @param			:	LED Configurations
 * @retval			:	Error State
 */
void LED_OFF(PinConfig_t * LED_Configs)
{
	if (NULL != LED_Configs)
	{
		GPIO_u8PinInit(&LED_Configs);
		GPIO_u8SetPinStatus(LED_Configs->Port_Num, LED_Configs->Pin_Num, LOW_V);
	}
	else
	{
		/*Error => Do Nothing*/
	}
}

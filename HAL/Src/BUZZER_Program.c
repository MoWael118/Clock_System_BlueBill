/*
 *@file		:	BUZZER_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program body for BUZZER Module
 */

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../Inc/BUZZER_Interface.h"
/*******************************************************/

/*
 * @function 		:	BUZZER_ON
 * @brief			:	Set BUZZER Pin to HIGH
 * @param			:	BUZZER_Configs
 * @retval			:	Error State
 */
void BUZZER_ON(PinConfig_t * BUZZER_Configs)
{
	if (NULL != BUZZER_Configs)
	{
		GPIO_u8PinInit(&BUZZER_Configs);
		GPIO_u8SetPinStatus(BUZZER_Configs->Port_Num, BUZZER_Configs->Pin_Num, HIGH_V);
	}
	else
	{
		/*Error => Do Nothing*/
	}
}

/*
 * @function 		:	BUZZER_OFF
 * @brief			:	Set BUZZER Pin to LOW
 * @param			:	BUZZER_Configs
 * @retval			:	Error State
 */
void BUZZER_OFF(PinConfig_t * BUZZER_Configs)
{
	if (NULL != BUZZER_Configs)
	{
		GPIO_u8PinInit(&BUZZER_Configs);
		GPIO_u8SetPinStatus(BUZZER_Configs->Port_Num, BUZZER_Configs->Pin_Num, LOW_V);
	}
	else
	{
		/*Error => Do Nothing*/
	}
}


/***************End of MAIN FUNCTIONS*******************/

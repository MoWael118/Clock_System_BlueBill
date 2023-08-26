/*
 *@file		:	BUZZER_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions for BUZZER Module
 */
#ifndef BUZZER_INC_BUZZER_INTERFACE_H_
#define BUZZER_INC_BUZZER_INTERFACE_H_

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../LIBRARY/ErrTypes.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
/*******************************************************/

/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	BUZZER_ON
 * @brief			:	Set BUZZER Pin to HIGH
 * @param			:	BUZZER_Configs
 * @retval			:	Error State
 */
void BUZZER_ON(PinConfig_t * BUZZER_Configs);

/*
 * @function 		:	BUZZER_OFF
 * @brief			:	Set BUZZER Pin to LOW
 * @param			:	BUZZER_Configs
 * @retval			:	Error State
 */
void BUZZER_OFF(PinConfig_t * BUZZER_Configs);


#endif /* BUZZER_INC_BUZZER_INTERFACE_H_ */

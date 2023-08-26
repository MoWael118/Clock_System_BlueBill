/*
 *@file		:	LED_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions for LED Module
 */
#ifndef LED_INC_LED_INTERFACE_H_
#define LED_INC_LED_INTERFACE_H_


/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../LIBRARY/ErrTypes.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
/*******************************************************/

/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	LED_ON
 * @brief			:	Set LED Pin to HIGH
 * @param			:	LED Configurations
 * @retval			:	Error State
 */
void LED_ON(PinConfig_t * LED_Configs);

/*
 * @function 		:	LED_OFF
 * @brief			:	Set LED Pin to LOW
 * @param			:	LED Configurations
 * @retval			:	Error State
 */
void LED_OFF(PinConfig_t * LED_Configs);


/***************End of MAIN FUNCTIONS*******************/


#endif /* LED_INC_LED_INTERFACE_H_ */

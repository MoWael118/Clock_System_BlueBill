/*
 *@file		:	LCD_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions for LCD
 */


#ifndef HAL_LCD_INC_LCD_INTERFACE_H_
#define HAL_LCD_INC_LCD_INTERFACE_H_
/*************************************************
 * Main Includes
 ************************************************/
#include "LCD_Config.h"
#include "LCD_Private.h"

#include "../../MCAL/Inc/RCC_Interface.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
#include "../../MCAL/Inc/SYSTICK_Interface.h"

#include "../../LIBRARY/ErrTypes.h"
#include "../../LIBRARY/BitMath.h"

/***********************************
 * @function 		:	LCD_voidInit
 * @brief			:	Initialization for LCD
 * @parameter[in]	:	void
 * @retval			:	void
 * @warning 		:	- This function will work properly with BLUEBILL Kit.
 *						- This Function initializes LCD at 4BIT mode
 */
void LCD_voidInit(void);


/***********************************
 * @function 		:	LCD_voidSendCommand
 * @brief			:	Send Command to LCD
 * @parameter[in]	:	uint8_t Command
 * @retval			:	void
 */
void LCD_voidSendCommand(uint8_t Command);


/***********************************
 * @function 		:	LCD_voidSendData
 * @brief			:	Send Data to LCD
 * @parameter[in]	:	uint8_t Data
 * @retval			:	void
 */
void LCD_voidSendData(uint8_t Data);


/***********************************
 * @function 		:	LCD_u8SendString
 * @brief			:	Send String to LCD
 * @parameter[in]	:	const char * String
 * @retval			:	uint8_t ErrorState
 */
uint8_t LCD_u8SendString(const char * String);


/***********************************
 * @function 		:	LCD_u8SendNumber
 * @brief			:	Send Number to LCD
 * @parameter[in]	:	uint32_t Number
 * @retval			:	uint8_t ErrorState
 */
uint8_t LCD_u8SendNumber(uint32_t Number);



#endif /* HAL_LCD_INC_LCD_INTERFACE_H_ */

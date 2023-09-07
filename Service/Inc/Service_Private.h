/*
 ******************************************************************************
 * @file           : Service_Private.h
 * @Author         : MOHAMMEDs & HEMA
 * @brief          : Service Private Header file
 * @Date           : Aug 30, 2023
 ******************************************************************************
 */
#ifndef _SERVICE_PRIVATE_H_
#define _SERVICE_PRIVATE_H_

#include "../../MCAL/Inc/SYSTICK_Interface.h"

/*****************************************************************************/
/************************   MACROS SECTION    ********************************/
/*****************************************************************************/

#define RED_LED_PORT PORTA
#define RED_LED_PIN PIN3

#define GREEN_LED_PORT PORTC
#define GREEN_LED_PIN   PIN15

#define DELAY_1S() (SYSTICK_u8Delay_ms(890))

typedef enum
{
	Seconds = 1,
	Minutes,
	Hours,
	Day,
	Month,
	Year,
	Date,
} Time_t;

typedef enum
{
	DS1307_SUNDAY = 1 , DS1307_MONDAY , DS1307_TUESDAY , DS1307_WEDNESDAY , DS1307_THURSDAY , DS1307_FRIDAY ,
	DS1307_SATURDAY
}DAYS_t ;


#endif

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

#define LED_PORT PORTA
#define LED_PIN PIN3

#define DELAY_1S() (SYSTICK_u8Delay_ms(930))

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



#endif

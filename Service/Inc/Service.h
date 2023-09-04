/*
 ******************************************************************************
 * @file           : Service.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Aug 28, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

#include "../../MCAL/Inc/SYSTICK_Interface.h"

/*****************************************************************************/
/************************   MACROS SECTION    ********************************/
/*****************************************************************************/

#define RED_LED_CODE  0x39

#define ALAEMCODE     100

#define LED_PORT   PORTA
#define LED_PIN    PIN3

#define DELAY_1S()  ( SYSTICK_u8Delay_ms(1000) )

/*****************************************************************************/
/******************   FUNCTIONS PROTOTYPES SECTION    ************************/
/*****************************************************************************/

void Clock_Init(void) ;

void Display_Time(void);

void Count_Time(void);

void Pins_Init( void ) ;
void Display_Date(void);

void SPI1_Init( void ) ;
void Interrupts_Init( void ) ;

void SPI1_CallBack( void ) ;

void TURN_ON_LED( void ) ;

void Receive_withInterrupt( void ) ;

/*==============================================================================================================================================
 *@fn      :  void EXTI1_ISR()
 *@brief  :  This Function Is Responsible For Handling The Interrupt Of EXTI1
 *@retval void :
 *==============================================================================================================================================*/
void EXTI1_ISR();
/*==============================================================================================================================================
 *@fn      :  void SPI1_CallBack()
 *@brief  :  This Function Is Responsible For Handling The Interrupt Of SPI1
 *@retval void :
 *==============================================================================================================================================*/

void SPI1_CallBack(void);

/*==============================================================================================================================================
 *@fn      :  void DisplayAlarmInfo()
 *@brief  :  This Function Is Responsible For Displaying The Alarm Info On The LCD which used as SPI Callback Function
 *@retval void :
 *==============================================================================================================================================*/
void DisplayAlarmInfo(void);

#endif /* INC_SERVICE_H_ */

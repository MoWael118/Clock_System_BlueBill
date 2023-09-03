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

/*****************************************************************************/
/************************   MACROS SECTION    ********************************/
/*****************************************************************************/

#define RED_LED_CODE  0x39

#define LED_PORT   PORTA
#define LED_PIN    PIN3


/*****************************************************************************/
/******************   FUNCTIONS PROTOTYPES SECTION    ************************/
/*****************************************************************************/

void Clock_Init(void) ;
void Recive_Time(void);

void Display_Time(void);

void Count_Time(void);

void Pins_Init( void ) ;
void Display_Date(void);

void SPI1_Init( void ) ;
void Interrupts_Init( void ) ;

void SPI1_CallBack( void ) ;

void TURN_ON_LED( void ) ;

void Receive_withInterrupt( void ) ;

#endif /* INC_SERVICE_H_ */

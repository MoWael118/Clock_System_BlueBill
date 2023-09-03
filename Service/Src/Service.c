/*
 ******************************************************************************
 * @file           : Service.c
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

#include <stdint.h>

#include "../../MCAL/Inc/SYSTICK_Interface.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
#include "../../MCAL/Inc/RCC_Interface.h"
#include "../../HAL/Inc/LCD_Interface.h"
#include "../../HAL/Inc/LED_Interface.h"
#include "../../HAL/Inc/BUZZER_Interface.h"
#include "../../MCAL/Inc/SPI_Interface.h"
#include "../../MCAL/Inc/NVIC_Interface.h"


#include "../Inc/Service.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/
SPI_CONFIGS_t * SPICONFIG ;

PinConfig_t * REDLED1_CONFIG ;


typedef enum
{
	Seconds=0,
	Minutes,
	Hours,
	Day,
	Month,
	Year,
	Date,
}Time_t;

volatile uint8_t flag = 0;

uint8_t Received[7]={0};



/*****************************************************************************/
/****************   FUNCTIONS IMPLEMENTATION SECTION    **********************/
/*****************************************************************************/

void Clock_Init( void )
{
	/* Enable GPIO PORTA Clock */
	RCC_Set_APB2Peripheral_CLK( APB2_IOPA , ENABLED ) ;

	/* Enable SPI1 Clock */
	RCC_Set_APB2Peripheral_CLK( APB2_SPI1 , ENABLED ) ;
}

void Pins_Init( void )
{
	/* RED LED1 GPIO Configuration */
	static PinConfig_t RED_LED1 =
	{
			.Port_Num = LED_PORT , .Pin_Num = LED_PIN , .Output_Type =  GP_PUSH_PULL , .Mode = OUTPUT_LSPEED
	};

	/* Initialize LEDs GPIO Configuration */
	GPIO_u8PinInit(&RED_LED1);

	/* Intialize Both LED Configuration Structs Globally */
	REDLED1_CONFIG = &RED_LED1 ;

	/* SPI Pins Configuration
	 * MOSI Pin
	 */
	PinConfig_t MOSI_PIN = { .Mode = INPUT , .Input_Type = FLOATING_INPUT , .Pin_Num = PIN7 , .Port_Num = PORTA } ;

	/* SCK Pin */
	PinConfig_t SCK_PIN = { .Mode = INPUT , .Input_Type = FLOATING_INPUT , .Pin_Num = PIN5 , .Port_Num = PORTA } ;

	/* Initialize SPI1 Pins */
	GPIO_u8PinInit(&MOSI_PIN);
	GPIO_u8PinInit(&SCK_PIN);
	LCD_voidInit();
}

void SPI1_Init( void )
{
	/* Initialize SPI Configuration */
	static SPI_CONFIGS_t SPI1_Config =
	{
			.CRC_State = CRC_STATE_DISABLED , .Chip_Mode = CHIP_MODE_SLAVE , .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST , .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW ,
			.Frame_Size = DATA_FRAME_SIZE_8BITS , .Frame_Type = FRAME_FORMAT_MSB_FIRST , .SPI_Num = SPI_NUMBER1 , .Slave_Manage_State = SLAVE_MANAGE_SW_SLAVE_ACTIVE ,
			.Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX
	};

	/* Initialize SPI */
	SPI_Init(&SPI1_Config) ;

	/* Initialize SPI1 Configuration Struct Globally */
	SPICONFIG = & SPI1_Config ;
}

void Interrupts_Init( void )
{
	/* Enable SPI1 Generate Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ) ;
}

void TURN_ON_LED( void )
{
	/* Turn ON Both RED Leds */
	LED_ON(REDLED1_CONFIG) ;
}

void Receive_withInterrupt( void )
{
	/* Receive with SPI Through Interrupt */
	SPI_Receive_IT(SPICONFIG, Received, 7,&SPI1_CallBack);
}



void Display_Time(void)
{
	LCD_VoidGoToXY(1, 0);

	if(Received[Hours] <=9 )
	{
		LCD_u8SendNumber(0);

	}

	LCD_u8SendNumber(Received[Hours]);

	LCD_voidSendData(':');

	if(Received[Minutes] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(Received[Minutes]);

	LCD_voidSendData(':');

	if(Received[Seconds] <=9 )
	{
		LCD_u8SendNumber(0);
	}

	LCD_u8SendNumber(Received[Seconds]);
}




void Count_Time(void)
{
	Received[Seconds]++;

	if(Received[Hours] > 23)
	{
		Received[Hours]=0;
	}

	if(Received[Minutes] > 59)
	{
		Received[Minutes]=0;
		Received[Hours]++;
	}

	if(Received[Seconds] > 59)
	{
		Received[Seconds]=0;
		Received[Minutes]++;
	}


}


void Display_Date(void)
{
	/* Display Date on LCD */

	LCD_VoidGoToXY(0, 0);
	if( Received[Date] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(Received[Date]);

	LCD_voidSendData('/');

	if( Received[Month] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(Received[Month]);

	LCD_voidSendData('/');

	LCD_u8SendNumber(2000 + Received[Year]);
}





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
#include "../../MCAL/Inc/RCC_Private.h"
#include "../../MCAL/Inc/NVIC_Interface.h"


#include "../Inc/Service.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/

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



PinConfig_t SPI_MOSI ={.Port_Num= PORTA, .Pin_Num= PIN7, .Input_Type=FLOATING_INPUT, .Mode=INPUT  };
PinConfig_t SPI_SCK  ={.Port_Num= PORTA, .Pin_Num= PIN5, .Input_Type=FLOATING_INPUT, .Mode=INPUT  };

SPI_CONFIGS_t SPI = {.SPI_Num = SPI_NUMBER1, .Transfer_Mode= TRANSFER_MODE_FULL_DUPLEX,
		.Frame_Size= DATA_FRAME_SIZE_8BITS, .Chip_Mode= CHIP_MODE_SLAVE, .Slave_Manage_State= SLAVE_MANAGE_SW_SLAVE_ACTIVE ,
		.CRC_State=CRC_STATE_DISABLED, .Clock_Phase=CLOCK_PHASE_CAPTURE_FIRST,.Clock_Polarity=CLOCK_POLARITY_IDLE_LOW,.Frame_Type=FRAME_FORMAT_MSB_FIRST,
};



/*****************************************************************************/
/****************   FUNCTIONS IMPLEMENTATION SECTION    **********************/
/*****************************************************************************/
void Clock_Init(void)
{
	RCC_Set_APB2Peripheral_CLK(APB2_SPI1, ENABLED);

	NVIC_EnableIRQ(SPI1_IRQ) ;

	GPIO_u8PinInit(&SPI_MOSI);

	GPIO_u8PinInit(&SPI_SCK);

	SPI_Init(&SPI);

	LCD_voidInit();
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


void Recive_Time(void)
{
	SPI_Receive_IT(&SPI, Received, 7,&SPI_CALL_BACK);
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



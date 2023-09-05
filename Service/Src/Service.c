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

#include <stdint.h>
#include "../../LIBRARY/STM32F103xx.h"
#include "../../LIBRARY/ErrTypes.h"
#include "../../LIBRARY/BitMath.h"
#include "../../MCAL/Inc/EXTI_Interface.h"
#include "../../MCAL/Inc/AFIO_Interface.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
#include "../../MCAL/Inc/NVIC_Interface.h"
#include "../../MCAL/Inc/SPI_Interface.h"
#include "../../MCAL/Inc/SYSTICK_Interface.h"
#include "../../MCAL/Inc/RCC_Interface.h"
#include "../Inc/Service.h"
#include "../../HAL/Inc/LCD_Interface.h"
#include "../../HAL/Inc/LED_Interface.h"
#include "../../HAL/Inc/BUZZER_Interface.h"
/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/

/* Global Pointer to PinConfig_t Struct to be used in Buzzer_on Function */
PinConfig_t *BuzzerPIN;

/* Global Pointer to SPI_CONFIGS_t Struct */
SPI_CONFIGS_t *SPICONFIG;

/* Global Array to store the received data from the SPI
 * The first element is the alarm number
 * The Rest of the elements are the Alarm Name until the first 13(ASCII of Enter) */
uint8_t RecivedData[30] = {0};

SPI_CONFIGS_t * SPICONFIG ;

PinConfig_t * REDLED1_CONFIG ;


typedef enum
{
	Seconds=1,
	Minutes,
	Hours,
	Day,
	Month,
	Year,
	Date,
}Time_t;


/*****************************************************************************/
/****************   FUNCTIONS IMPLEMENTATION SECTION    **********************/
/*****************************************************************************/
/*==============================================================================================================================================
 *@fn      :  void ClockInit()
 *@brief  :  This Function Is Responsible For Initializing The Clocks For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void Clock_Init()
{
/* Enable GPIO PORTA Clock */
	RCC_Set_APB2Peripheral_CLK( APB2_IOPA , ENABLED ) ;
	
	/* Enable RCC For GPIOB */
	RCC_Set_APB2Peripheral_CLK(APB2_IOPB, ENABLED);

	/* Enable RCC For GPIOC */
	RCC_Set_APB2Peripheral_CLK(APB2_IOPC, ENABLED);

	/* Enable RCC For SPI1 */
	RCC_Set_APB2Peripheral_CLK(APB2_SPI1, ENABLED);

	/* Enable RCC For AFIO */
	RCC_Set_APB2Peripheral_CLK(APB2_AFIO, ENABLED);
}
/*==============================================================================================================================================
 *@fn      :  void PinInit()
 *@brief  :  This Function Is Responsible For Initializing The Pins For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void Pins_Init()
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

	/* Initialize the Buzzer Pin */
	static PinConfig_t Buzzer_PB5 = {.Port_Num = PORTB, .Pin_Num = PIN5, .Mode = OUTPUT_LSPEED, .Output_Type = GP_PUSH_PULL};

	/* Initialize the Buzzer Pin Globally */
	BuzzerPIN = &Buzzer_PB5;

	/* Initialize the EXTI Pin */
	PinConfig_t EXTI_PC1 = {.Port_Num = PORTC, .Pin_Num = PIN13, .Mode = INPUT, .PullType = PULLDOWN, .Input_Type = PULLUPDOWN_INPUT};

	/* Initialize the EXTI Pin */
	GPIO_u8PinInit(&EXTI_PC1);

	/* Initialize the LED Pin */
	GPIO_u8PinInit(&Buzzer_PB5);

	/* SPI Pins Configuration */
	/* MOSI Pin */
	PinConfig_t MOSI_PIN = {.Mode = INPUT, .Input_Type = FLOATING_INPUT, .Pin_Num = PIN7, .Port_Num = PORTA};

	/* SCK Pin */
	PinConfig_t SCK_PIN = {.Mode = INPUT, .Input_Type = FLOATING_INPUT, .Pin_Num = PIN5, .Port_Num = PORTA};

	/* Initialize SPI1 Pins */
	GPIO_u8PinInit(&MOSI_PIN);
	GPIO_u8PinInit(&SCK_PIN);
}
/*==============================================================================================================================================
 *@fn      :  void SPI1_Init()
 *@brief  :  This Function Is Responsible For Initializing The SPI1 Peripheral
 *@retval void :
 *==============================================================================================================================================*/
void SPI1_Init(void)
{
	/* Initialize SPI Configuration */
	static SPI_CONFIGS_t SPI1_Config =
		{
			.CRC_State = CRC_STATE_DISABLED, .Chip_Mode = CHIP_MODE_SLAVE, .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST, .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW, .Frame_Size = DATA_FRAME_SIZE_8BITS, .Frame_Type = FRAME_FORMAT_MSB_FIRST, .SPI_Num = SPI_NUMBER1, .Slave_Manage_State = SLAVE_MANAGE_SW_SLAVE_ACTIVE, .Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX};

	/* Initialize SPI */
	SPI_Init(&SPI1_Config);

	/* Initialize SPI1 Configuration Struct Globally */
	SPICONFIG = &SPI1_Config;
}
/*==============================================================================================================================================
 *@fn      :  void EXTI1_ISR()
 *@brief  :  This Function Is Responsible For Handling The Interrupt Of EXTI1
 *@retval void :
 *==============================================================================================================================================*/

/*==============================================================================================================================================
 *@fn      :  void SPI1_CallBack()
 *@brief  :  This Function Is Responsible For Handling The Interrupt Of SPI1
 *@retval void :
 *==============================================================================================================================================*/
void DisplayAlarmInfo(void)
{
	/* Clear the LCD */
	LCD_voidSendCommand(1);

	/* Local Variable to store the Counter used in the looping */
	uint8_t Counter = 0;

	/* Loop until the first 13(ASCII of Enter) */
	for (Counter = 1; RecivedData[Counter] != 13; Counter++)
	{
		/* Check if the Counter is 0 to print the Alarm Number */
		if (Counter == 1)
		{
			/* Print the Alarm Number */
			LCD_u8SendString("Alarm Number=");
			LCD_voidSendData(RecivedData[Counter] + 48);
			/* Go to the second line */
			LCD_VoidGoToXY(1, 0);
		}
		/* Check if the Counter is greater than 0 to print the Alarm Name */
		else
		{

			/* Print the Alarm Name */
			LCD_voidSendData(RecivedData[Counter]);
		}
	}
	for( Counter = 0 ; Counter < 4 ; Counter++ )
	{
		DELAY_1S( ) ;
	}
	LCD_voidSendCommand(1) ;
}

void Display_Date(void)
{
	/* Display Date on LCD */

	LCD_VoidGoToXY(0, 0);
	if( RecivedData[Date] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(RecivedData[Date]);

	LCD_voidSendData('/');

	if( RecivedData[Month] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(RecivedData[Month]);

	LCD_voidSendData('/');

	LCD_u8SendNumber(2000 + RecivedData[Year]);
}

void Interrupts_Init( void )
{
	/* Enable SPI1 Generate Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ) ;
	
	
	/* Enable the EXTI13 Interrupt */
	NVIC_EnableIRQ(EXTI15_10_IRQ);

	/* Enable the SPI1 Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ);

}

void TURN_ON_LED( void )
{
	/* Turn ON Both RED Leds */
	LED_ON(REDLED1_CONFIG) ;
}

void Receive_withInterrupt( void )
{
	/* Receive with SPI Through Interrupt */
	SPI_Receive_IT(SPICONFIG, RecivedData, 30 ,&SPI1_CallBack);
}



void Display_Time(void)
{
	LCD_VoidGoToXY(1, 0);

	if(RecivedData[Hours] <=9 )
	{
		LCD_u8SendNumber(0);

	}

	LCD_u8SendNumber(RecivedData[Hours]);

	LCD_voidSendData(':');

	if(RecivedData[Minutes] <=9 )
	{
		LCD_u8SendNumber(0);
	}
	LCD_u8SendNumber(RecivedData[Minutes]);

	LCD_voidSendData(':');

	if(RecivedData[Seconds] <=9 )
	{
		LCD_u8SendNumber(0);
	}

	LCD_u8SendNumber(RecivedData[Seconds]);
}




void Count_Time(void)
{
	RecivedData[Seconds]++;

	if(RecivedData[Hours] > 23)
	{
		RecivedData[Hours]=0;
	}

	if(RecivedData[Minutes] > 59)
	{
		RecivedData[Minutes]=0;
		RecivedData[Hours]++;
	}

	if(RecivedData[Seconds] > 59)
	{
		RecivedData[Seconds]=0;
		RecivedData[Minutes]++;
	}


}

void EXTI13_Init( void )
{
	/* Initialize the EXTI Configuration */
	EXTI_Confg PC1_EXTIConfig = {.LINE = EXTI13, .Mode = Enable, .Trigger = RaisingEdge, .EXTI_CallBackFunc = &EXTI1_ISR};

	/* Initialize the EXTI */
	EXTI_Init(&PC1_EXTIConfig);

	/* Initialize the AFIO For EXTI13 */
	AFIO_U8SetEXTIPort(AFIO_EXTI13, AFIO_PortC);
}


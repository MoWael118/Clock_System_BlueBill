/*
 ******************************************************************************
 * @file           : Service.c
 * @Author         : MOHAMMEDs & HEMA
 * @brief          : Service Functions
 * @Date           : Aug 28, 2023
 ******************************************************************************
 */

#include <stdint.h>

#include "../../LIBRARY/STM32F103xx.h"
#include "../../LIBRARY/ErrTypes.h"

#include "../../MCAL/Inc/EXTI_Interface.h"
#include "../../MCAL/Inc/AFIO_Interface.h"
#include "../../MCAL/Inc/GPIO_Interface.h"
#include "../../MCAL/Inc/NVIC_Interface.h"
#include "../../MCAL/Inc/SPI_Interface.h"
#include "../../MCAL/Inc/SYSTICK_Interface.h"
#include "../../MCAL/Inc/RCC_Interface.h"

#include "../../HAL/Inc/LCD_Interface.h"
#include "../../HAL/Inc/LED_Interface.h"
#include "../../HAL/Inc/BUZZER_Interface.h"

#include "../Inc/Service.h"
#include "../Inc/Service_Private.h"

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

/* Global Pointer to SPI_CONFIGS_t Struct to Be Used in SPI Receive Function */
SPI_CONFIGS_t *SPICONFIG;

/* Global Pointer to PinConfig_t Struct to be used in LED_ON Function */
PinConfig_t *REDLED1_CONFIG;

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
	RCC_Set_APB2Peripheral_CLK(APB2_IOPA, ENABLED);

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
			.Port_Num = LED_PORT, .Pin_Num = LED_PIN, .Output_Type = GP_PUSH_PULL, .Mode = OUTPUT_LSPEED};

	/* Initialize LEDs GPIO Configuration */
	GPIO_u8PinInit(&RED_LED1);

	/* Intialize Both LED Configuration Structs Globally */
	REDLED1_CONFIG = &RED_LED1;

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

/*=======================================================================================
 * @fn		 		:	EXTI13_Init
 * @brief			:	Initialize the EXTI13
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void EXTI13_Init(void)
{
	/* Initialize the EXTI Configuration */
	EXTI_Confg PC1_EXTIConfig = {.LINE = EXTI13, .Mode = Enable, .Trigger = RaisingEdge, .EXTI_CallBackFunc = &EXTI13_ISR};

	/* Initialize the EXTI */
	EXTI_Init(&PC1_EXTIConfig);

	/* Initialize the AFIO For EXTI13 */
	AFIO_U8SetEXTIPort(AFIO_EXTI13, AFIO_PortC);
}

/*==============================================================================================================================================
 *@fn     :  void InterruptsInit (void)
 *@brief  :  This Function Is Responsible For Initializing The Interrupts
 *@retval :  void
 *==============================================================================================================================================*/
void Interrupts_Init(void)
{
	/* Enable SPI1 Generate Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ);

	/* Enable the EXTI13 Interrupt */
	NVIC_EnableIRQ(EXTI15_10_IRQ);

	/* Enable the SPI1 Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ);
}

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
	for (Counter = 0; Counter < 4; Counter++)
	{
		DELAY_1S();
	}
	LCD_voidSendCommand(1);
}

/*==============================================================================================================================================
 *@fn     : void Display_Time()
 *@brief  : This Function Is Responsible For Display time on The LCD
 *@retval : void
 *==============================================================================================================================================*/
void Display_Time(void)
{
	/* Go to the second line */
	LCD_VoidGoToXY(1, 0);

	/* Check if Hour is smaller than 9 */
	if (RecivedData[Hours] <= 9)
	{
		/* Display 0 before Hour */
		LCD_u8SendNumber(0);
	}
	/* Display Minutes */
	LCD_u8SendNumber(RecivedData[Hours]);

	/* Display : */
	LCD_voidSendData(':');

	/* Check if Minute is smaller than 9 */
	if (RecivedData[Minutes] <= 9)
	{
		/* Display 0 before Minute */
		LCD_u8SendNumber(0);
	}
	/* Display Minutes */
	LCD_u8SendNumber(RecivedData[Minutes]);

	/* Display : */
	LCD_voidSendData(':');

	/* Check if Second is smaller than 9 */
	if (RecivedData[Seconds] <= 9)
	{
		/* Display 0 before Second */
		LCD_u8SendNumber(0);
	}
	/* Display Seconds */
	LCD_u8SendNumber(RecivedData[Seconds]);
}

/*==============================================================================================================================================
 *@fn     : void Count_Time()
 *@brief  : This Function Is Responsible For Counte time 
 *@retval : void
 *==============================================================================================================================================*/
void Count_Time(void)
{
	/* Increment Seconds */
	RecivedData[Seconds]++;

	/* Check if Hours > 23 */
	if (RecivedData[Hours] > 23)
	{
		/* Clear Hours*/
		RecivedData[Hours] = 0;
	}

	/* Check if Minutes > 59 */
	if (RecivedData[Minutes] > 59)
	{
		/* Clear Minutes*/
		RecivedData[Minutes] = 0;
		/* Increment Hours */
		RecivedData[Hours]++;
	}

	/* Check if Seconds > 59 */
	if (RecivedData[Seconds] > 59)
	{
		/* Clear Seconds*/
		RecivedData[Seconds] = 0;
		/* Increment Minutes */
		RecivedData[Minutes]++;
	}
}

/*==============================================================================================================================================
 *@fn     : void Display_Date()
 *@brief  : This Function Is Responsible For Display Date on The LCD
 *@retval : void
 *==============================================================================================================================================*/
void Display_Date(void)
{
	/* Go to the first line */
	LCD_VoidGoToXY(0, 0);
	
	/* Check if Date > 9 */
	if (RecivedData[Date] <= 9)
	{
		/* Display 0 before Minute */
		LCD_u8SendNumber(0);
	}
	/* Display Day */
	LCD_u8SendNumber(RecivedData[Date]);

	/* Display / */
	LCD_voidSendData('/');

	/* Check if Month > 9 */
	if (RecivedData[Month] <= 9)
	{
		LCD_u8SendNumber(0);
	}
	/* Display Month */
	LCD_u8SendNumber(RecivedData[Month]);

	/* Display / */
	LCD_voidSendData('/');

	/* Display Year */
	LCD_u8SendNumber(2000 + RecivedData[Year]);
}

/*==============================================================================================================================================
 *@fn     : void TURN_ON_LED()
 *@brief  : This Function Is Responsible For Turning On The LED
 *@retval : void
 *==============================================================================================================================================*/
void TURN_ON_LED(void)
{
	/* Turn ON  RED Led */
	LED_ON(REDLED1_CONFIG);
}

/*==============================================================================================================================================
 *@fn     : void CLEAR_DISPLAY()
 *@brief  : This Function Is Responsible For Clearing The LCD
 *@retval : void
 *==============================================================================================================================================*/
void CLEAR_DISPLAY(void)
{
	/* CLEAR LCD Display */
	LCD_voidSendCommand(CLEAR_LCD);
}

/*==============================================================================================================================================
 *@fn     : void Receive_withInterrupt()
 *@brief  : This Function Is Responsible For Receiving Data With SPI Through Interrupt
 *@retval : void
 *==============================================================================================================================================*/
void Receive_withInterrupt(void)
{
	/* Receive with SPI Through Interrupt */
	SPI_Receive_IT(SPICONFIG, RecivedData, 30, &SPI1_CallBack);
}

/*==============================================================================================================================================
 *@fn     : void _delay_1s()
 *@brief  : This Function Is Used to Give a Delay of 1 Second
 *@retval : void
 *==============================================================================================================================================*/
void _delay_1s( void )
{
	DELAY_1S( ) ;
}

/*******************************************************************************
 * Filename              :   Service.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Aug 31, 2023
 * Version               :   1.0.0
 * Compiler              :   GCC ARM Embedded Toolchain
 * Target                :
 * Notes                 :   None
 **
 *******************************************************************************/
/************************ SOURCE REVISION LOG *********************************
 *
 *    Date    Version   Author             Description
 *  14/10/20   1.0.0   Mohamemd Waleed   Initial Release.
 *
 *******************************************************************************/

#include <stdint.h>
#include "../LIBRARY/STM32F103xx.h"
#include "../LIBRARY/ErrTypes.h"
#include "../LIBRARY/BitMath.h"
#include "../MCAL/Inc/EXTI_Interface.h"
#include "../MCAL/Inc/AFIO_Interface.h"
#include "../MCAL/Inc/GPIO_Interface.h"
#include "../MCAL/Inc/NVIC_Interface.h"
#include "../MCAL/Inc/SPI_Interface.h"
#include "../MCAL/Inc/SYSTICK_Interface.h"
#include "../MCAL/Inc/RCC_Interface.h"
#include "../Inc/Service.h"
#include "../HAL/Inc/LCD_Interface.h"
#include "../HAL/Inc/LED_Interface.h"
#include "../HAL/Inc/BUZZER_Interface.h"
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

/*****************************************************************************/
/****************   FUNCTIONS IMPLEMENTATION SECTION    **********************/
/*****************************************************************************/
/*==============================================================================================================================================
 *@fn      :  void ClockInit()
 *@brief  :  This Function Is Responsible For Initializing The Clocks For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void ClockInit()
{

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
void PinInit()
{

	/* Initialize the Buzzer Pin */
	static PinConfig_t Buzzer_PB5 = {.Port_Num = PORTB, .Pin_Num = PIN5, .Mode = OUTPUT_LSPEED, .Output_Type = GP_PUSH_PULL};

	/* Initialize the Buzzer Pin Globally */
	BuzzerPIN = &Buzzer_PB5;

	/* Initialize the EXTI Pin */
	PinConfig_t EXTI_PC1 = {.Port_Num = PORTC, .Pin_Num = PIN13, .Mode = INPUT, .PullType = PULLUP, .Input_Type = PULLUPDOWN_INPUT};

	/* Initialize the EXTI Pin */
	GPIO_u8PinInit(&EXTI_PC1);

	/* Initialize the LED Pin */
	GPIO_u8PinInit(&Buzzer_PB5);

	/* Initialize the EXTI Configuration */
	EXTI_Confg PC1_EXTIConfig = {.LINE = EXTI13, .Mode = Enable, .Trigger = FallingEdge, .EXTI_CallBackFunc = &EXTI1_ISR};

	/* Initialize the EXTI */
	EXTI_Init(&PC1_EXTIConfig);

	/* Initialize the AFIO For EXTI13 */
	AFIO_U8SetEXTIPort(AFIO_EXTI13, AFIO_PortC);

	/* Enable the EXTI13 Interrupt */
	NVIC_EnableIRQ(EXTI15_10_IRQ);

	/* Enable the SPI1 Interrupt */
	NVIC_EnableIRQ(SPI1_IRQ);

	/* Initialize the LCD Pins */
	LCD_voidInit();

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
void EXTI1_ISR()
{
	/* Local Variable to store the Counter used in the looping */
	uint8_t Counter1 = 0;

	/* Turn on the Buzzer */
	BUZZER_ON(BuzzerPIN);

	/* Delay for 5 Seconds */
	for (Counter1 = 0; Counter1 < 5; Counter1++)
	{
		SYSTICK_u8Delay_ms(1000);
	}

	/* Turn off the Buzzer */
	BUZZER_OFF(BuzzerPIN);
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
	for (Counter = 0; RecivedData[Counter] != 13; Counter++)
	{
		/* Check if the Counter is 0 to print the Alarm Number */
		if (Counter == 0)
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
}

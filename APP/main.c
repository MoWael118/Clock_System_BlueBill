/**
 ******************************************************************************
 * @file           : main.c
 * @author         : MOHAMMEDs & HEMA
 * @brief          : Main program body For BlueBill in Clock System Project
 ******************************************************************************
 */
/************************ SOURCE REVISION LOG *********************************
 *    Date        Author             Description
 *  16/08/23     Mohamemd Wael      Initial Release ( INCLUDES )
 *  27/08/23     Mohammed Ayman     Adding Basic Funtionality of Receiveing Data From Nucleo
 *  28/08/23     Mohammed Ayman     Adding Documentation and Solving Bugs
 *  28/08/23     Mohammed Waled     Initial Release For Functions ( ClockInit - PinInit - SPIInit)
 *  29/08/23     Mohammed Waled     Initial Release For Functions (SPI1_CallBack - EXTI1_ISR)
 *  30/08/23     Mohammed Waled     Adding Some Documentation And Fixing Some Bugs
 *  30/08/23     Ibrahim Mohammed   Implement Funtionality of Display Data on Lcd
 *  30/08/23     Ibrahim Mohammed   Adding Some Documentation 
 
 *******************************************************************************/

/*****************************************************************************/
/********************    MAIN INCLUDES SECTION    ****************************/
/*****************************************************************************/

#include <stdint.h>

#include "../LIBRARY/STM32F103xx.h"
#include "../LIBRARY/ErrTypes.h"

#include "../MCAL/Inc/SPI_Interface.h"

#include "../HAL/Inc/LCD_Interface.h"
#include "../HAL/Inc/BUZZER_Interface.h"
#include "../HAL/Inc/LED_Interface.h"

#include "../Service/Inc/Service.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/

/* Global Pointer to Buzzer Struct */
extern PinConfig_t *BuzzerPIN;

/* Global Pointer to PinConfig_t Struct to be used in LED_ON Function */
extern PinConfig_t *GREEN_LED_CONFIG;

/* Global Array to store the received data from the SPI
 * The first Element is A Code to Determine the Type of Data Received
  1- RED_LED_CODE
  2- ALARMCODE
  3- DISPLAY_CODE
 */
extern uint8_t RecivedData[30];

/* Indication of Receving Via SPI */
volatile RECEIVING_VAL_t RECEIVING_VALUE = NO_RECEIVE;

/*****************************************************************************/
/***********************   MAIN CODE SECTION    ******************************/
/*****************************************************************************/
int main(void)
{
	/* Initialize Clocks For Needed Peripherals */
	Clock_Init();

	/* Pins Initialization through GPIO */
	Pins_Init();

	/* Initialize the LCD  */
	LCD_voidInit();

	/* EXTI Configuration Setting */
	EXTI13_Init();

	/* Initialize SPI Peripheral */
	SPI1_Init();

	/* Intterupts Enable Through NVIC */
	Interrupts_Init();

	/* Receive Using SPI with Interrupt */
	Receive_withInterrupt();

	/* Loop forever */
	while (1)
	{
		/* Wait Until SPI Receive Any Data */
		while (!RECEIVING_VALUE)
			;

		/* Check on the Received Data */
		switch (RECEIVING_VALUE)
		{
			/* RED LED CODE Received */
		case RED_LED_RECEIVED:

			/* Turn RED LED ON */
			TURN_ON_LED();

			/* Clear LCD */
			CLEAR_DISPLAY();

			/* Shut Down */
			while (1)
				;

			break;

			/* ALARM CODE Received */
		case ALARM_RECEIVED:

			/* Display Alarm Info on LCD :
			 1st Line : Alarm Time ,
			 2nd Line : Alarm Date
			 */
			DisplayAlarmInfo();

			/* Turn Buzzer OFF */
			BUZZER_OFF(BuzzerPIN);

			/* Reset The Receiving Value */
			RECEIVING_VALUE = NO_RECEIVE;

			break;

		case DISPLAY_RECEIVED:

			/* Execution Time of Functions Below Can't be Determined Exactly
			 * So We Approxematly Put A Value of It & Subtracted it From 1 Second Delay ,
			 * So The Differnce Between the Exact Exc Time & The Approxmate One Could Finally Make A Big Difference
			 */

			/* Display Date on LCD */
			Display_Date(); /* 48ms Execution Time Approxmate */

			/* Calculate the Time */
			Count_Time(); /* 1 ms Approxmate */

			/* Delay 1 sec */
			_delay_1s( ) ;

			/* Display Time on LCD */
			Display_Time(); /* 36 ms Execution Time Approxmate */

			break;

		case GREEN_LED_RECEIVED :

			LED_ON(GREEN_LED_CONFIG) ;

			RECEIVING_VALUE = NO_RECEIVE ;

		default:
			/* Do Nothing */
			break;
		}
	}
}

/*****************************************************************************/
/***********************      ISRs SECTION      ******************************/
/*****************************************************************************/

/*==============================================================================================================================================
 *@fn      :  void SPI1_CallBack()
 *@brief   :  SPI Callback Function to handle the received data from the master
 *@retval  :  void
 *==============================================================================================================================================*/
void SPI1_CallBack(void)
{
	/* Check on First Element of the Received Data */
	if (RecivedData[0] == RED_LED_CODE)
	{
		RECEIVING_VALUE = RED_LED_RECEIVED;
	}
	else if (RecivedData[0] == ALARMCODE)
	{
		RECEIVING_VALUE = ALARM_RECEIVED;
	}
	else if (RecivedData[0] == DISPLAY_CODE)
	{
		RECEIVING_VALUE = DISPLAY_RECEIVED;
	}
	else if ( RecivedData[0] == GREEN_LED_CODE )
	{
		RECEIVING_VALUE = GREEN_LED_RECEIVED ;
	}

	/* Receive New Data */
	Receive_withInterrupt();
}

/*==============================================================================================================================================
 *@fn      :  void EXTI13_ISR()
 *@brief  :  This Function Is Responsible For Handling The Interrupt Of EXTI13
 *@retval void :
 *==============================================================================================================================================*/
void EXTI13_ISR()
{
	/* Turn on the Buzzer */
	BUZZER_ON(BuzzerPIN);
}

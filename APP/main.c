/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mohamed Wael
 * @brief          : Main program body For BlueBill in Clock System Project
 ******************************************************************************
 */
/************************ SOURCE REVISION LOG *********************************
 *    Date    Version   Author             Description
 *  16/08/23   1.0.0   Mohamemd Wael     Initial Release ( INCLUDES )
 *  28/8/23    1.0.0   Mohamed Waled     Initial Release For Functions ( ClockInit - PinInit - SPIInit)
 *  29/8/23    1.0.0   Mohamed Waled     Initial Release For Functions (SPI1_CallBack - EXTI1_ISR)
 *  30/8/23    1.0.0   Mohamed Waled     Adding Some Documentation And Fixing Some Bugs
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

#include "../Service/Inc/Service.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/


/* Global Pointer to SPI_CONFIGS_t Struct */
extern SPI_CONFIGS_t *SPICONFIG;

/* Global Pointer to Buzzer Struct */
extern PinConfig_t *BuzzerPIN ;

/* Global Array to store the received data from the SPI
 * The first element is the alarm number
 * The Rest of the elements are the Alarm Name until the first 13(ASCII of Enter) */
extern uint8_t RecivedData[30];

/* Indication of Receving Via SPI */
volatile RECEIVING_VAL_t RECEIVING_VALUE = NO_RECEIVE ;




/*****************************************************************************/
/***********************   MAIN CODE SECTION    ******************************/
/*****************************************************************************/
int main(void)
{
	/* Initialize Clocks For Needed Peripherals */
	Clock_Init( ) ;

	/* Pins Initialization through GPIO */
	Pins_Init( ) ;

	/* Initialize the LCD  */
	LCD_voidInit();

	/* EXTI Configuration Setting */
	EXTI13_Init( ) ;

	/* Initialize SPI Peripheral */
	SPI1_Init( ) ;

	/* Intterupts Enable Through NVIC */
	Interrupts_Init( ) ;

	/* Receive Using SPI with Interrupt */
	Receive_withInterrupt( ) ;

	/* Loop forever */
	while(1)
	{

		while( !RECEIVING_VALUE ) ;

		switch ( RECEIVING_VALUE )
		{
		case RED_LED_RECEIVED :

			TURN_ON_LED(  ) ;
			LCD_voidSendCommand( 1 );
			/* Shut Down */
			while(1);

			break ;

		case ALARM_RECEIVED :

			DisplayAlarmInfo(  ) ;

			BUZZER_OFF(BuzzerPIN);

			RECEIVING_VALUE = NO_RECEIVE ;

			break ;


		case   DISPLAY_RECEIVED :

			/* Execution Time of Functions Below Can't be Determined Exactly
			 * So We Approxematly Put A Value of It & Subtracted it From 1 Second Delay ,
			 * So The Differnce Between the Exact Exc Time & The Approxmate One Could Finally Make A Big Difference
			 */

			/* Display Date on LCD */
			Display_Date();     /* 48ms Execution Time */

			/* Calculate the Time */
			Count_Time(); /* 1 ms */

			/* Delay 1 sec */
			DELAY_1S();

			/* Display Time on LCD */
			Display_Time();  /* 36 ms Execution Time */

			break ;

		default :
			break ;
		}



	}

}



/*****************************************************************************/
/***********************      ISRs SECTION      ******************************/
/*****************************************************************************/

void SPI1_CallBack( void )
{
	if( RecivedData[ 0 ] == RED_LED_CODE )
	{
		RECEIVING_VALUE = RED_LED_RECEIVED ;
	}
	else if( RecivedData[ 0 ] == ALARMCODE )
	{
		RECEIVING_VALUE = ALARM_RECEIVED ;
	}
	else if ( RecivedData[ 0 ] == DISPLAY_CODE )
	{
		RECEIVING_VALUE = DISPLAY_RECEIVED ;
	}
	else
	{
		/* Do Nothing */
	}

	/* Receive New Data */
	Receive_withInterrupt( ) ;

}


void EXTI1_ISR()
{
	/* Turn on the Buzzer */
	BUZZER_ON(BuzzerPIN);
}



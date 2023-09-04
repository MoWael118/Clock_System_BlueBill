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

#include "../HAL/Inc/LCD_Interface.h"
#include "../HAL/Inc/LED_Interface.h"
#include "../HAL/Inc/BUZZER_Interface.h"
#include "../Service/Inc/Service.h"
#include "../LIBRARY/STM32F103xx.h"
#include "../LIBRARY/ErrTypes.h"
#include "../LIBRARY/BitMath.h"
#include "../Service/Inc/Service.h"
#include "../MCAL/Inc/SPI_Interface.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/


/* Global Pointer to SPI_CONFIGS_t Struct */
extern SPI_CONFIGS_t *SPICONFIG;

/* Global Array to store the received data from the SPI
 * The first element is the alarm number
 * The Rest of the elements are the Alarm Name until the first 13(ASCII of Enter) */
extern uint8_t RecivedData[30];

extern volatile uint8_t flag ;

volatile uint8_t RECEIVED = 0 ;

uint8_t  DATA_ReceivedFromSPI = 0 ;

extern uint8_t Received[7];

/*****************************************************************************/
/***********************   MAIN CODE SECTION    ******************************/
/*****************************************************************************/
int main(void)
{
	/* Initialize Clocks For Needed Peripherals */
	Clock_Init( ) ;

	/* Pins Initialization through GPIO */
	Pins_Init( ) ;

	/* Initialize SPI Peripheral */
	SPI1_Init( ) ;

	/* Intterupts Enable Through NVIC */
	Interrupts_Init( ) ;

	
	/* Receive Using SPI with Interrupt */
	Receive_withInterrupt( ) ;


	/* Loop forever */
	while(1)
	{
		/* Check if data is receive */
		if(flag ==1)
		{
			/* Check if receive is repeated */
			if( RECEIVED == 0 )
			{
				/* Receive Time form Nucleo*/
				Receive_withInterrupt( ) ;
				/* to perevent calling the Function more than one time without receiving data */
				RECEIVED = 1 ;
			}
			/* Display Date on LCD */
			Display_Date();
			/* Calculate the Time */
			Count_Time();
			/* Delay 1 sec */
			DELAY_1S();
			/* Display Time on LCD */
			Display_Time();

		}
	}
}



/*****************************************************************************/
/***********************      ISRs SECTION      ******************************/
/*****************************************************************************/

void SPI1_CallBack( void )
{
	if( Received[0] == RED_LED_CODE )
	{
		/* Turn RED LED ON */
		TURN_ON_LED( ) ;
	}
	else if (Received[0] == ALAEMCODE)
	{
		DisplayAlarmInfo();
	}
	else
	{
		flag=1;
		RECEIVED = 0 ;
	}
}




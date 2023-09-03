/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mohamed Wael
 * @brief          : Main program body For BlueBill in Clock System Project
 ******************************************************************************
 */

/*****************************************************************************/
/********************    MAIN INCLUDES SECTION    ****************************/
/*****************************************************************************/

#include <stdint.h>

#include "../Service/Inc/Service.h"

/* ================================================== *
 *  GLOBAL VARIABLES                                  *
 * ================================================== */
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
	else
	{
		flag=1;
		RECEIVED = 0 ;
	}
}




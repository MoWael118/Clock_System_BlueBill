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

uint8_t  DATA_ReceivedFromSPI = 0 ;

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

	}
}

/*****************************************************************************/
/***********************      ISRs SECTION      ******************************/
/*****************************************************************************/

void SPI1_CallBack( void )
{
	if( DATA_ReceivedFromSPI == RED_LED_CODE )
	{
		/* Turn RED LED ON */
		TURN_ON_LED( ) ;
	}
}





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

#include "../LIBRARY/ErrTypes.h"
#include "../LIBRARY/BitMath.h"
#include "../LIBRARY/STM32F103xx.h"
#include "../MCAL/Inc/SYSTICK_Interface.h"

#include "../Service/Inc/Service.h"

/*****************************************************************************/
/*******************   GLOBAL VARIABLES SECTION    ***************************/
/*****************************************************************************/

extern volatile uint8_t flag ;


volatile uint8_t RECEIVED = 0 ;
/*****************************************************************************/
/************************   MACROS SECTION    ********************************/
/*****************************************************************************/



/*****************************************************************************/
/******************   FUNCTIONS PROTOTYPES SECTION    ************************/
/*****************************************************************************/



/*****************************************************************************/
/***********************   MAIN CODE SECTION    ******************************/
/*****************************************************************************/
int main(void)
{

    /* Initialization Clock */
	Clock_Init( ) ;

	/* Receive Time form Nucleo*/
	Recive_Time();

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
				   Recive_Time( ) ;
				   /* to perevent calling the Function more than one time without receiving data */
				   RECEIVED = 1 ;
			}
			/* Display Date on LCD */
			Display_Date();
			/* Calculate the Time */
			Count_Time();
			/* Delay 1 sec */
			SYSTICK_u8Delay_ms(1000);
			/* Display Time on LCD */
			Display_Time();




		}


	}
}


/*****************************************************************************/
/****************   FUNCTIONS IMPLEMENTATION SECTION    **********************/
/*****************************************************************************/



void SPI_CALL_BACK(void)
{
	flag=1;
   RECEIVED = 0 ;
}

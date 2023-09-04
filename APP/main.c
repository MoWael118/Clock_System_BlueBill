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
#include "../LIBRARY/BitMath.h"
#include "../Service/Inc/Service.h"
#include "../MCAL/Inc/SPI_Interface.h"

/*****************************************************************************/
/************************   MACROS SECTION    ********************************/
/*****************************************************************************/
/* Global Pointer to SPI_CONFIGS_t Struct */
extern SPI_CONFIGS_t *SPICONFIG;

/* Global Array to store the received data from the SPI
 * The first element is the alarm number
 * The Rest of the elements are the Alarm Name until the first 13(ASCII of Enter) */
extern uint8_t RecivedData[30];

/*****************************************************************************/
/***********************   MAIN CODE SECTION    ******************************/
/*****************************************************************************/
int main(void)
{
	/* Initialize the Clock */
	ClockInit();

	/* Initialize the Pins */
	PinInit();

	/* Initialize the SPI1 */
	SPI1_Init();

	/* Enabling Receive Interrupt of SPI and store the received data in RecivedData Array */
	SPI_Receive_IT(SPICONFIG, RecivedData, 30, &SPI1_CallBack);

	/* Loop forever */
	while (1)
	{
	}
}
/*****************************************************************************/
/***********************   FUNCTIONS IMPLEMENTATION    ***********************/
/*****************************************************************************/
/*==============================================================================================================================================
 *@fn      :  void ClockInit()
 *@brief  :  This Function Is Responsible For Initializing The Clocks For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void SPI1_CallBack()
{
	DisplayAlarmInfo();
}

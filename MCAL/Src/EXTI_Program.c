/*
 ******************************************************************************
 * @file           : EXTI_Program.c
 * @Author         : Ibrahim Mohamed
 * @brief          : EXTI Main Program File
 * @Date           : Jul 17, 2023
 ******************************************************************************
 */


/**********************   Includes section *****************************/
#include "stdint.h"
#include "BIT_MATH.h"
#include "Stm32f103xx.h"
#include "ErrType.h"
#include "EXTI_Interface.h"


/**********************   Global Variables section *****************************/
static void(*EXTI_GPCallBackFunc[Number_EXTILine])(void)= {NULL};



/**********************   Function Implementation section *****************************/

/*******************************************************
 * @func EXTI_Init
 * @brief the function initializes the ُEXTI according to the input parameters
 * @parameter[in] EXTI_Confg : the initialization values of the EXTI
 * @retval ErrorStatus
 */

uint8_t EXTI_Init(EXTI_Confg* Copy_EXTI)
{
	uint8_t Error_state =OK;

	if( (Copy_EXTI != NULL) && (Copy_EXTI->EXTI_CallBackFunc != NULL) )
	{
		EXTI_GPCallBackFunc[Copy_EXTI->LINE] = (Copy_EXTI->EXTI_CallBackFunc) ;

		if( ( (Copy_EXTI->LINE) < Max_Number_EXTILine ) )
		{
			if( (Copy_EXTI->Mode) == Enable)
			{
				/* enable EXTI Line */
				EXTI->IMR |= ( 1 << (Copy_EXTI->LINE)) ;
			}
			else if( (Copy_EXTI->Mode) == Disable)
			{
				/* disable EXTI Line */
				EXTI->IMR &= ~( 1 << (Copy_EXTI->LINE)) ;
			}
			else
			{
				Error_state =NOK;
			}

			if( (Copy_EXTI->Trigger) == FallingEdge)
			{
				/* set trigger configuration to Falling edge */
				EXTI->FTSR |= ( 1 << (Copy_EXTI->LINE)) ;
				/* Clear trigger configuration to Raising edge */
				EXTI->RTSR &= ~( 1 << (Copy_EXTI->LINE)) ;
			}
			else if( (Copy_EXTI->Trigger) == RaisingEdge)
			{
				/* set trigger configuration to Raising edge */
				EXTI->RTSR |= ( 1 << (Copy_EXTI->LINE)) ;
				/* Clear trigger configuration to Falling edge */
				EXTI->FTSR &= ~( 1 << (Copy_EXTI->LINE)) ;
			}
			else if( (Copy_EXTI->Trigger) == Both)
			{
				/* set trigger configuration to Raising edge */
				EXTI->RTSR |= ( 1 << (Copy_EXTI->LINE)) ;
				/* set trigger configuration to Falling edge */
				EXTI->FTSR |= ( 1 << (Copy_EXTI->LINE)) ;
			}
			else
			{
				Error_state =NOK;
			}
		}
		else
		{
			Error_state =NOK;
		}
	}

	else
	{
		Error_state =NOK;
	}
	return Error_state;
}




/*******************************************************
 * @func EXTI_Enable
 * @brief the function Enable the ُEXTI Line
 * @parameter[in] EXTI_Line : Enable the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_Enable(EXTI_Line Copy_EXTI_Line)
{
	uint8_t Error_state =OK;

	if( Copy_EXTI_Line < Max_Number_EXTILine  )
	{
		/* enable EXTI Line */
		EXTI->IMR |= ( 1 << (Copy_EXTI_Line)) ;
	}
	else
	{
		Error_state =NOK;
	}

	return Error_state;
}



/*******************************************************
 * @func EXTI_Disable
 * @brief the function Disable the ُEXTI Line
 * @parameter[in] EXTI_Line : Disable the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_Disable(EXTI_Line Copy_EXTI_Line)
{
	uint8_t Error_state =OK;

	if( Copy_EXTI_Line < Max_Number_EXTILine  )
	{
		/* Disable EXTI Line */
		EXTI->IMR &= ~( 1 << (Copy_EXTI_Line)) ;
	}
	else
	{
		Error_state =NOK;
	}

	return Error_state;
}




/*******************************************************
 * @func EXTI_ClearPendingFlag
 * @brief the function Clear Pending Flag for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_ClearPendingFlag(EXTI_Line Copy_EXTI_Line)
{
	uint8_t Error_state =OK;

	if( Copy_EXTI_Line < Max_Number_EXTILine  )
	{
		/* Clear Pending Flag for EXTI Line */
		EXTI->PR = ( 1 << (Copy_EXTI_Line)) ;
	}
	else
	{
		Error_state =NOK;
	}

	return Error_state;
}



/*******************************************************
 * @func EXTI_ReadPendingFlag
 * @brief the function Read Pending Flag for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_ReadPendingFlag(EXTI_Line Copy_EXTI_Line, uint8_t *Copy_u8Value)
{
	uint8_t Error_state =OK;

	if( Copy_EXTI_Line < Max_Number_EXTILine && ( Copy_u8Value != NULL ) )
	{
		/* Read Pending Flag for EXTI Line */
		*Copy_u8Value = ( ( (EXTI->PR) >> Copy_EXTI_Line ) & 0x01 );
	}
	else
	{
		Error_state =NOK;
	}

	return Error_state;
}


/*******************************************************
 * @func EXTI_SetTrigger
 * @brief the function Set Trigger for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @parameter[in] TriggerMode
 * @retval ErrorStatus
 */
uint8_t EXTI_SetTrigger(EXTI_Line Copy_EXTI_Line, TriggerMode Copy_trigger)
{
	uint8_t Error_state =OK;

	if( (Copy_trigger) == FallingEdge)
	{
		/* set trigger configuration to Falling edge */
		EXTI->FTSR |= ( 1 << (Copy_EXTI_Line)) ;
		/* Clear trigger configuration to Raising edge */
		EXTI->RTSR &= ~( 1 << (Copy_EXTI_Line)) ;
	}
	else if( (Copy_trigger) == RaisingEdge)
	{
		/* set trigger configuration to Raising edge */
		EXTI->RTSR |= ( 1 << (Copy_EXTI_Line)) ;
		/* Clear trigger configuration to Falling edge */
		EXTI->FTSR &= ~( 1 << (Copy_EXTI_Line)) ;
	}
	else if( (Copy_trigger) == Both)
	{
		/* set trigger configuration to Raising edge */
		EXTI->RTSR |= ( 1 << (Copy_EXTI_Line)) ;
		/* set trigger configuration to Falling edge */
		EXTI->FTSR |= ( 1 << (Copy_EXTI_Line)) ;
	}
	else
	{
		Error_state =NOK;
	}
	return Error_state;
}



void EXTI0_IRQHandler(void)
{
	/* Clear Pending Flag */
	EXTI->PR |= ( 1 << (EXTI0)) ;
	/* Call Back Function */
	EXTI_GPCallBackFunc[EXTI0]();

}


void EXTI1_IRQHandler(void)
{
	/* Clear Pending Flag */
	EXTI->PR |= ( 1 << (EXTI1)) ;
	/* Call Back Function */
	EXTI_GPCallBackFunc[EXTI1]();

}

void EXTI2_IRQHandler(void)
{
	/* Clear Pending Flag */
	EXTI->PR |= ( 1 << (EXTI2)) ;
	/* Call Back Function */
	EXTI_GPCallBackFunc[EXTI2]();

}

void EXTI3_IRQHandler(void)
{
	/* Clear Pending Flag */
	EXTI->PR |= ( 1 << (EXTI3)) ;
	/* Call Back Function */
	EXTI_GPCallBackFunc[EXTI3]();

}

void EXTI4_IRQHandler(void)
{
	/* Clear Pending Flag */
	EXTI->PR |= ( 1 << (EXTI4)) ;
	/* Call Back Function */
	EXTI_GPCallBackFunc[EXTI4]();

}



void EXTI9_5_IRQHandler(void)
{
	if(GET_BIT( (EXTI->PR),EXTI5 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI5)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI5]();

	}
	if(GET_BIT( (EXTI->PR),EXTI6 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI6)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI6]();

	}
	if(GET_BIT( (EXTI->PR),EXTI7 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI7)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI7]();

	}
	if(GET_BIT( (EXTI->PR),EXTI8 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI8)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI8]();


	}
	if(GET_BIT( (EXTI->PR),EXTI9 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI9)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI9]();

	}
}



void EXTI15_10_IRQHandler(void)
{
	if(GET_BIT( (EXTI->PR),EXTI10 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI10)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI10]();

	}
	if(GET_BIT( (EXTI->PR),EXTI11 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI11)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI11]();

	}
	if(GET_BIT( (EXTI->PR),EXTI12 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI12)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI12]();

	}
	if(GET_BIT( (EXTI->PR),EXTI13 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI13)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI13]();


	}
	if(GET_BIT( (EXTI->PR),EXTI14 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI14)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI14]();

	}
	if(GET_BIT( (EXTI->PR),EXTI15 ))
	{
		/* Clear Pending Flag */
		EXTI->PR |= ( 1 << (EXTI15)) ;
		/* Call Back Function */
		EXTI_GPCallBackFunc[EXTI15]();

	}

}



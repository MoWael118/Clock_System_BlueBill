/*
 ******************************************************************************
 * @file           : NVIC_Program.c
 * @Author         : Ibrahim Mohamed
 * @brief          : NVIC Main Program File
 * @Date           : Jul 4, 2023
 ******************************************************************************
 */


#include <stdint.h>
#include "stdint.h"
#include "../../LIBRARY/BitMath.h"
#include "../../LIBRARY/STM32F103xx.h"
#include "../../LIBRARY/ErrTypes.h"
#include "../Inc/NVIC_Interface.h"
#include "../Inc/NVIC_Private.h"


/****************************************************************************
 *@fn NVIC_EnableIRQ
 *@brief the function Enables NVIC interrupt for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to enable the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_EnableIRQ(IRQ_Type IRQ_Num)
{
	uint8_t Local_u8ErrorStatus = OK;
	if(IRQ_Num <= 31)
	{
		NVIC->ISER[0]= 1 << IRQ_BIT_NUM(IRQ_Num);

	}
	else if((IRQ_Num >=32) && (IRQ_Num <= 59))
	{
		NVIC->ISER[1]= 1 << IRQ_BIT_NUM(IRQ_Num);
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;
}



/****************************************************************************
 *@fn NVIC_DiableIRQ
 *@brief the function Disables NVIC interrupt for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to disable the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_DiableIRQ(IRQ_Type IRQ_Num)
{
	uint8_t Local_u8ErrorStatus = OK;
	if(IRQ_Num <= 31)
	{
		NVIC->ICER[0]= 1 << IRQ_BIT_NUM(IRQ_Num);

	}
	else if((IRQ_Num >=32) && (IRQ_Num <= 59))
	{
		NVIC->ICER[1]= 1 << IRQ_BIT_NUM(IRQ_Num);
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;
}



/****************************************************************************
 *@fn NVIC_SetPendingFlag
 *@brief the function Sets the pending flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to set the pending flag of the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_SetPendingFlag(IRQ_Type IRQ_Num)
{
	uint8_t Local_u8ErrorStatus = OK;
	if(IRQ_Num <= 31)
	{
		NVIC->ISPR[0]= 1 << IRQ_BIT_NUM(IRQ_Num);

	}
	else if((IRQ_Num >=32) && (IRQ_Num <= 59))
	{
		NVIC->ISPR[1]= 1 << IRQ_BIT_NUM(IRQ_Num);
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;
}



/****************************************************************************
 *@fn NVIC_ClearPendingFlag
 *@brief the function Clears the pending flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to clear the pending flag of the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_ClearPendingFlag(IRQ_Type IRQ_Num)
{
	uint8_t Local_u8ErrorStatus = OK;
	if(IRQ_Num <= 31)
	{
		NVIC->ICPR[0]= 1 << IRQ_BIT_NUM(IRQ_Num);

	}
	else if((IRQ_Num >=32) && (IRQ_Num <= 59))
	{
		NVIC->ICPR[1]= 1 << IRQ_BIT_NUM(IRQ_Num);
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;
}




/****************************************************************************
 *@fn NVIC_GetActiveFlag
 *@brief the function Gets the Active flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to get the Active flag of the interrupt
 *@paramter[in] ActiveFlag_Value :  Pointer to a Variable that User Takes From it the Active State
 *@retval ErrorStatus
 */
uint8_t NVIC_GetActiveFlag(IRQ_Type IRQ_Num, uint8_t *ActiveFlag_Value)
{
	uint8_t Local_u8ErrorStatus = OK;
	if((IRQ_Num <= 31) && (ActiveFlag_Value != NULL))
	{
		*ActiveFlag_Value = ( NVIC->IABR[0] >> IRQ_BIT_NUM(IRQ_Num) ) & 0x1 ;

	}
	else if((IRQ_Num >=32) && (IRQ_Num <= 59)&& (ActiveFlag_Value != NULL))
	{
		*ActiveFlag_Value = ( NVIC->IABR[0] >> IRQ_BIT_NUM(IRQ_Num) ) & 0x1 ;
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;

}



/****************************************************************************
 *@fn NVIC_setPriority
 *@brief the function Sets the Priority and Sub priority for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to set the Priority to the interrupt
 *@paramter[in] Priority : Priority of the Specified IRQ
 *@retval Void
 */
void NVIC_setPriority(IRQ_Type a_IRQn, uint8_t Priority )
{
	/* set the priority for a_IRQn */
	NVIC->IPR[a_IRQn] = ( Priority << 4 ) ;
}





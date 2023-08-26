/*
 ******************************************************************************
 * @file           : NVIC_Interface.h
 * @Author         : Ibrahim Mohamed
 * @brief          : NVIC Interface File
 * @Date           : Jul 4, 2023
 ******************************************************************************
 */

#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_


/* ---------------------------------------- ENUMS ---------------------------------------- */

/*
 * Enum Name: IRQn_Type
 *
 * Enum Description:
 * This enum is responsible for providing easier interface with IRQ numbers by mapping the
 * numbers to the vector table acronyms of STM32F10xxx.
 */
typedef enum
{
	WWDG_IRQ,              			/* Window Watchdog interrupt                                          */
  	PVD_IRQ,               			/* PVD through EXTI line detection interrupt                          */
  	TAMP_STAMP_IRQ,        			/* Tamper and TimeStamp interrupts through the EXTI line              */
  	RTC_WKUP_IRQ,          			/* RTC Wakeup interrupt through the EXTI line                         */
  	FLASH_IRQ,             			/* Flash global interrupt                                             */
  	RCC_IRQ,               			/* RCC global interrupt                                               */
  	EXTI0_IRQ,             			/* EXTI Line0 interrupt                                               */
  	EXTI1_IRQ,             			/* EXTI Line1 interrupt                                               */
  	EXTI2_IRQ,             			/* EXTI Line2 interrupt                                               */
  	EXTI3_IRQ,             			/* EXTI Line3 interrupt                                               */
  	EXTI4_IRQ,             			/* EXTI Line4 interrupt                                               */
  	DMA1_Channel1_IRQ,     			/* DMA1 Channel1 global interrupt                                     */
  	DMA1_Channel2_IRQ,     			/* DMA1 Channel2 global interrupt                                     */
  	DMA1_Channel3_IRQ,    			/* DMA1 Channel3 global interrupt                                     */
  	DMA1_Channel4_IRQ,     			/* DMA1 Channel4 global interrupt                                     */
  	DMA1_Channel5_IRQ,     			/* DMA1 Channel5 global interrupt                                     */
  	DMA1_Channel6_IRQ,      		/* DMA1 Channel6 global interrupt                                     */
	DMA1_Channel7_IRQ,      		/* DMA1 Channel7 global interrupt                                     */
	ADC1_2_IRQ,               		/* ADC1 global interrupt                                              */
	USB_HP_CAN_TX_IRQ,           	/* USB High Priority or CAN TX interrupts                             */
	USB_LP_CAN__RX0_IRQ,            /* USB Low Priority or CAN RX0 interrupts                             */
  	CAN_RX1_IRQ,          			/* CAN RX1 interrupts                                                 */
  	CAN_SCE_IRQ,          			/* CAN SCE interrupt                                                  */
  	EXTI9_5_IRQ,           			/* EXTI Line[9:5] interrupts                                          */
  	TIM1_BRK_TIM9_IRQ,     			/* TIM1 Break interrupt and TIM9 global interrupt                     */
  	TIM1_UP_TIM10_IRQ,     			/* TIM1 Update interrupt and TIM10 global interrupt                   */
  	TIM1_TRG_COM_IRQ,			    /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt */
  	TIM1_CC_IRQ,           			/* TIM1 Capture Compare interrupt                                     */
  	TIM2_IRQ,              			/* TIM2 global interrupt                                              */
  	TIM3_IRQ,              			/* TIM3 global interrupt                                              */
  	TIM4_IRQ,              			/* TIM4 global interrupt                                              */
  	I2C1_EV_IRQ,           			/* I2C1 event interrupt                                               */
  	I2C1_ER_IRQ,           			/* I2C1 error interrupt                                               */
  	I2C2_EV_IRQ,           			/* I2C2 event interrupt                                               */
  	I2C2_ER_IRQ,           			/* I2C2 error interrupt                                               */
  	SPI1_IRQ,              			/* SPI1 global interrupt                                              */
  	SPI2_IRQ,              			/* SPI2 global interrupt                                              */
  	USART1_IRQ,            			/* USART1 global interrupt                                            */
  	USART2_IRQ,            			/* USART2 global interrupt                                            */
  	USART3_IRQ,            			/* USART3 global interrupt                                            */
  	EXTI15_10_IRQ,         			/* EXTI Line[15:10] interrupts                                        */
  	RTC_Alarm_IRQ,         			/* RTC Alarms (A and B) through EXTI line interrupt                   */
    USB_Wakeup_IRQ,                 /* USB wakeup from suspend through EXTI line interrupt                */
	TIM8_BRK,                       /* TIM8 Break interrupt                                               */
	TIM8_UP,                        /* TIM8 Update interrupt                                              */
	TIM8_TRG_COM,                   /* TIM8 Trigger and Commutation interrupts                            */
	TIM8_CC,                        /* TIM8 Capture Compare interrupt                                     */
	ADC3,                           /* ADC3 global interrup                                               */
	FSMC,                           /*  FSMC global interrupt                                             */
	SDIO,                           /*  SDIO global interrup                                              */
  	TIM5_IRQ,    	        		/* TIM5 global interrupt                                              */
  	SPI3_IRQ,     	        		/* SPI3 global interrupt                                              */
  	UART4_IRQ,		            	/* UART4 global interrupt                                             */
	UART5_IRQ,           			/* UART5 global interrupt                                             */
  	DMA2_Channel1_IRQ,      		/* DMA2 Channel1 global interrupt                                     */
  	DMA2_Channel2_IRQ,      		/* DMA2 Channel2 global interrupt                                     */
  	DMA2_Channel3_IRQ,      		/* DMA2 Channel3 global interrupt                                     */
  	DMA2_Channel4_5_IRQ,     		/* DMA2 Channel4 and DMA2 Channel5 global interrupt                   */


}IRQ_Type;


/****************************************************************************
 *@fn NVIC_EnableIRQ
 *@brief the function Enables NVIC interrupt for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to enable the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_EnableIRQ(IRQ_Type IRQ_Num);


/****************************************************************************
 *@fn NVIC_DiableIRQ
 *@brief the function Disables NVIC interrupt for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to disable the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_DiableIRQ(IRQ_Type IRQ_Num);


/****************************************************************************
 *@fn NVIC_SetPendingFlag
 *@brief the function Sets the pending flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to set the pending flag of the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_SetPendingFlag(IRQ_Type IRQ_Num);


/****************************************************************************
 *@fn NVIC_ClearPendingFlag
 *@brief the function Clears the pending flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to clear the pending flag of the interrupt
 *@retval ErrorStatus
 */
uint8_t NVIC_ClearPendingFlag(IRQ_Type IRQ_Num);


/****************************************************************************
 *@fn NVIC_GetActiveFlag
 *@brief the function Gets the Active flag for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to get the Active flag of the interrupt
 *@paramter[in] ActiveFlag_Value :  Pointer to a Variable that User Takes From it the Active State
 *@retval ErrorStatus
 */
uint8_t NVIC_GetActiveFlag(IRQ_Type IRQ_Num, uint8_t *ActiveFlag_Value);


/****************************************************************************
 *@fn NVIC_setPriority
 *@brief the function Sets the Priority and Sub priority for the given IRQ
 *@paramter[in] IRQ_Num : indicate the IRQ number to set the Priority to the interrupt
 *@paramter[in] Priority : Priority of the Specified IRQ
 *@retval Void
 */
void NVIC_setPriority(IRQ_Type a_IRQn, uint8_t Priority );

#endif /* NVIC_INTERFACE_H_ */

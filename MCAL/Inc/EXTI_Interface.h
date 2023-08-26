/*
 ******************************************************************************
 * @file           : EXTI_Interface.h
 * @Author         : Ibrahim Mohamed
 * @brief          : EXTI Interface File
 * @Date           : Jul 17, 2023
 ******************************************************************************
 */


#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_



/**********************   enum section *****************************/

typedef enum
{
	EXTI0=0,
	EXTI1,
	EXTI2,
	EXTI3,
	EXTI4,
	EXTI5,
	EXTI6,
	EXTI7,
	EXTI8,
	EXTI9,
	EXTI10,
	EXTI11,
	EXTI12,
	EXTI13,
	EXTI14,
	EXTI15,

}EXTI_Line;

typedef enum
{
	FallingEdge=0,
	RaisingEdge,
	Both
}TriggerMode;


typedef enum
{
	Enable=0,
	Disable
}E_D_Mode;



typedef struct
{
	EXTI_Line LINE;
	TriggerMode Trigger;
	E_D_Mode Mode;
	void(*EXTI_CallBackFunc)(void);
}EXTI_Confg;



/**********************   Macros section *****************************/

#define Number_EXTILine            16

#define Max_Number_EXTILine        16





/**********************   Functions section *****************************/

/*******************************************************
 * @func EXTI_Init
 * @brief the function initializes the ُEXTI according to the input parameters
 * @parameter[in] EXTI_Confg : the initialization values of the EXTI
 * @retval ErrorStatus
 */
uint8_t EXTI_Init(EXTI_Confg* Copy_EXTI);


/*******************************************************
 * @func EXTI_Enable
 * @brief the function Enable the ُEXTI Line
 * @parameter[in] EXTI_Line : Enable the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_Enable(EXTI_Line Copy_EXTI_Line);


/*******************************************************
 * @func EXTI_Disable
 * @brief the function Disable the ُEXTI Line
 * @parameter[in] EXTI_Line : Disable the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_Disable(EXTI_Line Copy_EXTI_Line);


/*******************************************************
 * @func EXTI_ClearPendingFlag
 * @brief the function Clear Pending Flag for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_ClearPendingFlag(EXTI_Line Copy_EXTI_Line);


/*******************************************************
 * @func EXTI_ReadPendingFlag
 * @brief the function Read Pending Flag for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @retval ErrorStatus
 */
uint8_t EXTI_ReadPendingFlag(EXTI_Line Copy_EXTI_Line, uint8_t *Copy_u8Value);


/*******************************************************
 * @func EXTI_SetTrigger
 * @brief the function Set Trigger for the ُEXTI Line
 * @parameter[in] EXTI_Line :  the EXTI Line
 * @parameter[in] TriggerMode
 * @retval ErrorStatus
 */
uint8_t EXTI_SetTrigger(EXTI_Line Copy_EXTI_Line, TriggerMode Copy_trigger);


#endif /* EXTI_INTERFACE_H_ */

/**
  ******************************************************************************
  * @file    Templates/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "usart.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
#define NVIC_HFSR    (*(volatile unsigned int*)  (0xE000ED2Cu))  // Hard Fault Status Register
/*****
__asm void HardFault_Handler(void){
        extern dump_context;
        // This version is for Cortex M3, Cortex M4 and Cortex M4F
        tst    LR, #4            ;// Check EXC_RETURN in Link register bit 2.
        ite    EQ
        mrseq  R0, MSP           ;// Stacking was using MSP.
        mrsne  R0, PSP           ;// Stacking was using PSP.
        b      dump_context  ;// Stack pointer passed through R0.
        
     //   END
}***/
uint8_t Tx_str22[] = "hello\r\n" ;
typedef struct  stack_context {
	//uint32_t exc;
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12; // ip
	uint32_t lr;
	uint32_t pc; // return address
	uint32_t xpsr;
} stack_context;
 void dump_context(stack_context *ctx) {
	//printf(" exc  =0x%08x\r\n", ctx->exc);
	printf(" r0  =0x%08x\r\n", ctx->r0);
	printf(" r1  =0x%08x\r\n", ctx->r1);
	printf(" r2  =0x%08x\r\n", ctx->r2);
	printf(" r3  =0x%08x\r\n", ctx->r3);
	printf(" r12 =0x%08x\r\n", ctx->r12);
	printf(" lr  =0x%08x\r\n", ctx->lr);
	printf(" pc  =0x%08x\r\n", ctx->pc);
	printf(" xpsr=0x%08x\r\n", ctx->xpsr);
	ctx->pc += 2u;
}
void HardFaultHandler(unsigned int* pStack) {


	  *(pStack + 6u) += 2u;         // PC is located on stack at SP + 24 bytes. Increment PC by 2 to skip break instruction.

    //printf("hhh");
	

 
}
/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

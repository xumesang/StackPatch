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
#include "./SYSTEM/SYS/sys.h" 
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
/*
 * fault exception handler
 */
//void(*ptr)();

struct exception_info
{
  uint32_t exc_return;   //*(pStack)
	uint32_t r4;    //*(pStack + 1u)
  uint32_t r5;    //*(pStack + 2u)
	uint32_t r6;    //*(pStack + 3u)
	uint32_t r7;    //*(pStack + 4u)
	uint32_t r8;    //*(pStack + 5u)
	uint32_t r9;    //*(pStack + 6u)
	uint32_t r10;   //*(pStack + 7u)
	uint32_t r11;   //*(pStack + 8u)
	uint32_t r0;    //*(pStack + 9u)
	uint32_t r1;    //*(pStack + 10u)
	uint32_t r2;    //*(pStack + 11u)
	uint32_t r3;    //*(pStack + 12u)
	uint32_t r12;   //*(pStack + 13u)
	uint32_t lr;    //*(pStack + 14u)
	uint32_t pc;    //*(pStack + 15u)
	uint32_t psr;   //*(pStack + 16u)
};
void rt_hw_hard_fault_exception(struct exception_info * exception_info)
{ 
      printf("this is hard fault\r\n");
	  printf("exc: 0x%08x\r\n", exception_info->exc_return);
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r08: 0x%08x\r\n", exception_info->r8);
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
    printf("r12: 0x%08x\r\n", exception_info->r12);
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);
    //exception_info->pc +=2u;


}

void rt_hw_hard_fault_exception1(struct exception_info * exception_info)
{ /*
 printf("this is debugmon fault\r\n");
	  printf("exc: 0x%08x\r\n", exception_info->exc_return);
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r08: 0x%08x\r\n", exception_info->r8);
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
    printf("r12: 0x%08x\r\n", exception_info->r12);
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);
    //exception_info->pc +=2u;

		 fixs: 
	if (uxHeaderLenger->xDataLength - ipSIZE_OF_ETH_HEADER ||
 	uxHeaderLength < ipSIZE_OF_IPv4_HEADER)
 	{
		return -1;
	}*/
	if (exception_info->r3 > *(uint32_t *)(exception_info->r4+ 8) - 14u ||
 	    exception_info->r3 < 20u)
 	{ exception_info->r0 = -1;
		exception_info->pc += 2u;
	}else{
	  exception_info->r0 = 0;
		exception_info->pc += 2u;
	}

}

PatchTableItem PatchTable[64] ={0};
//int n;
unsigned int BinaySearch(unsigned int target)
{  // printf("BINARY");
    int left = 0;
    int right = 63;	
    while (left <= right) {	
        int middle = left + ((right - left) / 2);
        if (PatchTable[middle].vul_address > target) {
            	left = middle + 1;
        } else if (PatchTable[middle].vul_address < target) {
            	right = middle - 1;
        } else {	
            return PatchTable[middle].patch_address ;
        }
    }
    
		
    return 0x20009001;
}
#define __NAKE __attribute__((naked))
__NAKE __asm void patch_dispatch(unsigned int* pStack)  {
	 IMPORT BinaySearch
	 MOV R4, R0
	 LDR  R0, [R0,#0x3C]
	 BL   BinaySearch
	 MOV R1, R0
	 MOV R0, R4
	 MOV PC, R1
}
/*
/*
void HardFaultHandlerd_dispatch(unsigned int* pStack) {
 *(pStack + 15u) += 2u;
	printf("hhh");
}

 */

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
#if (!SYS_SUPPORT_OS)
void SVC_Handler(void)
{
}
#endif 
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */




void DebugMon_dispatch(uint32_t *sp) {
	if (*(sp + 12u) > *(uint32_t *)(*(sp + 1u) + 8) - 14u ||
 	    *(sp + 12u) < 20u)
 	{ *(sp + 9u) = -1;
		//exception_info->lr += 2u;
	}
}



/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if (!SYS_SUPPORT_OS) 
void PendSV_Handler(void)
{
}
#endif 
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if (!SYS_SUPPORT_OS) 
void SysTick_Handler(void)
{
  HAL_IncTick();
}
#endif
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

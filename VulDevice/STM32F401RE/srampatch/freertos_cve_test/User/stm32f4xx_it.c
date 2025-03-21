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
#define NVIC_HFSR    (*(volatile unsigned int*)  (0xE000ED2Cu))  // Hard Fault Status Register
extern volatile uint32_t back_instruction_address;
void rt_hw_hard_fault_exception(struct exception_info * exception_info)
{  // ptr=(void(*)())0x20008001;
	 // ptr();
	 // NVIC_HFSR |=  (1u << 31);     // Reset Hard Fault status
  
	   //  ptr=(void(*)())0x20008001;
    //ptr();
    // patchfunc();
	 // NVIC_HFSR |=  (1u << 31);
	  uint16_t bpkt_inst;
	  uint32_t addr;
	  addr=exception_info->pc;
	  bpkt_inst = *((uint16_t*)addr)&0xff;
	  
	  //exception_info->pc += 2u ;
	  //exception_info->r4 = 0x2;
	  
   // uint32_t *app_sp;
   // int i;
   // app_sp = (uint32_t *)(exception_info + 1);  /* context + 16*4 */
	
	
    while(1){
		printf("instr: 0x%08x\r\n", bpkt_inst);
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
  //   printf("%08x ", *app_sp);
  //   app_sp++;
  //   i++;
   //   if (i % 16 == 0)
   //    printf("\r\n");
		
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
    printf("r12: 0x%08x\r\n", exception_info->r12);
  //  printf("stacks: \r\n");
  //  i = 0;
  //   for (i = 0; i < 1024; )
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);
  //  {
   //  
   //  }
      printf("\r\n");
	  }

}


 __asm void HardFaultHandlerd_dispatch(unsigned int* pStack) __attribute__((section(".ARM.__at_0x20007000")));





__asm void HardFaultHandlerd_dispatch(unsigned int* pStack) {
	// IMPORT MemManage_Handler
	 LDR           R1, [R0,#0x3C]   
	 LDR.w           R2,=0x20002004
   CMP           R1, R2
	 BEQ           patch1
	 LDR.w           R2,=0x20002004
   CMP           R1, R2
	 BEQ           patch2
patch1
	 LDR PC,=0x2009001
patch2
	 LDR PC,=0x200090
patch3
	 LDR PC,=0x2000900
patch4
	 LDR PC,=0x2000901
patch5
	 LDR PC,=0x20009001
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
void DebugMon_Handler(void)
{
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

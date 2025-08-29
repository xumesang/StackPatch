  /**
  ******************************************************************************
  * @file    stm32_xx_it.c
  * @author  Central LAB
  * @version V2.1.0
  * @date    17-May-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32_xx_it.h"
#include "wifi_module.h"
#include "stm32_spwf_wifi.h" 
#include "wifi_globals.h"
#include "main.h"
#include "stm32_xx_it.h"
#include "string.h"
#include "stdbool.h" 
#include "X_NUCLEO_PLC01A1.h"
#include "stm32_spwf_wifi.h" 
#include "Ladder_Lib.h"
#include "stm32f4xx_hal.h"
#include "PLC_Handling.h"

    
 /** @addtogroup PLC_Demo_Example     PLC_Demo Example 
  * @{
  */

/** @addtogroup User        User
  * @{
  */   
    
    
    
/** @defgroup Interrupt_Private_Variables          Interrupt Private Variables
* @{
*/


uint8_t Query_CLT=0;
uint8_t WD_Refresh=0;
uint16_t npolling=0;
uint8_t fault_tick=0;
uint8_t refresh=0;


/**
  * @} // End Interrupt_Private_Variables 
  */

/** @defgroup Interrupt_Exported_Variables    Interrupt Exported Variables
* @{
*/
/* Private variables ---------------------------------------------------------*/
extern bool VNI_Transmission;
extern bool Timer_Running;
extern bool Standby_Timer_Running;
extern bool AT_Cmd_Processing;
extern uint32_t tickcount;
extern uint32_t standby_time;
extern uint8_t CLT_READ;
extern bool Deep_Sleep_Timer;
extern bool Deep_Sleep_Enabled;
extern bool Deep_Sleep_Callback;
extern bool polling;
extern bool RX_ClientFrame;
extern uint32_t sleep_count;
extern uint16_t WD_Reset;
extern TimerStruct_Typedef   tim_setting[MAX_COMPONENT_NUMBER];
extern BOARD_Typedef Board_State;
extern uint8_t timer1_cnt;
extern uint8_t timer2_cnt;
extern uint8_t cnt1TH,cnt2TH;
extern uint8_t start_delay[];
extern uint8_t index_c;
extern uint16_t delay_CNT[];
extern int8_t POG;
extern uint16_t cnt1[],cnt4;
extern uint8_t htim4_idx,htim1_idx;
/**
  * @} // End Interrupt_Exported_Variables 
  */


/** @defgroup Interrupt_Exported_Typedef    Interrupt Exported Typedef
* @{
*/

/* Private typedef -----------------------------------------------------------*/
extern CounterStruct_Typedef    counter_up[];
extern UART_HandleTypeDef UartWiFiHandle,UartMsgHandle;
extern TIM_HandleTypeDef    TimHandle, PushTimHandle;
extern PLC_State_TypeDef PLC_State;
extern CLT_CheckState_Typedef CLT_State;
extern wifi_state_t wifi_state;                         /*!< wifi status variable    */
extern Decode_Status_Typedef Frame_decoding;            /*!< frame decoding variable */
extern BOARD_Typedef Board_State;                       /*!< state machine variable  */
extern TimerStruct_Typedef  tim_setting[MAX_COMPONENT_NUMBER];
extern TIM_HandleTypeDef htim2,htim1,htim,htim4;
/**
  * @} // End Interrupt_Exported_Typedef 
  */

/** @defgroup Interrupt_Private_Define    Interrupt Private define
* @{
*/

/* Private define ------------------------------------------------------------*/
#define listening_time 2000 //ms   
#define loop_time 20//10000

/**
  * @}
  */

/** @defgroup STM32xx_IT_Private_Variables
* @{
*/

/** @addtogroup STM32xx_HAL_Examples
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
void USARTx_IRQHandler(void);    
void USARTx_PRINT_IRQHandler(void);
void USARTx_EXTI_IRQHandler(void);
void TIMx_IRQHandler(void);
void TIMp_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
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
//void HardFault_Handler(void)
//{
  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
 //   BSP_LED_On(LED2); 
//  }
//}
void HardFaultHandlerd_dispatch(unsigned int* pStack) {
*(pStack + 15u)+=2u;

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
    BSP_LED_On(LED2); 
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
    BSP_LED_On(LED2); 
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
    BSP_LED_On(LED2); 
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
  BSP_LED_On(LED2); 
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
  BSP_LED_On(LED2); 
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMx_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMp_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&PushTimHandle);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{   /******** PLC ***********/
    do{
      if((counter_up[index_c].CNT_val!=0)&&(start_delay[index_c]==1))
      {
        delay_CNT[index_c]++;
      }
      index_c++;
    }while(index_c<MAX_COMPONENT_NUMBER);
    index_c=0;

#if (defined PLC01A1_OUT01A1) ||   (defined PLC01A1)  
    
    if(WD_Reset>100)
    {
      WD_Reset++;
      npolling=0;
    } 
    else
      WD_Reset=0;
    /* end RESET SPURIOUS */

    if(WD_Refresh>=1)
    {
      WD_Refresh=0;
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //CS pin
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_SET);//CS1 watchdog refresh
    }
    else
    {
      WD_Refresh++;      
    }
    
    if(POG!=1)
    {
    
      if(Board_State==BOARD_LOOP)
      {               
        PLC_Polling ();
        POG=-1;
      }
      else
      {   
        if((Board_State==BOARD_LISTENING)||(Get_FlagStatus()==1))
        {  
          //        if(POG!=1)
          PLC_Polling();
          POG=-1;
        }
      }
    }
    else
    {
   
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //CS pin
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_SET);//CS1 watchdog refresh
   
    }    
#elif (defined OUT01A1)

      if(POG!=1)
      {
        
        if(Board_State==BOARD_LOOP)
        {               
          PLC_Polling ();
          POG=-1;
        }
        else
        {   
          if((Board_State==BOARD_LISTENING)||(Get_FlagStatus()==1))
          {  
            //        if(POG!=1)
            PLC_Polling();
            POG=-1;
          }
        }
      }

#endif
  HAL_IncTick();
  Wifi_SysTick_Isr();
}


/********************  PLC Timer component **********/

/**
* @brief This function handles TIM1 Update interrupt and TIM10 global interrupt.
*/
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
//   uint16_t pippo=0;
//pippo=htim.Instance->CNT;
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}
/****** end******/


/**
  * @brief  This function GPIO EXTI Callback.
  * @param  Pin number of the GPIO generating the EXTI IRQ
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 
  //RX_EXTI_Isr(GPIO_Pin);
}

/**
* @brief  Period elapsed callback in non blocking mode
*         This timer is used for calling back User registered functions with information
* @param  htim : TIM handle
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
      if(htim==&htim1)
        {
          if(cnt1[htim1_idx]>=cnt1TH)
            {
//              if((POG!=1)&&(Board_State!=BOARD_LISTENING))
//              {
//                PLC_Polling();
//              }
              timer1_cnt=1;
            }
            else
              cnt1[htim1_idx]++; 

        }
        else
        {
          if(htim==&htim4)
          { 
            if(cnt1[htim4_idx]>=cnt2TH)
            {
//              if((POG!=1)&&(Board_State!=BOARD_LISTENING))
//              {
//                PLC_Polling();
//              }
              timer2_cnt=1;
            }
            else
              cnt1[htim4_idx]++; 

          }
          else
          Wifi_TIM_Handler(htim);
        }
}

/**
* @brief  HAL_UART_RxCpltCallback
*         Rx Transfer completed callback
* @param  UsartHandle: UART handle 
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandleArg)
{
  WiFi_HAL_UART_RxCpltCallback(UartHandleArg);
}

/**
* @brief  HAL_UART_TxCpltCallback
*         Tx Transfer completed callback
* @param  UsartHandle: UART handle 
* @retval None
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandleArg)
{
  WiFi_HAL_UART_TxCpltCallback(UartHandleArg);
}

/**
  * @brief  UART error callbacks
  * @param  UsartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    WiFi_HAL_UART_ErrorCallback(UartHandle);
}

/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles USARTx Handler.
  * @param  None
  * @retval None
  */
void USARTx_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UartWiFiHandle);
}

/**
  * @brief  This function handles USARTx vcom Handler.
  * @param  None
  * @retval None
  */
#ifdef USART_PRINT_MSG
void USARTx_PRINT_IRQHandler(void)
{
     HAL_UART_IRQHandler(&UartMsgHandle);
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */
/**
  * @}
  */ 

/**
  * @}
  */
/**
  * @} 
  */
/**
  * @} 
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

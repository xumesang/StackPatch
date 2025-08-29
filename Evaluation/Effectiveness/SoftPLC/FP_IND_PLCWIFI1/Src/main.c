  /**
  ******************************************************************************
  * @file    main.c
  * @author  Central LAB
  * @version V2.1.0
  * @date    17-May-2015
  * @brief   Main program body
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
#include "main.h"
#include "wifi_interface.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "wifi_module.h"
#include "event_buffer.h"


#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_nucleo.h"
#include "stm32_hal_legacy.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_conf.h" 

#include "PLC_main.h" /*!< include file for PLC function prototype*/
    
    
/** @addtogroup PLC_Demo_Example     PLC_Demo Example 
  * @{
  */


/** @addtogroup Drivers     Drivers
  * @{
  */

/** @addtogroup BSP     BSP
  * @{
  * 
  */

/**
  * @}  
  */

/**
  * @}  
  */

/** @addtogroup User        User
  * @{
  */

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/** @defgroup    PLC_Private_variables    PLC Private variables
  * @{
  */


/* Private variables ---------------------------------------------------------*/
   
uint16_t WD_Reset = 0;    
uint8_t CLT_READ = 0;
int8_t  ServerData_RX[512];
uint8_t output_status=0;
uint16_t ToISOPLC =0;
uint8_t ToSPI=0;
uint8_t ToIPS=0;
uint8_t rung_conf[MAX_SERVERDATA_RX];
volatile uint16_t Array_Index=0;
uint16_t* param_array;
char FB_ARRAY[300];
char* var;
bool RX_ClientFrame=WIFI_FALSE;
bool client_connection=WIFI_FALSE;

/**
  * @}  // end Private_variables 
  */



/** @defgroup    PLC_Private_typedef    PLC Private typedef
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
PLC_State_TypeDef PLC_State;
CLT_CheckState_Typedef CLT_State;
PLC_State_TypeDef PLC_State;
BOARD_Typedef Board_State;
TimerParam_Typedef Tim_Struct;
TIM_HandleTypeDef htim2,htim1,htim,htim4;
TimerParam_Typedef Tim_Setting;
Decode_Status_Typedef Frame_decoding;

/**
  * @}  // end Private_typedef 
  */



/** @defgroup    Exported_variables    Exported variables
  * @{
  */

extern uint8_t Input_CHS;
extern bool data_mode;
extern TIM_HandleTypeDef  TimHandle;
extern OutputStructure_Typedef output[MAX_OUTPUT_NUMBER];
extern OutputStructure_Typedef output_temp[MAX_OUTPUT_NUMBER];
extern uint8_t CLT_Device;
extern uint16_t npolling;
extern UART_HandleTypeDef UartHandle,UartMsgHandle;
extern char print_msg_buff[512];
extern char UserDataBuff[];
extern uint8_t output_buffer,VNI_SPI;
extern int8_t POG;
/**
  * @}  // end Exported_variables 
  */

/** @defgroup PLC_WIFI_Private_typedef    PLC WIFI Private typedef
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
WiFi_Status_t wifi_get_AP_settings(void);
extern UART_HandleTypeDef UartHandle,UartMsgHandle;
wifi_state_t wifi_state;
wifi_config config;
wifi_bool set_AP_config = WIFI_FALSE;
WiFi_Priv_Mode mode = None;//WPA2_Personal; 
/**
  * @} 
  */


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** @defgroup PLC_WIFI_Private_typedef    PLC WIFI Private typedef
  * @{
  */
int8_t console_input[1], console_count=0;
uint8_t * ssid = "Ladder Demo Board";
uint8_t channel_num = 6;   
uint16_t len;

char console_ssid[40];
char *answer;
char * seckey = "pacman76";
char echo[512];
char print_msg_buff[512];

/**
  * @} 
  */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Private functions ---------------------------------------------------------*/ 
/* Private functions ---------------------------------------------------------*/

 /**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  WiFi_Status_t status = WiFi_MODULE_SUCCESS;
  char *protocol = "t";
  uint32_t portnumber = 32000;
  Board_State=BOARD_RESET;
  HAL_Init();

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();

  /* configure the timers  */
  Timer_Config( );
  UART_Configuration(115200);
  
#ifdef USART_PRINT_MSG
  UART_Msg_Gpio_Init();
  USART_PRINT_MSG_Configuration(115200);
#endif  
  /* configure GPIO & SPI of PLC  */      
  GPIO_Config();
#if (defined PLC01A1) || (defined PLC01A1_OUT01A1)
  SPI1_Config();
#endif   
  
  /* Init the wi-fi module */  
  
  config.power=wifi_active;
  config.power_level=high;
  config.dhcp=on;//use DHCP IP address
  wifi_state = wifi_state_idle;
  
  // status = wifi_get_AP_settings();
  memcpy(console_ssid, (const char*)ssid, strlen((char*)ssid)); 
  status = wifi_init(&config);

  if(status!=WiFi_MODULE_SUCCESS)
  {
    printf("Error in Config");
    return 0;
  }
    initializePlc(); /*!< PLC Initialization routine*/

  while (1)
  {       
          PLC_WD();        
switch (wifi_state) 
        {
      case wifi_state_reset:
      break;

      case wifi_state_ready:

//        printf("\r\n >>setting up miniAP mode...\r\n");
        
        if(set_AP_config)
            wifi_ap_start((uint8_t *)console_ssid, channel_num);
        else
            wifi_ap_start((uint8_t *)ssid, channel_num);

//       wifi_connect(console_ssid,seckey, mode);

        wifi_state = wifi_state_idle;
      break;

      case wifi_state_connected:
//        printf("\r\n >>connected...\r\n");

        wifi_state = wifi_state_socket;
      break;

      case wifi_state_disconnected:
        wifi_state = wifi_state_reset;
      break;

      case wifi_state_socket:
//      printf("\r\n >>WiFi_RW_Data\r\n");

      /* Read Write Socket data */        
      status = wifi_socket_server_open(portnumber, (uint8_t *)protocol);

//      if(status == WiFi_MODULE_SUCCESS)
//      {
//        printf("\r\n >>Server Socket Open OK \r\n");          
//      }
        wifi_state = wifi_state_idle;

      break;

    case wifi_state_write:
//        printf("\r\n >>Writing data to client\r\n");
        len = strlen(echo);
        /* Read Write Socket data */        
//        status = wifi_socket_server_write(len, echo);
            WiFiServer_Answer((uint8_t*)answer);
//        if(status == WiFi_MODULE_SUCCESS)
//        {
//          printf("\r\n >>Server Socket Write OK \r\n");                  
//        }
        wifi_state = wifi_state_idle;

        break;

    case wifi_state_idle:
//      printf(".");
//      fflush(stdout);
      HAL_Delay(500);
      break;

    default:
      break;
    }  
         
          PLC_WiFi_Interface();
        
		     
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL(HSE)//PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 8//16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */


void SystemClock_Config(void)
{
  
  /******* HSE Bypass*****/
  
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 256;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  
  /****HSI****/
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//
//  /* Enable Power Control clock */
//  __PWR_CLK_ENABLE();
//  
//  /* The voltage scaling allows optimizing the power consumption when the device is 
//     clocked below the maximum system frequency, to update the voltage scaling value 
//     regarding system frequency refer to product datasheet.  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
//  
//  /* Enable HSI Oscillator and activate PLL with HSI as source */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = 0x10;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLM = 16;
//  RCC_OscInitStruct.PLL.PLLN = 336;
//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
//  RCC_OscInitStruct.PLL.PLLQ = 7;
//  HAL_RCC_OscConfig(&RCC_OscInitStruct);
//   
//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//     clocks dividers */
//  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief  Query the User for SSID, password and encryption mode
  * @param  None
  * @retval WiFi_Status_t
  */
WiFi_Status_t wifi_get_AP_settings(void)
{
  WiFi_Status_t status = WiFi_MODULE_SUCCESS;
  printf("\r\n\n/********************************************************\n");
  printf("\r *                                                      *\n");
  printf("\r * X-CUBE-WIFI1 Expansion Software V2.1.1               *\n");
  printf("\r * X-NUCLEO-IDW01M1 Wi-Fi Mini-AP Configuration.        *\n");
  printf("\r * Server-Socket Example                                *\n");
  printf("\r *                                                      *\n");
  printf("\r *******************************************************/\n");
  printf("\r\nDo you want to setup SSID?(y/n):");
  fflush(stdout);
  scanf("%s",console_input);
  
  //HAL_UART_Receive(&UartMsgHandle, (uint8_t *)console_input, 1, 100000);
  if(console_input[0]=='y') 
        {
              set_AP_config = WIFI_TRUE;  
              printf("\r\nEnter the SSID for mini-AP:");
              fflush(stdout);

              console_count=0;
              console_count=scanf("%s",console_ssid);
              printf("\r\n");

                if(console_count==39) 
                    {
                        printf("Exceeded number of ssid characters permitted");
                        return WiFi_NOT_SUPPORTED;
                    }

        } else 
            {
                printf("\r\n\nModule will connect with default settings.");
                memcpy(console_ssid, (const char*)ssid, strlen((char*)ssid));             
            }

  printf("\r\n/*************************************************************\r\n");
  printf("\r\n * Configuration Complete                                     \r\n");
  printf("\r\n * Please make sure a server is listening at given hostname   \r\n");
  printf("\r\n *************************************************************\r\n");

  return status;
}

/******** Wi-Fi Indication User Callback *********/

void ind_wifi_socket_data_received(uint8_t socket_id, uint8_t * data_ptr, uint32_t message_size, uint32_t chunk_size)
{
//  printf("\r\nData Receive Callback...\r\n");
////memcpy(echo, data_ptr, 50);
//  printf((const char*)echo);
//  printf("\r\nsocket ID: %d\r\n",socket_id);
//  printf("msg size: %lu\r\n",(unsigned long)message_size);
//  printf("chunk size: %lu\r\n",(unsigned long)chunk_size);
//  fflush(stdout);
  //wifi_state = wifi_state_write;
  
  uint16_t datalen= strlen((const char*)data_ptr);
  npolling=0;
 
  if(datalen>MAX_SERVERDATA_RX)
  {
    answer="<OVERSIZE>";
    wifi_state = wifi_state_write;
  }
  else
  {
    if(Frame_decoding!=Started)
    {
      memset(rung_conf,0,MAX_SERVERDATA_RX);
      for(uint16_t i=0;i<MAX_SERVERDATA_RX && i<datalen;i++) 
      {
        rung_conf[i]=data_ptr[i];
      }
       
       RX_ClientFrame =WIFI_TRUE;
    }
    else
    {
       answer="<BUSY>";
        wifi_state = wifi_state_write;
    }
  }
}

void ind_wifi_on()
{
    wifi_state = wifi_state_ready;
}

void ind_wifi_connected()
{
  wifi_state = wifi_state_connected;
}

void ind_socket_server_client_joined(void)
{
//  printf("\r\nUser callback: Client joined...\r\n");
//  fflush(stdout);

  client_connection=WIFI_TRUE;
  answer= "<OK>";
  wifi_state = wifi_state_write;
}

void ind_socket_server_client_left(void)
{
  printf("\r\nUser callback: Client left...\r\n");
  fflush(stdout);
}

void ind_wifi_ap_client_joined(uint8_t * client_mac_address)
{
//  printf("\r\n>>client joined callback...\r\n");
//  printf((const char*)client_mac_address);
//  fflush(stdout); 

}

void ind_wifi_ap_client_left(uint8_t * client_mac_address)
{
  printf("\r\n>>client left callback...\r\n");
  printf((const char*)client_mac_address);
  fflush(stdout);  
}

/**
  * @}
  */
  
/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

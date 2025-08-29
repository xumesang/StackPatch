#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "cvefunc.h"
void SystemClock_Config(void);
void Error_Handler(void);
volatile uint32_t break_point_instruction_addr;
volatile uint32_t  patch_address = 0x08000000;

static uint8_t patch_2017_14199_1[] = ""
"\x41\x6A\x02\x29\x03\xD3\xC1\x6B\x09\x1F\xC1\x63\x05\xE0\xC1\x68"//16
"\x09\x89\xC1\x62\xC1\x6B\x89\x1C\xC1\x63"  //10
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2017_14199_2[] = ""
"\x81\x68\x49\x8B\x81\x62\x01\x8D\x42\x6A\x51\x80\x41\x68\x20\x31"//16
"\x41\x62\xC1\x6B\x89\x1C\xC1\x63"  //8
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_10019[] = ""
"\x81\x6A\x40\x29\x01\xD9\x40\x21\x81\x62\x00\x21\x41\x62\xC1\x6B"//16
"\x89\x1C\xC1\x63"  //4
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2020_10021[] = ""
"\x08\x4A\x41\x68\x12\x68\xB2\xEB\x41\x2F\x03\xD8\xC1\x6B\x0C\x31"//16
"\xC1\x63\x05\xE0\x41\x68\x49\x02\x41\x62\xC1\x6B\x89\x1C\xC1\x63"  
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_10023[] = ""
"\x42\x69\x81\x68\x89\x1A\x01\x63\xC1\x6B\x89\x1C\xC1\x63"//14
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_10059[] = ""
"\x01\x21\x01\x63\xC1\x6B\x89\x1C\xC1\x63"//10
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_10066[] = ""
"\x81\x68\xC1\x62\xC1\x6B\x89\x1C\xC1\x63"//10
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_10069[] = ""
"\x81\x6A\x02\x29\x05\xD3\x41\x6A\x05\x29\x02\xD3\x41\x6A\x10\x29"//16
"\x03\xD9\xC1\x6B\x0C\x31\xC1\x63\x07\xE0\x41\x68\x90\xF8\x24\x20"
"\x4A\x70\xC2\x6B\x92\x1C\xC2\x63\x00\xBF"//10
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2020_10062[] = ""
"\x81\x68\xB1\xF1\x80\x5F\x06\xD3\x6F\xF0\x15\x01\x41\x62\xC1\x6B"//16
"\x0C\x31\xC1\x63\x04\xE0\x81\x68\x81\x62\xC1\x6B\x89\x1C\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";


static uint8_t patch_2020_10063[] = ""
"\xD0\xE9\x09\x12\x11\x44\x42\x6A\x91\x42\x03\xD2\xC1\x6B\x89\x1D"//16
"\xC1\x63\x06\xE0\x41\x68\x82\x8C\x0A\x80\xC2\x6B\x92\x1C\xC2\x63"
"\x00\xBF"//2
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2020_10070_1[] = ""
"\x42\x6A\xC1\x68\x91\x42\x03\xD8\xC1\x6B\x89\x1D\xC1\x63\x06\xE0"//16
"\x42\x6A\xC1\x68\x89\x1A\x01\x61\xC1\x6B\x89\x1C\xC1\x63"//14
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2020_10070_2[] = ""
"\x82\x6A\x01\x69\x11\x44\x42\x6A\x91\x42\x03\xD2\xC1\x6B\x89\x1D"//16
"\xC1\x63\x06\xE0\x02\x69\x81\x6A\x11\x44\x81\x62\xC1\x6B\x89\x1C"
"\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2023_6749[] = ""
"\x01\x69\x05\x29\x03\xD9\xC1\x6B\x1E\x31\xC1\x63\x05\xE0\x81\x68"//16
"\x49\x1E\x41\x62\xC1\x6B\x89\x1C\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2022_1841[] = ""
"\x41\x68\x29\xB1\x00\x21\x81\x62\xC1\x6B\x89\x1C\xC1\x63\x02\xE0"//16
"\xC1\x6B\x08\x31\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2021_3581[] = ""
"\x81\x6A\x40\x29\x05\xD9\x12\x21\x41\x62\xC1\x6B\x89\x1C\xC1\x63"//16
"\x04\xE0\x00\x21\x41\x62\xC1\x6B\x89\x1C\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2023_5184[] = ""
"\x00\xBF\xC1\x6A\x01\xF0\x40\x41\x41\x62\xC1\x6B\x09\x1D\xC1\x63"//16
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2023_4263[] = ""
"\x01\x69\x0A\x29\x06\xD9\x6F\xF0\x79\x01\x41\x62\xC1\x6B\x1E\x31"//16
"\xC1\x63\x04\xE0\x01\x21\x81\x61\xC1\x6B\x89\x1C\xC1\x63" 
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";
static uint8_t patch_2020_17445[] = ""
"\x90\xF8\x24\x20\x81\x6A\x91\x42\x01\xD8\x81\x6A\x31\xB9\x4F\xF0"//16
"\xFF\x31\x41\x62\xC1\x6B\x1E\x31\xC1\x63\x05\xE0\x90\xF8\x24\x10" 
"\x81\x60\xC1\x6B\x89\x1C\xC1\x63" 
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

static uint8_t patch_2020_24337[] = ""
"\x81\x6A\x89\x6A\x82\x6A\x52\x6A\x11\x44\x82\x6A\x52\x68\x83\x6A"//16
"\x9B\x68\x1A\x44\x91\x42\x06\xD8\x4F\xF0\xFF\x31\x41\x62\xC1\x6B" 
"\x34\x31\xC1\x63\x05\xE0\x81\x6A\xC9\x6B\x41\x62\xC1\x6B\x89\x1C" 
"\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";


static uint8_t patch_2024_0901[] = ""
"\x81\x6A\x19\xB9\xC1\x6B\x09\x1D\xC1\x63\x05\xE0\xC1\x6A\x49\x68"//16
"\x41\x62\xC1\x6B\x89\x1C\xC1\x63" 
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback 
"";

int main(void)
{ 
	SCB->VTOR = SRAM_BASE | (0x1000 & (uint32_t)0xFFFFFE00); 

	HAL_Init(); 
  SystemClock_Config();
  Led_Init();
	delay_init(84);
	usart_init(115200);

 while(1)
 {  
    Led_ON();
	  delay_ms(500);
	  Led_OFF();
	  delay_ms(500);
	  printf("this is app sram code\r\n");
	 
	 	 break_point_instruction_addr = 0x2000409c;/* The vulnerability entry point received from the update host is 0x08003274.*/
	   patch_address = 0x20009000;/* patch address is in idle RAM area, with a starting address of 0x20009000.*/
	   uint32_t i;
	   for ( i = 0; i < sizeof(patch_2020_10019); i=i+1){           
					  *(uint8_t *)(patch_address+i) = *(uint8_t *)(patch_2020_10019+i);/** Write the patch into the specified patch area. **/
        }
		 *(uint16_t *)(break_point_instruction_addr) = 0xBE00; 
				
	 // test_cve_2020_10019();
	  
	 
	 
}

}





void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
 
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

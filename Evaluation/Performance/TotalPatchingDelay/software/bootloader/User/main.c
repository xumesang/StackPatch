#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "iap.h"
void SystemClock_Config(void);
void Error_Handler(void);
//void patchfunc(void) __attribute__((section(".ARM.__at_0x08002100")));
//void patchfunc(void){
//   Led_OFF();
  // printf("patchfunc");
//}


#define  DEM_CR_TRCENA                        (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

#define  DWT_CYCCNT                              *(volatile uint32_t *)0xE0001004
#define  DWT_CR                                        *(volatile uint32_t *)0xE0001000
#define  DEM_CR                                        *(volatile uint32_t *)0xE000EDFC
#define  DBGMCU_CR                               *(volatile uint32_t *)0xE0042004


// static DEBUG_CONTROL_BLOCK *const DEG_MON = (DEBUG_CONTROL_BLOCK *) 0xE000EDF0;

#define DCB_DEMCR 0xE000EDFC // page 464
#define DCB_DEMCR_MON_EN (uint32_t) (1 << 16)  
#define SCB_HFSR 0xE000E02C 
#define SCB_DFSR 0xE000ED30 // page 390
#define SCB_SHP8 0xE000ED20 // page 263, debug monitor priority

#define MAX_BP 6 // six inst breakpoint in cortex-m4
#define FPB_CB_REG 5
typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t REMAP;
	volatile uint32_t COMP[MAX_BP];
} FPB_UNIT;

// address must be 
typedef struct {
	uint32_t inst[10];
	bool active;
} FPB_REMAP;

static FPB_UNIT *const FPB = (FPB_UNIT *) 0xE0002000;
static FPB_REMAP REMAP_ADDR;


static void fpb_enable(void) {
	FPB->CTRL |= 0x3;
}
static void fpb_disable(void) {
	FPB->CTRL = (FPB->CTRL & ~0x3) | 0x2;
}
static bool halting_debug_enabled(void) {
	volatile uint32_t *dhcsr = (uint32_t *)0xE000EDF0;
	return (((*dhcsr) & 0x1) != 0);
}
// init arm debug monitor
static void debug_monitor_init() {
	volatile uint32_t *scb_hfsr = (uint32_t *) SCB_HFSR;
	volatile uint32_t *scb_dfsr = (uint32_t *) SCB_DFSR;
	*scb_hfsr = (1 << 31);
//	*SCB_DFSR = 1;
//	*SCB_DFSR = (1 << 1);
	*scb_dfsr = (1 << 2);
	
	if (halting_debug_enabled()) {
		printf("Halting Debug Enabled - Can't Enable Monitor Mode Debug!");
		return;
	}
	
	volatile uint32_t *demcr = (uint32_t *) DCB_DEMCR;
	*demcr |= DCB_DEMCR_MON_EN; 
	//*demcr &= ~(1 << mon_en_bit); // DEG_MO = 0 && C_DEBUG = 1 -> HardFault
	fpb_enable();
	printf("debug_monitor_init setup dhcsr: %u\r\n", *demcr);
	
	// Priority for DebugMonitor Exception is bits[7:0].
	// We will use the lowest priority so other ISRs can
	// fire while in the DebugMonitor Interrupt
	// Configurable priority values are in the range 0-255. This means that the Reset (), HardFault (-1), and NMI exceptions (-2), 
	// with fixed negative priority values, always have higher priority than any other exception.
	volatile uint32_t *shp8 = (uint32_t *)0xE000ED20;
	//DEBUG_LOG("SCB_SHP[8] priority: %d\n", (int) ((char)*shp8));
	*shp8 = 0xff;
	*shp8 = -1;
}


int main(void)
{ 
	  //  uint8_t t;
   // uint8_t key;
    uint32_t lastcount = 0;     /* 上一次串口接收数据值 */
    uint32_t applenth = 0;      /* 接收到的app代码长度 */
  //  uint8_t clearflag = 0;
	HAL_Init(); 
  SystemClock_Config();
  Led_Init();
	delay_init(80);
	usart_init(115200);
//	printf("%x\n",patchfunc);
//	void(*ptr)();
	uint32_t a,b,c;
	 a = 10;
	 b = 10;
  	c = a+b;
	debug_monitor_init();
 while(1)
 {
    Led_ON();
	  delay_ms(500);
	// iap_load_app(0x08002100);
	// (void (*)())0x08002100;
	
	 //__asm volatile ("BKPT #01");
	  Led_OFF();
	  delay_ms(500);
	//  data = *(volatile uint32_t *)addr;
	  printf("hello");
	  
        if (g_usart_rx_cnt)
        {
            if (lastcount == g_usart_rx_cnt)   /* 新周期内,没有收到任何数据,认为本次数据接收完成 */
            {
                applenth = g_usart_rx_cnt;
                lastcount = 0;
                g_usart_rx_cnt = 0;
                printf("用户程序接收完成!\r\n");
                printf("代码长度:%dBytes\r\n", applenth);
            }
            else lastcount = g_usart_rx_cnt;
        }
			    	
				 if (applenth)
            { 
                if (((*(volatile uint32_t *)(0x20001000 + 4)) & 0xFF000000) == 0x20000000)   /* 判断是否为0X20XXXXXX */
            {   
					  		printf("开始执行SRAM用户代码!!\r\n\r\n");
                delay_ms(10);
                iap_load_app(0x20001000);   /* SRAM地址 */
            }
            else
            {
                printf("非SRAM应用程序,无法执行!\r\n");
               
            }
                
             }
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
  RCC_OscInitStruct.PLL.PLLN = 80;
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

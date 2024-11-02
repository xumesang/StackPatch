#include "patch_delay_test.h"

#include "usart.h"




#define SYSCoreClock 80000000 // 80mhz

#define COREDEBUG_DEMCR  (volatile uint32_t *) (0xE000EDFC)
#define DWT_CTRL (volatile uint32_t *) (0xE0001000) // page e156
#define DWT_CYCCNT (volatile uint32_t *) (0xE0001004) // page e156
#define MSK_DEMCR_TRCENA (1 << 24) // page 464
#define MSK_DWT_CYCCNTENA (1 << 0) // page e159
static uint32_t ticks_per_us = 1000;

float cycles_to_us(uint32_t cycles) {
	return cycles / (float) ticks_per_us;
}
void dwt_init() {
	ticks_per_us = SYSCoreClock / 1000000;
	*COREDEBUG_DEMCR |= MSK_DEMCR_TRCENA;
	*DWT_CYCCNT = 0;
	*DWT_CTRL |= MSK_DWT_CYCCNTENA;
}

void dwt_deinit() {
	*DWT_CTRL &= ~MSK_DWT_CYCCNTENA;
	*DWT_CYCCNT = 0;
	*COREDEBUG_DEMCR &= ~MSK_DEMCR_TRCENA;
}

uint32_t dwt_get_counter() {
	return *DWT_CYCCNT;
}




int cntPeriod;
int cntstart;
int cntend;
TIM_HandleTypeDef g_timx_handler;         /* 定时器参数句柄 */

void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* 定时器x */
    g_timx_handler.Init.Prescaler = psc;                          /* 分频 */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_timx_handler.Init.Period = arr;                             /* 自动装载值 */
	                               
    HAL_TIM_Base_Init(&g_timx_handler);
    
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* 使能定时器x和定时器更新中断 */
}

/**
 * @brief       定时器底层驱动，开启时钟，设置中断优先级
                此函数会被HAL_TIM_Base_Init()函数调用
 
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                     /* 使能TIMx时钟 */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3); /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);         /* 开启ITMx中断 */
    }
}

/**
 * @brief       基本定时器TIMX中断服务函数

 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);  /* 定时器回调函数 */
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
       cntPeriod++;
    }
}


void MeasureTimeStart(){
    cntPeriod = 0;
	  btim_timx_int_init(65536 - 1, 1 - 1);   //1us
	  cntstart = TIM3->CNT;
}

int MeasureTimeEnd(){
	   
	   cntend = TIM3->CNT;
	   
      int runtime = cntend - cntstart;
	    printf("%d\r\n",cntend);
	printf("%d\r\n",cntstart);
     return runtime;
}



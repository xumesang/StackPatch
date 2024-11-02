#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#define BTIM_TIMX_INT                       TIM3
#define BTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define BTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define BTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

/******************************************************************************************/
float cycles_to_us(uint32_t cycles);

void dwt_init();
void dwt_deinit();
uint32_t dwt_get_counter();
void btim_timx_int_init(uint16_t arr, uint16_t psc);    /* 基本定时器 定时中断初始化函数 */


void MeasureTimeStart();


int MeasureTimeEnd();
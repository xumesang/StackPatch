
#include "iap.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "flash.h"


iapfun jump2app;
uint32_t g_iapbuf[512];       /* 2K字节缓存 */

/**
 * @brief       IAP写入APP BIN
 * @param       appxaddr : 应用程序的起始地址
 * @param       appbuf   : 应用程序CODE
 * @param       appsize  : 应用程序大小(字节)
 * @retval      无
 */
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
    uint32_t t;
    uint16_t i = 0;
    uint32_t temp;
    uint32_t fwaddr = appxaddr; /* 当前写入的地址 */
    uint8_t *dfu = appbuf;

    for (t = 0; t < appsize; t += 4)
    {
        temp = (uint32_t)dfu[3] << 24;
        temp |= (uint32_t)dfu[2] << 16;
        temp |= (uint32_t)dfu[1] << 8;
        temp |= (uint32_t)dfu[0];
        dfu += 4;               /* 偏移2个字节 */
        g_iapbuf[i++] = temp;

        if (i == 512)
        {
            i = 0;
            stmflash_write(fwaddr, g_iapbuf, 512);
            fwaddr += 2048;     /* 偏移2048  16 = 2 * 8  所以要乘以2 */
        }
    }

    if (i)
    {
        stmflash_write(fwaddr, g_iapbuf, i);  /* 将最后的一些内容字节写进去 */
    }
}

/**
 * @brief       跳转到应用程序段(执行APP)
 * @param       appxaddr : 应用程序的起始地址
 * @retval      无
 */
void iap_load_app(uint32_t appxaddr)
{
    if (((*(volatile  uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)     /* 检查栈顶地址是否合法.可以放在内部SRAM共64KB(0x20000000) */
    {
        /* 用户代码区第二个字为程序开始地址(复位地址) */
        jump2app = (iapfun) * (volatile uint32_t *)(appxaddr + 4);
     //   jump2app = (iapfun) * (volatile uint32_t *)(appxaddr);
        /* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
        sys_msr_msp(*(volatile uint32_t *)appxaddr);
        
        /* 跳转到APP */
        jump2app();
    }
}









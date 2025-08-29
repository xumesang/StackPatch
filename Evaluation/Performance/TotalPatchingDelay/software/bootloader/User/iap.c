
#include "iap.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "flash.h"


iapfun jump2app;
uint32_t g_iapbuf[512];       


void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
    uint32_t t;
    uint16_t i = 0;
    uint32_t temp;
    uint32_t fwaddr = appxaddr; 
    uint8_t *dfu = appbuf;

    for (t = 0; t < appsize; t += 4)
    {
        temp = (uint32_t)dfu[3] << 24;
        temp |= (uint32_t)dfu[2] << 16;
        temp |= (uint32_t)dfu[1] << 8;
        temp |= (uint32_t)dfu[0];
        dfu += 4;            
        g_iapbuf[i++] = temp;

        if (i == 512)
        {
            i = 0;
            stmflash_write(fwaddr, g_iapbuf, 512);
            fwaddr += 2048;     
        }
    }

    if (i)
    {
        stmflash_write(fwaddr, g_iapbuf, i);  
    }
}


void iap_load_app(uint32_t appxaddr)
{
    if (((*(volatile  uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)     
    {
       
        jump2app = (iapfun) * (volatile uint32_t *)(appxaddr + 4);
     //   jump2app = (iapfun) * (volatile uint32_t *)(appxaddr);
       
        sys_msr_msp(*(volatile uint32_t *)appxaddr);
        
      
        jump2app();
    }
}









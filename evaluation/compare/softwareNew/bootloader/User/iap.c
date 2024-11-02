
#include "iap.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "flash.h"


iapfun jump2app;
uint32_t g_iapbuf[512];       /* 2K�ֽڻ��� */

/**
 * @brief       IAPд��APP BIN
 * @param       appxaddr : Ӧ�ó������ʼ��ַ
 * @param       appbuf   : Ӧ�ó���CODE
 * @param       appsize  : Ӧ�ó����С(�ֽ�)
 * @retval      ��
 */
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
    uint32_t t;
    uint16_t i = 0;
    uint32_t temp;
    uint32_t fwaddr = appxaddr; /* ��ǰд��ĵ�ַ */
    uint8_t *dfu = appbuf;

    for (t = 0; t < appsize; t += 4)
    {
        temp = (uint32_t)dfu[3] << 24;
        temp |= (uint32_t)dfu[2] << 16;
        temp |= (uint32_t)dfu[1] << 8;
        temp |= (uint32_t)dfu[0];
        dfu += 4;               /* ƫ��2���ֽ� */
        g_iapbuf[i++] = temp;

        if (i == 512)
        {
            i = 0;
            stmflash_write(fwaddr, g_iapbuf, 512);
            fwaddr += 2048;     /* ƫ��2048  16 = 2 * 8  ����Ҫ����2 */
        }
    }

    if (i)
    {
        stmflash_write(fwaddr, g_iapbuf, i);  /* ������һЩ�����ֽ�д��ȥ */
    }
}

/**
 * @brief       ��ת��Ӧ�ó����(ִ��APP)
 * @param       appxaddr : Ӧ�ó������ʼ��ַ
 * @retval      ��
 */
void iap_load_app(uint32_t appxaddr)
{
    if (((*(volatile  uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)     /* ���ջ����ַ�Ƿ�Ϸ�.���Է����ڲ�SRAM��64KB(0x20000000) */
    {
        /* �û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ) */
        jump2app = (iapfun) * (volatile uint32_t *)(appxaddr + 4);
     //   jump2app = (iapfun) * (volatile uint32_t *)(appxaddr);
        /* ��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ) */
        sys_msr_msp(*(volatile uint32_t *)appxaddr);
        
        /* ��ת��APP */
        jump2app();
    }
}









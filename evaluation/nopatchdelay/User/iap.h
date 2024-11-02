
#ifndef __IAP_H
#define __IAP_H

#include "./SYSTEM/sys/sys.h"


typedef void (*iapfun)(void);                   /* ����һ���������͵Ĳ��� */

#define FLASH_APP1_ADDR         0x08020000      /* ��һ��Ӧ�ó�����ʼ��ַ(������ڲ�FLASH)
                                                 * ���� 0x08000000~0x0800FFFF �Ŀռ�Ϊ Bootloader ʹ��(��64KB)
                                                 */


void iap_load_app(uint32_t appxaddr);           /* ��ת��APP����ִ�� */
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);   /* ��ָ����ַ��ʼ,д��bin */

#endif




















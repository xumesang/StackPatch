
#ifndef __IAP_H
#define __IAP_H

#include "./SYSTEM/sys/sys.h"


typedef void (*iapfun)(void);                  

#define FLASH_APP1_ADDR         0x08020000     


void iap_load_app(uint32_t appxaddr);          
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);   

#endif




















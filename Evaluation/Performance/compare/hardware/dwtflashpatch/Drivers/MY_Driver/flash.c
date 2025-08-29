

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "flash.h"

/**

 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       获取某个地址所在的flash扇区
 * @param       addr    : lash地址
 * @retval      0~11,即addr所在的扇区
 */
uint8_t  stmflash_get_flash_sector(uint32_t addr)
{
    if (addr < ADDR_FLASH_SECTOR_1) return FLASH_SECTOR_0;
    else if (addr < ADDR_FLASH_SECTOR_2) return FLASH_SECTOR_1;
    else if (addr < ADDR_FLASH_SECTOR_3) return FLASH_SECTOR_2;
    else if (addr < ADDR_FLASH_SECTOR_4) return FLASH_SECTOR_3;
    else if (addr < ADDR_FLASH_SECTOR_5) return FLASH_SECTOR_4;
    else if (addr < ADDR_FLASH_SECTOR_6) return FLASH_SECTOR_5;
    else if (addr < ADDR_FLASH_SECTOR_7) return FLASH_SECTOR_6;
   // else if (addr < ADDR_FLASH_SECTOR_8) return FLASH_SECTOR_7;
 //   else if (addr < ADDR_FLASH_SECTOR_9) return FLASH_SECTOR_8;
 //   else if (addr < ADDR_FLASH_SECTOR_10) return FLASH_SECTOR_9;
    //else if (addr < ADDR_FLASH_SECTOR_11) return FLASH_SECTOR_10;
    return FLASH_SECTOR_7;
}

/**

 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    FLASH_EraseInitTypeDef flasheraseinit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;

    uint32_t addrx = 0;
    uint32_t endaddr = 0;
    uint32_t sectorerror=0;
    
    if (waddr < STM32_FLASH_BASE || waddr % 4 ||       
        waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE))  
    {
        return;
    }

    HAL_FLASH_Unlock();             /*  */
    FLASH->ACR &= ~(1 << 10);       /*  */

    addrx = waddr;                  /* */
    endaddr = waddr + length * 4;   

    if (addrx < 0X1FFF0000)         
    {
        while (addrx < endaddr)    
        {
            if (stmflash_read_word(addrx) != 0XFFFFFFFF)    
            {
                flasheraseinit.TypeErase=FLASH_TYPEERASE_SECTORS;       
                flasheraseinit.Sector=stmflash_get_flash_sector(addrx); 
                flasheraseinit.NbSectors=1;                            
                flasheraseinit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      

                if(HAL_FLASHEx_Erase(&flasheraseinit, &sectorerror) != HAL_OK) 
                {
                    break;
                }

            }
            else
            {
                addrx += 4;
            }
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                
        }
    }

    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            

    if (FlashStatus==HAL_OK)
    {
        while (waddr < endaddr)    
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, waddr, *pbuf) != HAL_OK)  
            {
                break;              
            }

            waddr += 4;
            pbuf++;
        }
    }
    
    FLASH->ACR |= 1 << 10;          

    HAL_FLASH_Lock();              
}


void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);    
        raddr += 4; 
    }
}


void test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write(waddr, &wdata, 1);   
}


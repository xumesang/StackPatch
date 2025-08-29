/*!
    \file    systick.c
    \brief   the systick configuration file

    \version 2019-06-05, V1.0.0, demo for GD32VF103
    \version 2020-12-18, V1.1.0, demo for GD32VF103
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32vf103.h"
#include "systick.h"

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
   // uint64_t start_mtime, delta_mtime;

    /* don't start measuring until we see an mtime tick */ 
    uint64_t tmp = get_timer_value();
    while(1){

    	if(get_timer_value() - tmp >=(SystemCoreClock/4000.0 *count)){
    	   break;

    	}
    
    }
//    do{
//        start_mtime = get_timer_value();
//    }while(start_mtime == tmp);
//
//    do{
//        delta_mtime = get_timer_value() - start_mtime;
//    }while(delta_mtime <(SystemCoreClock/4000.0 *count));
}
void delay_1us(uint32_t count)
{
   // uint64_t start_mtime, delta_mtime;

    /* don't start measuring until we see an mtime tick */
    uint64_t tmp = get_timer_value();
    while(1){

    	if(get_timer_value() - tmp >=(SystemCoreClock/4000000.0 *count)){
    	   break;

    	}

    }
//    do{
//        start_mtime = get_timer_value();
//    }while(start_mtime == tmp);
//
//    do{
//        delta_mtime = get_timer_value() - start_mtime;
//    }while(delta_mtime <(SystemCoreClock/4000.0 *count));
}
void usart0_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART4);


    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);


    /* USART configure */
    usart_deinit(UART4);
    usart_baudrate_set(UART4, 115200U);
    usart_word_length_set(UART4, USART_WL_8BIT);
    usart_stop_bit_set(UART4, USART_STB_1BIT);
    usart_parity_config(UART4, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART4, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART4, USART_CTS_DISABLE);
    usart_receive_config(UART4, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);
    usart_enable(UART4);
}

/* retarget the C library printf function to the USART */
int _put_char(int ch)
{
    usart_data_transmit(UART4, (uint8_t) ch );
    while ( usart_flag_get(UART4, USART_FLAG_TBE)== RESET){
    }

    return ch;
}

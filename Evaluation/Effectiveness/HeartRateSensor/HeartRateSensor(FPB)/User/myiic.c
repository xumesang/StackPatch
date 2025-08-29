/**
 ****************************************************************************************************
 * @file        myiic.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       IIC ��������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211023
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "myiic.h"
#include "delay.h"



void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    IIC_SCL_GPIO_CLK_ENABLE();  
    IIC_SDA_GPIO_CLK_ENABLE();  

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;       
    gpio_init_struct.Pull = GPIO_PULLUP;               
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;        
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);
   

    iic_stop();    
}


static void iic_delay(void)
{
    delay_us(2);    /* 2us����ʱ, ��д�ٶ���250Khz���� */
}


void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);     
    iic_delay();
    IIC_SCL(0);     
    iic_delay();
}


void iic_stop(void)
{
    IIC_SDA(0);    
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);    
    iic_delay();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic_delay();

    while (IIC_READ_SDA)    /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, ����ACK��� */
    iic_delay();
    return rack;
}

/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1 ʱ SDA = 0,��ʾӦ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1);     /* �����ͷ�SDA�� */
    iic_delay();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* ��λ�ȷ��� */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
    IIC_SDA(1);         /* �������, �����ͷ�SDA�� */
}

/**
 * @brief       IIC��ȡһ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        IIC_SCL(1);
        iic_delay();

        if (IIC_READ_SDA)
        {
            receive++;
        }
        
        IIC_SCL(0);
        iic_delay();
    }

    if (!ack)
    {
        iic_nack();     /* ����nACK */
    }
    else
    {
        iic_ack();      /* ����ACK */
    }

    return receive;
}




void IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength)
{		
	uint8_t i;	
    iic_start();  

	iic_send_byte(WriteAddr);	    //����д����
	iic_wait_ack();
	
	for(i=0;i<dataLength;i++)
	{
		iic_send_byte(data[i]);
		iic_wait_ack();
	}				    	   
    iic_stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

void IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength)
{		
	uint8_t i;	
    iic_start();  

	iic_send_byte(deviceAddr);	    //����д����
	iic_wait_ack();
	iic_send_byte(writeAddr);
	iic_wait_ack();
	iic_send_byte(deviceAddr|0X01);//�������ģʽ			   
	iic_wait_ack();
	
	for(i=0;i<dataLength-1;i++)
	{
		data[i] = iic_read_byte(1);
	}		
	data[dataLength-1] = iic_read_byte(0);	
    iic_stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

void IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data)
{				  	  	    																 
    iic_start();  
	
	iic_send_byte(daddr);	   //����д����
	iic_wait_ack();
	iic_send_byte(addr);//���͵�ַ
	iic_wait_ack();		 
	iic_start();  	 	   
	iic_send_byte(daddr|0X01);//�������ģʽ			   
	iic_wait_ack();	 
    *data = iic_read_byte(0);		   
    iic_stop();//����һ��ֹͣ����	    
}

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    iic_start();  
	
	iic_send_byte(daddr);	    //����д����
	iic_wait_ack();
	iic_send_byte(addr);//���͵�ַ
	iic_wait_ack();	   	 										  		   
	iic_send_byte(data);     //�����ֽ�							   
	iic_wait_ack();  		    	   
    iic_stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}






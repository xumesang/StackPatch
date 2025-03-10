#include "myiic.h"
#include "max30102.h"

#include "delay.h"

uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data)
{

	
  iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_WR);	

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}


	iic_send_byte(Register_Address);
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}
	
	
	iic_send_byte(Word_Data);

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}

	
	iic_stop();
	return 1;	

cmd_fail: 
	iic_stop();
	return 0;
}



uint8_t max30102_Bus_Read(uint8_t Register_Address)
{
	uint8_t  data;


	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_WR);	

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}

	
	iic_send_byte((uint8_t)Register_Address);
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}
	

	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_RD);	

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}

	
	{
		data = iic_read_byte(0);	

		iic_nack();	
	}

	iic_stop();
	return data;	

cmd_fail: 
	iic_stop();
	return 0;
}


void max30102_FIFO_ReadWords(uint8_t Register_Address,uint8_t Word_Data[][2],uint8_t count)
{
	uint8_t i=0;
	uint8_t no = count;
	uint8_t data1, data2;
	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_WR);	

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}


	iic_send_byte((uint8_t)Register_Address);
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}
	

	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_RD);	


	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}


	while (no)
	{
		data1 = iic_read_byte(0);	
		iic_ack();
		data2 = iic_read_byte(0);
		iic_ack();
		Word_Data[i][0] = (((uint16_t)data1 << 8) | data2);  //

		
		data1 = iic_read_byte(0);	
		iic_ack();
		data2 = iic_read_byte(0);
		if(1==no)
			iic_nack();	
		else
			iic_ack();
		Word_Data[i][1] = (((uint16_t)data1 << 8) | data2); 

		no--;	
		i++;
	}

	iic_stop();

cmd_fail: 
	iic_stop();
}

void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t* Data)
{	
	max30102_Bus_Read(REG_INTR_STATUS_1);
	max30102_Bus_Read(REG_INTR_STATUS_2);
	
	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_WR);	


	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}

	
	iic_send_byte((uint8_t)Register_Address);
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}
	

	
	iic_start();

	
	iic_send_byte(max30102_WR_address | I2C_RD);	

	
	if (iic_wait_ack() != 0)
	{
		goto cmd_fail;	
	}

	
	Data[0] = iic_read_byte(1);	
	Data[1] = iic_read_byte(1);	
	Data[2] = iic_read_byte(1);	
	Data[3] = iic_read_byte(1);
	Data[4] = iic_read_byte(1);	
	Data[5] = iic_read_byte(0);
	

	iic_stop();

cmd_fail: 
	iic_stop();

//	u8 i;
//	u8 fifo_wr_ptr;
//	u8 firo_rd_ptr;
//	u8 number_tp_read;
//	//Get the FIFO_WR_PTR
//	fifo_wr_ptr = max30102_Bus_Read(REG_FIFO_WR_PTR);
//	//Get the FIFO_RD_PTR
//	firo_rd_ptr = max30102_Bus_Read(REG_FIFO_RD_PTR);
//	
//	number_tp_read = fifo_wr_ptr - firo_rd_ptr;
//	
//	//for(i=0;i<number_tp_read;i++){
//	if(number_tp_read>0){
//		IIC_ReadBytes(max30102_WR_address,REG_FIFO_DATA,Data,6);
//	}
	
	//max30102_Bus_Write(REG_FIFO_RD_PTR,fifo_wr_ptr);
}

void max30102_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 /****************	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	******************/
	iic_init();
	
	max30102_reset();
	
//	max30102_Bus_Write(REG_MODE_CONFIG, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
//	max30102_Bus_Write(REG_INTR_STATUS_2, 0xF0); //open all of interrupt
//	max30102_Bus_Write(REG_INTR_STATUS_1, 0x00); //all interrupt clear
//	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x02); //DIE_TEMP_RDY_EN
//	max30102_Bus_Write(REG_TEMP_CONFIG, 0x01); //SET   TEMP_EN

//	max30102_Bus_Write(REG_SPO2_CONFIG, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

//	max30102_Bus_Write(REG_LED1_PA, 0x47); 
//	max30102_Bus_Write(REG_LED2_PA, 0x47); 
	
	
	
	max30102_Bus_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	max30102_Bus_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	max30102_Bus_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED


	
//	// Interrupt Enable 1 Register. Set PPG_RDY_EN (data available in FIFO)
//	max30102_Bus_Write(0x2, 1<<6);

//	// FIFO configuration register
//	// SMP_AVE: 16 samples averaged per FIFO sample
//	// FIFO_ROLLOVER_EN=1
//	//max30102_Bus_Write(0x8,  1<<4);
//	max30102_Bus_Write(0x8, (0<<5) | 1<<4);

//	// Mode Configuration Register
//	// SPO2 mode
//	max30102_Bus_Write(0x9, 3);

//	// SPO2 Configuration Register
//	max30102_Bus_Write(0xa,
//			(3<<5)  // SPO2_ADC_RGE 2 = full scale 8192 nA (LSB size 31.25pA); 3 = 16384nA
//			| (1<<2) // sample rate: 0 = 50sps; 1 = 100sps; 2 = 200sps
//			| (3<<0) // LED_PW 3 = 411��s, ADC resolution 18 bits
//	);

//	// LED1 (red) power (0 = 0mA; 255 = 50mA)
//	max30102_Bus_Write(0xc, 0xb0);

//	// LED (IR) power
//	max30102_Bus_Write(0xd, 0xa0);
											
}

void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}






void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
//  char ach_i2c_data[2];
//  ach_i2c_data[0]=uch_addr;
//  ach_i2c_data[1]=uch_data;
//	
//  IIC_WriteBytes(I2C_WRITE_ADDR, ach_i2c_data, 2);
	IIC_Write_One_Byte(I2C_WRITE_ADDR,uch_addr,uch_data);
}

void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
//  char ch_i2c_data;
//  ch_i2c_data=uch_addr;
//  IIC_WriteBytes(I2C_WRITE_ADDR, &ch_i2c_data, 1);
//	
//  i2c.read(I2C_READ_ADDR, &ch_i2c_data, 1);
//  
//   *puch_data=(uint8_t) ch_i2c_data;
	IIC_Read_One_Byte(I2C_WRITE_ADDR,uch_addr,puch_data);
}

void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	unsigned char uch_temp;
	char ach_i2c_data[6];
	*pun_red_led=0;
	*pun_ir_led=0;

  
  //read and clear status register
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
  
  IIC_ReadBytes(I2C_WRITE_ADDR,REG_FIFO_DATA,(uint8_t *)ach_i2c_data,6);
  
  un_temp=(unsigned char) ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=(unsigned char) ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
}

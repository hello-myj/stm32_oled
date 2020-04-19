#include "oled_driver.h"

#if (TRANSFER_METHOD ==HW_IIC)
//I2C_Configuration，初始化硬件IIC引脚
void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(IIC_RCC_APB1Periph_I2CX,ENABLE);
	RCC_APB2PeriphClockCmd(IIC_RCC_APB2Periph_GPIOX,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  IIC_SCL_Pin_X | IIC_SDA_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(IIC_GPIOX, &GPIO_InitStructure);

	I2C_DeInit(I2CX);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 1320000;

	I2C_Cmd(I2CX, ENABLE);
	I2C_Init(I2CX, &I2C_InitStructure);
	WaitTimeMs(200);
}

 /**
  * @brief  I2C_WriteByte，向OLED寄存器地址写一个byte的数据
  * @param  addr：寄存器地址
	*					data：要写入的数据
  * @retval 无
  */
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2CX, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2CX, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2CX, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2CX, addr);//寄存器地址
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2CX, data);//发送数据
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2CX, ENABLE);//关闭I2C1总线
}

void WriteCmd(unsigned char cmd)//写命令
{
		I2C_WriteByte(0x00, cmd);
}

void WriteDat(unsigned char dat)//写数据
{
		I2C_WriteByte(0x40, dat);
}

void OLED_FILL(unsigned char BMP[])
{
		unsigned int n;
		unsigned char *p;
		p=BMP;
		WriteCmd(0xb0);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		while(I2C_GetFlagStatus(I2CX, I2C_FLAG_BUSY));	
		I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
		while(!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
		I2C_Send7bitAddress(I2CX, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
		while(!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		I2C_SendData(I2CX, 0x40);//寄存器地址
		while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
		for(n=0;n<128*8;n++)
		{
			I2C_SendData(I2CX, *p++);//发送数据
			while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		}	
		I2C_GenerateSTOP(I2CX, ENABLE);//关闭I2C1总线
}



#elif  (TRANSFER_METHOD ==SW_IIC)

#elif  (TRANSFER_METHOD ==HW_SPI)

	#define OLED_RESET_LOW()					GPIO_ResetBits(SPI_RES_GPIOX, SPI_RES_PIN)	//低电平复位
	#define OLED_RESET_HIGH()					GPIO_SetBits(SPI_RES_GPIOX, SPI_RES_PIN)
	 
	#define OLED_CMD_MODE()						GPIO_ResetBits(SPI_DC_GPIOX, SPI_DC_PIN) 		//命令模式
	#define OLED_DATA_MODE()					GPIO_SetBits(SPI_DC_GPIOX, SPI_DC_PIN)			//数据模式

	#define OLED_CS_HIGH()   					GPIO_SetBits(SPI_CS_GPIOX, SPI_CS_Pin_X)
	#define OLED_CS_LOW()  		  			GPIO_ResetBits(SPI_CS_GPIOX, SPI_CS_Pin_X)
	
	
void SPI_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	#if 	(USE_HW_SPI==SPI_2)
		RCC_APB1PeriphClockCmd(SPI_RCC_APB1Periph_SPIX, ENABLE);
	#elif (USE_HW_SPI==SPI_1)
		RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_SPIX, ENABLE);
	#endif
	RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_GPIOX|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_CS_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CS_GPIOX, &GPIO_InitStructure);  
	OLED_CS_HIGH();
	
	GPIO_InitStructure.GPIO_Pin = SPI_HW_ALL_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_HW_ALL_GPIOX, &GPIO_InitStructure);   
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI_Direction_1Line_Tx; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 			
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  			
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPIX, &SPI_InitStructure);  
	SPI_Cmd(SPIX, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_RES_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_RES_GPIOX, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = SPI_DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_DC_GPIOX, &GPIO_InitStructure);  
	OLED_DATA_MODE();
	
	OLED_RESET_HIGH();
	WaitTimeMs(100);
	OLED_RESET_LOW();
	WaitTimeMs(100);
	OLED_RESET_HIGH();
}

void SPI_WriterByte(unsigned char dat)
{

	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET ){}; //检查指定的SPI标志位设置与否:发送缓存空标志位	  
		SPI_I2S_SendData(SPIX, dat); //通过外设SPIx发送一个数据
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_RXNE) == RESET){};//检查指定的SPI标志位设置与否:接受缓存非空标志位  						    
	  SPI_I2S_ReceiveData(SPIX); //返回通过SPIx最近接收的数据	

}

void WriteCmd(unsigned char cmd)
{
	OLED_CMD_MODE();
	OLED_CS_LOW();
	SPI_WriterByte(cmd);
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void WriteDat(unsigned char dat)
{
	OLED_DATA_MODE();
	OLED_CS_LOW();
	SPI_WriterByte(dat);
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void OLED_FILL(unsigned char BMP[])
{
	u8 i,j;
	unsigned char *p;
	p=BMP;

  for(i=0;i<8;i++)
	{
		WriteCmd(0xb0+i);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);	

		for(j=0;j<128;j++)
		{
			WriteDat(*p++);
		}
	}
}

#endif //(TRANSFER_METHOD ==HW_IIC)

void OLED_Init(void)
{
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}


void OLED_CLS(void)//清屏
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
				WriteDat(0x00);
		}
	}
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}



void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}




#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#if (USE_HW_IIC ==IIC1)
/*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
#define RCC_APB1Periph_I2CX   RCC_APB1Periph_I2C1
#define RCC_APB2Periph_GPIOX  RCC_APB2Periph_GPIOB
#define GPIOX                	GPIOB
#define GPIO_SCL_Pin_X				GPIO_Pin_6
#define GPIO_SDA_Pin_X        GPIO_Pin_7
#define I2CX                  I2C1
#elif (USE_HW_IIC ==IIC2)
#define RCC_APB1Periph_I2CX   RCC_APB1Periph_I2C2
#define RCC_APB2Periph_GPIOX  RCC_APB2Periph_GPIOB
#define GPIOX                	GPIOB
#define GPIO_SCL_Pin_X				GPIO_Pin_6
#define GPIO_SDA_Pin_X        GPIO_Pin_7
#define I2CX                  I2C2


#endif


#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_FILL(unsigned char BMP[]);

#endif

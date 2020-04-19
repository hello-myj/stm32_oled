#include "oled_config.h"
/* 屏幕驱动文件引用 */
#include "oled_driver.h"

extern unsigned char  ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN];
extern unsigned char TempBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN];
unsigned int OledTimeMs=0;

//初始化图形库，请将硬件初始化信息放入此中
void DriverInit(void)
{
	#if (TRANSFER_METHOD ==HW_IIC)
	I2C_Configuration();
	#elif (TRANSFER_METHOD ==HW_SPI)
	SPI_Configuration();
	#endif
	OLED_Init();
}

//为上层提供硬件操作接口OLED_FILL
void UpdateScreenBuffer(void)
{
	OLED_FILL(ScreenBuffer[0]);
}
//为上层提供硬件操作接口OLED_FILL
void UpdateTempBuffer(void)
{
	OLED_FILL(TempBuffer[0]);
}

//请将此函数放入1ms中断里，为图形提供时基
void OledTimeMsFunc(void)
{
	if (OledTimeMs != 0x00)
	{ 
	 OledTimeMs--;
	}
}
////写入一个字节
//static void WriteByteBuffer(u8 colnum,u8 row,u8 data)
//{
//	Screen_buffer[colnum][row]=data;
//}

////读一个字节
//static unsigned char ReadByteBuffer(unsigned char colnum,unsigned char row)
//{
//	return Screen_buffer[colnum][row];
//}



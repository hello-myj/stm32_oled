# stm32_oled


oled库说明：
	该库只是一个绘图图形库，只能适用于stm32系列的12864oled屏，驱动芯片为ssd1306，若需被其他平台使用，请自行移植

连接引脚：
	STM32的硬件I2C：
		IIC_1: PB6 -- SCL; PB7 -- SDA
		IIC_2: PB10-- SCL; PB11 --SDA（还未验证）
	STM32的硬件SPI：
		SPI_1:暂未添加，若需请在oled_driver.h自行配置
		SPI_2:PB10--RES; PB11--DC; PB12--CS;PB13--D0;PB15--D1;
	暂未添加软件模拟，后续会添加
	
图形库文件说明：
	draw_api.h:所需的各类api绘图方法
	oled_config.h:配置o文件，用于配置led屏
	oled_config.c:配置文件所需的接口
	oled_driver.c：oled屏驱动
	oled_draw.c:各类绘图方法
	oled_basic.c:底层和算法支撑
	oled_color.c:颜色控制
	oled_buffer.c:屏幕缓冲区和临时缓冲区
	oled_font.c:字体
	oled_bmp.c:bmp取模图形存放位置，
	oled_debug.c可调用OledPrintf方法，效果与printf一致。

其他：
	使用的取模工具为：PCtoLCD2002，已放入tool文件夹中，使用方法请参考tool/取模设置
	注意：硬件IIC出现卡死现象，请关掉串口初始化并重新上电
	该库还不完善，还在持续更新，
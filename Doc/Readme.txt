STM32的硬件I2C
PB6 -- SCL; PB7 -- SDA


图形库资源放在oledlib中；
oled配置文件在oled_config.h和oled_config.c
oled驱动在oled_driver.c
常用的绘图api函数在draw_api.h

该例程使用的是stm32f103rct6运行，若使用其他系列请自行移植
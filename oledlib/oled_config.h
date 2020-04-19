#ifndef OLED_CONFIG_H
#define OLED_CONFIG_H
#include "oled_basic.h"

//配置图形库
/*****************************************************************/
//选择屏幕类型
//1.HW_IIC    硬件IIC 
//2.SW_IIC    软件IIC
//3.HW_SPI		硬件SPI
//4.SW_SPI		软件IIC
//5.HW_8080   硬件8080
//现在只支持硬件iic 和硬件SPI
/*****************************************************************/
#define  TRANSFER_METHOD   (HW_SPI)

/*****************************************************************/
//选择字库来源
//0.FONT_FROM_NULL		 不使用字库
//1.FONT_FROM_SCREEN	 屏幕自带
//2.FONT_FROM_EXTL		 引用外部
//3.FONT_FROM_FLASH		 从flash读取
//4.FONT_FROM_EEMPROM  从eeprom读取
//5.FONT_FROM_SOFT 		 软件取模，这个会占用很多内存
//现在只有软件取模字库，
/*****************************************************************/
#define FONT_SOURCE   (FONT_FROM_SOFT)

/*****************************************************************/
//选择屏幕类型
//1.OLED_12864
//2.OLED_12832  
//3.CUSTOMIZE  自定义
//现在仅支持OLED_12864
/*****************************************************************/
#define  SCREEN_TYPE   (OLED_12864)



	#if (SCREEN_TYPE==OLED_12864)
		#define   OLED_12864
		#define 	SCREEN_PAGE_NUM			   	(8)
		#define   SCREEN_PAGEDATA_NUM   	(128)
		#define 	SCREEN_COLUMN 					(128)
		#define  	SCREEN_ROW 							(64)
		#if 0
		#define 	SCREEN_PHY_X             (21.744)
		#define 	SCREEN_PHY_Y						 (10.864) 
		#endif

	#elif (SCREEN_TYPE==OLED_12832)
		#define 	SCREEN_PAGE_NUM			   	(4)
		#define   SCREEN_PAGEDATA_NUM   	(128)
		#define 	SCREEN_COLUMN 					(128)
		#define  	SCREEN_ROW 							(32)
		#if 0
		#define 	SCREEN_PHY_X             ()
		#define 	SCREEN_PHY_Y						 ()
		#endif

	#elif (SCREEN_TYPE==CUSTOMIZE)
		/*调整点阵分辨率128*64,宽度:128列，高度:64行*/
		#define 	SCREEN_COLUMN 					(128)
		#define  	SCREEN_ROW 							(64)
		/*配置一帧数据能写多少页*/
		#define 	SCREEN_PAGE_NUM			   	(8)
		/*配置一页能写多少数据*/
		#define   SCREEN_PAGEDATA_NUM   	(128)
		#if 0
		/*调整物理尺寸*/
		#define 	SCREEN_PHY_X             ()
		#define 	SCREEN_PHY_Y						 ()
		#endif
	#endif

//驱动还未完善，现在使用的IIC1
	#if (TRANSFER_METHOD==HW_IIC)
			//IIC_1: PB6 -- SCL; PB7 -- SDA
			//IIC_2: PB10-- SCL; PB11 --SDA
			#define USE_HW_IIC		IIC_1
	#elif (TRANSFER_METHOD==SW_IIC)

	#elif (TRANSFER_METHOD==HW_SPI)
			//如需更换片选、复位、控制等引脚去oled_driver.h设置
			//SPI_1: 没添加SPI1，如需要SPI1，到oled_driver.h,自行添加
			//SPI_2: PB10--RES; PB11--DC; PB12--CS;PB13--D0;PB15--D1;
			#define USE_HW_SPI		SPI_2
	#elif (TRANSFER_METHOD==SW_SPI)

	#endif

#endif



#ifndef OLED_COLOR_H
#define OLED_COLOR_H
//绘制颜色，简单绘制or复杂绘制
#define  COLOR_CHOOSE_DRAW   (SIMPLE_DRAW)   


typedef enum
{
	pix_black,
	pix_white,
	//............
}Type_color;

//获取当前设置的像素点色
Type_color GetDrawColor(void);
//设置将要绘制的像素点色
void SetDrawColor(Type_color value);

Type_color GetFillColor(void);
void SetFillcolor(Type_color value);
#endif



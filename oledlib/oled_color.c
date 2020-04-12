#include "oled_color.h"



static Type_color _Draw=pix_white;
static Type_color _fill=pix_white;
//static Type_color _line=pix_white;
//static Type_color _text=pix_white;
//static Type_color _textbk=pix_black;

void SetDrawColor(Type_color value)
{
	_Draw=value;
}

Type_color GetDrawColor(void)
{
	return _Draw;
}

void SetFillcolor(Type_color value)
{
	_fill=value;

}

Type_color GetFillColor(void)
{

	return _fill;
}

//void SetLineColor(Type_color value)
//{
//	_line=value;
//}


//Type_color GetLineColor(void)
//{
//	return _line;
//}


//void SetTextColor(Type_color value)
//{
//	_text=value;
//}

//Type_color GetTextColor(void)
//{

//	return _text;
//}

//void SetTextBkColor(Type_color value)
//{
//	_textbk=value;
//}

//Type_color GetTextBkColor(void)
//{
//	return _textbk;
//}


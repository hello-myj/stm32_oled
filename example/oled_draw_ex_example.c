#include "oled_draw_ex_example.h"
#include "draw_api.h"




void DrawDrawRectangle2rotate(int left,int top,int width,int height)
{
		//SetRotateValue()
		int tempy=GetY();
		int tempx=GetX();
		TypeXY temp1,temp2,temp3,temp4;
		//SetRotateValue(value.center.x,value.center.y,value.angle,value.direct);
		temp1=GetRotateXY(left,top); //1
		temp2=GetRotateXY(left+width,top);//2 
		temp3=GetRotateXY(left+width,top+height);//3
		temp4=GetRotateXY(left,top+height);//4
		MoveTo(temp1.x,temp1.y);
		LineTo(temp2.x,temp2.y);
		LineTo(temp3.x,temp3.y);
		LineTo(temp4.x,temp4.y);
		LineTo(temp1.x,temp1.y);
		MoveTo(tempx,tempy);
}


//钟,角度还没有同步还需要调整，
void RoundClock(int hours ,int minute ,int sec)
{
	//FrameRateUpdateScreen(60);
	unsigned char i=0;
	TypeXY hourspoint,minutepoint,secpoint,tmp1,tmp2;
	//时针
	SetRotateValue(63,31,hours*30+(minute*30)/60,1);
	hourspoint=GetRotateXY(63-14,31);
	DrawLine(63,31,hourspoint.x,hourspoint.y);
	//分针
	SetRotateValue(63,31,minute*6+(sec*6)/60,1);
	minutepoint=GetRotateXY(63-21,31);
	DrawLine(63,31,minutepoint.x,minutepoint.y);	
	//秒针
	SetRotateValue(63,31,sec*6,1);
	secpoint=GetRotateXY(63-28,31);
	DrawLine(63,31,secpoint.x,secpoint.y);
	//表盘
	for(i=0;i<12;i++)
	{
		SetRotateValue(63,31,i*30,1);
		tmp1=GetRotateXY(63-29,31);
		tmp2=GetRotateXY(63-24,31);
		DrawLine(tmp1.x,tmp1.y,tmp2.x,tmp2.y);
	}
	DrawFillCircle(63,31,2);
	DrawCircle(63,31,30);
	UpdateScreen();
	ClearScreen();
}











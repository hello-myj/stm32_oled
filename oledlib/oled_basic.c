#include "oled_basic.h"
#include "oled_config.h"
#include "oled_buffer.h"
#include "oled_color.h"
#include "math.h"

TypeRoate _RoateValue={{0,0},0,1};
static unsigned char _BitTableS[8]={0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff};
static void Rotate(int x0,int y0,int*x,int*y,double angle,int direction);
extern void DriverInit(void);
extern unsigned int OledTimeMs;
void InitGraph(void)
{
	DriverInit();
}


void ClearScreen(void)
{
	ClearScreenBuffer(0);
}

void UpdateScreen(void)
{
	UpdateScreenDisplay();
}

void FillByte(int page,int x,unsigned  char byte)
{
		if(GetFillColor())
			WriteByteBuffer(page,x,ReadByteBuffer(page,x)|byte);
	else
			WriteByteBuffer(page,x,ReadByteBuffer(page,x)&(~byte));
}

void FillRect(int x,int y,int width,int height)
{
	int i,j;
	int temp =(y+height-1)/8-y/8;
	if(x>SCREEN_COLUMN ||y>SCREEN_ROW)   
			return;
	for(i=x;i<x+width&&i<128;i++)
	{
		if( temp==0 )
		{
			//ScreenBuffer[y/8][i] |=_BitTableS[height-1]<<(y%8);
			FillByte(y/8,i,_BitTableS[height-1]<<(y%8));
		}
		else
		{
			//ScreenBuffer[y/8][i] |=_BitTableS[(8-y%8)-1]<<(y%8);
			FillByte(y/8,i,_BitTableS[(8-y%8)-1]<<(y%8));
			for(j=1;j<temp;j++)
			{
				//ScreenBuffer[y/8+j][i] |=0xff;
				FillByte(y/8+j,i,0xff);
			}
			//ScreenBuffer[y/8+temp][i] |=_BitTableS[(height-1+y)%8];
			FillByte(y/8+temp,i,_BitTableS[(height-1+y)%8]);
			
		}	
	}
}


void FillVerticalLine(int x,int y,int height,int value)
{
	int temp =(y+height-1)/8-y/8,j;

		if( temp==0 )
			{
				//ScreenBuffer[y/8][x] |=(_BitTableS[height-1]<<(y%8));
				FillByte(y/8,x,_BitTableS[height-1]<<(y%8));
			}
			else
			{
				//ScreenBuffer[y/8][x] |=(_BitTableS[(8-y%8)-1]<<(y%8));
				FillByte(y/8,x,_BitTableS[(8-y%8)-1]<<(y%8));
				for(j=1;j<temp;j++)
				{
				//	ScreenBuffer[y/8+j][x] |=0xff;
					FillByte(y/8+j,x,0xff);
				}
			//	ScreenBuffer[y/8+temp][x] |=_BitTableS[(height-1+y)%8];
				FillByte(y/8+temp,x,_BitTableS[(height-1+y)%8]);
			}	
}
//点x，y绕x0，y0旋转angle弧度

float mySqrt(float x)
{
float a = x;
unsigned int i = *(unsigned int *)&x;
i = (i + 0x3f76cf62) >> 1;
x = *(float *)&i;
x = (x + a / x) * 0.5;
return x;
}
//————————————————
//版权声明：本文为CSDN博主「xtlisk」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/xtlisk/article/details/51249371
unsigned int sqrt_16(unsigned long M)  
{  
    unsigned int N, i;  
    unsigned long tmp, ttp;   // 结果、循环计数  
    if (M == 0)               // 被开方数，开方结果也为0  
        return 0;  
    N = 0;  
    tmp = (M >> 30);          // 获取最高位：B[m-1]  
    M <<= 2;  
    if (tmp > 1)              // 最高位为1  
    {  
        N ++;                 // 结果当前位为1，否则为默认的0  
        tmp -= N;  
    }  
    for (i=15; i>0; i--)      // 求剩余的15位  
    {  
        N <<= 1;              // 左移一位  

        tmp <<= 2;  
        tmp += (M >> 30);     // 假设  

        ttp = N;  
        ttp = (ttp<<1)+1;  

        M <<= 2;  
        if (tmp >= ttp)       // 假设成立  
        {  
            tmp -= ttp;  
            N ++;  
        }  
    }  
    return N;  
} 




//需要优化atant2 cos sin算法
static void Rotate(int x0,int y0,int*x,int*y,double angle,int direction)
{

	//double r=sqrt((*y-y0)*(*y-y0)+(*x-x0)*(*x-x0));
	int temp=(*y-y0)*(*y-y0)+(*x-x0)*(*x-x0);
	double r=mySqrt(temp);
	double a0=atan2(*x-x0,*y-y0);
	if(direction)
	{
		*x=x0+r*cos(a0+angle);
		*y=y0+r*sin(a0+angle);
	}
	else
	{
		*x=x0+r*cos(a0-angle);
		*y=y0+r*sin(a0-angle);	
	}
}

void SetAngle(float angle)
{
	_RoateValue.angle=RADIAN(angle);
}

void SetAnggleDir(int direction)
{
	_RoateValue.direct=direction;
}

void SetRotateCenter(int x0,int y0)
{
	_RoateValue.center.x=x0;
	_RoateValue.center.y=y0;
}
void SetRotateValue(int x,int y,float angle,int direct)
{
	SetRotateCenter(x,y);
	SetAnggleDir(direct);
	SetAngle(angle);
}
TypeXY GetRotateXY(int x,int y)
{
	TypeXY temp;
	int m=x,n=y;
//	if(_RoateValue.angle!=0)
		Rotate(_RoateValue.center.x,_RoateValue.center.y,&m,&n, _RoateValue.angle ,_RoateValue.direct);
	temp.x=m;
	temp.y=n;
	return temp;
}

void FloodFill(unsigned char x,unsigned char y,int oldcolor,int newcolor)
{		
		UpdateScreen();
		if(GetPointBuffer(x,y)==oldcolor)
		{
				SetPointBuffer(x,y,newcolor);
				FloodFill(x-1,y,oldcolor,newcolor);
				FloodFill(x+1,y,oldcolor,newcolor);
				FloodFill(x,y+1,oldcolor,newcolor);
				FloodFill(x,y-1,oldcolor,newcolor);
		}
}


unsigned char GetLengthUp(unsigned char x,unsigned char y,unsigned char value)
{
	if(GetPointBuffer(x,y)==value)
	{
		if(y==0)
			return 0;
		return 1+GetLengthUp(x,y-1,value);

	}
	return 0;
}
unsigned char GetLengthDown(unsigned char x,unsigned char y,unsigned char value)
{
	if(GetPointBuffer(x,y)==value)
	{
		if(y==63)
			return 0;
		return 1+GetLengthUp(x,y+1,value);

	}
	return 0;
}




void FloodFill2(unsigned char x,unsigned char y,int oldcolor,int newcolor)
{
		unsigned char h1=0,h2=0;
		short tempx=x;	
		while(GetPointBuffer(tempx,y)==oldcolor && tempx<128)
		{		
			h1=GetLengthDown(tempx,y,oldcolor);
			h2=GetLengthUp(tempx,y,oldcolor);
			FillVerticalLine(tempx,y-h2,h1+h2,newcolor);
			tempx++;
		}
		tempx=x-1;
		while(GetPointBuffer(tempx,y)==oldcolor&&tempx>0)
		{
			
			h1=GetLengthDown(tempx,y,oldcolor);
			h2=GetLengthUp(tempx,y,oldcolor);
			FillVerticalLine(tempx,y-h2,h1+h2,newcolor);
			tempx--;
		}
}

unsigned char pgm_read_byte(const unsigned char * addr)
{

	return *addr;
}
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}	



unsigned char FrameRateUpdateScreen(int value)
{
		
		if(OledTimeMs==0)
		{
			UpdateScreen();
			ClearScreen();
			OledTimeMs=1000/value;
			return 1;
		}
		return 0;
}

void WaitTimeMs(unsigned int time)
{
	OledTimeMs=time;
	while(OledTimeMs);
}

void UpdateScreenPiece(int x, int y, unsigned char w,unsigned char h)
{


}

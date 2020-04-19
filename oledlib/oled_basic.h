#ifndef OLED_BASIC_H
#define OLED_BASIC_H

typedef struct COORDINATE 
{
	int x;
	int y;
}TypeXY;
#define point TypeXY 
typedef struct ROATE
{
	TypeXY center;
	float angle;
	int direct;
}TypeRoate;

#define RADIAN(angle)  ((angle==0)?0:(3.14159*angle/180))
#define MAX(x,y)  		((x)>(y)? (x):(y))
#define MIN(x,y)  		((x)<(y)? (x):(y))
#define SWAP(x, y) \
	(y) = (x) + (y); \
	(x) = (y) - (x); \
	(y) = (y) - (x);

#define HW_IIC    (0)
#define SW_IIC    (1)
#define HW_SPI		(2)
#define SW_SPI		(3)
#define HW_8080   (4)

#define IIC_1     (10)
#define IIC_2     (11)


#define SPI_1     (20)
#define SPI_2	    (21)	


//设置背景模式，0为透明，1为正常。
void SetTextBkMode(unsigned char value);
//获取当前背景模式
unsigned char GetTextBkMode(void);
void SetPointBuffer(int x,int y,int value);
unsigned char GetPointBuffer(int x,int y);
void UpdateScreen(void);
void ClearScreen(void);
void InitGraph(void);

TypeXY GetRotateXY(int x,int y);
void SetRotateValue(int x,int y,float angle,int direct);
void SetRotateCenter(int x0,int y0);
void SetAnggleDir(int direction);
void SetAngle(float angle);
void FloodFill(unsigned char x,unsigned char y,int oldcolor,int newcolor);
void FillRect(int x,int y,int width,int height);
void FloodFill2(unsigned char x,unsigned char y,int oldcolor,int newcolor);
void FillVerticalLine(int x,int y,int height,int value);
void FillByte(int page,int x,unsigned  char byte);

unsigned char pgm_read_byte(const unsigned char * addr);
unsigned int oled_pow(unsigned char m,unsigned char n);
unsigned char FrameRateUpdateScreen(int value);
void WaitTimeMs(unsigned int time);
#endif


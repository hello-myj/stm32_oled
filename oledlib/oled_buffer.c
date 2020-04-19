#include "oled_buffer.h"
#include "oled_config.h"
#include "oled_color.h"
#include "string.h"

//¶¨Òå»º³å£¬
unsigned char ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};
unsigned char TempBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};
static _Bool _SelectedBuffer=SCREEN_BUFFER;
#define BUFFERSIZE  sizeof(ScreenBuffer)
extern void UpdateTempBuffer(void);
extern void UpdateScreenBuffer(void);


void SetTempBuffer(void)
{

	_SelectedBuffer=TEMP_BUFFER;
}

void SetScreenBuffer(void)
{
	_SelectedBuffer=SCREEN_BUFFER;
}
unsigned char GetSelectedBuffer(void)
{
	return _SelectedBuffer;
}

unsigned char ReadByteBuffer(int page,int x)
{
	return _SelectedBuffer? ScreenBuffer[page][x] :TempBuffer[page][x];
}
	

void WriteByteBuffer(int page,int x,unsigned char byte)
{
	if(_SelectedBuffer)
	{
			ScreenBuffer[page][x] =byte;
	}
	else
	{
			TempBuffer[page][x] =byte;
	}
}

void SetPointBuffer(int x,int y,int value)
{
	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //³¬³ö·¶Î§
		return;
	if(_SelectedBuffer)
	{
		if(value)
			ScreenBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
		else
			ScreenBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	
	}
	else
	{
		if(value)
			TempBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
		else
			TempBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	
	
	}
}


unsigned char GetPointBuffer(int x,int y)
{
	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //³¬³ö·¶Î§
		return 0;
	if(_SelectedBuffer)
		return (ScreenBuffer[y/SCREEN_PAGE_NUM][x]>>(y%SCREEN_PAGE_NUM))&0x01;
	else
		return (TempBuffer[y/SCREEN_PAGE_NUM][x]>>(y%SCREEN_PAGE_NUM))&0x01;
}

void UpdateScreenDisplay(void)
{
	UpdateScreenBuffer();
}


void ClearScreenBuffer(unsigned char val)
{
	memset(ScreenBuffer,val,sizeof(ScreenBuffer));
}

void ClearTempBuffer(void)
{
	memset(TempBuffer,0,sizeof(TempBuffer));
}
//1.TEMPBUFF_COPY_TO_SCREEN
//2.TEMPBUFF_CLEAN
//3.TEMPBUFF_COVER_H
//4.TEMPBUFF_COVER_L
void TempBufferFunc(int func)
{
	int i,j;
	switch (func)
	{
		case TEMPBUFF_COPY_TO_SCREEN:
			memcpy(ScreenBuffer,TempBuffer,BUFFERSIZE);
			break;
		case TEMPBUFF_CLEAN:
			ClearTempBuffer();
			break;
		case TEMPBUFF_COVER_H:
				for(i=0;i<8;i++)
					for(j=0;j<128;j++)
						ScreenBuffer[i][j] |=TempBuffer[i][j];
				break;
		case TEMPBUFF_COVER_L:
				for(i=0;i<8;i++)
					for(j=0;j<128;j++)
						ScreenBuffer[i][j] &=~TempBuffer[i][j];
				break;
		default:
			break;
	
	
	}
	
}

//void WritePointToTempBuffer(int x,int y,int value)
//{
//	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //³¬³ö·¶Î§
//		return;
//	if(value)
//		TempBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
//	else
//		TempBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	
//}

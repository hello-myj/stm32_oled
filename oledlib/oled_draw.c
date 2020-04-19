#include "oled_draw.h"
#include "oled_buffer.h"
#include "oled_config.h"
#include "stdlib.h"
#include "math.h"
#include "oled_bmp.h"
#include "oled_font.h"


static int  _pointx=0;
static int 	_pointy=0;
void DrawPixel(int x,int y)
{
	SetPointBuffer(x,y,GetDrawColor());
}
Type_color GetPixel(int x,int y)
{
		if(GetPointBuffer(x,y))
			return pix_white;
		else
			return pix_black;
}

void MoveTo(int x,int y)
{
	_pointx=x;
	_pointy=y;
}
TypeXY GetXY(void)
{
	
	TypeXY m;
	m.x=_pointx;
	m.y=_pointy;
	return m;
}
int GetX(void)
{
	return _pointx;
}
int GetY(void)
{
	return _pointy;
}
void LineTo(int x,int y)
{
	DrawLine(_pointx,_pointy,x,y);
	_pointx=x;
	_pointy=y;

}
//int GetPixel(int x,int y)
//{
//	//return ;
//}
void DrawLine(int x1,int y1,int x2,int y2)
{
	unsigned short us; 
	unsigned short usX_Current, usY_Current;
	
	int lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int lIncrease_X, lIncrease_Y; 	
	
	lDelta_X = x2 - x1; //计算坐标增量 
	lDelta_Y = y2 - y1; 
	
	usX_Current = x1; 
	usY_Current = y1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //设置单步方向 
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//垂直线 
	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//水平线 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //选取基本增量坐标轴 
	
	else 
		lDistance = lDelta_Y; 
	
	for ( us = 0; us <= lDistance + 1; us ++ )//画线输出 
	{  	
		SetPointBuffer(usX_Current,usY_Current,GetDrawColor());//画点 
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 		
	}  
}

void DrawFastHLine(int x, int y, unsigned char w)
{
  int end = x+w;
	int a;
	Type_color color =GetDrawColor();
  for ( a = MAX(0,x); a < MIN(end,SCREEN_COLUMN); a++)
  {
    SetPointBuffer(a,y,color);
  }
}
void DrawFastVLine(int x, int y, unsigned char h)
{
  int end = y+h;
	int a;
	Type_color color =GetDrawColor();
  for (a = MAX(0,y); a < MIN(end,SCREEN_ROW); a++)
  {
    SetPointBuffer(x,a,color);
  }
}

void DrawPolyLineTo(const TypeXY *points,int num)
{
		int i=0;
		MoveTo(points[0].x,points[0].y);
		for(i=1;i<num;i++)
		{
			LineTo(points[i].x,points[i].y);
		}
}

void DrawRect1(int left,int top,int right,int bottom)
{
		DrawLine ( left, top, right, top );
		DrawLine ( left, bottom , right , bottom );
		DrawLine ( left, top, left, bottom );
		DrawLine ( right , top, right , bottom );	
}

void DrawFillRect1(int left,int top,int right,int bottom)
{

		DrawRect1(left,top,right,bottom);
		FillRect(left+1,top+1,right-left-1,bottom-top-1);	
}

void DrawRect2(int left,int top,int width,int height)
{
		DrawLine ( left, top, left+width-1, top );
		DrawLine ( left, top+height-1 , left+width-1 , top+height-1 );
		DrawLine ( left, top, left, top+height-1);
		DrawLine ( left+width-1 , top, left+width-1 , top+height-1);	
}
void DrawFillRect2(int left,int top,int width,int height)
{
		DrawRect2(left,top,width,height);
		FillRect(left+1,top+1,width-1,height-1);	
}



void DrawCircle ( int usX_Center, int usY_Center, int usRadius)
{
	short sCurrentX, sCurrentY;
	short sError;
	sCurrentX = 0; sCurrentY = usRadius;	  
	sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志
	while ( sCurrentX <= sCurrentY )
	{      
			SetPointBuffer ( usX_Center + sCurrentX, usY_Center + sCurrentY	,GetDrawColor());             //1，研究对象
			SetPointBuffer ( usX_Center - sCurrentX, usY_Center + sCurrentY ,GetDrawColor());             //2      
			SetPointBuffer ( usX_Center - sCurrentY, usY_Center + sCurrentX ,GetDrawColor());             //3
			SetPointBuffer ( usX_Center - sCurrentY, usY_Center - sCurrentX ,GetDrawColor());             //4
			SetPointBuffer ( usX_Center - sCurrentX, usY_Center - sCurrentY ,GetDrawColor());             //5       
			SetPointBuffer ( usX_Center + sCurrentX, usY_Center - sCurrentY ,GetDrawColor());             //6
			SetPointBuffer ( usX_Center + sCurrentY, usY_Center - sCurrentX ,GetDrawColor());             //7 
			SetPointBuffer ( usX_Center + sCurrentY, usY_Center + sCurrentX ,GetDrawColor());             //0
			sCurrentX ++;		
			if ( sError < 0 ) 
				sError += 4 * sCurrentX + 6;	  
			else
			{
				sError += 10 + 4 * ( sCurrentX - sCurrentY );   
				sCurrentY --;
			} 		
	}
}
void DrawFillCircle( int usX_Center, int usY_Center, int r)
{
			DrawFastVLine(usX_Center, usY_Center-r, 2*r+1);
			DrawFillCircleHelper(usX_Center,usY_Center, r, 3, 0);
}


#if 0
void DrawFillCircle( int usX_Center, int usY_Center, int usRadius)
{
	short sCurrentX, sCurrentY;
	short sError;
	DrawCircle(usX_Center,usY_Center,usRadius);
	usRadius=usRadius-1;
	sCurrentX = 0; sCurrentY = usRadius;	  
	sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志
	while ( sCurrentX <= sCurrentY )
	{
			short sCountY;   
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{
				SetPointBuffer ( usX_Center + sCurrentX, usY_Center + sCountY ,GetFillColor());             //1，研究对象
				SetPointBuffer ( usX_Center - sCurrentX, usY_Center + sCountY  ,GetFillColor());             //2      
				SetPointBuffer ( usX_Center - sCountY,   usY_Center + sCurrentX ,GetFillColor());             //3
				SetPointBuffer ( usX_Center - sCountY,   usY_Center - sCurrentX ,GetFillColor());             //4
				SetPointBuffer ( usX_Center - sCurrentX, usY_Center - sCountY ,GetFillColor());             //5       
				SetPointBuffer ( usX_Center + sCurrentX, usY_Center - sCountY ,GetFillColor());             //6
				SetPointBuffer ( usX_Center + sCountY,   usY_Center - sCurrentX ,GetFillColor());             //7 
				SetPointBuffer ( usX_Center + sCountY,   usY_Center + sCurrentX ,GetFillColor());             //0
			}
			sCurrentX ++;		
			if ( sError < 0 ) 
				sError += 4 * sCurrentX + 6;	  
			else
			{
				sError += 10 + 4 * ( sCurrentX - sCurrentY );   
				sCurrentY --;
			} 		
	}
}
#endif

void DrawCircleHelper(int x0, int y0, unsigned char r, unsigned char cornername)
{
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;
	Type_color color=GetDrawColor();
  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x4)
    {
      SetPointBuffer(x0 + x, y0 + y,color);
      SetPointBuffer(x0 + y, y0 + x,color);
    }
    if (cornername & 0x2)
    {
      SetPointBuffer(x0 + x, y0 - y, color);
      SetPointBuffer(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8)
    {
      SetPointBuffer(x0 - y, y0 + x, color);
      SetPointBuffer(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1)
    {
      SetPointBuffer(x0 - y, y0 - x, color);
      SetPointBuffer(x0 - x, y0 - y, color);
    }
  }
}

void DrawFillCircleHelper(int x0, int y0, unsigned char r, unsigned char cornername, int delta)
{
  // used to do circles and roundrects!
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;
	Type_color color=GetDrawColor();
  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x1)
    {
      DrawFastVLine(x0+x, y0-y, 2*y+1+delta);
      DrawFastVLine(x0+y, y0-x, 2*x+1+delta);
    }

    if (cornername & 0x2)
    {
      DrawFastVLine(x0-x, y0-y, 2*y+1+delta);
      DrawFastVLine(x0-y, y0-x, 2*x+1+delta);
    }
  }
}
//需改进
void DrawArc(int x,int y,unsigned char r,int angle_start,int angle_end)
{
	float i=0;
	TypeXY m,temp;
	temp=GetXY();
	SetRotateCenter(x,y);
	SetAnggleDir(0);
	if(angle_end>360)
		angle_end=360;
	SetAngle(0);
	m=GetRotateXY(x,y+r);
	MoveTo(m.x,m.y);
	for(i=angle_start;i<angle_end;i+=5)
	{
		SetAngle(i);
		m=GetRotateXY(x,y+r);
		LineTo(m.x,m.y);
		//SetPointBuffer(m.x,m.y,GetDrawColor());
	}
	LineTo(x+r,y);
	MoveTo(temp.x,temp.y);
}
void DrawFillArc(int x,int y,unsigned char r,int angle_start,int angle_end)
{

}

void DrawRoundRect(int x, int y, unsigned char w, unsigned char h, unsigned char r)
{
  // smarter version
  DrawFastHLine(x+r, y, w-2*r); // Top
  DrawFastHLine(x+r, y+h-1, w-2*r); // Bottom
  DrawFastVLine(x, y+r, h-2*r); // Left
  DrawFastVLine(x+w-1, y+r, h-2*r); // Right
  // draw four corners
  DrawCircleHelper(x+r, y+r, r, 1);
  DrawCircleHelper(x+w-r-1, y+r, r, 2);
  DrawCircleHelper(x+w-r-1, y+h-r-1, r, 4);
  DrawCircleHelper(x+r, y+h-r-1, r, 8);
}

void DrawfillRoundRect(int x, int y, unsigned char w, unsigned char h, unsigned char r)
{
  // smarter version
  DrawFillRect2(x+r, y, w-2*r, h);

  // draw four corners
  DrawFillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1);
  DrawFillCircleHelper(x+r, y+r, r, 2, h-2*r-1);
}

#if 0//以下注释的这段代码是画椭圆的算法，但运算过大，不适合使用，已经修改为Bresenham 算法
void DrawEllipse(int x0,int y0,int a,int b)
{
	int x,y;double r=0.0,end=3.14159*2;
	int tempy=GetY();
	int tempx=GetX();
	x=x0+a;y=y0;
	MoveTo(x,y);
	for(r=0;r<=end;r+=0.05)
	{
		x=x0+a*cos(r);
		y=y0+b*sin(r);
		LineTo(x,y);
	}
	MoveTo(tempx,tempy);
}
void DrawFillEllipse(int x0,int y0,int a,int b)
{
	int x,y;double r=0.0,end=3.14159*2;
	int tempy=GetY();
	int tempx=GetX();

	while(a--)
	{
		x=x0+a;y=y0;
		MoveTo(x,y);
		for(r=0;r<=end;r+=0.05)
		{
			x=x0+a*cos(r);
			y=y0+b*sin(r);
			LineTo(x,y);
		}
	}
	MoveTo(tempx,tempy);
}
#endif
void DrawEllipse(int xCenter,int yCenter,int Rx,int Ry)
{
		int Rx2=Rx*Rx;
		int Ry2=Ry*Ry;
		int twoRx2=2*Rx2;
		int twoRy2=2*Ry2;
		int p;
		int x=0;
		int y=Ry;
		int px = 0;
		int py = twoRx2*y;
		SetPointBuffer(xCenter+x,yCenter+y,GetDrawColor());
		SetPointBuffer(xCenter-x,yCenter+y,GetDrawColor());
		SetPointBuffer(xCenter+x,yCenter-y,GetDrawColor());
		SetPointBuffer(xCenter-x,yCenter-y,GetDrawColor());
		//Region?1
		p=(int)(Ry2-Rx2*Ry+0.25*Rx2);
		while(px<py)
		{
				x++;
				px+=twoRy2;
				if(p<0)
					p+=Ry2+px;
				else
				{
					y--;
					py-=twoRx2;
					p+=Ry2+px-py;
				}
			SetPointBuffer(xCenter+x,yCenter+y,GetDrawColor());
			SetPointBuffer(xCenter-x,yCenter+y,GetDrawColor());
			SetPointBuffer(xCenter+x,yCenter-y,GetDrawColor());
			SetPointBuffer(xCenter-x,yCenter-y,GetDrawColor());
		}
		//Region?2
		p=(int)(Ry2*(x+0.5)*(x+0.5)+Rx2*(y-1)*(y-1)-Rx2*Ry2);
		while(y>0)
		{
			y--;
			py-=twoRx2;
			if(p>0)
				p+=Rx2-py;
			else
			{
				x++;
				px+=twoRy2;
				p+=Rx2-py+px;
			}
			SetPointBuffer(xCenter+x,yCenter+y,GetDrawColor());
			SetPointBuffer(xCenter-x,yCenter+y,GetDrawColor());
			SetPointBuffer(xCenter+x,yCenter-y,GetDrawColor());
			SetPointBuffer(xCenter-x,yCenter-y,GetDrawColor());
		}
}
#if 0
void DrawFillEllipse(int xCenter,int yCenter,int Rx,int Ry)
{		
			Type_color temp1=GetDrawColor();
			Type_color temp2=GetFillColor();
			SetTempBuffer();
			SetDrawColor(pix_white);
			DrawEllipse(xCenter,yCenter,Rx,Ry);
			SetDrawColor(temp1);
			SetFillcolor(pix_white);
			FloodFill2(xCenter,yCenter,0,1);
			SetFillcolor(temp2);
			if(GetFillColor())
				TempBufferFunc(TEMPBUFF_COVER_H);
			else
				TempBufferFunc(TEMPBUFF_COVER_L);
			ClearTempBuffer();
			SetScreenBuffer();
			DrawEllipse(xCenter,yCenter,Rx,Ry);	
}
#else

void DrawFillEllipse(int x0, int y0,int rx,int ry)
{
  int x, y;
  int xchg, ychg;
  int err;
  int rxrx2;
  int ryry2;
  int stopx, stopy;
  
  rxrx2 = rx;
  rxrx2 *= rx;
  rxrx2 *= 2;
  
  ryry2 = ry;
  ryry2 *= ry;
  ryry2 *= 2;
  
  x = rx;
  y = 0;
  
  xchg = 1;
  xchg -= rx;
  xchg -= rx;
  xchg *= ry;
  xchg *= ry;
  
  ychg = rx;
  ychg *= rx;
  
  err = 0;
  
  stopx = ryry2;
  stopx *= rx;
  stopy = 0;
  
  while( stopx >= stopy )
  {
		
		DrawFastVLine( x0+x, y0-y, y+1);
		DrawFastVLine( x0-x, y0-y, y+1);
		DrawFastVLine( x0+x, y0, y+1);
		DrawFastVLine( x0-x, y0, y+1);
    //draw_filled_ellipse_section(u8g, x, y, x0, y0, option);
    y++;
    stopy += rxrx2;
    err += ychg;
    ychg += rxrx2;
    if ( 2*err+xchg > 0 )
    {
      x--;
      stopx -= ryry2;
      err += xchg;
      xchg += ryry2;      
    }
  }

  x = 0;
  y = ry;
  
  xchg = ry;
  xchg *= ry;
  
  ychg = 1;
  ychg -= ry;
  ychg -= ry;
  ychg *= rx;
  ychg *= rx;
  
  err = 0;
  
  stopx = 0;

  stopy = rxrx2;
  stopy *= ry;
  

  while( stopx <= stopy )
  {
		DrawFastVLine( x0+x, y0-y, y+1);
		DrawFastVLine( x0-x, y0-y, y+1);
		DrawFastVLine( x0+x, y0, y+1);
		DrawFastVLine( x0-x, y0, y+1);
   // u8g_draw_filled_ellipse_section(u8g, x, y, x0, y0, option);
    x++;
    stopx += ryry2;
    err += xchg;
    xchg += ryry2;
    if ( 2*err+ychg > 0 )
    {
      y--;
      stopy -= rxrx2;
      err += ychg;
      ychg += rxrx2;
    }
  }
  
}
#endif

void DrawEllipseRect( int x0, int y0, int x1, int y1)
{
	int a = abs(x1 - x0);
	int b = abs(y1 - y0);	//get diameters
	int b1 = b&1;
	long dx = 4*(1-a)*b*b;
	long dy = 4*(b1+1)*a*a;
	long err = dx+dy+b1*a*a;
	long e2;
	
	if (x0 > x1) { x0 = x1; x1 += a; }
	if (y0 > y1) { y0 = y1; } 
	y0 += (b+1)/2;
	y1 = y0-b1;
	a *= 8*a;
	b1 = 8*b*b;
	
	do {
		DrawPixel( x1, y0);
		DrawPixel( x0, y0);
		DrawPixel( x0, y1);
		DrawPixel( x1, y1);
		e2 = 2*err;
		if (e2 >= dx) {
			x0++;
			x1--;
			err += dx += b1;
		}
		if (e2 <= dy) {
			y0++;
			y1--;
			err += dy += a;
		}
	} while (x0 <= x1);
	
	while (y0-y1 < b) {
		DrawPixel( x0-1, y0);
		DrawPixel( x1+1, y0++);
		DrawPixel( x0-1, y1);
		DrawPixel( x1+1, y1--);
	}
}




void DrawTriangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  DrawLine(x0, y0, x1, y1);
  DrawLine(x1, y1, x2, y2);
  DrawLine(x2, y2, x0, y0);
}

void DrawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
  int a, b, y, last;
	 int dx01, dy01,dx02,dy02,dx12,dy12,sa = 0,sb = 0;
	Type_color tmpcolor;
	tmpcolor =GetDrawColor();
	SetDrawColor(GetFillColor());
  if (y0 > y1)
  {
    SWAP(y0, y1); SWAP(x0, x1);
  }
  if (y1 > y2)
  {
    SWAP(y2, y1); SWAP(x2, x1);
  }
  if (y0 > y1)
  {
    SWAP(y0, y1); SWAP(x0, x1);
  }

  if(y0 == y2)
  {
    a = b = x0;
    if(x1 < a)
    {
      a = x1;
    }
    else if(x1 > b)
    {
      b = x1;
    }
    if(x2 < a)
    {
      a = x2;
    }
    else if(x2 > b)
    {
      b = x2;
    }
    DrawFastHLine(a, y0, b-a+1);
    return;
  }
			dx01 = x1 - x0,
      dy01 = y1 - y0,
      dx02 = x2 - x0,
      dy02 = y2 - y0,
      dx12 = x2 - x1,
      dy12 = y2 - y1,
      sa = 0,
      sb = 0;
  if (y1 == y2)
  {
    last = y1;   // Include y1 scanline
  }
  else
  {
    last = y1-1; // Skip it
  }


  for(y = y0; y <= last; y++)
  {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b)
    {
      SWAP(a,b);
    }

    DrawFastHLine(a, y, b-a+1);
  }
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y <= y2; y++)
  {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if(a > b)
    {
      SWAP(a,b);
    }
    DrawFastHLine(a, y, b-a+1);
  }
	SetDrawColor(tmpcolor);
}




void DrawBitmap(int x, int y, const unsigned char *bitmap, unsigned char w, unsigned char h)
{
	int iCol,a;
  int yOffset = abs(y) % 8;
  int sRow = y / 8;
	int rows = h/8;

	if (x+w < 0 || x > SCREEN_COLUMN-1 || y+h < 0 || y > SCREEN_ROW-1)
    return;
	if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  if (h%8!=0) rows++;
  for (a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (SCREEN_ROW/8)-1) break;
    if (bRow > -2) {
      for (iCol = 0; iCol<w; iCol++) {
        if (iCol + x > (SCREEN_COLUMN-1)) break;
        if (iCol + x >= 0) {
          if (bRow >= 0) {
						if(GetDrawColor() == pix_white)
						{
									unsigned char temp = ReadByteBuffer(bRow,x + iCol);
									temp|=pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
									WriteByteBuffer(bRow,x + iCol,temp);
						}
						else if(GetDrawColor() == pix_black)
						{
									unsigned char temp = ReadByteBuffer(bRow,x + iCol);
									temp&=~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
									WriteByteBuffer(bRow,x + iCol,temp);
						}
						else
						{
									unsigned char temp = ReadByteBuffer(bRow,x + iCol);
									temp^=(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
									WriteByteBuffer(bRow,x + iCol,temp);
						}
          }
          if (yOffset && bRow<(SCREEN_ROW/8)-1 && bRow > -2) {
						if(GetDrawColor() == pix_white) 
						{
									unsigned char temp = ReadByteBuffer(bRow+1,x + iCol);
									temp|=pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
									WriteByteBuffer(bRow+1,x + iCol,temp);
						}
						else if (GetDrawColor() == pix_black)
						{
									unsigned char temp = ReadByteBuffer(bRow+1,x + iCol);
									temp&=~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
									WriteByteBuffer(bRow+1,x + iCol,temp);
						}
						else
						{
									unsigned char temp = ReadByteBuffer(bRow+1,x + iCol);
									temp^=pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
									WriteByteBuffer( bRow+1,x + iCol,temp);						
						}
          }
        }
      }
    }
  }
}

void DrawSlowBitmap(int x, int y, const unsigned char *bitmap, unsigned char w, unsigned char h)
{
  // no need to dar at all of we're offscreen
	int xi, yi, byteWidth = (w + 7) / 8;
 	if (x+w < 0 || x > SCREEN_COLUMN-1 || y+h < 0 || y > SCREEN_ROW-1)
    return; 
  for(yi = 0; yi < h; yi++) {
    for(xi = 0; xi < w; xi++ ) {
      if(pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
        SetPointBuffer(x + xi, y + yi,GetDrawColor());
      }
    }
  }
}

void DrawChar(int x, int y, unsigned char c)
{
	int i,j;
  unsigned char draw_background,bg,a,b,size,color;
	size=GetFontSize();
	color=GetDrawColor();
	bg=GetTextBkMode();
	draw_background= bg != color;
  if ((x >= SCREEN_COLUMN) ||         // Clip right
    (y >= SCREEN_ROW) ||        // Clip bottom
    ((x + 5 * size - 1) < 0) ||   // Clip left
    ((y + 8 * size - 1) < 0)    // Clip top
  )
  {
    return;
  }

  for ( i=0; i<6; i++ )
  {
    int line;
    if (i == 5)
    {
      line = 0x0;
    }
    else
    {
      line = pgm_read_byte(font5x7+(c*5)+i);
    }

    for ( j = 0; j<8; j++)
    {
      unsigned char draw_color = (line & 0x1) ? color : bg;

      if (draw_color || draw_background) {
        for ( a = 0; a < size; a++ ) {
          for ( b = 0; b < size; b++ ) {
            SetPointBuffer(x + (i * size) + a, y + (j * size) + b, draw_color);
          }
        }
      }
      line >>= 1;
    }
  }
}

void DrawString(int x, int y,char *str)
{
	unsigned char j=0,tempx=x,tempy=y;
	unsigned char size=GetFontSize();
	while (str[j]!='\0')
	{		
		//if(str[j]=='\n'||tempx+size*6>127)
		if(str[j]=='\n')
		{
			tempy+=8*size;
			tempx=x;
			j++;
			continue;
		}
		DrawChar(tempx,tempy,str[j]);
		tempx+=size*6;
		j++;
	}
}

void DrawNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;			
	unsigned char size=GetFontSize();
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				DrawChar(x+(size*6)*t,y,'0');
				continue;
			}else enshow=1; 	 
		}
	 DrawChar(x+(size*6)*t,y,temp+'0'); 
	}
} 



extern int FpsCount;
void DrawFps()
{
	//获取的时间为1s，就打印FPS，否则就是一直++

}




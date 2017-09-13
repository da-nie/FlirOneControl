#include "cdecorator_scale.h"

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CDecorator_Scale::CDecorator_Scale(CIImage *cIImage_Set_Ptr,unsigned long NewWidth,unsigned long NewHeight):CDecorator_CIImage(cIImage_Set_Ptr)
{ 
 Width=NewWidth;
 Height=NewHeight;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CDecorator_Scale::~CDecorator_Scale()
{ 
}
//----------------------------------------------------------------------------------------------------
//получить RGBA изображение
//----------------------------------------------------------------------------------------------------
void CDecorator_Scale::GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)
{
 //создаём увеличенное изображение
 vector<unsigned long> vector_oldimage;//данные изображения
 //запрашиваем изображение
 CDecorator_CIImage::GetRGBAImage(width,height,vector_oldimage);
 //производим масштабирование
 unsigned long x;
 unsigned long y;
 vector_image.resize(Width*Height);

 unsigned long *i_ptr=&(vector_image[0]);
 unsigned long *oldi_ptr=&(vector_oldimage[0]);
 for(y=0;y<Height;y++)
 {
  float tmp=static_cast<float>(y)/static_cast<float>(Height-1)*static_cast<float>(height-1);
  long ys=static_cast<long>(floor(tmp));
  if (ys<0) ys=0;
  if (ys>=height-1) ys=height-2;
  float u=tmp-ys;
  for(x=0;x<Width;x++,i_ptr++)
  {
   float tmp=static_cast<float>(x)/static_cast<float>(Width-1)*static_cast<float>(width-1);
   long xs=static_cast<long>(floor(tmp));
   if (xs<0) xs=0;
   if (xs>=width-1) xs=width-2;
   float v=tmp-xs;
   //коэффициенты
   float d1=(1-v)*(1-u);
   float d2=v*(1-u);
   float d3=v*u;
   float d4=(1-v)*u;
   //окрестные пиксели
   unsigned long p1=oldi_ptr[xs+ys*width];
   unsigned long p2=oldi_ptr[xs+1+ys*width];
   unsigned long p3=oldi_ptr[xs+1+(ys+1)*width];
   unsigned long p4=oldi_ptr[xs+(ys+1)*width];
   //вычисляем новое значение пикселя
   unsigned long color=0;
   unsigned long mask=0xFF;
   unsigned long offset=0;
   for(unsigned char k=0;k<4;k++,mask<<=8,offset+=8)
   {
	unsigned long kp1=(p1&mask)>>offset;
	unsigned long kp2=(p2&mask)>>offset;
	unsigned long kp3=(p3&mask)>>offset;
	unsigned long kp4=(p4&mask)>>offset;
    unsigned long c=static_cast<short>(kp1*d1+kp2*d2+kp3*d3+kp4*d4);
	if (c>=0xFF) c=0xFF;
	color|=(c<<offset);
   }
   *i_ptr=color;
  }
 } 
 width=Width;
 height=Height;
}
//----------------------------------------------------------------------------------------------------
//задать RGBA изображение
//----------------------------------------------------------------------------------------------------
void CDecorator_Scale::SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image)
{
 CDecorator_CIImage::SetRGBAImage(width,height,vector_image);
}
//----------------------------------------------------------------------------------------------------
//задать размер изображения
//----------------------------------------------------------------------------------------------------
void CDecorator_Scale::SetSize(unsigned long width,unsigned long height)
{
 Width=width;
 Height=height;
 //CDecorator_CIImage::SetSize(width,height);
}
//----------------------------------------------------------------------------------------------------
//задать точку
//----------------------------------------------------------------------------------------------------
void CDecorator_Scale::SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)
{
 CDecorator_CIImage::SetRGBAPixel(x,y,color);
}
//----------------------------------------------------------------------------------------------------
//получить точку
//----------------------------------------------------------------------------------------------------
unsigned long CDecorator_Scale::GetRGBAPixel(unsigned long x,unsigned long y)
{
 return(CDecorator_CIImage::GetRGBAPixel(x,y));
}


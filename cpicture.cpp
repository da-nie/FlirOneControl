#include "cpicture.h"

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CPicture::CPicture(void)
{
 Width=0;
 Height=0;
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CPicture::~CPicture()
{
}
//----------------------------------------------------------------------------------------------------
//�������� ����������� � ������� RGBA
//----------------------------------------------------------------------------------------------------
void CPicture::GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)
{
 width=Width;
 height=Height;
 vector_image=vector_Image;
}
//----------------------------------------------------------------------------------------------------
//������ ����������� � ������� RGBA
//----------------------------------------------------------------------------------------------------
void CPicture::SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image)
{
 Width=width;
 Height=height;
 vector_Image=vector_image;
}
//----------------------------------------------------------------------------------------------------
//������ ������ ����������� � �������� ������
//----------------------------------------------------------------------------------------------------
void CPicture::SetSize(unsigned long width,unsigned long height)
{
 Width=width;
 Height=height;
 vector_Image.resize(Width*Height);
}
//----------------------------------------------------------------------------------------------------
//������ �����
//----------------------------------------------------------------------------------------------------
void CPicture::SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)
{
 if (x>=Width) return;
 if (y>=Height) return;
 vector_Image[x+y*Width]=color;
}
//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
unsigned long CPicture::GetRGBAPixel(unsigned long x,unsigned long y)
{
 if (x>=Width) return(0);
 if (y>=Height) return(0);
 return(vector_Image[x+y*Width]);
}




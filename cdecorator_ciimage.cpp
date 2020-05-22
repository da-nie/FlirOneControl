#include "cdecorator_ciimage.h"

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CDecorator_CIImage::CDecorator_CIImage(CIImage *cIImage_Set_Ptr)
{
 cIImage_Ptr=cIImage_Set_Ptr;
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CDecorator_CIImage::~CDecorator_CIImage()
{
 delete(cIImage_Ptr);
}
//----------------------------------------------------------------------------------------------------
//�������� RGBA �����������
//----------------------------------------------------------------------------------------------------
void CDecorator_CIImage::GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)
{
 cIImage_Ptr->GetRGBAImage(width,height,vector_image);
}
//----------------------------------------------------------------------------------------------------
//������ RGBA �����������
//----------------------------------------------------------------------------------------------------
void CDecorator_CIImage::SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image)
{
 cIImage_Ptr->SetRGBAImage(width,height,vector_image);
}
//----------------------------------------------------------------------------------------------------
//������ ������ ����������� � �������� ������
//----------------------------------------------------------------------------------------------------
void CDecorator_CIImage::SetSize(unsigned long width,unsigned long height)
{
 cIImage_Ptr->SetSize(width,height);
}
//----------------------------------------------------------------------------------------------------
//������ �����
//----------------------------------------------------------------------------------------------------
void CDecorator_CIImage::SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)
{
 cIImage_Ptr->SetRGBAPixel(x,y,color);
}
//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
unsigned long CDecorator_CIImage::GetRGBAPixel(unsigned long x,unsigned long y)
{
 return(cIImage_Ptr->GetRGBAPixel(x,y));
}


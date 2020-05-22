#ifndef CDECORATOR_CIIMAGE_H
#define CDECORATOR_CIIMAGE_H

#include "ciimage.h"

//��������� ��� CIImage
class CDecorator_CIImage:public CIImage
{
 protected:
  CIImage *cIImage_Ptr;
 public:
  CDecorator_CIImage(CIImage *cIImage_Set_Ptr);//�����������
  ~CDecorator_CIImage();//����������
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//�������� RGBA �����������
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//������ RGBA �����������
  void SetSize(unsigned long width,unsigned long height);//������ ������ ����������� � �������� ������
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//������ �����
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//�������� �����
};

#endif




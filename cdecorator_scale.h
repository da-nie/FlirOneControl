#ifndef CDECORATOR_SCALE_H
#define CDECORATOR_SCALE_H

#include "cdecorator_ciimage.h"

//��������� ��� ��������� �������� ����������� � �������������� ���������� ������������
class CDecorator_Scale:public CDecorator_CIImage
{
 protected:
  long Width;//������
  long Height;//������
 public:
  CDecorator_Scale(CIImage *cIImage_Set_Ptr,unsigned long NewWidth,unsigned long NewHeight);//�����������
  ~CDecorator_Scale();//����������
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//�������� RGBA �����������
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//������ RGBA �����������
  void SetSize(unsigned long width,unsigned long height);//������ ������ ����������� � �������� ������
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//������ �����
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//�������� �����
};

#endif

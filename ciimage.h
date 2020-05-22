#ifndef CIIMAGE_H
#define CIIMAGE_H

#include "stdafx.h"

#include <vector>

using namespace std;

//��������� � ������ �����������
class CIImage
{
 public:
  virtual ~CIImage() {};//����������
  virtual void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)=0;//�������� ����������� � ������� RGBA
  virtual void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image)=0;//������ ����������� � ������� RGBA
  virtual void SetSize(unsigned long width,unsigned long height)=0;//������ ������ ����������� � �������� ������
  virtual void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)=0;//������ �����
  virtual unsigned long GetRGBAPixel(unsigned long x,unsigned long y)=0;//�������� �����
};


#endif
#ifndef CPICTURE_H
#define CPICTURE_H

#include "stdafx.h"

#include "ciimage.h"

#include <vector>

using namespace std;

//����� �������� �����������
class CPicture:public CIImage
{
 protected:
  long Width;//������
  long Height;//������
  vector<unsigned long> vector_Image;//������ �����������
 public:
  CPicture(void);//�����������
  ~CPicture();//����������
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//�������� ����������� � ������� RGBA
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//������ ����������� � ������� RGBA
  void SetSize(unsigned long width,unsigned long height);//������ ������ ����������� � �������� ������
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//������ �����
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//�������� �����
};


#endif
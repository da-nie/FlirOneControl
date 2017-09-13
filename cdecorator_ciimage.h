#ifndef CDECORATOR_CIIMAGE_H
#define CDECORATOR_CIIMAGE_H

#include "ciimage.h"

//декоратор для CIImage
class CDecorator_CIImage:public CIImage
{
 protected:
  CIImage *cIImage_Ptr;
 public:
  CDecorator_CIImage(CIImage *cIImage_Set_Ptr);//конструктор
  ~CDecorator_CIImage();//деструктор
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//получить RGBA изображение
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//задать RGBA изображение
  void SetSize(unsigned long width,unsigned long height);//задать размер изображения и выделить память
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//задать точку
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//получить точку
};

#endif




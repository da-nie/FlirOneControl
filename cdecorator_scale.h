#ifndef CDECORATOR_SCALE_H
#define CDECORATOR_SCALE_H

#include "cdecorator_ciimage.h"

//декоратор для изменения размеров изображения с использованием билинейной интерполяции
class CDecorator_Scale:public CDecorator_CIImage
{
 protected:
  long Width;//ширина
  long Height;//высота
 public:
  CDecorator_Scale(CIImage *cIImage_Set_Ptr,unsigned long NewWidth,unsigned long NewHeight);//конструктор
  ~CDecorator_Scale();//деструктор
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//получить RGBA изображение
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//задать RGBA изображение
  void SetSize(unsigned long width,unsigned long height);//задать размер изображения и выделить память
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//задать точку
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//получить точку
};

#endif

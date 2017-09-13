#ifndef CPICTURE_H
#define CPICTURE_H

#include "stdafx.h"

#include "ciimage.h"

#include <vector>

using namespace std;

//класс простого изображения
class CPicture:public CIImage
{
 protected:
  long Width;//ширина
  long Height;//высота
  vector<unsigned long> vector_Image;//данные изображения
 public:
  CPicture(void);//конструктор
  ~CPicture();//деструктор
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//получить изображение в формате RGBA
  void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image);//задать изображение в формате RGBA
  void SetSize(unsigned long width,unsigned long height);//задать размер изображения и выделить память
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//задать точку
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//получить точку
};


#endif
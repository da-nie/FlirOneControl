#ifndef CIIMAGE_H
#define CIIMAGE_H

#include "stdafx.h"

#include <vector>

using namespace std;

//интерфейс к любому изображению
class CIImage
{
 public:
  virtual ~CIImage() {};//деструктор
  virtual void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)=0;//получить изображение в формате RGBA
  virtual void SetRGBAImage(const unsigned long &width,const unsigned long &height,const vector<unsigned long> &vector_image)=0;//задать изображение в формате RGBA
  virtual void SetSize(unsigned long width,unsigned long height)=0;//задать размер изображения и выделить память
  virtual void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)=0;//задать точку
  virtual unsigned long GetRGBAPixel(unsigned long x,unsigned long y)=0;//получить точку
};


#endif
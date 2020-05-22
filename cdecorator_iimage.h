#ifndef C_DECORATOR__I_IMAGE_H
#define C_DECORATOR__I_IMAGE_H

//****************************************************************************************************
//��������� ��� IImage
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "iimage.h"
#include <stdint.h>
#include <vector>

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************

//****************************************************************************************************
//��������� ��� IImage
//****************************************************************************************************
class CDecorator_IImage:public IImage
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  IImage *iImage_Ptr;
 public:
  //-�����������----------------------------------------------------------------------------------------
  CDecorator_IImage(IImage *iImage_Set_Ptr);
  //-����������-----------------------------------------------------------------------------------------
  ~CDecorator_IImage();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  void GetRGBAImage(uint32_t &width,uint32_t &height,std::vector<uint32_t> &vector_image);//�������� RGBA �����������
  void SetRGBAImage(const uint32_t &width,const uint32_t &height,const std::vector<uint32_t> &vector_image);//������ RGBA �����������
  void SetSize(uint32_t width,uint32_t height);//������ ������ ����������� � �������� ������
  void SetRGBAPixel(uint32_t x,uint32_t y,uint32_t color);//������ �����
  uint32_t GetRGBAPixel(uint32_t x,uint32_t y);//�������� �����
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif
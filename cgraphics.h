#ifndef C_GRAPHICS_H
#define C_GRAPHICS_H

//****************************************************************************************************
//����� ��������� � ������ �����������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <stdint.h>

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
//����� ��������� � ������ �����������
//****************************************************************************************************
class CGraphics
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  static const int32_t FONT_WIDTH=8;
  static const int32_t FONT_HEIGHT=14;
 private:
  //-����������-----------------------------------------------------------------------------------------
  uint32_t *VideoPtr;//��������� �� ���������� (RGBA)
  int32_t ScreenWidth;//������ �����������
  int32_t ScreenHeight;//������ �����������
  int32_t LineSize;//������ ������ �����������
 public:
  //-�����������----------------------------------------------------------------------------------------
  CGraphics(void);
  //-����������-----------------------------------------------------------------------------------------
  ~CGraphics();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  void Init(uint32_t *video_ptr,int32_t screen_width,int32_t screen_height);//�������������
  void PutSymbol(int32_t x,int32_t y,char symbol,uint32_t color);//����� ������� � �������
  void PutString(int32_t x,int32_t y,const char *string,uint32_t color);//����� ������� � �������
  void SolidFill(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t color);//��������� �������������
  void PutSymbolInversion(int32_t x,int32_t y,char symbol);//����� ������� � ������� � ��������� �����
  void PutStringInversion(int32_t x,int32_t y,const char *string);//����� ������� � ������� � ��������� �����
  void DrawPointInversion(int32_t x,int32_t y);//��������� ����� � ������� � ��������� �����
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif

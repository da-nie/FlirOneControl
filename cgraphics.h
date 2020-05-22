#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string.h>


#define FONT_WIDTH  8
#define FONT_HEIGHT 14

//����� ��������� � ������ �����������
class CGraphics
{
 protected:
  //-���������� ������-------------------------------------------------------  
  unsigned long *VideoPtr;//��������� �� ���������� (RGBA)
  unsigned long ScreenWidth;//������ �����������
  unsigned long ScreenHeight;//������ �����������
  unsigned long LineSize;//������ ������ �����������
 public:
  //-����������� ������------------------------------------------------------
  CGraphics(void);
  //-���������� ������-------------------------------------------------------
  ~CGraphics();
  //-������� ������----------------------------------------------------------
  void Init(unsigned long *video_ptr,unsigned long screen_width,unsigned long screen_height);//�������������
  void PutSymbol(unsigned long x,unsigned long y,char symbol,unsigned long color);//����� ������� � �������
  void PutString(unsigned long x,unsigned long y,const char *string,unsigned long color);//����� ������� � �������
  void SolidFill(unsigned long x,unsigned long y,unsigned long width,unsigned long height,unsigned long color);//��������� �������������
  void PutSymbolInversion(unsigned long x,unsigned long y,char symbol);//����� ������� � ������� � ��������� �����
  void PutStringInversion(unsigned long x,unsigned long y,const char *string);//����� ������� � ������� � ��������� �����
  void DrawPointInversion(unsigned long x,unsigned long y);//��������� ����� � ������� � ��������� �����

};
//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------

#endif

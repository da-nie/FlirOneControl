#ifndef CCOLORBUTTON_H
#define CCOLORBUTTON_H

#include "stdafx.h"

//������ ������ ����������� � �������� OwnerDraw!!!

//---------------------------------------------------------------------------
//����� ������� ������
//---------------------------------------------------------------------------
class CColorButton:public CButton
{
 protected:
  //-���������� ������-------------------------------------------------------
  //LOGFONT lf_Text;
  COLORREF Color_Text;//���� ������
  COLORREF Color_BackGround;//���� ������
  COLORREF Color_PressedText;//���� ����������� ������
  COLORREF Color_PressedBackGround;//���� ���������� ������
  int FrameSize;//������ �����
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CColorButton(void); 
  //-���������� ������-------------------------------------------------------
  ~CColorButton(void); 
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  BOOL Attach(int nID,CWnd* pParent);//������������ ������
  void SetBackGroundColor(COLORREF color);//������ ���� ������
  void SetPressedBackGroundColor(COLORREF color);//������ ���� ������� ������
  void SetTextColor(COLORREF color);//������ ���� ������ ������
  void SetPressedTextColor(COLORREF color);//������ ���� ������ ������� ������
  void SetFrameSize(int size);//������ ������ �����

  COLORREF GetBackGroundColor(void);//�������� ���� ������
  COLORREF GetPressedBackGroundColor(void);//�������� ���� ������� ������
  COLORREF GetTextColor(void);//�������� ���� ������ ������
  COLORREF GetPressedTextColor(void);//�������� ���� ������ ������� ������
  int GetFrameSize(void);//�������� ������ �����

 protected:
  void DrawItem(LPDRAWITEMSTRUCT lpDIS);
  void DrawFrame(CDC *DC,CRect R,int Inset);
  void DrawFilledRect(CDC *DC,CRect R,COLORREF color);
  void DrawLine(CDC *DC,CRect EndPoints,COLORREF color);
  void DrawLine(CDC *DC,long left,long top,long right,long bottom,COLORREF color);
  void DrawButtonText(CDC *DC,CRect R,const char *Buf,COLORREF TextColor);
  //-������� ��������� ��������� ������--------------------------------------
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CColorButton) 
};
#endif

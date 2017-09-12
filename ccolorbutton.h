#ifndef CCOLORBUTTON_H
#define CCOLORBUTTON_H

#include "stdafx.h"

//кнопка должна создаваться с галочкой OwnerDraw!!!

//---------------------------------------------------------------------------
//класс цветных кнопок
//---------------------------------------------------------------------------
class CColorButton:public CButton
{
 protected:
  //-Переменные класса-------------------------------------------------------
  //LOGFONT lf_Text;
  COLORREF Color_Text;//цвет текста
  COLORREF Color_BackGround;//цвет кнопки
  COLORREF Color_PressedText;//цвет выделенного текста
  COLORREF Color_PressedBackGround;//цвет выделенной кнопки
  int FrameSize;//размер рамки
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CColorButton(void); 
  //-Деструктор класса-------------------------------------------------------
  ~CColorButton(void); 
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  BOOL Attach(int nID,CWnd* pParent);//присоединить кнопку
  void SetBackGroundColor(COLORREF color);//задать цвет кнопки
  void SetPressedBackGroundColor(COLORREF color);//задать цвет нажатой кнопки
  void SetTextColor(COLORREF color);//задать цвет текста кнопки
  void SetPressedTextColor(COLORREF color);//задать цвет текста нажатой кнопки
  void SetFrameSize(int size);//задать ширину рамки

  COLORREF GetBackGroundColor(void);//получить цвет кнопки
  COLORREF GetPressedBackGroundColor(void);//получить цвет нажатой кнопки
  COLORREF GetTextColor(void);//получить цвет текста кнопки
  COLORREF GetPressedTextColor(void);//получить цвет текста нажатой кнопки
  int GetFrameSize(void);//получить ширину рамки

 protected:
  void DrawItem(LPDRAWITEMSTRUCT lpDIS);
  void DrawFrame(CDC *DC,CRect R,int Inset);
  void DrawFilledRect(CDC *DC,CRect R,COLORREF color);
  void DrawLine(CDC *DC,CRect EndPoints,COLORREF color);
  void DrawLine(CDC *DC,long left,long top,long right,long bottom,COLORREF color);
  void DrawButtonText(CDC *DC,CRect R,const char *Buf,COLORREF TextColor);
  //-Функции обработки сообщений класса--------------------------------------
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CColorButton) 
};
#endif

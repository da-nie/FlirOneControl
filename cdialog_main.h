#ifndef CDIALOG_MAIN_H
#define CDIALOG_MAIN_H

#include "stdafx.h"
#include "cflironecontrol.h"
#include "ccolorbutton.h"
#include "cgraphics.h"
#include <vector>
#include <string>

#include "cpicture.h"
#include "cdecorator_scale.h"

using namespace std;

//размер области измерения температуры
#define FRAME_SIZE 3

#define IMAGE_VIEW_WIDTH (IMAGE_WIDTH+4+FONT_WIDTH*7)
#define IMAGE_VIEW_HEIGHT (IMAGE_HEIGHT)

class CDialog_Main:public CDialog
{
 protected:
  //-Переменные класса-------------------------------------------------------
  unsigned long *ViewImagePtr;//изображение, выводимое на экран
  unsigned long *SaveViewImagePtr;//изображение, сохраняемое в файл (если сохранается полная картинка со шкалой)

  unsigned long *ColorImagePtr;//раскрашенное изображение
  unsigned short *ThermalImagePtr;//тепловое изображение
  unsigned long *VideoImagePtr;//изображение с видеокамеры
  float *TemperatureImagePtr;//температурное изображение

  unsigned long LastReceivedFrameIndex;//индекс последнего принятого изображения

  CFlirOneControl cFlirOneControl;//класс управления устройством Flir One Get 2 
  CGraphics cGraphics;//класс для рисования на изображении

  vector<string> vector_PaletteFileName;//список палитр

  CIImage *cIImage_VideoPtr;//выводимый на экран кадр изображения с видеокамеры
  CIImage *cIImage_ViewPtr;//выводимое на экран изображение

  //параметры для расчёта температуры
  double PlanckR1;
  double PlanckB;
  double PlanckF;
  double PlanckO;
  double PlanckR2;

  double TempReflected;//температура болометров
  double Emissivity;//коэффициент излучения

  CColorButton cColorButton_SaveAllFrame;//кнопка "сохранять все кадры"
  bool SaveAllFrame;//сохранять ли все кадры

  CColorButton cColorButton_SaveImageNoScale;//кнопка "сохранять изображение без шкалы"
  bool SaveImageNoScale;//сохранять без шкалы

  CColorButton cColorButton_SaveImageCross;//кнопка "рисовать перекрестье"
  bool SaveImageCross;//рисовать перекрестье

  CColorButton cColorButton_SaveRAW;//кнопка "сохранять RAW"
  bool SaveRAW;//сохранять RAW

  CColorButton cColorButton_SaveVideo;//кнопка "сохранять кадры с видеокамеры"
  bool SaveVideo;//сохранять картинку с видеокамеры


  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CDialog_Main(LPCTSTR lpszTemplateName,CWnd* pParentWnd);
  //-Деструктор класса-------------------------------------------------------
  ~CDialog_Main();
  //-Переменные класса-------------------------------------------------------
  //-Замещённые функции предка-----------------------------------------------
  afx_msg void OnOK(void);
  afx_msg void OnCancel(void);
  afx_msg void OnClose(void);
  afx_msg BOOL OnInitDialog(void);
  //-Новые функции класса----------------------------------------------------
  bool GetTemperature(unsigned short raw14,double &value);//вычислить температуру
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnPaint(void);  
  afx_msg void OnButton_SaveFrame(void);//сохранить кадр
  afx_msg void OnButton_SaveAllFrame(void);//сохранять все кадры
  afx_msg void OnButton_SaveImageNoScale(void);//сохранять изображение без шкалы
  afx_msg void OnButton_SaveImageCross(void);//рисовать перекрестье
  afx_msg void OnButton_SaveRAW(void);//сохранять RAW
  afx_msg void OnButton_SaveVideo(void);//сохранять кадры с видеокамеры
  afx_msg void OnButton_ApplyPalette(void);//применить палитру
 protected:
  //-Новые функции класса----------------------------------------------------  
  void SaveThermalImage(void);//сохранить тепловое изображение
  void SaveColorImage(void);//сохранить раскрашенное изображение
  void SaveVideoImage(void);//сохранить кадр с видеокамеры
  //-Прочее------------------------------------------------------------------ 
};



#endif
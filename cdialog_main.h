#ifndef CDIALOG_MAIN_H
#define CDIALOG_MAIN_H

#include "stdafx.h"
#include "cflironecontrol.h"
#include "ccolorbutton.h"
#include "cgraphics.h"
#include <vector>
#include <string>
#include <stdint.h>

#include "cpicture.h"
#include "cdecorator_scale.h"

class CDialog_Main:public CDialog
{
 private:
  //-перечисления---------------------------------------------------------------------------------------
  //-структуры------------------------------------------------------------------------------------------
  //-константы------------------------------------------------------------------------------------------
  static const uint32_t TIMER_INTERVAL_MS=100;//интервал срабатывания таймера, мс
  static const int32_t FRAME_SIZE=3;//размер области измерения температуры
  static const int32_t IMAGE_VIEW_WIDTH=(CFlirOneReceiver::IMAGE_WIDTH+4+CGraphics::FONT_WIDTH*7);
  static const int32_t IMAGE_VIEW_HEIGHT=(CFlirOneReceiver::IMAGE_HEIGHT);
  static const size_t PATH_STRING_BUFFER_SIZE=1024;//размер строки для пути
  static const size_t STRING_BUFFER_SIZE=255;//размер строки

  static const uint32_t COLOR_WHITE=0xFFFFFFFF;//белый цвет
  static const uint32_t COLOR_BLACK=0xFF000000;//чёрный цвет

  static const uint32_t ALARM_MAX_TEMPER_COUNTER_MAX_VALUE=5;//максимальное значение количества кадров с температурой выше максимума до срабатывания сигнала
  static const uint32_t ALARM_MIN_TEMPER_COUNTER_MAX_VALUE=5;//максимальное значение количества кадров с температурой ниже минимума до срабатывания сигнала

 protected:
  //-Переменные класса-------------------------------------------------------
 private:
  //-переменные-----------------------------------------------------------------------------------------
  CFlirOneControl cFlirOneControl;//класс управления тепловизором Flir One

  uint32_t AlarmMaxTemperCounter;//счётчик количества кадров с температурой выше максимума до срабатывания сигнала
  uint32_t AlarmMinTemperCounter;//счётчик количества кадров в температурой ниже минимума до срабатывания сигнала

  std::vector<uint32_t> ViewImage;//изображение, выводимое на экран
  std::vector<uint32_t> SaveViewImage;//изображение, сохраняемое в файл (если сохранается полная картинка со шкалой)

  std::vector<uint32_t> ColorImage;//раскрашенное изображение
  std::vector<uint16_t> ThermalImage;//тепловое изображение
  std::vector<uint32_t> VideoImage;//изображение с видеокамеры
  std::vector<float> TemperatureImage;//температурное изображение

  uint32_t LastReceivedFrameIndex;//индекс последнего принятого изображения

  CGraphics cGraphics;//класс для рисования на изображении

  std::vector<std::string> vector_PaletteFileName;//список палитр

  IImage *iImage_VideoPtr;//выводимый на экран кадр изображения с видеокамеры
  IImage *iImage_ViewPtr;//выводимое на экран изображение

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

  CColorButton cColorButton_ShowVideo;//кнопка "показывать видео"
  bool ShowVideo;//показывать видео

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
  void ChangeState(bool &state,CColorButton &cColorButton);//смена состояния кнопки и значения флага
  void SetState(bool &state,CColorButton &cColorButton,bool set_state);//задать состояние кнопки и флага
  bool GetTemperature(unsigned short raw14,double &value);//вычислить температуру
  void NewFrame(void);//новый кадр
  bool MinTemperControl(float t);//контроль минимальной температуры
  bool MaxTemperControl(float t);//контроль максимальной температуры
  void FindPalette(void);//поиск палитр
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
  afx_msg void OnButton_ShowVideo(void);//показывать видео
  afx_msg void OnButton_ApplyPalette(void);//применить палитру
 protected:
  //-Новые функции класса----------------------------------------------------  
  void SaveThermalImage(void);//сохранить тепловое изображение
  void SaveColorImage(void);//сохранить раскрашенное изображение
  void SaveVideoImage(void);//сохранить кадр с видеокамеры
  //-Прочее------------------------------------------------------------------ 
};



#endif
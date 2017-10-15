#ifndef CFLIRONECONTROL_H
#define CFLIRONECONTROL_H


#include "stdafx.h"

#include <setupapi.h>
#include <basetyps.h>
#include <assert.h>

#include "tga.h"
#include "cflironereceiver.h"
#include "cflironedriver.h"
#include "craiiccriticalsection.h"

//бесконечное малое
#define EPS 0.00000001

//класс управления Flir One
class CFlirOneControl
{
 friend UINT ProcessingThread(LPVOID pParam);//поток обработки
 protected:
  //-Переменные класса-------------------------------------------------------    
  long Delay;//пауза для работы потока

  struct SProtected
  {
   CCriticalSection cCriticalSection;//критическая секция для доступа к данным
   CFlirOneReceiver cFlirOneReceiver;//класс приёма данных от Flir One
  } sProtected;//защищённые переменные

  CFlirOneDriver cFlirOneDriver;//класс работы с драйверами Flir One

  CEvent cEvent_Exit;//событие завершения потока
  CWinThread *cWinThread_ProcessingThread;//поток обработки  
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CFlirOneControl(void);
  //-Деструктор класса-------------------------------------------------------
  ~CFlirOneControl();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  bool Open(void);//подключиться к устройству
  void Close(void);//отключиться от устройства
  bool LoadColorMap(char *filename);//загрузить карту перекодировки изображения
  bool CopyColorImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//скопировать раскрашенное изображение в буфер
  bool CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index);//скопировать тепловое изображение в буфер
  bool CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//скопировать изображение с видеокамеры в буфер
  bool CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size);//скопировать палитру
  void SetShowVideo(bool state);//показывать ли видео 
 protected:
  bool Processing(void);//обработка (вызывается только потоком)
  void AddLog(char *string);//добавить строку в log-файл
  void AddLog(long value);//добавить число в log-файл
  void StartThread(void);//запустить поток
  void StopThread(void);//остановить поток
};

#endif

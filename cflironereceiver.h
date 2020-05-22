#ifndef CFLIRONERECEIVER_H
#define CFLIRONERECEIVER_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//подключаемые библиотеки
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "cringbuffer.h"
#include "stdafx.h"

#include <vector>

using namespace std;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//макроопределения
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//размер буфера приёма данных
#define IMAGE_BUFFER_SIZE 1048576

//исходные размеры изображения (не перевёрнутые)
#define ORIGINAL_IMAGE_WIDTH 160
#define ORIGINAL_IMAGE_HEIGHT 120

//исходные размеры видео (не перевёрнутые)
#define ORIGINAL_VIDEO_WIDTH 640
#define ORIGINAL_VIDEO_HEIGHT 480

//размеры изображения после обработки и переворота в вертикальное положение
#define IMAGE_WIDTH 120
#define IMAGE_HEIGHT 160

//размеры видео после обработки
#define VIDEO_WIDTH 480
#define VIDEO_HEIGHT 640

//размер изображений
#define THERMAL_IMAGE_SIZE_SHORT (IMAGE_WIDTH*IMAGE_HEIGHT)
#define COLOR_IMAGE_SIZE_LONG (IMAGE_WIDTH*IMAGE_HEIGHT)
#define VIDEO_IMAGE_SIZE_LONG (VIDEO_WIDTH*VIDEO_HEIGHT)

//класс приёма данных от Flir One
class CFlirOneReceiver
{
 private:
  //-Переменные класса-------------------------------------------------------    
  //заголовок кадра тепловизора
  struct SHeader
  {
   unsigned char MagicByte[4];//магические байты
   unsigned char Unknow1[4];//неизвестные данные
   unsigned long FrameSize;//полный размер кадра
   unsigned long ThermalSize;//размер кадра тепловизора
   unsigned long JpgSize;//размер изображения с камеры
   unsigned long StatusSize;//размер слова состояния 	 
   unsigned char Unknow2[4];//неизвестные данные
  };
  #pragma pack()

  //палитра перекодировки изображения
  unsigned char ColorMap_R[256];
  unsigned char ColorMap_G[256];
  unsigned char ColorMap_B[256];

  CRingBuffer *cRingBuffer_Ptr;//кольцевой буфер приёма данных

  SHeader sHeader;//заголовок пакета
  bool HeaderIsReceived;//заголовок принят

  unsigned char MagicPos;//позиция анализируемого магического байта
  unsigned long MagicHeadPos;//позиция головы, когда встречился первый магический байт

  unsigned short *ThermalImage;//тепловое изображение (raw14)
  unsigned long *VideoImage;//изображение с видеокамеры
  unsigned long *ColorImage;//раскрашенное изображение

  vector<unsigned char> JPGImage;//прямые данные с видеокамеры (картинка в jpg)

  long FrameIndex;//номер текущего кадра

  bool ShowVideo;//показывать ли видео
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CFlirOneReceiver(void);
  //-Деструктор класса-------------------------------------------------------
  ~CFlirOneReceiver();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  bool LoadColorMap(char *filename);//загрузить карту перекодировки изображения
  bool CopyColorImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//скопировать раскрашенное изображение в буфер
  bool CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index);//скопировать тепловое изображение в буфер
  bool CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//скопировать изображение с видеокамеры в буфер
  bool CopyJPGImage(vector<unsigned char> &vector_jpg,unsigned long &index);//скопировать данные с видеокамеры в буфер
  bool CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size);//скопировать палитру
  bool CreateImage(unsigned char *buffer,unsigned long size);//создать изображение
  void SetShowVideo(bool state);//показывать ли видео
  //-Прочее------------------------------------------------------------------
 private:
  void CalculateCRC(unsigned short &crc,unsigned char byte);//вычислить crc
};
#endif

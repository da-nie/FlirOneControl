#ifndef CFLIRONEDRIVER_H
#define CFLIRONEDRIVER_H

#include "stdafx.h"

#include <setupapi.h>
#include <basetyps.h>
#include <assert.h>

//максимальная длина имени драйвера
#define MAX_DEVPATH_LENGTH 256

//команды управления для iAP
//остановить устройство Frame
#define IOCTL_FLIR_ONE_GEN2_FRAME_STOP		1
//запустить устройство Frame
#define IOCTL_FLIR_ONE_GEN2_FRAME_START		2
//остановить устройство FileIO
#define IOCTL_FLIR_ONE_GEN2_FILEIO_STOP		4
//запустить устройство FileIO
#define IOCTL_FLIR_ONE_GEN2_FILEIO_START	8

//размер буфера приёма данных
#define RECEIVE_BUFFER_SIZE 1024*1024;

//класс работы с драйверами Flir One
class CFlirOneDriver
{
 protected:
  //-Переменные класса-------------------------------------------------------    
  char *Data;//буфер приёма данных
  unsigned long SizeOfData;//размер буфера приёма данных


  HANDLE hFile_FileIO;//интерфейс FileIO
  HANDLE hFile_Frame;//интерфейс Frame
  HANDLE hFile_iAP;//интерфейс iAP

  OVERLAPPED Overlapped_FileIO;//для асинхронного чтения FileIO
  OVERLAPPED Overlapped_Frame;//для асинхронного чтения Frame
  OVERLAPPED Overlapped_iAP;//для асинхронного чтения iAP

  bool Start_FileIO;
  bool Start_Frame;
  bool Start_iAP;

  long Delay;//пауза для работы потока
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CFlirOneDriver(void);
  //-Деструктор класса-------------------------------------------------------
  ~CFlirOneDriver();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  bool Open(void);//подключиться к устройству
  void Close(void);//отключиться от устройства
  bool Processing(void);//обработка (вызывается только потоком)
  bool SendControlCommand(unsigned long code);//отправить команду управления
  bool ReadStream(unsigned char* &ptr,unsigned long &size);//чтение данных из устройств 
 private:
  bool GetDevicePath(IN LPGUID InterfaceGuid,PCHAR DevicePath,size_t BufLen);//найти драйвер по GUID


};

#endif

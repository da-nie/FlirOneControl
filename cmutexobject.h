#ifndef C_MUTEX_OBJECT_H
#define C_MUTEX_OBJECT_H

//****************************************************************************************************
//Класс работы с мютексом
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "stdafx.h"

//****************************************************************************************************
//макроопределения
//****************************************************************************************************

//****************************************************************************************************
//Класс работы с мютексом
//****************************************************************************************************

class CMutexObject
{
 private:
  CRITICAL_SECTION CriticalSection;
 public:
  //конструктор
  CMutexObject(void);
  //конструктор копирования
  CMutexObject(const CMutexObject& cMutexObject);
  //деструктор
  ~CMutexObject();
 public:
  void Lock(void);//заблокировать мютекс
  void Unlock(void);//разблокировать мютекс
};

#endif



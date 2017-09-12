#ifndef CRAII_CCRITICAL_SECTION_H
#define CRAII_CCRITICAL_SECTION_H

//====================================================================================================
//описание
//====================================================================================================

//Класс RAII для работы с критической секцией

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "stdafx.h"

//====================================================================================================
//класс RAII критической секции
//====================================================================================================
class CRAIICCriticalSection
{
 private:
  CCriticalSection *cCriticalSection_Ptr;//указатель на критическую секцию
  bool Locked;//состояние критической секции
 public:
  CRAIICCriticalSection(CCriticalSection *cCriticalSection_Set_Ptr)//конструктор
  {
   Locked=false;
   if (cCriticalSection_Set_Ptr==NULL) return;
   cCriticalSection_Ptr=cCriticalSection_Set_Ptr;   
   Lock();
  }
  ~CRAIICCriticalSection()
  {
   if (cCriticalSection_Ptr==NULL) return;
   Unlock();
  }
  void Lock(void)
  {
   if (cCriticalSection_Ptr==NULL) return;
   if (Locked==true) return;
   cCriticalSection_Ptr->Lock();
   Locked=true;
  }
  void Unlock(void)
  {
   if (cCriticalSection_Ptr==NULL) return;
   if (Locked==false) return;
   cCriticalSection_Ptr->Unlock();
   Locked=false;
  }
};

#endif
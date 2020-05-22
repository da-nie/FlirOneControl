#ifndef C_RAII_CMUTEX_H
#define C_RAII_CMUTEX_H

//****************************************************************************************************
//��������
//****************************************************************************************************

//����� RAII ��� ������ � ��������

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************

#include "cmutexobject.h"

//****************************************************************************************************
//����� RAII ����������� ������
//****************************************************************************************************
class CRAIICMutex
{
 private:
  CMutexObject *cMutex_Ptr;//��������� �� ������
  bool Locked;//��������� �������
 public:
  CRAIICMutex(CMutexObject *cMutex_Set_Ptr)//�����������
  {
   Locked=false;
   if (cMutex_Set_Ptr==NULL) return;
   cMutex_Ptr=cMutex_Set_Ptr;

   Lock();
  }
  ~CRAIICMutex()
  {
   if (cMutex_Ptr==NULL) return;
   Unlock();
  }
  void Lock(void)
  {
   if (cMutex_Ptr==NULL) return;
   if (Locked==true) return;
   cMutex_Ptr->Lock();
   Locked=true;
  }
  void Unlock(void)
  {
   if (cMutex_Ptr==NULL) return;
   if (Locked==false) return;
   cMutex_Ptr->Unlock();
   Locked=false;
  }
};

#endif

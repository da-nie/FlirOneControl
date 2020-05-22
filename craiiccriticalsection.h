#ifndef CRAII_CCRITICAL_SECTION_H
#define CRAII_CCRITICAL_SECTION_H

//====================================================================================================
//��������
//====================================================================================================

//����� RAII ��� ������ � ����������� �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "stdafx.h"

//====================================================================================================
//����� RAII ����������� ������
//====================================================================================================
class CRAIICCriticalSection
{
 private:
  CCriticalSection *cCriticalSection_Ptr;//��������� �� ����������� ������
  bool Locked;//��������� ����������� ������
 public:
  CRAIICCriticalSection(CCriticalSection *cCriticalSection_Set_Ptr)//�����������
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
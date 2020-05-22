#include "cflironecontrol.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-����������� ������---------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneControl::CFlirOneControl(void)
{ 
 Delay=1;
 cWinThread_ProcessingThread=NULL;
 //��������� ����� ���������
 StartThread();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-���������� ������----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneControl::~CFlirOneControl()
{
 StopThread();
 Close();
}
//-������� ������------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ������� ������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//�������� ������ � log-����
//----------------------------------------------------------------------------------------------------
void CFlirOneControl::AddLog(char *string)
{
 return;
 FILE *file=fopen("log.txt","ab");
 if (file==NULL) return;
 fprintf(file,"%s",string);
 fclose(file);
}
//----------------------------------------------------------------------------------------------------
//�������� ����� � log-����
//----------------------------------------------------------------------------------------------------
void CFlirOneControl::AddLog(long value)
{
 return;
 FILE *file=fopen("log.txt","ab");
 if (file==NULL) return;
 fprintf(file,"%i",value);
 fclose(file);
}

//---------------------------------------------------------------------------
//��������� �����
//---------------------------------------------------------------------------
void CFlirOneControl::StartThread(void)
{
 StopThread();
 //��������� ����� ���������
 cEvent_Exit.ResetEvent();
 cWinThread_ProcessingThread=AfxBeginThread((AFX_THREADPROC)ProcessingThread,this);
 cWinThread_ProcessingThread->m_bAutoDelete=FALSE;
 SetThreadPriority(cWinThread_ProcessingThread->m_hThread,THREAD_PRIORITY_TIME_CRITICAL);
}
//---------------------------------------------------------------------------
//���������� �����
//---------------------------------------------------------------------------
void CFlirOneControl::StopThread(void)
{
 //��������� ����� ���������
 if (cWinThread_ProcessingThread!=NULL)
 {
  cEvent_Exit.SetEvent();
  if (cWinThread_ProcessingThread->m_hThread) WaitForSingleObject(cWinThread_ProcessingThread->m_hThread,INFINITE);
  delete(cWinThread_ProcessingThread);
 }
 cWinThread_ProcessingThread=NULL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ������� ������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ � ����������
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::Open(void)
{
 Close();
 return(cFlirOneDriver.Open());
}
//----------------------------------------------------------------------------------------------------
//����������� �� ����������
//----------------------------------------------------------------------------------------------------
void CFlirOneControl::Close(void)
{
 cFlirOneDriver.Close();
}
//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::Processing(void)
{
 if (WaitForSingleObject(cEvent_Exit.m_hObject,Delay)==WAIT_OBJECT_0) return(false);//��� �������
 Delay=1;
 unsigned char *ptr;
 unsigned long size;
 if (cFlirOneDriver.ReadStream(ptr,size)==false)
 {
  Open();
  Delay=1000;
 }
 else
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
  {
   if (size>0)
   {
    sProtected.cFlirOneReceiver.CreateImage(ptr,size);
   }
  }
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//��������� ����� ������������� �����������
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::LoadColorMap(char *filename)
{ 	
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.LoadColorMap(filename));
 }
}

//----------------------------------------------------------------------------------------------------
//����������� ������������ ����������� � �����
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyColorImage(unsigned long *image_ptr,unsigned long size,unsigned long &index)
{ 
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyColorImage(image_ptr,size,index));
 }
}
//----------------------------------------------------------------------------------------------------
//����������� �������� ����������� � �����
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index)
{ 
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyThermalImage(image_ptr,size,index));
 }
}
//----------------------------------------------------------------------------------------------------
//����������� ����������� � ����������� � �����
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyVideoImage(image_ptr,size,index));
 }
}
//---------------------------------------------------------------------------
//����������� �������
//---------------------------------------------------------------------------
bool CFlirOneControl::CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyColorMap(R,G,B,size));
 }
}
//---------------------------------------------------------------------------
//���������� �� �����
//---------------------------------------------------------------------------
void CFlirOneControl::SetShowVideo(bool state)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.cFlirOneReceiver.SetShowVideo(state);
 }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
//����� ���������
//---------------------------------------------------------------------------
UINT ProcessingThread(LPVOID pParam)
{
 timeBeginPeriod(1); 
 CFlirOneControl *cFlirOneControl_Ptr=reinterpret_cast<CFlirOneControl *>(pParam); 
 while(1)
 {  
  if (cFlirOneControl_Ptr->Processing()==false) break;
 }
 timeEndPeriod(1);
 return(0);
}



//-������--------------------------------------------------------------------


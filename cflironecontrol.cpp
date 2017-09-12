#include "cflironecontrol.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-Конструктор класса---------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneControl::CFlirOneControl(void)
{ 
 Delay=1;
 cWinThread_ProcessingThread=NULL;
 //запускаем поток обработки
 StartThread();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-Деструктор класса----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneControl::~CFlirOneControl()
{
 StopThread();
 Close();
}
//-Функции класса------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//закрытые функции класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//добавить строку в log-файл
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
//добавить число в log-файл
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
//запустить поток
//---------------------------------------------------------------------------
void CFlirOneControl::StartThread(void)
{
 StopThread();
 //запускаем поток обработки
 cEvent_Exit.ResetEvent();
 cWinThread_ProcessingThread=AfxBeginThread((AFX_THREADPROC)ProcessingThread,this);
 cWinThread_ProcessingThread->m_bAutoDelete=FALSE;
 SetThreadPriority(cWinThread_ProcessingThread->m_hThread,THREAD_PRIORITY_TIME_CRITICAL);
}
//---------------------------------------------------------------------------
//остановить поток
//---------------------------------------------------------------------------
void CFlirOneControl::StopThread(void)
{
 //завершаем поток обработки
 if (cWinThread_ProcessingThread!=NULL)
 {
  cEvent_Exit.SetEvent();
  if (cWinThread_ProcessingThread->m_hThread) WaitForSingleObject(cWinThread_ProcessingThread->m_hThread,INFINITE);
  delete(cWinThread_ProcessingThread);
 }
 cWinThread_ProcessingThread=NULL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//открытые функции класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключиться к устройству
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::Open(void)
{
 Close();
 return(cFlirOneDriver.Open());
}
//----------------------------------------------------------------------------------------------------
//отключиться от устройства
//----------------------------------------------------------------------------------------------------
void CFlirOneControl::Close(void)
{
 cFlirOneDriver.Close();
}
//----------------------------------------------------------------------------------------------------
//обработка
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::Processing(void)
{
 if (WaitForSingleObject(cEvent_Exit.m_hObject,Delay)==WAIT_OBJECT_0) return(false);//ждём события
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
//загрузить карту перекодировки изображения
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::LoadColorMap(char *filename)
{ 	
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.LoadColorMap(filename));
 }
}

//----------------------------------------------------------------------------------------------------
//скопировать раскрашенное изображение в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyColorImage(unsigned long *image_ptr,unsigned long size,unsigned long &index)
{ 
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyColorImage(image_ptr,size,index));
 }
}
//----------------------------------------------------------------------------------------------------
//скопировать тепловое изображение в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index)
{ 
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyThermalImage(image_ptr,size,index));
 }
}
//----------------------------------------------------------------------------------------------------
//скопировать изображение с видеокамеры в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneControl::CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyVideoImage(image_ptr,size,index));
 }
}
//---------------------------------------------------------------------------
//скопировать палитру
//---------------------------------------------------------------------------
bool CFlirOneControl::CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.cFlirOneReceiver.CopyColorMap(R,G,B,size));
 }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
//поток обработки
//---------------------------------------------------------------------------
UINT ProcessingThread(LPVOID pParam)
{
 unsigned long delay=0;
 timeBeginPeriod(1); 
 CFlirOneControl *cFlirOneControl_Ptr=reinterpret_cast<CFlirOneControl *>(pParam); 
 while(1)
 {  
  if (cFlirOneControl_Ptr->Processing()==false) break;
 }
 timeEndPeriod(1);
 return(0);
}



//-Прочее--------------------------------------------------------------------


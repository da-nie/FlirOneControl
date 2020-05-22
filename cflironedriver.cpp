#include "cflironedriver.h"
#include <initguid.h>
//GUID устройств
DEFINE_GUID(GUID_DEVINTERFACE_FLIRONEGEN2_IAP,0x41a83025, 0x9dc8, 0x4e75, 0x87, 0x5, 0x12, 0x9, 0x22, 0x2f, 0xdc, 0x17);
DEFINE_GUID(GUID_DEVINTERFACE_FLIRONEGEN2_FRAME,0x1b2e6756, 0x344, 0x449e, 0x8b, 0x5b, 0xbd, 0x57, 0xf5, 0x82, 0x76, 0x6b);
DEFINE_GUID(GUID_DEVINTERFACE_FLIRONEGEN2_FILEIO,0xaa0dfc0c, 0xe043, 0x4778, 0x9d, 0xba, 0x28, 0x74, 0x9a, 0x3a, 0xa9, 0xf5);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-Конструктор класса---------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneDriver::CFlirOneDriver(void)
{ 
 hFile_FileIO=INVALID_HANDLE_VALUE;
 hFile_Frame=INVALID_HANDLE_VALUE;
 hFile_iAP=INVALID_HANDLE_VALUE;

 Overlapped_Frame.Internal=0;
 Overlapped_Frame.InternalHigh=0;
 Overlapped_Frame.Offset=0;
 Overlapped_Frame.OffsetHigh=0;
 Overlapped_Frame.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

 Overlapped_FileIO.Internal=0;
 Overlapped_FileIO.InternalHigh=0;
 Overlapped_FileIO.Offset=0;
 Overlapped_FileIO.OffsetHigh=0;
 Overlapped_FileIO.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

 Overlapped_iAP.Internal=0;
 Overlapped_iAP.InternalHigh=0;
 Overlapped_iAP.Offset=0;
 Overlapped_iAP.OffsetHigh=0;
 Overlapped_iAP.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

 Start_FileIO=true;
 Start_Frame=true;
 Start_iAP=true;

 Data.resize(RECEIVE_BUFFER_SIZE);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-Деструктор класса----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneDriver::~CFlirOneDriver()
{
 DeleteObject(Overlapped_Frame.hEvent);
 DeleteObject(Overlapped_FileIO.hEvent);
 DeleteObject(Overlapped_iAP.hEvent);
}
//-Функции класса------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//закрытые функции класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//найти драйвер по GUID
//----------------------------------------------------------------------------------------------------
bool CFlirOneDriver::GetDevicePath(IN LPGUID InterfaceGuid,PCHAR DevicePath,size_t BufLen)
{
 HDEVINFO HardwareDeviceInfo;
 SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
 PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData=NULL;
 ULONG Length,RequiredLength=0;
 BOOL bResult;

 HardwareDeviceInfo=SetupDiGetClassDevs(InterfaceGuid,NULL,NULL,(DIGCF_PRESENT|DIGCF_DEVICEINTERFACE));
 if (HardwareDeviceInfo==INVALID_HANDLE_VALUE) 
 {
  //AddLog("[ОШИБКА] Ошибка выполнения SetupDiGetClassDevs!\r\n");
  return(false);
 }

 DeviceInterfaceData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);
 bResult=SetupDiEnumDeviceInterfaces(HardwareDeviceInfo,0,InterfaceGuid,0,&DeviceInterfaceData);
 if (bResult==FALSE) 
 {
  //AddLog("[ОШИБКА] Ошибка выполнения SetupDiEnumDeviceInterfaces!\r\n");
  SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
  return(false);
 }
 SetupDiGetDeviceInterfaceDetail(HardwareDeviceInfo,&DeviceInterfaceData,NULL,0,&RequiredLength,NULL);
 DeviceInterfaceDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)
 LocalAlloc(LMEM_FIXED,RequiredLength);

 if (DeviceInterfaceDetailData==NULL) 
 {
  SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
  //AddLog("[ОШИБКА] Ошибка выделения памяти!\r\n");
  return(false);
 }
 DeviceInterfaceDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
 Length=RequiredLength;

 bResult=SetupDiGetDeviceInterfaceDetail(HardwareDeviceInfo,&DeviceInterfaceData,DeviceInterfaceDetailData,Length,&RequiredLength,NULL);
 if (bResult==FALSE) 
 {
  SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
  //AddLog("[ОШИБКА] Ошибка выполнения SetupDiGetDeviceInterfaceDetail!\r\n");
  return(false);
 }
 if (strlen(DeviceInterfaceDetailData->DevicePath)>=BufLen-1)
 {
  SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
  LocalFree(DeviceInterfaceDetailData);
  return(false);
 }
 strcpy(DevicePath,DeviceInterfaceDetailData->DevicePath);
 SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
 LocalFree(DeviceInterfaceDetailData);
 return(true);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//открытые функции класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//отправить команду управления
//----------------------------------------------------------------------------------------------------
bool CFlirOneDriver::SendControlCommand(uint32_t code)
{
 DWORD nBytes;
 char buffer[256];
 uint32_t size=sizeof(buffer);  
 if (hFile_iAP==INVALID_HANDLE_VALUE) return(false);
 if (DeviceIoControl(hFile_iAP,code,buffer,size,buffer,size,(PULONG)&nBytes,NULL)==FALSE)
 {
  if (GetLastError()!=ERROR_IO_PENDING) 
  {
   /*
   AddLog("[ОШИБКА] Ошибка выполнения DeviceIoControl с кодом ");
   AddLog(code);
   AddLog("\r\n");
   */
   Close();
   return(false);  
  }
 }
 else return(true);
 if (WaitForSingleObject(Overlapped_iAP.hEvent,500)==WAIT_OBJECT_0)//команда выполнена
 {
  //AddLog("Event iAP!\r\n");
  if (GetOverlappedResult(hFile_iAP,&Overlapped_iAP,&nBytes,TRUE)==TRUE) return(true);
 }
 return(true);//не дождались
}

//----------------------------------------------------------------------------------------------------
//чтение данных из устройств
//----------------------------------------------------------------------------------------------------
bool CFlirOneDriver::ReadStream(std::vector<uint8_t> &ret)
{
 static const size_t BUFFER_SIZE=10000;
 static uint8_t Buffer[BUFFER_SIZE];

 ret.clear();
 if (hFile_FileIO==INVALID_HANDLE_VALUE) return(false);
 if (hFile_Frame==INVALID_HANDLE_VALUE) return(false);
 if (hFile_iAP==INVALID_HANDLE_VALUE) return(false);
 DWORD readen; 
 //ждём прихода данных
 if (Start_FileIO==true)//запускаем чтение
 {
  if (ReadFile(hFile_FileIO,Buffer,BUFFER_SIZE,&readen,&Overlapped_FileIO)==FALSE)//ошибка
  {   
   if (GetLastError()!=ERROR_IO_PENDING) 
   {
    //AddLog("FileIO read error!\r\n");
	return(false);
   }
  }
  Start_FileIO=false;
 }
 if (Start_FileIO==false)//ждём результата чтения
 {
  if (WaitForSingleObject(Overlapped_FileIO.hEvent,0)==WAIT_OBJECT_0)//пришли данные
  {
   //AddLog("Event FileIO!\r\n");
   if (GetOverlappedResult(hFile_FileIO,&Overlapped_FileIO,&readen,TRUE)==TRUE)
   {
    Start_FileIO=true;//запускаем чтение сначала
	//AddLog("FileIO data!\r\n");
   }
   else return(false);
  }
 }

 if (Start_iAP==true)//запускаем чтение
 {
  if (ReadFile(hFile_iAP,Buffer,BUFFER_SIZE,&readen,&Overlapped_iAP)==FALSE)//ошибка
  {
   if (GetLastError()!=ERROR_IO_PENDING) 
   {
    //AddLog("iAP read error!\r\n");
    return(false);
   }
  }
  Start_iAP=false;
 }
 if (Start_iAP==false)//ждём результата чтения
 {
  if (WaitForSingleObject(Overlapped_iAP.hEvent,0)==WAIT_OBJECT_0)//пришли данные
  {
   //AddLog("Event iAP!\r\n");
   if (GetOverlappedResult(hFile_iAP,&Overlapped_iAP,&readen,TRUE)==TRUE)
   {
    Start_iAP=true;//запускаем чтение сначала
	//AddLog("iAP data!\r\n");
   }
   else return(false);
  }
 }

 if (Start_Frame==true)//запускаем чтение
 {
  if (ReadFile(hFile_Frame,&Data[0],Data.size(),&readen,&Overlapped_Frame)==FALSE)//ошибка
  {
   if (GetLastError()!=ERROR_IO_PENDING)
   {
    //AddLog("Frame read error!\r\n");
    return(false);
   }
  }
  Start_Frame=false;
 }
 if (Start_Frame==false)//ждём результата чтения
 {
  if (WaitForSingleObject(Overlapped_Frame.hEvent,0)==WAIT_OBJECT_0)//пришли данные
  {
   //AddLog("Event Frame!\r\n");
   if (GetOverlappedResult(hFile_Frame,&Overlapped_Frame,&readen,TRUE)==TRUE)
   {
	//AddLog("Frame data!\r\n");
    Start_Frame=true;//запускаем чтение сначала
	ret.resize(readen);
	for(size_t n=0;n<readen;n++) ret[n]=Data[n];
   }
   else return(false);
  }
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//подключиться к устройству
//----------------------------------------------------------------------------------------------------
bool CFlirOneDriver::Open(void)
{
 Close();
 //AddLog("Open!\r\n");
 char completeDeviceName_iAP[MAX_DEVPATH_LENGTH];
 char completeDeviceName_Frame[MAX_DEVPATH_LENGTH];
 char completeDeviceName_FileIO[MAX_DEVPATH_LENGTH];
 if (GetDevicePath((LPGUID)&GUID_DEVINTERFACE_FLIRONEGEN2_IAP,completeDeviceName_iAP,sizeof(completeDeviceName_iAP))==false)//не удалось найти устройство в системе
 {
  //AddLog("[ОШИБКА] Ошибка поиска драйвера iAP по GUID!\r\n");
  return(false);
 }
 if (GetDevicePath((LPGUID)&GUID_DEVINTERFACE_FLIRONEGEN2_FRAME,completeDeviceName_Frame,sizeof(completeDeviceName_Frame))==false)//не удалось найти устройство в системе
 {
  //AddLog("[ОШИБКА] Ошибка поиска драйвера Frame по GUID!\r\n");
  return(false);
 }
 if (GetDevicePath((LPGUID)&GUID_DEVINTERFACE_FLIRONEGEN2_FILEIO,completeDeviceName_FileIO,sizeof(completeDeviceName_FileIO))==false)//не удалось найти устройство в системе
 {
  //AddLog("[ОШИБКА] Ошибка поиска драйвера Frame по GUID!\r\n");
  return(false);
 }
 /*
 AddLog("iAP: ");
 AddLog(completeDeviceName_iAP);
 AddLog("\r\n");
 AddLog("Frame: ");
 AddLog(completeDeviceName_Frame);
 AddLog("\r\n");
 AddLog("FileIO: ");
 AddLog(completeDeviceName_FileIO);
 AddLog("\r\n");
 */
 //подключаемся к драйверам
 hFile_iAP=CreateFile(completeDeviceName_iAP,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
 hFile_Frame=CreateFile(completeDeviceName_Frame,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
 hFile_FileIO=CreateFile(completeDeviceName_FileIO,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);

 if (hFile_iAP==INVALID_HANDLE_VALUE)
 {
  //AddLog("[ОШИБКА] Ошибка подключения к iAP!\r\n");
  Close();
  return(false);
 }
 if (hFile_Frame==INVALID_HANDLE_VALUE)
 {
 //AddLog("[ОШИБКА] Ошибка подключения к Frame!\r\n");
 Close();
 return(false);
 }
 if (hFile_FileIO==INVALID_HANDLE_VALUE)
 {
 //AddLog("[ОШИБКА] Ошибка подключения к FileIO!\r\n");
 Close();
 return(false);
 }
 //производим включение устройства 

 if (SendControlCommand(IOCTL_FLIR_ONE_GEN2_FRAME_STOP)==false)
 {
  Close();
  return(false);  
 }
 if (SendControlCommand(IOCTL_FLIR_ONE_GEN2_FILEIO_STOP)==false)
 {
  Close();
  return(false);  
 }
 if (SendControlCommand(IOCTL_FLIR_ONE_GEN2_FILEIO_START)==false)
 {
  Close();
  return(false);  
 }
 if (SendControlCommand(IOCTL_FLIR_ONE_GEN2_FRAME_START)==false)
 {
  Close();
  return(false);  
 }

 return(true);
}
//----------------------------------------------------------------------------------------------------
//отключиться от устройства
//----------------------------------------------------------------------------------------------------
void CFlirOneDriver::Close(void)
{
 //AddLog("Close!\r\n");
 if (hFile_FileIO!=INVALID_HANDLE_VALUE)
 {
  //AddLog("FileIO отключён.\r\n");
  CloseHandle(hFile_FileIO);
 }
 if (hFile_Frame!=INVALID_HANDLE_VALUE)
 {
  //AddLog("Frame отключён.\r\n");
  CloseHandle(hFile_Frame);
 }
 if (hFile_iAP!=INVALID_HANDLE_VALUE)
 {
  //AddLog("iAP отключён.\r\n");
  CloseHandle(hFile_iAP);
 }
 hFile_FileIO=INVALID_HANDLE_VALUE;
 hFile_Frame=INVALID_HANDLE_VALUE;
 hFile_iAP=INVALID_HANDLE_VALUE;
 Start_FileIO=true;
 Start_Frame=true;
 Start_iAP=true;
}

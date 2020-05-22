#ifndef CFLIRONEDRIVER_H
#define CFLIRONEDRIVER_H

#include "stdafx.h"

#include <setupapi.h>
#include <basetyps.h>
#include <assert.h>

//������������ ����� ����� ��������
#define MAX_DEVPATH_LENGTH 256

//������� ���������� ��� iAP
//���������� ���������� Frame
#define IOCTL_FLIR_ONE_GEN2_FRAME_STOP		1
//��������� ���������� Frame
#define IOCTL_FLIR_ONE_GEN2_FRAME_START		2
//���������� ���������� FileIO
#define IOCTL_FLIR_ONE_GEN2_FILEIO_STOP		4
//��������� ���������� FileIO
#define IOCTL_FLIR_ONE_GEN2_FILEIO_START	8

//������ ������ ����� ������
#define RECEIVE_BUFFER_SIZE 1024*1024;

//����� ������ � ���������� Flir One
class CFlirOneDriver
{
 protected:
  //-���������� ������-------------------------------------------------------    
  char *Data;//����� ����� ������
  unsigned long SizeOfData;//������ ������ ����� ������


  HANDLE hFile_FileIO;//��������� FileIO
  HANDLE hFile_Frame;//��������� Frame
  HANDLE hFile_iAP;//��������� iAP

  OVERLAPPED Overlapped_FileIO;//��� ������������ ������ FileIO
  OVERLAPPED Overlapped_Frame;//��� ������������ ������ Frame
  OVERLAPPED Overlapped_iAP;//��� ������������ ������ iAP

  bool Start_FileIO;
  bool Start_Frame;
  bool Start_iAP;

  long Delay;//����� ��� ������ ������
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CFlirOneDriver(void);
  //-���������� ������-------------------------------------------------------
  ~CFlirOneDriver();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  bool Open(void);//������������ � ����������
  void Close(void);//����������� �� ����������
  bool Processing(void);//��������� (���������� ������ �������)
  bool SendControlCommand(unsigned long code);//��������� ������� ����������
  bool ReadStream(unsigned char* &ptr,unsigned long &size);//������ ������ �� ��������� 
 private:
  bool GetDevicePath(IN LPGUID InterfaceGuid,PCHAR DevicePath,size_t BufLen);//����� ������� �� GUID


};

#endif

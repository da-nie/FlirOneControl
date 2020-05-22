#ifndef CFLIRONEDRIVER_H
#define CFLIRONEDRIVER_H

#include "stdafx.h"
#include <stdint.h>
#include <vector>

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

//����� ������ � ���������� Flir One
class CFlirOneDriver
{
 static const size_t RECEIVE_BUFFER_SIZE=1048576;//������ ������ ����� ������
 protected:
  //-���������� ������-------------------------------------------------------    
  std::vector<uint8_t> Data;//����� ����� ������

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
  bool SendControlCommand(uint32_t code);//��������� ������� ����������
  bool ReadStream(std::vector<uint8_t> &ret);//������ ������ �� ��������� 
 private:
  bool GetDevicePath(IN LPGUID InterfaceGuid,PCHAR DevicePath,size_t BufLen);//����� ������� �� GUID


};

#endif

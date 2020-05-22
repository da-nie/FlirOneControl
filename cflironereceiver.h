#ifndef CFLIRONERECEIVER_H
#define CFLIRONERECEIVER_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������������ ����������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "cringbuffer.h"
#include "stdafx.h"

#include <vector>

using namespace std;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//������ ������ ����� ������
#define IMAGE_BUFFER_SIZE 1048576

//�������� ������� ����������� (�� �����������)
#define ORIGINAL_IMAGE_WIDTH 160
#define ORIGINAL_IMAGE_HEIGHT 120

//�������� ������� ����� (�� �����������)
#define ORIGINAL_VIDEO_WIDTH 640
#define ORIGINAL_VIDEO_HEIGHT 480

//������� ����������� ����� ��������� � ���������� � ������������ ���������
#define IMAGE_WIDTH 120
#define IMAGE_HEIGHT 160

//������� ����� ����� ���������
#define VIDEO_WIDTH 480
#define VIDEO_HEIGHT 640

//������ �����������
#define THERMAL_IMAGE_SIZE_SHORT (IMAGE_WIDTH*IMAGE_HEIGHT)
#define COLOR_IMAGE_SIZE_LONG (IMAGE_WIDTH*IMAGE_HEIGHT)
#define VIDEO_IMAGE_SIZE_LONG (VIDEO_WIDTH*VIDEO_HEIGHT)

//����� ����� ������ �� Flir One
class CFlirOneReceiver
{
 private:
  //-���������� ������-------------------------------------------------------    
  //��������� ����� �����������
  struct SHeader
  {
   unsigned char MagicByte[4];//���������� �����
   unsigned char Unknow1[4];//����������� ������
   unsigned long FrameSize;//������ ������ �����
   unsigned long ThermalSize;//������ ����� �����������
   unsigned long JpgSize;//������ ����������� � ������
   unsigned long StatusSize;//������ ����� ��������� 	 
   unsigned char Unknow2[4];//����������� ������
  };
  #pragma pack()

  //������� ������������� �����������
  unsigned char ColorMap_R[256];
  unsigned char ColorMap_G[256];
  unsigned char ColorMap_B[256];

  CRingBuffer *cRingBuffer_Ptr;//��������� ����� ����� ������

  SHeader sHeader;//��������� ������
  bool HeaderIsReceived;//��������� ������

  unsigned char MagicPos;//������� �������������� ����������� �����
  unsigned long MagicHeadPos;//������� ������, ����� ���������� ������ ���������� ����

  unsigned short *ThermalImage;//�������� ����������� (raw14)
  unsigned long *VideoImage;//����������� � �����������
  unsigned long *ColorImage;//������������ �����������

  vector<unsigned char> JPGImage;//������ ������ � ����������� (�������� � jpg)

  long FrameIndex;//����� �������� �����

  bool ShowVideo;//���������� �� �����
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CFlirOneReceiver(void);
  //-���������� ������-------------------------------------------------------
  ~CFlirOneReceiver();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  bool LoadColorMap(char *filename);//��������� ����� ������������� �����������
  bool CopyColorImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//����������� ������������ ����������� � �����
  bool CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index);//����������� �������� ����������� � �����
  bool CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index);//����������� ����������� � ����������� � �����
  bool CopyJPGImage(vector<unsigned char> &vector_jpg,unsigned long &index);//����������� ������ � ����������� � �����
  bool CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size);//����������� �������
  bool CreateImage(unsigned char *buffer,unsigned long size);//������� �����������
  void SetShowVideo(bool state);//���������� �� �����
  //-������------------------------------------------------------------------
 private:
  void CalculateCRC(unsigned short &crc,unsigned char byte);//��������� crc
};
#endif

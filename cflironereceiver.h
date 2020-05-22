#ifndef C_FLIR_ONE_RECEIVER_H
#define C_FLIR_ONE_RECEIVER_H

//****************************************************************************************************
//����� ����� ������ �� Flir One
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include "cringbuffer.h"

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************

//****************************************************************************************************
//����� ����� ������ �� Flir One
//****************************************************************************************************
class CFlirOneReceiver
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------

  //������ ������ ����� ������
  static const int32_t IMAGE_BUFFER_SIZE=1048576;

  //�������� ������� ����������� (�� �����������)
  static const int32_t ORIGINAL_IMAGE_WIDTH=160;
  static const int32_t ORIGINAL_IMAGE_HEIGHT=120;

  //�������� ������� ����� (�� �����������)
  static const int32_t ORIGINAL_VIDEO_WIDTH=640;
  static const int32_t ORIGINAL_VIDEO_HEIGHT=480;

  //������� ����������� ����� ��������� � ���������� � ������������ ���������
  static const int32_t IMAGE_WIDTH=120;
  static const int32_t IMAGE_HEIGHT=160;

  //������� ����� ����� ���������
  static const int32_t VIDEO_WIDTH=480;
  static const int32_t VIDEO_HEIGHT=640;

  //������ �����������
  static const int32_t THERMAL_IMAGE_SIZE=(IMAGE_WIDTH*IMAGE_HEIGHT);
  static const int32_t COLOR_IMAGE_SIZE=(IMAGE_WIDTH*IMAGE_HEIGHT);
  static const int32_t VIDEO_IMAGE_SIZE=(VIDEO_WIDTH*VIDEO_HEIGHT);

  static const int32_t COLOR_MAP_UNIT=256;//���������� ������ � �������
  static const int32_t COLOR_SIZE=3;//����� ���� �� ���� ���� (RGB)
  static const int32_t MAX_COLOR_INDEX=(COLOR_MAP_UNIT-1);//������������ ������ �����
 private:
  //-���������------------------------------------------------------------------------------------------

  #pragma pack(1)
  //��������� ����� �����������
  struct SHeader
  {
   uint8_t MagicByte[4];//���������� �����
   uint8_t Unknow1[4];//����������� ������
   uint32_t FrameSize;//������ ������ �����
   uint32_t ThermalSize;//������ ����� �����������
   uint32_t JpgSize;//������ ����������� � ������
   uint32_t StatusSize;//������ ����� ���������
   uint8_t Unknow2[4];//����������� ������
  };
  #pragma pack()

  //-����������-----------------------------------------------------------------------------------------

  //������� ������������� �����������
  uint8_t ColorMap_R[COLOR_MAP_UNIT];
  uint8_t ColorMap_G[COLOR_MAP_UNIT];
  uint8_t ColorMap_B[COLOR_MAP_UNIT];

  std::unique_ptr<CRingBuffer> cRingBuffer_Ptr;//��������� ����� ����� ������

  SHeader sHeader;//��������� ������
  bool HeaderIsReceived;//��������� ������

  uint8_t MagicPos;//������� �������������� ����������� �����
  uint32_t MagicHeadPos;//������� ������, ����� ���������� ������ ���������� ����

  std::vector<uint16_t> ThermalImage;//�������� ����������� (raw14)
  std::vector<uint32_t> VideoImage;//����������� � �����������
  std::vector<uint32_t> ColorImage;//������������ �����������

  std::vector<uint8_t> JPGImage;//������ ������ � ����������� (�������� � jpg)

  long FrameIndex;//����� �������� �����

  bool ShowVideo;//���������� �� �����
 public:
  //-�����������----------------------------------------------------------------------------------------
  CFlirOneReceiver(void);
  //-����������-----------------------------------------------------------------------------------------
  ~CFlirOneReceiver();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  bool LoadColorMap(const std::string &filename);//��������� ����� ������������� �����������
  bool CopyColorImage(std::vector<uint32_t> &image,uint32_t &index);//����������� ������������ ����������� � �����
  bool CopyThermalImage(std::vector<uint16_t> &image,uint32_t &index);//����������� �������� ����������� � �����
  bool CopyVideoImage(std::vector<uint32_t> &image,uint32_t &index);//����������� ����������� � ����������� � �����
  bool CopyJPGImage(std::vector<uint8_t> &vector_jpg,uint32_t &index);//����������� ������ � ����������� � �����
  bool CopyColorMap(uint8_t R[COLOR_MAP_UNIT],uint8_t G[COLOR_MAP_UNIT],uint8_t B[COLOR_MAP_UNIT],uint32_t size);//����������� �������
  bool CreateImage(uint8_t *buffer,uint32_t size);//������� �����������
  void SetShowVideo(bool state);//���������� �� �����
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
  void CalculateCRC(uint16_t &crc,uint8_t byte);//��������� crc
};

#endif

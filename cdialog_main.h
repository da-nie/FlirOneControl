#ifndef CDIALOG_MAIN_H
#define CDIALOG_MAIN_H

#include "stdafx.h"
#include "cflironecontrol.h"
#include "ccolorbutton.h"
#include "cgraphics.h"
#include <vector>
#include <string>
#include <stdint.h>

#include "cpicture.h"
#include "cdecorator_scale.h"

class CDialog_Main:public CDialog
{
 private:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  static const uint32_t TIMER_INTERVAL_MS=100;//�������� ������������ �������, ��
  static const int32_t FRAME_SIZE=3;//������ ������� ��������� �����������
  static const int32_t IMAGE_VIEW_WIDTH=(CFlirOneReceiver::IMAGE_WIDTH+4+CGraphics::FONT_WIDTH*7);
  static const int32_t IMAGE_VIEW_HEIGHT=(CFlirOneReceiver::IMAGE_HEIGHT);
  static const size_t PATH_STRING_BUFFER_SIZE=1024;//������ ������ ��� ����
  static const size_t STRING_BUFFER_SIZE=255;//������ ������

  static const uint32_t COLOR_WHITE=0xFFFFFFFF;//����� ����
  static const uint32_t COLOR_BLACK=0xFF000000;//������ ����

  static const uint32_t ALARM_MAX_TEMPER_COUNTER_MAX_VALUE=5;//������������ �������� ���������� ������ � ������������ ���� ��������� �� ������������ �������
  static const uint32_t ALARM_MIN_TEMPER_COUNTER_MAX_VALUE=5;//������������ �������� ���������� ������ � ������������ ���� �������� �� ������������ �������

 protected:
  //-���������� ������-------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  CFlirOneControl cFlirOneControl;//����� ���������� ������������ Flir One

  uint32_t AlarmMaxTemperCounter;//������� ���������� ������ � ������������ ���� ��������� �� ������������ �������
  uint32_t AlarmMinTemperCounter;//������� ���������� ������ � ������������ ���� �������� �� ������������ �������

  std::vector<uint32_t> ViewImage;//�����������, ��������� �� �����
  std::vector<uint32_t> SaveViewImage;//�����������, ����������� � ���� (���� ����������� ������ �������� �� ������)

  std::vector<uint32_t> ColorImage;//������������ �����������
  std::vector<uint16_t> ThermalImage;//�������� �����������
  std::vector<uint32_t> VideoImage;//����������� � �����������
  std::vector<float> TemperatureImage;//������������� �����������

  uint32_t LastReceivedFrameIndex;//������ ���������� ��������� �����������

  CGraphics cGraphics;//����� ��� ��������� �� �����������

  std::vector<std::string> vector_PaletteFileName;//������ ������

  IImage *iImage_VideoPtr;//��������� �� ����� ���� ����������� � �����������
  IImage *iImage_ViewPtr;//��������� �� ����� �����������

  //��������� ��� ������� �����������
  double PlanckR1;
  double PlanckB;
  double PlanckF;
  double PlanckO;
  double PlanckR2;

  double TempReflected;//����������� ����������
  double Emissivity;//����������� ���������

  CColorButton cColorButton_SaveAllFrame;//������ "��������� ��� �����"
  bool SaveAllFrame;//��������� �� ��� �����

  CColorButton cColorButton_SaveImageNoScale;//������ "��������� ����������� ��� �����"
  bool SaveImageNoScale;//��������� ��� �����

  CColorButton cColorButton_SaveImageCross;//������ "�������� �����������"
  bool SaveImageCross;//�������� �����������

  CColorButton cColorButton_SaveRAW;//������ "��������� RAW"
  bool SaveRAW;//��������� RAW

  CColorButton cColorButton_SaveVideo;//������ "��������� ����� � �����������"
  bool SaveVideo;//��������� �������� � �����������

  CColorButton cColorButton_ShowVideo;//������ "���������� �����"
  bool ShowVideo;//���������� �����

  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CDialog_Main(LPCTSTR lpszTemplateName,CWnd* pParentWnd);
  //-���������� ������-------------------------------------------------------
  ~CDialog_Main();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  afx_msg void OnOK(void);
  afx_msg void OnCancel(void);
  afx_msg void OnClose(void);
  afx_msg BOOL OnInitDialog(void);
  //-����� ������� ������----------------------------------------------------
  void ChangeState(bool &state,CColorButton &cColorButton);//����� ��������� ������ � �������� �����
  void SetState(bool &state,CColorButton &cColorButton,bool set_state);//������ ��������� ������ � �����
  bool GetTemperature(unsigned short raw14,double &value);//��������� �����������
  void NewFrame(void);//����� ����
  bool MinTemperControl(float t);//�������� ����������� �����������
  bool MaxTemperControl(float t);//�������� ������������ �����������
  void FindPalette(void);//����� ������
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnPaint(void);  
  afx_msg void OnButton_SaveFrame(void);//��������� ����
  afx_msg void OnButton_SaveAllFrame(void);//��������� ��� �����
  afx_msg void OnButton_SaveImageNoScale(void);//��������� ����������� ��� �����
  afx_msg void OnButton_SaveImageCross(void);//�������� �����������
  afx_msg void OnButton_SaveRAW(void);//��������� RAW
  afx_msg void OnButton_SaveVideo(void);//��������� ����� � �����������
  afx_msg void OnButton_ShowVideo(void);//���������� �����
  afx_msg void OnButton_ApplyPalette(void);//��������� �������
 protected:
  //-����� ������� ������----------------------------------------------------  
  void SaveThermalImage(void);//��������� �������� �����������
  void SaveColorImage(void);//��������� ������������ �����������
  void SaveVideoImage(void);//��������� ���� � �����������
  //-������------------------------------------------------------------------ 
};



#endif
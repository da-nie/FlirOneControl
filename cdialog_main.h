#ifndef CDIALOG_MAIN_H
#define CDIALOG_MAIN_H

#include "stdafx.h"
#include "cflironecontrol.h"
#include "ccolorbutton.h"
#include "cgraphics.h"
#include <vector>
#include <string>

#include "cpicture.h"
#include "cdecorator_scale.h"

using namespace std;

//������ ������� ��������� �����������
#define FRAME_SIZE 3

#define IMAGE_VIEW_WIDTH (IMAGE_WIDTH+4+FONT_WIDTH*7)
#define IMAGE_VIEW_HEIGHT (IMAGE_HEIGHT)

class CDialog_Main:public CDialog
{
 protected:
  //-���������� ������-------------------------------------------------------
  unsigned long *ViewImagePtr;//�����������, ��������� �� �����
  unsigned long *SaveViewImagePtr;//�����������, ����������� � ���� (���� ����������� ������ �������� �� ������)

  unsigned long *ColorImagePtr;//������������ �����������
  unsigned short *ThermalImagePtr;//�������� �����������
  unsigned long *VideoImagePtr;//����������� � �����������
  float *TemperatureImagePtr;//������������� �����������

  unsigned long LastReceivedFrameIndex;//������ ���������� ��������� �����������

  CFlirOneControl cFlirOneControl;//����� ���������� ����������� Flir One Get 2 
  CGraphics cGraphics;//����� ��� ��������� �� �����������

  vector<string> vector_PaletteFileName;//������ ������

  CIImage *cIImage_VideoPtr;//��������� �� ����� ���� ����������� � �����������
  CIImage *cIImage_ViewPtr;//��������� �� ����� �����������

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
  bool GetTemperature(unsigned short raw14,double &value);//��������� �����������
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
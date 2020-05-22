#ifndef C_LOG_H
#define C_LOG_H

//****************************************************************************************************
//����� ������ ������ ���������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <memory>
#include <string>
#include <stdint.h>

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
//����� ������ ������ ���������
//****************************************************************************************************
class CLog
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  static std::unique_ptr<CLog> cLog_Ptr;//��������� �� ����� ������ ������ ���������
 private:
  //-�����������----------------------------------------------------------------------------------------
  CLog(void);
 public:
  //-����������-----------------------------------------------------------------------------------------
  ~CLog();
 public:
  //-����������� �������� �������-----------------------------------------------------------------------
  static CLog* GetPtr(void);//�������� ��������� �� ����� ������ ������ ���������
  //-�������� �������-----------------------------------------------------------------------------------
  void AddLog(const std::string &string);//�������� ������ � log-����
  void AddLog(int32_t value);//�������� ����� � log-����
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif
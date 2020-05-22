#ifndef C_RAII_FILE_OUT_H
#define C_RAII_FILE_OUT_H
//****************************************************************************************************
//����� ������ � ������� � �������������� ��������� RAII
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <stdio.h>
#include <string>
#include <fstream>

//****************************************************************************************************
//����� ������ � ������� � �������������� ��������� RAII
//****************************************************************************************************
class CRAIIFileOut
{
 private:
  std::ofstream File;
  std::string FileName;//��� �����
 public:
  //�����������
  CRAIIFileOut(void)
  {
  }
  //�����������
  CRAIIFileOut(const std::string &file_name,std::ios_base::openmode mode):FileName(file_name)
  {
   File.open(file_name.c_str(),mode);   
  }
  //����������
  ~CRAIIFileOut(void)
  {
   Close();
  }
  //�������� ����� �����
  std::ofstream& GetHandle(void)
  {
   return(File);
  }
  //������, ������ �� ����
  bool IsOpened(void)
  {
   return(File.is_open());
  }
  //�������� ��� �����
  std::string& GetFileName(void)
  {
   return(FileName);
  }
  //������� ����
  void Close(void)
  {
   FileName="";
   File.close();
  }
  //������� ����
  void Open(const std::string &file_name,std::ios_base::openmode mode)
  {
   Close();
   FileName=file_name;   
   File.open(file_name.c_str(),mode);
  }
};


#endif

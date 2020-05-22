#ifndef C_RAII_FILE_IN_H
#define C_RAII_FILE_IN_H
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
class CRAIIFileIn
{
 private:
  std::ifstream File;
  std::string FileName;//��� �����
 public:
  //�����������
  CRAIIFileIn(const std::string &file_name,std::ios_base::openmode mode):FileName(file_name)
  {
   File.open(file_name.c_str(),mode);   
  }
  //����������
  ~CRAIIFileIn(void)
  {
   Close();
  }
  //�������� ����� �����
  std::ifstream& GetHandle(void)
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

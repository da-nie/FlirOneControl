//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "system.h"

#include <string.h>
#include <windows.h>

//****************************************************************************************************
//реализация функций
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//создать список файлов директории
//----------------------------------------------------------------------------------------------------
void CreateFileList(const std::string &path,std::vector<std::string> &vector_file_name,std::vector<std::string> &vector_file_name_without_path)
{
 vector_file_name.clear();
 vector_file_name_without_path.clear();
 char current_directory[MAX_PATH];
 if (GetCurrentDirectory(MAX_PATH,current_directory)==FALSE) return;
 if (SetCurrentDirectory(path.c_str())==FALSE) return;

 WIN32_FIND_DATA wfd;
 HANDLE handle=FindFirstFile("*",&wfd);
 if (handle==INVALID_HANDLE_VALUE)
 {
  SetCurrentDirectory(current_directory);
  return;
 }
 while(true)
 {
  if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))//если это файл
  {
   //добавляем файл в список
   std::string file_name=path;
   file_name+=GetPathDivider();
   file_name+=wfd.cFileName;
   vector_file_name.push_back(file_name);
   vector_file_name_without_path.push_back(wfd.cFileName);
  }
  if (FindNextFile(handle,&wfd)==FALSE) break;
 }
 FindClose(handle);
 SetCurrentDirectory(current_directory);
}

//----------------------------------------------------------------------------------------------------
//создать список каталогов директории
//----------------------------------------------------------------------------------------------------
void CreateDirectoryList(const std::string &path,std::vector<std::string> &vector_directory_name)
{
 vector_directory_name.clear();
 char current_directory[MAX_PATH];
 if (GetCurrentDirectory(MAX_PATH,current_directory)==FALSE) return;
 if (SetCurrentDirectory(path.c_str())==FALSE) return;

 WIN32_FIND_DATA wfd;
 HANDLE handle=FindFirstFile("*",&wfd);
 if (handle==INVALID_HANDLE_VALUE)
 {
  SetCurrentDirectory(current_directory);
  return;
 }
 while(true)
 {
  if (wfd.cFileName[0]!='.' && (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))//если это директория
  {
   std::string new_path=path;
   new_path+=GetPathDivider();
   new_path+=wfd.cFileName;
   vector_directory_name.push_back(new_path);
  }
  if (FindNextFile(handle,&wfd)==FALSE) break;
 }
 FindClose(handle);
 SetCurrentDirectory(current_directory);
}

//----------------------------------------------------------------------------------------------------
//создать каталог
//----------------------------------------------------------------------------------------------------
void MakeDirectory(const std::string &directory_name)
{
 CreateDirectory(directory_name.c_str(),NULL);
}

//----------------------------------------------------------------------------------------------------
//получить прошедшее время в секундах
//----------------------------------------------------------------------------------------------------
long double GetSecondCounter(void)
{
 return(GetTickCount()/1000.0);
}
//----------------------------------------------------------------------------------------------------
//пауза в миллисекундах
//----------------------------------------------------------------------------------------------------
void PauseInMs(size_t ms)
{
 HANDLE hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
 ResetEvent(hEvent);
 WaitForSingleObject(hEvent,ms);
 CloseHandle(hEvent);
}
//----------------------------------------------------------------------------------------------------
//получить текущую директорию
//----------------------------------------------------------------------------------------------------
std::string GetCurrentPath(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path);
 return(std::string(path));
}
//----------------------------------------------------------------------------------------------------
//получить разделитель каталого
//----------------------------------------------------------------------------------------------------
std::string GetPathDivider(void)
{
 return(std::string("\\"));
}
//----------------------------------------------------------------------------------------------------
//вывести сообщение
//----------------------------------------------------------------------------------------------------
void PutMessage(const std::string &message)
{
 MessageBox(NULL,message.c_str(),"Сообщение",MB_OK);
}


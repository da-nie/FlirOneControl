#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string.h>


#define FONT_WIDTH  8
#define FONT_HEIGHT 14

//класс рисования в буфере изображения
class CGraphics
{
 protected:
  //-Переменные класса-------------------------------------------------------  
  unsigned long *VideoPtr;//указатель на видеобуфер (RGBA)
  unsigned long ScreenWidth;//ширина изображения
  unsigned long ScreenHeight;//высота изображения
  unsigned long LineSize;//расмер строки изображения
 public:
  //-Конструктор класса------------------------------------------------------
  CGraphics(void);
  //-Деструктор класса-------------------------------------------------------
  ~CGraphics();
  //-Функции класса----------------------------------------------------------
  void Init(unsigned long *video_ptr,unsigned long screen_width,unsigned long screen_height);//инициализация
  void PutSymbol(unsigned long x,unsigned long y,char symbol,unsigned long color);//вывод символа в позицию
  void PutString(unsigned long x,unsigned long y,const char *string,unsigned long color);//вывод строчки в позицию
  void SolidFill(unsigned long x,unsigned long y,unsigned long width,unsigned long height,unsigned long color);//закрасить прямоугольник
  void PutSymbolInversion(unsigned long x,unsigned long y,char symbol);//вывод символа в позицию с инверсией цвета
  void PutStringInversion(unsigned long x,unsigned long y,const char *string);//вывод строчки в позицию с инверсией цвета
  void DrawPointInversion(unsigned long x,unsigned long y);//поставить точку в позицию с инверсией цвета

};
//----------------------------------------------------------------------------------------------------
//прототипы функций
//----------------------------------------------------------------------------------------------------

#endif

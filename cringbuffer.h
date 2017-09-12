#ifndef CRINGBUFFER_H
#define CRINGBUFFER_H

//класс кольцевого буффера
class CRingBuffer
{
 protected:
  //-Переменные класса-------------------------------------------------------    
  unsigned char *Buffer;//буфер данных
  unsigned long Head;//голова в буфере
  unsigned long Tail;//хвост в буфере
  unsigned long Size;//размер буфера
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CRingBuffer(unsigned long size);
  //-Деструктор класса-------------------------------------------------------
  ~CRingBuffer();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 //protected:
 public:
  void PushByte(unsigned char b);//добавить байт в кольцевой буфер
  bool PopByte(unsigned char *b);//забрать байт из кольцевого буфера
  bool PopShort(unsigned short *s);//забрать два байта из кольцевого буфера
  unsigned long GetDataSize(void) const;//получить сколько байт в кольцевом буфере
  unsigned long GetHeadPos(void) const;//получить позицию головы буфера
  unsigned long GetTailPos(void) const;//получить позицию хвоста буфера
  unsigned long GetBufferSize(void) const;//получить общий размер буфера
  void Reset(void);//удалить данные из буфера
  unsigned char GetByte(unsigned long pos) const;//получить значение из буфера
  void SetTailPos(unsigned long pos);//задать позицию хвоста буфера
  void SetHeadPos(unsigned long pos);//задать позицию головы буфера
};

#endif

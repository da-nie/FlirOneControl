#ifndef CRINGBUFFER_H
#define CRINGBUFFER_H

//����� ���������� �������
class CRingBuffer
{
 protected:
  //-���������� ������-------------------------------------------------------    
  unsigned char *Buffer;//����� ������
  unsigned long Head;//������ � ������
  unsigned long Tail;//����� � ������
  unsigned long Size;//������ ������
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CRingBuffer(unsigned long size);
  //-���������� ������-------------------------------------------------------
  ~CRingBuffer();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 //protected:
 public:
  void PushByte(unsigned char b);//�������� ���� � ��������� �����
  bool PopByte(unsigned char *b);//������� ���� �� ���������� ������
  bool PopShort(unsigned short *s);//������� ��� ����� �� ���������� ������
  unsigned long GetDataSize(void) const;//�������� ������� ���� � ��������� ������
  unsigned long GetHeadPos(void) const;//�������� ������� ������ ������
  unsigned long GetTailPos(void) const;//�������� ������� ������ ������
  unsigned long GetBufferSize(void) const;//�������� ����� ������ ������
  void Reset(void);//������� ������ �� ������
  unsigned char GetByte(unsigned long pos) const;//�������� �������� �� ������
  void SetTailPos(unsigned long pos);//������ ������� ������ ������
  void SetHeadPos(unsigned long pos);//������ ������� ������ ������
};

#endif

#include "cflironereceiver.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//- онструктор класса---------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneReceiver::CFlirOneReceiver(void)
{ 
 long n;
 ThermalImage=new unsigned short[THERMAL_IMAGE_SIZE_SHORT];
 ColorImage=new unsigned long[COLOR_IMAGE_SIZE_LONG];
 VideoImage=new unsigned long[VIDEO_IMAGE_SIZE_LONG];

 for(n=0;n<IMAGE_WIDTH*IMAGE_HEIGHT;n++)
 {
  ThermalImage[n]=0x00;
  ColorImage[n]=0xFF808080;
 }
 for(n=0;n<VIDEO_WIDTH*VIDEO_HEIGHT;n++) VideoImage[n]=0xFF808080;

 cRingBuffer_Ptr=new CRingBuffer(IMAGE_BUFFER_SIZE);

 FrameIndex=0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-ƒеструктор класса----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFlirOneReceiver::~CFlirOneReceiver()
{
 delete[](ThermalImage);
 delete[](ColorImage);
 delete[](VideoImage);
 delete(cRingBuffer_Ptr); 
}
//-‘ункции класса------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//закрытые функции класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//вычислить crc
//----------------------------------------------------------------------------------------------------
void CFlirOneReceiver::CalculateCRC(unsigned short &crc,unsigned char byte)
{
 crc^=(byte<<8);
 for(unsigned char n=0;n<8;n++) 
 { 
  if (crc&0x8000) crc=(crc<<1)^0x1021;
             else crc<<=1;
 }
}
//----------------------------------------------------------------------------------------------------
//создать изображение
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::CreateImage(unsigned char *buffer,unsigned long size)
{
 unsigned long n;
 long x;
 long y;

 //если в прин€тых данных встретились 4 специальных байта, то начинаем сборку заново
 static unsigned char magic_byte[4]={0xEF,0xBE,0x00,0x00};  
 //заполн€ем буфер
 for(n=0;n<size;n++)
 {
  unsigned char b=buffer[n];  
  if (b==magic_byte[MagicPos])
  {
   if (MagicPos==0) MagicHeadPos=cRingBuffer_Ptr->GetHeadPos();
   MagicPos++;
   if (MagicPos==4)
   {
    MagicPos=0;
	HeaderIsReceived=false;
    cRingBuffer_Ptr->SetTailPos(MagicHeadPos);//переносим хвост к голове
   }
  }
  else MagicPos=0;
  cRingBuffer_Ptr->PushByte(b);
 }
 //узнаем, сколько данных в буфере
 unsigned long length=cRingBuffer_Ptr->GetDataSize();  
 //прочтем заголовок, так, чтобы не повредить данные в буфере
 if (length<sizeof(SHeader)) return(false);//заголовок не собран
 if (HeaderIsReceived==false)//собираем заголовок
 {
  unsigned char *sheader_ptr=(unsigned char*)(&sHeader);
  unsigned long tail=cRingBuffer_Ptr->GetTailPos();
  for(n=0;n<sizeof(SHeader);n++,sheader_ptr++)
  {
   if (cRingBuffer_Ptr->GetHeadPos()==tail) return(false);//данных почему-то не хватило
   *sheader_ptr=cRingBuffer_Ptr->GetByte(tail);
   tail++;
   if (tail==cRingBuffer_Ptr->GetBufferSize()) tail=0;  	 
  }
  if (sHeader.MagicByte[0]!=magic_byte[0]) return(false);
  if (sHeader.MagicByte[1]!=magic_byte[1]) return(false);
  if (sHeader.MagicByte[2]!=magic_byte[2]) return(false);
  if (sHeader.MagicByte[3]!=magic_byte[3]) return(false);
  HeaderIsReceived=true;
 }
 if (sHeader.FrameSize+sizeof(SHeader)>length) return(false);//кадр ещЄ не собран 
 //кадр собран, считываем заголовок кадра
 unsigned char b; 
 for(n=0;n<sizeof(SHeader);n++) cRingBuffer_Ptr->PopByte(&b);  
 //кадр собран
 for(n=0;n<IMAGE_WIDTH*IMAGE_HEIGHT;n++)
 {
  ThermalImage[n]=0x00;
  ColorImage[n]=0xFF808080;
 }
 for(n=0;n<VIDEO_WIDTH*VIDEO_HEIGHT;n++) VideoImage[n]=0xFF808080; 
 JPGImage.clear();
 //создаЄм тепловое изображение (164 числа в строке)
 long min=0x10000;
 long max=0x0;
 bool enabled=true;
 for(y=0;y<ORIGINAL_IMAGE_HEIGHT;y++) 
 {  
  unsigned short crc=0;
  unsigned short id;
  unsigned short crc_package;
  //читаем заголовок
  cRingBuffer_Ptr->PopShort(&id);
  id&=0x0FFF;
  CalculateCRC(crc,(id>>8)&0xFF);
  CalculateCRC(crc,id&0xFF);
  
  cRingBuffer_Ptr->PopShort(&crc_package);
  CalculateCRC(crc,0);
  CalculateCRC(crc,0);
  
  for(x=0;x<ORIGINAL_IMAGE_WIDTH/2;x++)
  {   
   unsigned short value;
   cRingBuffer_Ptr->PopShort(&value);
   //считаем CRC
   CalculateCRC(crc,*((unsigned char*)(&value)+1));
   CalculateCRC(crc,*((unsigned char*)(&value)));
   
   unsigned long offset=y+(ORIGINAL_IMAGE_WIDTH-1-x)*ORIGINAL_IMAGE_HEIGHT;   
   ThermalImage[offset]=value;
   if (value<min) min=value;
   if (value>max) max=value;
  }
  if (crc!=crc_package)//CRC не совпало
  {
   enabled=false;
   break;
  }

  crc=0;
  //читаем заголовок
  cRingBuffer_Ptr->PopShort(&id);
  id&=0x0FFF;
  CalculateCRC(crc,(id>>8)&0xFF);
  CalculateCRC(crc,id&0xFF);
  
  cRingBuffer_Ptr->PopShort(&crc_package);
  CalculateCRC(crc,0);
  CalculateCRC(crc,0);
  
  for(x=ORIGINAL_IMAGE_WIDTH/2;x<ORIGINAL_IMAGE_WIDTH;x++)
  {   
   unsigned short value;
   cRingBuffer_Ptr->PopShort(&value);
   //считаем CRC
   CalculateCRC(crc,*((unsigned char*)(&value)+1));
   CalculateCRC(crc,*((unsigned char*)(&value)));
   unsigned long offset=y+(ORIGINAL_IMAGE_WIDTH-1-x)*ORIGINAL_IMAGE_HEIGHT;   
   ThermalImage[offset]=value;
   if (value<min) min=value;
   if (value>max) max=value;
  }
  if (crc!=crc_package)//CRC не совпало
  {
   enabled=false;
   break;
  }
 }
 if (enabled==true)
 {
  bool ok=true;	 
  for(unsigned long n=ORIGINAL_IMAGE_HEIGHT*(ORIGINAL_IMAGE_WIDTH+4)*2;n<sHeader.ThermalSize;n++)
  {
   unsigned char b;
   if (cRingBuffer_Ptr->PopByte(&b)==false)
   {
    ok=false;
	break;
   }
  }  
  if (ok==true)
  {
   JPGImage.reserve(sHeader.JpgSize);
   //сохран€ем jpg-картинку, дл€ чего создаЄм поток
   IStream *pStream=NULL;
   if (SUCCEEDED(CreateStreamOnHGlobal(NULL,true,&pStream)))
   {   
    for(unsigned long n=0;n<sHeader.JpgSize;n++)
    {    
     unsigned char b;
     if (cRingBuffer_Ptr->PopByte(&b)==false) 
	 {
	  JPGImage.clear();
	  ok=false;
	  break;
	 }
     pStream->Write(&b,1,0);//записываем в поток наши данные	   
	 JPGImage.push_back(b);
	} 
	if (ok==true)
	{
     //создаЄм изображение
     Bitmap bitmap(pStream,0);     
     //копируем данные изображени€ в массив   
     long width=bitmap.GetWidth();
     long height=bitmap.GetHeight();
     for(long x=0;x<width;x++)
     {
      for(long y=0;y<height;y++)
	  { 
       Color color; 	 
       bitmap.GetPixel(x,y,&color);
	   unsigned long offset=(width-1-x)*ORIGINAL_VIDEO_HEIGHT+y;
	   unsigned long r=color.GetRed();
	   unsigned long g=color.GetGreen();
	   unsigned long b=color.GetBlue();
       VideoImage[offset]=b|(g<<8)|(r<<16)|(0xFF<<24);	 
	  }
	 }
	}
	pStream->Release();
   }    
  }
 }

 cRingBuffer_Ptr->Reset();
 MagicPos=0;
 MagicHeadPos=0;
 HeaderIsReceived=false;

 if (enabled==true)
 {
  //создаЄм раскрашенное изображение
  long delta=max-min;
  if (delta==0) delta=1;
  for(x=0;x<ORIGINAL_IMAGE_WIDTH;x++)
  {
   for(y=0;y<ORIGINAL_IMAGE_HEIGHT;y++)
   {
    long offset=x+y*ORIGINAL_IMAGE_WIDTH;
    long value=ThermalImage[offset];
    value-=min;
    value=(value*255)/delta;
    offset=x+y*ORIGINAL_IMAGE_WIDTH;
    value&=0xff;
	unsigned long color=0;
    color|=0xff;
	color<<=8;
    color|=ColorMap_R[value];
	color<<=8;
    color|=ColorMap_G[value];
	color<<=8;
    color|=ColorMap_B[value];
	ColorImage[offset]=color;
   }
  }
  FrameIndex++;
  //return(true);
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//загрузить карту перекодировки изображени€
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::LoadColorMap(char *filename)
{ 
 FILE *file=fopen(filename,"rb");
 if (file==NULL) return(false);
 unsigned char colormap[768]; 
 if (fread(colormap,sizeof(unsigned char),768,file)<768)
 {
  fclose(file);
  return(false);
 }
 fclose(file);
 for(long n=0;n<256;n++)
 {
  ColorMap_R[n]=colormap[n*3+0];
  ColorMap_G[n]=colormap[n*3+1];
  ColorMap_B[n]=colormap[n*3+2];
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//скопировать раскрашенное изображение в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::CopyColorImage(unsigned long*image_ptr,unsigned long size,unsigned long &index)
{ 
 if (size<COLOR_IMAGE_SIZE_LONG) return(false);//буфер слишком мал
 index=FrameIndex;
 memcpy(image_ptr,ColorImage,COLOR_IMAGE_SIZE_LONG*sizeof(unsigned long));
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//скопировать тепловое изображение в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::CopyThermalImage(unsigned short *image_ptr,unsigned long size,unsigned long &index)
{ 
 if (size<THERMAL_IMAGE_SIZE_SHORT) return(false);//буфер слишком мал
 index=FrameIndex;
 memcpy(image_ptr,ThermalImage,THERMAL_IMAGE_SIZE_SHORT*sizeof(unsigned short));
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//скопировать изображение с видеокамеры в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::CopyVideoImage(unsigned long *image_ptr,unsigned long size,unsigned long &index)
{
 if (size<VIDEO_IMAGE_SIZE_LONG) return(false);//буфер слишком мал
 index=FrameIndex;
 memcpy(image_ptr,VideoImage,VIDEO_IMAGE_SIZE_LONG*sizeof(unsigned long));
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//скопировать данные с видеокамеры в буфер
//----------------------------------------------------------------------------------------------------
bool CFlirOneReceiver::CopyJPGImage(vector<unsigned char> &vector_jpg,unsigned long &index)
{
 vector_jpg=JPGImage;
 index=FrameIndex;
 return(true); 
}
//---------------------------------------------------------------------------
//скопировать палитру
//---------------------------------------------------------------------------
bool CFlirOneReceiver::CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size)
{
 if (size<256) return(false);//буфер слишком мал
 for(long n=0;n<256;n++)
 {
  R[n]=ColorMap_R[n];
  G[n]=ColorMap_G[n];
  B[n]=ColorMap_B[n];
 }
 return(true);
}
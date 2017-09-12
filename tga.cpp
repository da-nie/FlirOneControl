#include "tga.h"

unsigned char *LoadTGAFromFile(char *FileName,long &Width,long &Height)
{
 //пробуем считать изображение
 STGAHeader sTGAHeader;
 FILE *file;
 file=fopen(FileName,"rb");//открываем файл
 if (file==NULL) return(NULL);//ошибка
 //читаем заголовок
 if (fread(&sTGAHeader,sizeof(STGAHeader),1,file)<=0)//ошибка - мало данных
 {
  fclose(file);
  return(NULL);
 } 
 //проверяем на возможность чтения
 if (sTGAHeader.imageType&8)
 {
  fclose(file);
  return(NULL);//RLE не поддерживаем
 }
 if ((sTGAHeader.imageType&7)==0 || (sTGAHeader.imageType&7)==3)
 {
  fclose(file);
  return(NULL);//градации серого и отсутствие изображения не поддерживаем
 }
 //задаём параметры изображения
 Width=sTGAHeader.width;
 Height=sTGAHeader.height;
 long LineLength=sTGAHeader.width*4;
 //выделяем память для изображения
 long image_length=sTGAHeader.width*sTGAHeader.height*sTGAHeader.bits/8;
 unsigned char *i_buffer=new unsigned char[image_length];
 //считываем изображение
 fseek(file,sizeof(struct STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.colorMapLength*sTGAHeader.colorMapBits/8,SEEK_SET);
 if (fread(i_buffer,image_length,1,file)<=0)
 {
  fclose(file);
  delete[](i_buffer);
  return(NULL);
 }
 //а теперь анализируем формат
 if (sTGAHeader.bits==24)//BGR - модицифируем для четвёрок байт
 {
  fclose(file);
  unsigned char *out_image=new unsigned char[sTGAHeader.width*sTGAHeader.height*4];
  long y,x;
  if (sTGAHeader.descriptor==32)//прямой формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*3)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     unsigned char b=*(i_ptrc);i_ptrc++;
     unsigned char g=*(i_ptrc);i_ptrc++;
     unsigned char r=*(i_ptrc);i_ptrc++;
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader.descriptor==8)//обратный формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer+sTGAHeader.width*sTGAHeader.height*3-1;
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*3)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     unsigned char b=*(i_ptrc);i_ptrc++;
     unsigned char g=*(i_ptrc);i_ptrc++;
     unsigned char r=*(i_ptrc);i_ptrc++;
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);
 }
 if (sTGAHeader.colorMapType==1 && sTGAHeader.colorMapBits/8==3)//есть палитра по 24 бита
 {
  fseek(file,sTGAHeader.colorMapStart+sizeof(struct STGAHeader),SEEK_SET);
  //читаем палитру
  unsigned char *color_map=new unsigned char[sTGAHeader.colorMapLength*3];
  if (fread(color_map,sTGAHeader.colorMapLength*3,1,file)<=0)
  {
   fclose(file);
   delete[](color_map);
   delete[](i_buffer);
   return(NULL);
  }
  //нам потребуется изменить формат
  unsigned char *out_image=new unsigned char[sTGAHeader.width*sTGAHeader.height*4];
  long y,x;
  if (sTGAHeader.descriptor==32)//прямой формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
     long index=(*i_ptrc)*3;
     unsigned char b=color_map[index];
     unsigned char g=color_map[index+1];
     unsigned char r=color_map[index+2];
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader.descriptor==8)//формат перевёрнутый
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer+sTGAHeader.width*(sTGAHeader.height-1);
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
     long index=(*i_ptrc)*3;
     unsigned char b=color_map[index];
     unsigned char g=color_map[index+1];
     unsigned char r=color_map[index+2];
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  fclose(file);
  delete[](i_buffer);
  delete[](color_map);
  return(out_image);  
 }
  //иные режимы не поддерживаем
 fclose(file);
 delete[](i_buffer);
 return(NULL);
}
unsigned char *LoadTGAFromResource(HMODULE hModule,long ID,long &Width,long &Height)
{
 HRSRC hRSRC=FindResource(hModule,(LPSTR)ID,RT_RCDATA);
 if (hRSRC==NULL) return(NULL);
 HGLOBAL hGlobal=LoadResource(hModule,hRSRC);
 if (hGlobal==NULL) return(NULL);
 unsigned char *Data=(unsigned char*)LockResource(hGlobal);
 long DataSize=SizeofResource(hModule,hRSRC);
 //а теперь считываем данные
 if (DataSize<sizeof(STGAHeader))//файл короткий
 {
  GlobalUnlock(hGlobal);
  return(NULL);
 }
 STGAHeader *sTGAHeader;
 //читаем заголовок
 sTGAHeader=(STGAHeader*)Data;
 //проверяем на возможность чтения
 if (sTGAHeader->imageType&8)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//RLE не поддерживаем
 }
 if (sTGAHeader->imageType&7==0 || sTGAHeader->imageType&7==3)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//градации серого и отсутствие изображения не поддерживаем
 }
 //задаём параметры изображения
 Width=sTGAHeader->width;
 Height=sTGAHeader->height;
 long LineLength=sTGAHeader->width*4;
 //выделяем память для изображения
 long image_length=sTGAHeader->width*sTGAHeader->height*sTGAHeader->bits/8;
 unsigned char *i_buffer=new unsigned char[image_length];
 //считываем изображение
 long offset=sizeof(STGAHeader)+sTGAHeader->colorMapStart+sTGAHeader->colorMapLength*sTGAHeader->colorMapBits/8;
 if (offset+image_length>DataSize)//файл короткий
 {
  GlobalUnlock(hGlobal);
  delete[](i_buffer);
  return(NULL);
 }
 memcpy(i_buffer,Data+offset,image_length);//копируем данные
 //а теперь анализируем формат
 if (sTGAHeader->bits==24)//BGR - модицифируем для четвёрок байт
 {
  GlobalUnlock(hGlobal);
  unsigned char *out_image=new unsigned char[sTGAHeader->width*sTGAHeader->height*4];
  long y,x;
  if (sTGAHeader->descriptor==32)//прямой формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader->height;y++,i_ptr+=sTGAHeader->width*3)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     unsigned char b=*(i_ptrc);i_ptrc++;
     unsigned char g=*(i_ptrc);i_ptrc++;
     unsigned char r=*(i_ptrc);i_ptrc++;
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader->descriptor==8)//обратный формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer+sTGAHeader->width*sTGAHeader->height*3-1;
   for(y=sTGAHeader->height-1;y>=0;y--,i_ptr-=sTGAHeader->width*3)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     unsigned char b=*(i_ptrc);i_ptrc++;
     unsigned char g=*(i_ptrc);i_ptrc++;
     unsigned char r=*(i_ptrc);i_ptrc++;
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);  
 }

 if (sTGAHeader->colorMapType==1 && sTGAHeader->colorMapBits/8==3)//есть палитра по 24 бита
 {
  long offset=sTGAHeader->colorMapStart+sizeof(STGAHeader);
  if (offset+sTGAHeader->colorMapLength*3>DataSize)
  {
   GlobalUnlock(hGlobal);
   delete[](i_buffer);
   return(NULL);
  }
  unsigned char *color_map=Data+offset;   
  //нам потребуется изменить формат
  unsigned char *out_image=new unsigned char[sTGAHeader->width*sTGAHeader->height*4];
  long y,x;
  if (sTGAHeader->descriptor==32)//прямой формат
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader->height;y++,i_ptr+=sTGAHeader->width)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++,i_ptrc++)
    {
     long index=(*i_ptrc)*3;
     unsigned char b=color_map[index];
     unsigned char g=color_map[index+1];
     unsigned char r=color_map[index+2];
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader->descriptor==8)//формат перевёрнутый
  {
   unsigned char *oi_ptr=out_image;
   unsigned char *i_ptr=i_buffer+sTGAHeader->width*(sTGAHeader->height-1);
   for(y=sTGAHeader->height-1;y>=0;y--,i_ptr-=sTGAHeader->width)
   {
    unsigned char *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++,i_ptrc++)
    {
     long index=(*i_ptrc)*3;
     unsigned char b=color_map[index];
     unsigned char g=color_map[index+1];
     unsigned char r=color_map[index+2];
     unsigned char a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  GlobalUnlock(hGlobal);
  delete[](i_buffer);
  delete[](color_map);
  return(out_image);  
 }
 //иные режимы не поддерживаем
 delete[](i_buffer);
 GlobalUnlock(hGlobal);
 return(NULL); 
}

bool SaveTGA(char *FileName,long Width,long Height,unsigned char *Image)
{
 struct STGAHeader sTGAHeader;
 sTGAHeader.identsize=0;
 sTGAHeader.colorMapType=0;
 sTGAHeader.imageType=2;
 sTGAHeader.colorMapStart=0;
 sTGAHeader.colorMapLength=0;
 sTGAHeader.colorMapBits=24;
 sTGAHeader.xstart=0;
 sTGAHeader.ystart=0;
 sTGAHeader.width=static_cast<unsigned short>(Width);
 sTGAHeader.height=static_cast<unsigned short>(Height);
 sTGAHeader.bits=32;
 sTGAHeader.descriptor=32;
 FILE *file=fopen(FileName,"wb");
 if (file==NULL) return(false);//ошибка создания файла
 //сохраняем заголовок
 if (fwrite(&sTGAHeader,1,sizeof(struct STGAHeader),file)<sizeof(struct STGAHeader))//ошибка записи
 {
  fclose(file);
  return(false);
 }
 //сохраняем в файл
 if (fwrite(Image,1,Width*Height*4,file)<(unsigned int)(Width*Height*4))//ошибка записи
 {
  fclose(file);
  return(false);
 }
 //закрываем файл
 fclose(file);
 return(true);
}

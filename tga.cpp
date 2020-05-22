#include "tga.h"

unsigned char *LoadTGAFromFile(char *FileName,long &Width,long &Height)
{
 //������� ������� �����������
 STGAHeader sTGAHeader;
 FILE *file;
 file=fopen(FileName,"rb");//��������� ����
 if (file==NULL) return(NULL);//������
 //������ ���������
 if (fread(&sTGAHeader,sizeof(STGAHeader),1,file)<=0)//������ - ���� ������
 {
  fclose(file);
  return(NULL);
 } 
 //��������� �� ����������� ������
 if (sTGAHeader.imageType&8)
 {
  fclose(file);
  return(NULL);//RLE �� ������������
 }
 if ((sTGAHeader.imageType&7)==0 || (sTGAHeader.imageType&7)==3)
 {
  fclose(file);
  return(NULL);//�������� ������ � ���������� ����������� �� ������������
 }
 //����� ��������� �����������
 Width=sTGAHeader.width;
 Height=sTGAHeader.height;
 long LineLength=sTGAHeader.width*4;
 //�������� ������ ��� �����������
 long image_length=sTGAHeader.width*sTGAHeader.height*sTGAHeader.bits/8;
 unsigned char *i_buffer=new unsigned char[image_length];
 //��������� �����������
 fseek(file,sizeof(struct STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.colorMapLength*sTGAHeader.colorMapBits/8,SEEK_SET);
 if (fread(i_buffer,image_length,1,file)<=0)
 {
  fclose(file);
  delete[](i_buffer);
  return(NULL);
 }
 //� ������ ����������� ������
 if (sTGAHeader.bits==24)//BGR - ������������ ��� ������� ����
 {
  fclose(file);
  unsigned char *out_image=new unsigned char[sTGAHeader.width*sTGAHeader.height*4];
  long y,x;
  if (sTGAHeader.descriptor==32)//������ ������
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
  if (sTGAHeader.descriptor==8)//�������� ������
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
 if (sTGAHeader.colorMapType==1 && sTGAHeader.colorMapBits/8==3)//���� ������� �� 24 ����
 {
  fseek(file,sTGAHeader.colorMapStart+sizeof(struct STGAHeader),SEEK_SET);
  //������ �������
  unsigned char *color_map=new unsigned char[sTGAHeader.colorMapLength*3];
  if (fread(color_map,sTGAHeader.colorMapLength*3,1,file)<=0)
  {
   fclose(file);
   delete[](color_map);
   delete[](i_buffer);
   return(NULL);
  }
  //��� ����������� �������� ������
  unsigned char *out_image=new unsigned char[sTGAHeader.width*sTGAHeader.height*4];
  long y,x;
  if (sTGAHeader.descriptor==32)//������ ������
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
  if (sTGAHeader.descriptor==8)//������ �����������
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
  //���� ������ �� ������������
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
 //� ������ ��������� ������
 if (DataSize<sizeof(STGAHeader))//���� ��������
 {
  GlobalUnlock(hGlobal);
  return(NULL);
 }
 STGAHeader *sTGAHeader;
 //������ ���������
 sTGAHeader=(STGAHeader*)Data;
 //��������� �� ����������� ������
 if (sTGAHeader->imageType&8)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//RLE �� ������������
 }
 if (sTGAHeader->imageType&7==0 || sTGAHeader->imageType&7==3)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//�������� ������ � ���������� ����������� �� ������������
 }
 //����� ��������� �����������
 Width=sTGAHeader->width;
 Height=sTGAHeader->height;
 long LineLength=sTGAHeader->width*4;
 //�������� ������ ��� �����������
 long image_length=sTGAHeader->width*sTGAHeader->height*sTGAHeader->bits/8;
 unsigned char *i_buffer=new unsigned char[image_length];
 //��������� �����������
 long offset=sizeof(STGAHeader)+sTGAHeader->colorMapStart+sTGAHeader->colorMapLength*sTGAHeader->colorMapBits/8;
 if (offset+image_length>DataSize)//���� ��������
 {
  GlobalUnlock(hGlobal);
  delete[](i_buffer);
  return(NULL);
 }
 memcpy(i_buffer,Data+offset,image_length);//�������� ������
 //� ������ ����������� ������
 if (sTGAHeader->bits==24)//BGR - ������������ ��� ������� ����
 {
  GlobalUnlock(hGlobal);
  unsigned char *out_image=new unsigned char[sTGAHeader->width*sTGAHeader->height*4];
  long y,x;
  if (sTGAHeader->descriptor==32)//������ ������
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
  if (sTGAHeader->descriptor==8)//�������� ������
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

 if (sTGAHeader->colorMapType==1 && sTGAHeader->colorMapBits/8==3)//���� ������� �� 24 ����
 {
  long offset=sTGAHeader->colorMapStart+sizeof(STGAHeader);
  if (offset+sTGAHeader->colorMapLength*3>DataSize)
  {
   GlobalUnlock(hGlobal);
   delete[](i_buffer);
   return(NULL);
  }
  unsigned char *color_map=Data+offset;   
  //��� ����������� �������� ������
  unsigned char *out_image=new unsigned char[sTGAHeader->width*sTGAHeader->height*4];
  long y,x;
  if (sTGAHeader->descriptor==32)//������ ������
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
  if (sTGAHeader->descriptor==8)//������ �����������
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
 //���� ������ �� ������������
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
 if (file==NULL) return(false);//������ �������� �����
 //��������� ���������
 if (fwrite(&sTGAHeader,1,sizeof(struct STGAHeader),file)<sizeof(struct STGAHeader))//������ ������
 {
  fclose(file);
  return(false);
 }
 //��������� � ����
 if (fwrite(Image,1,Width*Height*4,file)<(unsigned int)(Width*Height*4))//������ ������
 {
  fclose(file);
  return(false);
 }
 //��������� ����
 fclose(file);
 return(true);
}

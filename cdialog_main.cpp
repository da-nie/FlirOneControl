#include "cdialog_main.h"

//-Функции обработки сообщений класса----------------------------------------
BEGIN_MESSAGE_MAP(CDialog_Main,CDialog)
 ON_WM_DESTROY()
 ON_WM_CLOSE()
 ON_WM_PAINT()
 ON_WM_TIMER()
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_FRAME,OnButton_SaveFrame)
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_ALL_FRAME,OnButton_SaveAllFrame)
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_IMAGE_NO_SCALE,OnButton_SaveImageNoScale)
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_IMAGE_CROSS,OnButton_SaveImageCross)
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_RAW,OnButton_SaveRAW)
 ON_COMMAND(IDC_BUTTON_MAIN_SAVE_VIDEO,OnButton_SaveVideo)
 ON_COMMAND(IDC_BUTTON_MAIN_APPLY_PALETTE,OnButton_ApplyPalette)
 ON_COMMAND(IDC_BUTTON_MAIN_SHOW_VIDEO,OnButton_ShowVideo)
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CDialog_Main::CDialog_Main(LPCTSTR lpszTemplateName, CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
 long n;
 ViewImagePtr=new unsigned long[IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT];
 for(n=0;n<IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT;n++) ViewImagePtr[n]=0;

 SaveViewImagePtr=new unsigned long[IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT];
 for(n=0;n<IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT;n++) SaveViewImagePtr[n]=0;

 ColorImagePtr=new unsigned long[IMAGE_WIDTH*IMAGE_HEIGHT];
 for(n=0;n<IMAGE_WIDTH*IMAGE_HEIGHT;n++) ColorImagePtr[n]=0;

 VideoImagePtr=new unsigned long[VIDEO_WIDTH*VIDEO_HEIGHT];
 for(n=0;n<VIDEO_WIDTH*VIDEO_HEIGHT;n++) VideoImagePtr[n]=0;

 ThermalImagePtr=new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
 for(n=0;n<IMAGE_WIDTH*IMAGE_HEIGHT;n++)ThermalImagePtr[n]=0;

 TemperatureImagePtr=new float[IMAGE_WIDTH*IMAGE_HEIGHT];
 for(n=0;n<IMAGE_WIDTH*IMAGE_HEIGHT;n++)TemperatureImagePtr[n]=0;

 cGraphics.Init(ViewImagePtr,IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);

 LastReceivedFrameIndex=0;

 cIImage_VideoPtr=new CPicture;
 cIImage_VideoPtr->SetSize(VIDEO_WIDTH,VIDEO_HEIGHT);
 cIImage_VideoPtr=new CDecorator_Scale(cIImage_VideoPtr,VIDEO_WIDTH,VIDEO_HEIGHT);

 cIImage_ViewPtr=new CPicture;
 cIImage_ViewPtr->SetSize(IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);
 cIImage_ViewPtr=new CDecorator_Scale(cIImage_ViewPtr,IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);

 cFlirOneControl.LoadColorMap("iron.pal");//загружаем палитру

 PlanckR1=16528.178;
 PlanckB=1427.5;
 PlanckF=1.0;
 PlanckO=-1307.0;
 PlanckR2=0.012258549;

 TempReflected=20;
 Emissivity=0.95;

 SaveAllFrame=false;
 SaveImageNoScale=false;
 SaveImageCross=false;
 SaveRAW=false;
 SaveVideo=false;
 ShowVideo=false;

 cFlirOneControl.SetShowVideo(false);
}
//-Деструктор класса---------------------------------------------------------
CDialog_Main::~CDialog_Main()
{
 delete(cIImage_VideoPtr);

 delete[](ViewImagePtr);
 delete[](SaveViewImagePtr);
 delete[](ColorImagePtr);
 delete[](VideoImagePtr);
 delete[](ThermalImagePtr);
 delete[](TemperatureImagePtr);
}
//-Замещённые функции предка-------------------------------------------------
afx_msg void CDialog_Main::OnOK(void)
{
}
afx_msg void CDialog_Main::OnCancel(void)
{
}
afx_msg void CDialog_Main::OnClose(void)
{
 EndDialog(0);
}
afx_msg BOOL CDialog_Main::OnInitDialog(void)
{
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_TEMPERATURE))->SetWindowText("20");
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_EMISSION))->SetWindowText("0.95");
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_TEMPERATURE))->SetLimitText(10);
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_EMISSION))->SetLimitText(10);

  
 cColorButton_SaveAllFrame.Attach(IDC_BUTTON_MAIN_SAVE_ALL_FRAME,this);
 cColorButton_SaveAllFrame.SetBackGroundColor(RGB(192,192,192));

 cColorButton_ShowVideo.Attach(IDC_BUTTON_MAIN_SHOW_VIDEO,this);
 cColorButton_ShowVideo.SetBackGroundColor(RGB(192,192,192)); 

 cColorButton_SaveImageNoScale.Attach(IDC_BUTTON_MAIN_SAVE_IMAGE_NO_SCALE,this);
 cColorButton_SaveImageNoScale.SetBackGroundColor(RGB(192,192,192));

 cColorButton_SaveImageCross.Attach(IDC_BUTTON_MAIN_SAVE_IMAGE_CROSS,this);
 cColorButton_SaveImageCross.SetBackGroundColor(RGB(192,192,192));

 cColorButton_SaveRAW.Attach(IDC_BUTTON_MAIN_SAVE_RAW,this);
 cColorButton_SaveRAW.SetBackGroundColor(RGB(192,192,192));

 cColorButton_SaveVideo.Attach(IDC_BUTTON_MAIN_SAVE_VIDEO,this);
 cColorButton_SaveVideo.SetBackGroundColor(RGB(192,192,192));

 //ищем палитры
 ((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->ResetContent();
 vector_PaletteFileName.clear();
 WIN32_FIND_DATA wfd;
 //сначала ищем файлы
 HANDLE Handle_Find=FindFirstFile("*.pal",&wfd);
 if (Handle_Find!=INVALID_HANDLE_VALUE)
 {
  while(1)
  {
   //это файл
   if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
   {
    //добавляем в список
	((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->AddString(wfd.cFileName);
	vector_PaletteFileName.push_back(string(wfd.cFileName));
   }
   if (FindNextFile(Handle_Find,&wfd)==FALSE) break;
  }
  FindClose(Handle_Find);
 }
 ((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->SetCurSel(0);

 //подключим таймер
 SetTimer(ID_TIMER_UPDATE,30,NULL);
 return(CDialog::OnInitDialog());
}
//-Новые функции класса------------------------------------------------------
//-Функции обработки сообщений класса----------------------------------------
afx_msg void CDialog_Main::OnDestroy(void)
{
 KillTimer(ID_TIMER_UPDATE);
 CDialog::OnDestroy();
}
afx_msg void CDialog_Main::OnTimer(UINT nIDEvent)
{
 long x;
 long y;
 if (nIDEvent==ID_TIMER_UPDATE) 
 { 
  //считываем температуру болометров и коэффициент излучения
  char string[255];
  ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_TEMPERATURE))->GetWindowText(string,255);
  TempReflected=atof(string);
  ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_EMISSION))->GetWindowText(string,255);
  Emissivity=atof(string);
  //копируем изображение
  unsigned long index;
  //получаем тепловое изображение
  cFlirOneControl.CopyThermalImage(ThermalImagePtr,THERMAL_IMAGE_SIZE_SHORT,index);
  //получаем раскрашенное изображение
  cFlirOneControl.CopyColorImage(ColorImagePtr,COLOR_IMAGE_SIZE_LONG,index);
  //получаем видео изображение
  cFlirOneControl.CopyVideoImage(VideoImagePtr,VIDEO_IMAGE_SIZE_LONG,index);
  if (LastReceivedFrameIndex!=index)//изображение изменилось
  {
   //копируем кадр видео   
   cIImage_VideoPtr->SetRGBAImage(VIDEO_WIDTH,VIDEO_HEIGHT,vector<unsigned long>(VideoImagePtr,VideoImagePtr+VIDEO_IMAGE_SIZE_LONG-1));
   //копируем изображение
   unsigned long *v1_ptr=ViewImagePtr;
   unsigned long *v2_ptr=ColorImagePtr;
   for(y=0;y<IMAGE_HEIGHT;y++,v1_ptr+=IMAGE_VIEW_WIDTH,v2_ptr+=IMAGE_WIDTH)
   {
    unsigned long *v1_ptr_local=v1_ptr;
    unsigned long *v2_ptr_local=v2_ptr;
    for(x=0;x<IMAGE_WIDTH;x++,v1_ptr_local++,v2_ptr_local++) *v1_ptr_local=*v2_ptr_local;
   }
   //очищаем места для максимальной и минимальной температуры
   cGraphics.SolidFill(IMAGE_WIDTH+2,IMAGE_HEIGHT-FONT_HEIGHT*2-2-4,IMAGE_VIEW_WIDTH-(IMAGE_WIDTH+2),FONT_HEIGHT*2+4,0);
   cGraphics.SolidFill(IMAGE_WIDTH+2,2,IMAGE_VIEW_WIDTH-(IMAGE_WIDTH+2),FONT_HEIGHT,0);
   //рисуем градиентную шкалу
   unsigned long scale_height=IMAGE_VIEW_HEIGHT-8-FONT_HEIGHT*3-4;
   float c_index=255.0f;
   float dc_index=255.0f/static_cast<float>(scale_height);
   v1_ptr=ViewImagePtr+(4+FONT_HEIGHT)*IMAGE_VIEW_WIDTH+IMAGE_WIDTH+2;
   unsigned char R[256];
   unsigned char G[256];
   unsigned char B[256];
   cFlirOneControl.CopyColorMap(R,G,B,256);
   for(y=0;y<scale_height;y++,v1_ptr+=IMAGE_VIEW_WIDTH,c_index-=dc_index)
   {    
	unsigned long index=static_cast<unsigned long>(c_index);
	unsigned long color=0;
    color|=0xff;
	color<<=8;
    color|=R[index];
	color<<=8;
    color|=G[index];
	color<<=8;
    color|=B[index];
	unsigned long *v1_ptr_local=v1_ptr;
    for(x=0;x<IMAGE_VIEW_WIDTH-IMAGE_WIDTH-4;x++,v1_ptr_local++) *v1_ptr_local=color;
   }
   //считаем температуру по кадру
   char string[255];
   double t_max=-10000;
   double t_min=10000;
   double t=0;
   long point=0;//количество точек
   unsigned short *t_ptr=ThermalImagePtr;
   float *tmr_ptr=TemperatureImagePtr;
   unsigned long *v_ptr=ViewImagePtr;
   for(y=0;y<IMAGE_HEIGHT;y++,v_ptr+=IMAGE_VIEW_WIDTH)
   {
    unsigned long *v_ptr_local=v_ptr;
    for(x=0;x<IMAGE_WIDTH;x++,t_ptr++,v_ptr_local++,tmr_ptr++)
	{
	 //вычисляем температуру
     double pt;//температура в точке
	 *tmr_ptr=-100000;
	 if (GetTemperature(*t_ptr,pt)==false) continue;//ошибка вычисления температуры
	 *tmr_ptr=pt;
	 if (pt>t_max) t_max=pt;
	 if (pt<t_min) t_min=pt;
	 if (x>=IMAGE_WIDTH/2-FRAME_SIZE && x<=IMAGE_WIDTH/2+FRAME_SIZE)
	 {
      if (y>=IMAGE_HEIGHT/2-FRAME_SIZE && y<=IMAGE_HEIGHT/2+FRAME_SIZE)
	  {
       point++;
	   t+=pt;
	  }
	 }
	}
   }
   if (point!=0)
   {
    t/=static_cast<double>(point);
    //выводим температуру
	if (t>=-999 && t<=999)
	{
     sprintf(string,"%.1f!",t);
	 cGraphics.PutString(IMAGE_WIDTH+2,IMAGE_VIEW_HEIGHT-FONT_HEIGHT-2,string,0xFFFFFF);
	}
   }
   //выводим минимальную температуру
   if (t_min>=-999 && t_min<=999)
   {
    sprintf(string,"%.1f!",t_min);
    cGraphics.PutString(IMAGE_WIDTH+2,IMAGE_VIEW_HEIGHT-FONT_HEIGHT*2-2-4,string,0xFFFFFF);
   }
   //выводим максимальную температуру
   if (t_max>=-999 && t_max<=999)
   {
    sprintf(string,"%.1f!",t_max);
 	cGraphics.PutString(IMAGE_WIDTH+2,2,string,0xFFFFFF);
   }
   if (SaveImageCross==false) memcpy(SaveViewImagePtr,ViewImagePtr,IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT*sizeof(unsigned long));
   //рисуем рамку
   unsigned long *v_ptr_horizontal=ViewImagePtr+(IMAGE_VIEW_HEIGHT/2-FRAME_SIZE)*IMAGE_VIEW_WIDTH+IMAGE_WIDTH/2-FRAME_SIZE;
   unsigned long *v_ptr_vertical=v_ptr_horizontal;
   for(long n=0;n<=FRAME_SIZE*2;n++,v_ptr_horizontal++,v_ptr_vertical+=IMAGE_VIEW_WIDTH)
   {
    *v_ptr_horizontal^=0x00FFFFFF;
    *(v_ptr_horizontal+FRAME_SIZE*2*IMAGE_VIEW_WIDTH)^=0x00FFFFFF;
    if (v_ptr_horizontal==v_ptr_vertical) continue;
	if (v_ptr_horizontal+FRAME_SIZE*2*IMAGE_VIEW_WIDTH==v_ptr_vertical+FRAME_SIZE*2) continue;
    *v_ptr_vertical^=0x00FFFFFF;
    *(v_ptr_vertical+FRAME_SIZE*2)^=0x00FFFFFF;
   }
   if (SaveImageCross==true) memcpy(SaveViewImagePtr,ViewImagePtr,IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT*sizeof(unsigned long));
   if (SaveAllFrame==true) OnButton_SaveFrame();//сохраняем кадр
   //копируем изображение
   cIImage_ViewPtr->SetRGBAImage(IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT,vector<unsigned long>(ViewImagePtr,ViewImagePtr+IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT-1));
   LastReceivedFrameIndex=index;
   InvalidateRect(NULL,FALSE);
  }
 }
 CDialog::OnTimer(nIDEvent);
}


afx_msg void CDialog_Main::OnPaint(void)
{
 CDialog::OnPaint();
 CPaintDC cPaintDC_Main(((CStatic *)GetDlgItem(IDC_STATIC_MAIN)));
 CPaintDC cPaintDC_Video(((CStatic *)GetDlgItem(IDC_STATIC_VIDEO)));

 CRect cRect;
 ((CStatic *)GetDlgItem(IDC_STATIC_MAIN))->GetClientRect(&cRect);
 //выбираем максимальную сторону
 unsigned long width=cRect.right-cRect.left;
 unsigned long height=cRect.bottom-cRect.top;
 
 if (width<height) width=height*IMAGE_VIEW_WIDTH/IMAGE_VIEW_HEIGHT;
              else height=width*IMAGE_VIEW_HEIGHT/IMAGE_VIEW_WIDTH;

 cRect.right=cRect.left+width;
 cRect.bottom=cRect.top+height;
 
 BITMAPINFOHEADER bmih;
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=IMAGE_VIEW_WIDTH;
 bmih.biHeight=-IMAGE_VIEW_HEIGHT;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;
 RGBQUAD rgbq;
 BITMAPINFO info;
 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 StretchDIBits(cPaintDC_Main,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),ViewImagePtr,&info,DIB_RGB_COLORS,SRCCOPY);

 ((CStatic *)GetDlgItem(IDC_STATIC_VIDEO))->GetClientRect(&cRect);
 //выбираем максимальную сторону
 width=cRect.right-cRect.left;
 height=cRect.bottom-cRect.top;
 
 if (width<height) width=height*VIDEO_WIDTH/VIDEO_HEIGHT;
              else height=width*VIDEO_HEIGHT/VIDEO_WIDTH;
 cRect.right=cRect.left+width;
 cRect.bottom=cRect.top+height; 
 
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=VIDEO_WIDTH;
 bmih.biHeight=-VIDEO_HEIGHT;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;

 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 StretchDIBits(cPaintDC_Video,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),VideoImagePtr,&info,DIB_RGB_COLORS,SRCCOPY);


	/*
 //со сглаживанием

 CDialog::OnPaint();
 CPaintDC cPaintDC_Main(((CStatic *)GetDlgItem(IDC_STATIC_MAIN)));
 CPaintDC cPaintDC_Video(((CStatic *)GetDlgItem(IDC_STATIC_VIDEO)));
 
 CRect cRect;
 ((CStatic *)GetDlgItem(IDC_STATIC_MAIN))->GetClientRect(&cRect);
 //выбираем максимальную сторону
 unsigned long width=cRect.right-cRect.left;
 unsigned long height=cRect.bottom-cRect.top;
 
 if (width<height) width=height*IMAGE_VIEW_WIDTH/IMAGE_VIEW_HEIGHT;
              else height=width*IMAGE_VIEW_HEIGHT/IMAGE_VIEW_WIDTH;

 cRect.right=cRect.left+width;
 cRect.bottom=cRect.top+height;

 vector<unsigned long> vector_view;
 cIImage_ViewPtr->SetSize(width,height);
 cIImage_ViewPtr->GetRGBAImage(width,height,vector_view);
 
 BITMAPINFOHEADER bmih;
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=width;
 bmih.biHeight=-height;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;
 RGBQUAD rgbq;
 BITMAPINFO info;
 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 StretchDIBits(cPaintDC_Main,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),&vector_view[0],&info,DIB_RGB_COLORS,SRCCOPY);

 ((CStatic *)GetDlgItem(IDC_STATIC_VIDEO))->GetClientRect(&cRect);
 //выбираем максимальную сторону
 width=cRect.right-cRect.left;
 height=cRect.bottom-cRect.top;
 
 if (width<height) width=height*VIDEO_WIDTH/VIDEO_HEIGHT;
              else height=width*VIDEO_HEIGHT/VIDEO_WIDTH;
 cRect.right=cRect.left+width;
 cRect.bottom=cRect.top+height; 

 vector<unsigned long> vector_video;
 cIImage_VideoPtr->SetSize(width,height);
 cIImage_VideoPtr->GetRGBAImage(width,height,vector_video);

 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=width;
 bmih.biHeight=-height;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;

 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 StretchDIBits(cPaintDC_Video,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),&vector_video[0],&info,DIB_RGB_COLORS,SRCCOPY);
 */
}
//---------------------------------------------------------------------------
//сохранить кадр
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveFrame(void)
{
 SaveColorImage();
 if (SaveRAW==true) SaveThermalImage();
 if (SaveVideo==true) SaveVideoImage();
}
//---------------------------------------------------------------------------
//сохранять все кадры
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveAllFrame(void)
{
 if (SaveAllFrame==true) 
 {
  SaveAllFrame=false;
  cColorButton_SaveAllFrame.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  SaveAllFrame=true;
  cColorButton_SaveAllFrame.SetBackGroundColor(RGB(128,255,128));
 }
}
//---------------------------------------------------------------------------
//сохранять изображения без шкалы
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveImageNoScale(void)
{
 if (SaveImageNoScale==true) 
 {
  SaveImageNoScale=false;
  cColorButton_SaveImageNoScale.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  SaveImageNoScale=true;
  cColorButton_SaveImageNoScale.SetBackGroundColor(RGB(128,255,128));
 }
}
//---------------------------------------------------------------------------
//рисовать перекрестье
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveImageCross(void)
{
 if (SaveImageCross==true) 
 {
  SaveImageCross=false;
  cColorButton_SaveImageCross.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  SaveImageCross=true;
  cColorButton_SaveImageCross.SetBackGroundColor(RGB(128,255,128));
 }
}
//---------------------------------------------------------------------------
//сохранять RAW
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveRAW(void)
{
 if (SaveRAW==true) 
 {
  SaveRAW=false;
  cColorButton_SaveRAW.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  SaveRAW=true;
  cColorButton_SaveRAW.SetBackGroundColor(RGB(128,255,128));
 }
}

//---------------------------------------------------------------------------
//сохранять кадры с видеокамеры
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveVideo(void)
{
 if (SaveVideo==true) 
 {
  SaveVideo=false;
  cColorButton_SaveVideo.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  SaveVideo=true;
  cColorButton_SaveVideo.SetBackGroundColor(RGB(128,255,128));
 }
}
//---------------------------------------------------------------------------
//сохранять кадры с видеокамеры
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_ShowVideo(void)
{
 if (ShowVideo==true) 
 {
  ShowVideo=false;
  cColorButton_ShowVideo.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  ShowVideo=true;
  cColorButton_ShowVideo.SetBackGroundColor(RGB(128,255,128));
 }
 cFlirOneControl.SetShowVideo(ShowVideo);
}
//---------------------------------------------------------------------------
//применить палитру
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_ApplyPalette(void)
{
 long index=((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->GetCurSel();
 if (vector_PaletteFileName.size()<=index) return;
 cFlirOneControl.LoadColorMap(const_cast<char*>(vector_PaletteFileName[index].c_str())); 
}

//---------------------------------------------------------------------------
//вычислить температуру
//---------------------------------------------------------------------------
bool CDialog_Main::GetTemperature(unsigned short raw14,double &value)
{
 if (raw14>0x10000) return(false);
 raw14*=4;    
 double zn=(PlanckR2*(exp(PlanckB/(TempReflected+273.15))-PlanckF));
 if (fabs(zn)<EPS) return(false);
 double RAWrefl=PlanckR1/zn-PlanckO;
 double RAWobj=(raw14-(1-Emissivity)*RAWrefl)/Emissivity;
 double lgr=PlanckR1/(PlanckR2*(RAWobj+PlanckO)+PlanckF);
 if (lgr<=EPS) return(false);
 value=PlanckB/log(lgr)-273.15; 
 return(true);
}

//---------------------------------------------------------------------------
//сохранить раскрашенное изображение
//---------------------------------------------------------------------------
void CDialog_Main::SaveColorImage(void)
{
 CreateDirectory("Image",NULL);
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[1024];
 for(long index=1;index<1000;index++)	
 {
  sprintf(filename,"Image\\img[%04i.%02i.%02i %02i_%02i_%02i-%03ld].tga",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   if (SaveImageNoScale==false) SaveTGA(filename,IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT,reinterpret_cast<unsigned char*>(SaveViewImagePtr));
                           else SaveTGA(filename,IMAGE_WIDTH,IMAGE_HEIGHT,reinterpret_cast<unsigned char*>(ColorImagePtr));
   break;
  }
  else fclose(file);     
 }
}

//---------------------------------------------------------------------------
//сохранить кадр с видеокамеры
//---------------------------------------------------------------------------
void CDialog_Main::SaveVideoImage(void)
{
 CreateDirectory("Image",NULL);
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[1024];
 for(long index=1;index<1000;index++)	
 {
  sprintf(filename,"Image\\frame[%04i.%02i.%02i %02i_%02i_%02i-%03ld].tga",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   SaveTGA(filename,VIDEO_WIDTH,VIDEO_HEIGHT,reinterpret_cast<unsigned char*>(VideoImagePtr));
   break;
  }
  else fclose(file);     
 }
}

//---------------------------------------------------------------------------
//сохранить тепловое изображение
//---------------------------------------------------------------------------
void CDialog_Main::SaveThermalImage(void)
{
 CreateDirectory("Image",NULL);
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[1024];
 for(long index=1;index<1000;index++)	
 {
  sprintf(filename,"Image\\img[%04i.%02i.%02i %02i_%02i_%02i-%03ld].raw",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   file=fopen(filename,"wb");
   #pragma pack(1)
   struct SHeader
   {
    unsigned long Width;//ширина
	unsigned long Height;//высота
    //параметры для расчёта температуры
    double PlanckR1;
    double PlanckB;
    double PlanckF;
    double PlanckO;
    double PlanckR2;
    double TempReflected;//температура болометров
    double Emissivity;//коэффициент излучения
   } sHeader;
   #pragma pack()
   sHeader.Width=IMAGE_WIDTH;
   sHeader.Height=IMAGE_HEIGHT;
   sHeader.PlanckR1=PlanckR1;
   sHeader.PlanckB=PlanckB;
   sHeader.PlanckF=PlanckF;
   sHeader.PlanckO=PlanckO;
   sHeader.PlanckR2=PlanckR2;
   sHeader.TempReflected=TempReflected;
   sHeader.Emissivity=Emissivity;
   fwrite(&sHeader,1,sizeof(SHeader),file);//записываем заголовок
   //записываем данные
   fwrite(ThermalImagePtr,THERMAL_IMAGE_SIZE_SHORT,sizeof(unsigned short),file);
   fclose(file);  	
   break;
  }
  else fclose(file);     
 }
}

//-Новые функции класса------------------------------------------------------

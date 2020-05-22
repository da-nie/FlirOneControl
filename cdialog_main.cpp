#include "cdialog_main.h"
#include "system.h"
#include "tga.h"

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
 uint32_t color=COLOR_BLACK;
 ViewImage.resize(IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT,color);
 SaveViewImage.resize(IMAGE_VIEW_WIDTH*IMAGE_VIEW_HEIGHT,color);
 ColorImage.resize(CFlirOneReceiver::IMAGE_WIDTH*CFlirOneReceiver::IMAGE_HEIGHT,color);
 VideoImage.resize(CFlirOneReceiver::VIDEO_WIDTH*CFlirOneReceiver::VIDEO_HEIGHT,color);
 ThermalImage.resize(CFlirOneReceiver::IMAGE_WIDTH*CFlirOneReceiver::IMAGE_HEIGHT,0);

 TemperatureImage.resize(CFlirOneReceiver::IMAGE_WIDTH*CFlirOneReceiver::IMAGE_HEIGHT,0);
 cGraphics.Init(&ViewImage[0],IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);

 LastReceivedFrameIndex=0;

 iImage_VideoPtr=new CPicture;
 iImage_VideoPtr->SetSize(CFlirOneReceiver::VIDEO_WIDTH,CFlirOneReceiver::VIDEO_HEIGHT);
 //iImage_VideoPtr=new CDecorator_Scale(iImage_VideoPtr,CFlirOneReceiver::VIDEO_WIDTH,CFlirOneReceiver::VIDEO_HEIGHT);

 iImage_ViewPtr=new CPicture;
 iImage_ViewPtr->SetSize(IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);
 //iImage_ViewPtr=new CDecorator_Scale(iImage_ViewPtr,IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT);

 cFlirOneControl.LoadColorMap(".\\Palette\\iron.pal");//загружаем палитру

 PlanckR1=16528.178;
 PlanckB=1427.5;
 PlanckF=1.0;
 PlanckO=-1307.0;
 PlanckR2=0.012258549;

 TempReflected=20;
 Emissivity=0.95;

 AlarmMaxTemperCounter=ALARM_MAX_TEMPER_COUNTER_MAX_VALUE;
 AlarmMinTemperCounter=ALARM_MIN_TEMPER_COUNTER_MAX_VALUE;
}
//-Деструктор класса---------------------------------------------------------
CDialog_Main::~CDialog_Main()
{
 delete(iImage_VideoPtr);
 delete(iImage_ViewPtr);
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

 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MIN_T))->SetWindowText("-100");
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MIN_T))->SetLimitText(10);
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MAX_T))->SetWindowText("100");
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MAX_T))->SetLimitText(10);
   
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

 FindPalette();

 SetState(SaveAllFrame,cColorButton_SaveAllFrame,false);
 SetState(SaveImageNoScale,cColorButton_SaveImageNoScale,false);
 SetState(SaveImageCross,cColorButton_SaveImageCross,false);
 SetState(SaveRAW,cColorButton_SaveRAW,true);
 SetState(SaveVideo,cColorButton_SaveVideo,true);
 SetState(ShowVideo,cColorButton_ShowVideo,true);
 cFlirOneControl.SetShowVideo(ShowVideo);

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
 if (nIDEvent==ID_TIMER_UPDATE) 
 {
  //копируем изображение
  uint32_t index;
  //получаем тепловое изображение
  cFlirOneControl.CopyThermalImage(ThermalImage,index);
  //получаем раскрашенное изображение
  cFlirOneControl.CopyColorImage(ColorImage,index);
  //получаем видео изображение
  cFlirOneControl.CopyVideoImage(VideoImage,index);
  if (LastReceivedFrameIndex!=index)//изображение изменилось
  { 
   NewFrame();
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

 StretchDIBits(cPaintDC_Main,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),&ViewImage[0],&info,DIB_RGB_COLORS,SRCCOPY);

 ((CStatic *)GetDlgItem(IDC_STATIC_VIDEO))->GetClientRect(&cRect);
 //выбираем максимальную сторону
 width=cRect.right-cRect.left;
 height=cRect.bottom-cRect.top;
 
 if (width<height) width=height*CFlirOneReceiver::VIDEO_WIDTH/CFlirOneReceiver::VIDEO_HEIGHT;
              else height=width*CFlirOneReceiver::VIDEO_HEIGHT/CFlirOneReceiver::VIDEO_WIDTH;
 cRect.right=cRect.left+width;
 cRect.bottom=cRect.top+height; 
 
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=CFlirOneReceiver::VIDEO_WIDTH;
 bmih.biHeight=-CFlirOneReceiver::VIDEO_HEIGHT;
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

 StretchDIBits(cPaintDC_Video,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),&VideoImage[0],&info,DIB_RGB_COLORS,SRCCOPY);


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
 ChangeState(SaveAllFrame,cColorButton_SaveAllFrame);
}
//---------------------------------------------------------------------------
//сохранять изображения без шкалы
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveImageNoScale(void)
{
 ChangeState(SaveImageNoScale,cColorButton_SaveImageNoScale);
}
//---------------------------------------------------------------------------
//рисовать перекрестье
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveImageCross(void)
{
 ChangeState(SaveImageCross,cColorButton_SaveImageCross);
}
//---------------------------------------------------------------------------
//сохранять RAW
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveRAW(void)
{
 ChangeState(SaveRAW,cColorButton_SaveRAW);
}

//---------------------------------------------------------------------------
//сохранять кадры с видеокамеры
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_SaveVideo(void)
{
 ChangeState(SaveVideo,cColorButton_SaveVideo);
}
//---------------------------------------------------------------------------
//сохранять кадры с видеокамеры
//---------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_ShowVideo(void)
{
 ChangeState(ShowVideo,cColorButton_ShowVideo);
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


//----------------------------------------------------------------------------------------------------
//новый кадр
//----------------------------------------------------------------------------------------------------
void CDialog_Main::NewFrame(void)
{
 static const float T_MAX_INITIAL_VALUE=-10000;//начальное значение для максимальной температуры
 static const float T_MIN_INITIAL_VALUE=10000;//начальное значение для минимальной температуры
 static const float T_MIN_VALID_VALUE=-999;//минимальное допустимое значение температуры
 static const float T_MAX_VALID_VALUE=999;//максимальное допустимое значение температуры

 //считываем температуру болометров и коэффициент излучения
 char string[STRING_BUFFER_SIZE];
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_TEMPERATURE))->GetWindowText(string,STRING_BUFFER_SIZE);
 TempReflected=atof(string);
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_EMISSION))->GetWindowText(string,STRING_BUFFER_SIZE);
 Emissivity=atof(string);

 //копируем кадр видео
 iImage_VideoPtr->SetRGBAImage(CFlirOneReceiver::VIDEO_WIDTH,CFlirOneReceiver::VIDEO_HEIGHT,VideoImage);
 //копируем изображение
 uint32_t *v1_ptr=&ViewImage[0];
 uint32_t *v2_ptr=&ColorImage[0];
 for(int32_t y=0;y<CFlirOneReceiver::IMAGE_HEIGHT;y++,v1_ptr+=IMAGE_VIEW_WIDTH,v2_ptr+=CFlirOneReceiver::IMAGE_WIDTH)
 {
  uint32_t *v1_ptr_local=v1_ptr;
  uint32_t *v2_ptr_local=v2_ptr;
  for(int32_t x=0;x<CFlirOneReceiver::IMAGE_WIDTH;x++,v1_ptr_local++,v2_ptr_local++) *v1_ptr_local=*v2_ptr_local;
 }
 //очищаем места для максимальной и минимальной температуры
 cGraphics.SolidFill(CFlirOneReceiver::IMAGE_WIDTH+2,CFlirOneReceiver::IMAGE_HEIGHT-CGraphics::FONT_HEIGHT*2-2-4,IMAGE_VIEW_WIDTH-(CFlirOneReceiver::IMAGE_WIDTH+2),CGraphics::FONT_HEIGHT*2+4,COLOR_BLACK);
 cGraphics.SolidFill(CFlirOneReceiver::IMAGE_WIDTH+2,2,IMAGE_VIEW_WIDTH-(CFlirOneReceiver::IMAGE_WIDTH+2),CGraphics::FONT_HEIGHT,COLOR_BLACK);
 //рисуем градиентную шкалу
 uint32_t scale_height=IMAGE_VIEW_HEIGHT-8-CGraphics::FONT_HEIGHT*3-4;
 float c_index=CFlirOneReceiver::MAX_COLOR_INDEX;
 float dc_index=static_cast<float>(CFlirOneReceiver::MAX_COLOR_INDEX)/static_cast<float>(scale_height);
 v1_ptr=&ViewImage[0]+(4+CGraphics::FONT_HEIGHT)*IMAGE_VIEW_WIDTH+CFlirOneReceiver::IMAGE_WIDTH+2;
 uint8_t R[CFlirOneReceiver::COLOR_MAP_UNIT];
 uint8_t G[CFlirOneReceiver::COLOR_MAP_UNIT];
 uint8_t B[CFlirOneReceiver::COLOR_MAP_UNIT];
 cFlirOneControl.CopyColorMap(R,G,B,CFlirOneReceiver::COLOR_MAP_UNIT);
 for(int32_t y=0;y<scale_height;y++,v1_ptr+=IMAGE_VIEW_WIDTH,c_index-=dc_index)
 {
  uint32_t index=static_cast<uint32_t>(c_index);
  uint32_t color=0;
  color|=0xff;
  color<<=8;
  color|=R[index];
  color<<=8;
  color|=G[index];
  color<<=8;
  color|=B[index];
  uint32_t *v1_ptr_local=v1_ptr;
  for(int32_t x=0;x<IMAGE_VIEW_WIDTH-CFlirOneReceiver::IMAGE_WIDTH-4;x++,v1_ptr_local++) *v1_ptr_local=color;
 }
 //считаем температуру по кадру 
 double t_max=T_MAX_INITIAL_VALUE;
 double t_min=T_MIN_INITIAL_VALUE;
 double t=0;
 long point=0;//количество точек
 uint16_t *t_ptr=&ThermalImage[0];
 float *tmr_ptr=&TemperatureImage[0];
 uint32_t *v_ptr=&ViewImage[0];
 for(int32_t y=0;y<CFlirOneReceiver::IMAGE_HEIGHT;y++,v_ptr+=IMAGE_VIEW_WIDTH)
 {
  uint32_t *v_ptr_local=v_ptr;
  for(int32_t x=0;x<CFlirOneReceiver::IMAGE_WIDTH;x++,t_ptr++,v_ptr_local++,tmr_ptr++)
  {
   //вычисляем температуру
   double pt;//температура в точке
   *tmr_ptr=T_MAX_INITIAL_VALUE;
   if (GetTemperature(*t_ptr,pt)==false) continue;//ошибка вычисления температуры
   *tmr_ptr=pt;
   if (pt>t_max) t_max=pt;
   if (pt<t_min) t_min=pt;
   if (x>=CFlirOneReceiver::IMAGE_WIDTH/2-FRAME_SIZE && x<=CFlirOneReceiver::IMAGE_WIDTH/2+FRAME_SIZE)
   {
    if (y>=CFlirOneReceiver::IMAGE_HEIGHT/2-FRAME_SIZE && y<=CFlirOneReceiver::IMAGE_HEIGHT/2+FRAME_SIZE)
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
  if (t>=T_MIN_VALID_VALUE && t<=T_MAX_VALID_VALUE)
  {
   sprintf(string,"%.1f!",t);
   cGraphics.PutString(CFlirOneReceiver::IMAGE_WIDTH+2,IMAGE_VIEW_HEIGHT-CGraphics::FONT_HEIGHT-2,string,COLOR_WHITE);
  }
 }
 bool alarm_save_frame=false;
 //выводим минимальную температуру
 if (t_min>=T_MIN_VALID_VALUE && t_min<=T_MAX_VALID_VALUE)
 {
  sprintf(string,"%.1f!",t_min);
  cGraphics.PutString(CFlirOneReceiver::IMAGE_WIDTH+2,IMAGE_VIEW_HEIGHT-CGraphics::FONT_HEIGHT*2-2-4,string,COLOR_WHITE);
  if (MinTemperControl(t_min)==true)
  {
   if (AlarmMinTemperCounter>0) AlarmMinTemperCounter--;
   if (AlarmMinTemperCounter==0)
   {	 
    if (((CButton *)GetDlgItem(IDC_CHECK_MAIN_ALARM_SAVE_FRAME))->GetCheck()==TRUE) alarm_save_frame=true;
   }
  }
  else AlarmMinTemperCounter=ALARM_MIN_TEMPER_COUNTER_MAX_VALUE;
 }
 //выводим максимальную температуру
 if (t_max>=T_MIN_VALID_VALUE && t_max<=T_MAX_VALID_VALUE)
 {
  sprintf(string,"%.1f!",t_max);
  cGraphics.PutString(CFlirOneReceiver::IMAGE_WIDTH+2,2,string,COLOR_WHITE);
  if (MaxTemperControl(t_max)==true)
  {
   if (AlarmMaxTemperCounter>0) AlarmMaxTemperCounter--;
   if (AlarmMaxTemperCounter==0)
   {
    if (((CButton *)GetDlgItem(IDC_CHECK_MAIN_ALARM_SAVE_FRAME))->GetCheck()==TRUE) alarm_save_frame=true;
   }
  }
  else AlarmMaxTemperCounter=ALARM_MAX_TEMPER_COUNTER_MAX_VALUE;
 }
 if (SaveImageCross==false) SaveViewImage=ViewImage;
 //рисуем рамку
 uint32_t *v_ptr_horizontal=&ViewImage[0]+(IMAGE_VIEW_HEIGHT/2-FRAME_SIZE)*IMAGE_VIEW_WIDTH+CFlirOneReceiver::IMAGE_WIDTH/2-FRAME_SIZE;
 uint32_t *v_ptr_vertical=v_ptr_horizontal;
 for(int32_t n=0;n<=FRAME_SIZE*2;n++,v_ptr_horizontal++,v_ptr_vertical+=IMAGE_VIEW_WIDTH)
 {
  *v_ptr_horizontal^=COLOR_WHITE;
  *(v_ptr_horizontal+FRAME_SIZE*2*IMAGE_VIEW_WIDTH)^=COLOR_WHITE;
  if (v_ptr_horizontal==v_ptr_vertical) continue;
  if (v_ptr_horizontal+FRAME_SIZE*2*IMAGE_VIEW_WIDTH==v_ptr_vertical+FRAME_SIZE*2) continue;
  *v_ptr_vertical^=COLOR_WHITE;
  *(v_ptr_vertical+FRAME_SIZE*2)^=COLOR_WHITE;
 }
 if (SaveImageCross==true) SaveViewImage=ViewImage;
 if (SaveAllFrame==true || alarm_save_frame==true)  OnButton_SaveFrame();//сохраняем кадр
 //копируем изображение
 iImage_ViewPtr->SetRGBAImage(IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT,ViewImage);
}


//----------------------------------------------------------------------------------------------------
//контроль минимальной температуры
//----------------------------------------------------------------------------------------------------
bool CDialog_Main::MinTemperControl(float t)
{
 char string[STRING_BUFFER_SIZE];
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MIN_T))->GetWindowText(string,STRING_BUFFER_SIZE);
 float min_t=atof(string);
 if (t<=min_t) return(true);
 return(false);
}
//----------------------------------------------------------------------------------------------------
//контроль максимальной температуры
//----------------------------------------------------------------------------------------------------
bool CDialog_Main::MaxTemperControl(float t)
{
 char string[STRING_BUFFER_SIZE];
 ((CEdit *)GetDlgItem(IDC_EDIT_MAIN_MAX_T))->GetWindowText(string,STRING_BUFFER_SIZE);
 float max_t=atof(string);
 if (t>=max_t) return(true);
 return(false);
}
//----------------------------------------------------------------------------------------------------
//поиск палитр
//----------------------------------------------------------------------------------------------------
void CDialog_Main::FindPalette(void)
{
 ((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->ResetContent(); 
 std::vector<std::string> vector_file_name;
 std::vector<std::string> vector_file_name_without_path;
 CreateFileList("./Palette",vector_file_name,vector_file_name_without_path);
 vector_PaletteFileName.clear();
 static const size_t MIN_FILE_NAME_SIZE=4;//минимальная длина имени файла
 for(size_t n=0;n<vector_file_name.size();n++)
 {
  std::string &file_name=vector_file_name[n];
  size_t length=file_name.length();
  if (length<MIN_FILE_NAME_SIZE) continue;
  if (file_name[length-1]!='l' && file_name[length-1]!='L') continue;
  if (file_name[length-2]!='a' && file_name[length-1]!='A') continue;
  if (file_name[length-3]!='p' && file_name[length-1]!='P') continue;
  if (file_name[length-4]!='.') continue;
  vector_PaletteFileName.push_back(file_name);
  ((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->AddString(vector_file_name_without_path[n].c_str());
 }
 if (vector_file_name.size()>0) ((CComboBox *)GetDlgItem(IDC_COMBO_MAIN_PALETTE))->SetCurSel(0);
}

//----------------------------------------------------------------------------------------------------
//смена состояния кнопки и значения флага
//----------------------------------------------------------------------------------------------------
void CDialog_Main::ChangeState(bool &state,CColorButton &cColorButton)
{
 if (state==true)
 {
  state=false;
  cColorButton.SetBackGroundColor(RGB(192,192,192));
 }
 else
 {
  state=true;
  cColorButton.SetBackGroundColor(RGB(128,255,128));
 }
}

//----------------------------------------------------------------------------------------------------
//задать состояние кнопки и флага
//----------------------------------------------------------------------------------------------------
void CDialog_Main::SetState(bool &state,CColorButton &cColorButton,bool set_state)
{
 state=set_state; 
 if (state==false) cColorButton.SetBackGroundColor(RGB(192,192,192));
              else cColorButton.SetBackGroundColor(RGB(128,255,128));
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
 MakeDirectory("./Image");
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[PATH_STRING_BUFFER_SIZE];
 for(long index=1;index<1000;index++)
 {
  sprintf(filename,"./Image/img[%04i.%02i.%02i %02i_%02i_%02i-%03ld].tga",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   if (SaveImageNoScale==false) SaveTGA(filename,IMAGE_VIEW_WIDTH,IMAGE_VIEW_HEIGHT,reinterpret_cast<uint8_t*>(&SaveViewImage[0]));
                           else SaveTGA(filename,CFlirOneReceiver::IMAGE_WIDTH,CFlirOneReceiver::IMAGE_HEIGHT,reinterpret_cast<uint8_t*>(&ColorImage[0]));
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
 static const int32_t MIN_FILE_INDEX=1;//минимальный индекс файла для одинакового имени
 static const int32_t MAX_FILE_INDEX=100;//максимальный индекс файла для одинакового имени

 MakeDirectory("./Image");
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[PATH_STRING_BUFFER_SIZE];
 for(int32_t index=MIN_FILE_INDEX;index<MAX_FILE_INDEX;index++)
 {
  sprintf(filename,"./Image/frame[%04i.%02i.%02i %02i_%02i_%02i-%03ld].tga",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   SaveTGA(filename,CFlirOneReceiver::VIDEO_WIDTH,CFlirOneReceiver::VIDEO_HEIGHT,reinterpret_cast<uint8_t*>(&VideoImage[0]));
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
static const int32_t MIN_FILE_INDEX=1;//минимальный индекс файла для одинакового имени
 static const int32_t MAX_FILE_INDEX=100;//максимальный индекс файла для одинакового имени

 MakeDirectory("./Image");
 //считываем дату и время
 time_t time_main=time(NULL);
 tm *tm_main=localtime(&time_main);
 //ищем свободный файл
 char filename[PATH_STRING_BUFFER_SIZE];
 for(int32_t index=MIN_FILE_INDEX;index<MAX_FILE_INDEX;index++)
 {
  sprintf(filename,"./Image/img[%04i.%02i.%02i %02i_%02i_%02i-%03ld].raw",tm_main->tm_year+1900,tm_main->tm_mon+1,tm_main->tm_mday,tm_main->tm_hour,tm_main->tm_min,tm_main->tm_sec,index);
  FILE *file=fopen(filename,"rb");
  if (file==NULL)//такого файла ещё нет
  {
   file=fopen(filename,"wb");
   #pragma pack(1)
   struct SHeader
   {
    uint32_t Width;//ширина
    uint32_t Height;//высота
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
   sHeader.Width=CFlirOneReceiver::IMAGE_WIDTH;
   sHeader.Height=CFlirOneReceiver::IMAGE_HEIGHT;
   sHeader.PlanckR1=PlanckR1;
   sHeader.PlanckB=PlanckB;
   sHeader.PlanckF=PlanckF;
   sHeader.PlanckO=PlanckO;
   sHeader.PlanckR2=PlanckR2;
   sHeader.TempReflected=TempReflected;
   sHeader.Emissivity=Emissivity;
   fwrite(&sHeader,1,sizeof(SHeader),file);//записываем заголовок
   //записываем данные
   fwrite(&ThermalImage[0],ThermalImage.size(),sizeof(uint16_t),file);
   fclose(file);
   break;
  }
  else fclose(file);
 }
}

//-Новые функции класса------------------------------------------------------

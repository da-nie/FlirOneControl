#include "ccolorbutton.h"

//-Конструктор класса--------------------------------------------------------
CColorButton::CColorButton(void)
{ 
 Color_BackGround=RGB(212,208,200);
 Color_PressedBackGround=RGB(212,208,200);
 Color_Text=RGB(0,0,0);
 Color_PressedText=RGB(0,0,0);
 FrameSize=2;
}
//-Деструктор класса---------------------------------------------------------
CColorButton::~CColorButton(void) 
{
}
//-Функции обработки сообщений класса----------------------------------------
//-Функции класса------------------------------------------------------------
BOOL CColorButton::Attach(int nID,CWnd* pParent)
{
 if (!SubclassDlgItem(nID,pParent)) return(FALSE);
 return(TRUE);
}
void CColorButton::SetBackGroundColor(COLORREF color)
{
 Color_BackGround=color;
 InvalidateRect(NULL,FALSE);
}
void CColorButton::SetPressedBackGroundColor(COLORREF color)
{
 Color_PressedBackGround=color;
 InvalidateRect(NULL,FALSE);
}
void CColorButton::SetPressedTextColor(COLORREF color)
{
 Color_PressedText=color;
 InvalidateRect(NULL,FALSE);
}
void CColorButton::SetTextColor(COLORREF color)
{
 Color_Text=color;
 InvalidateRect(NULL,FALSE);
}
void CColorButton::SetFrameSize(int size)
{
 FrameSize=size;
 InvalidateRect(NULL,FALSE);
}


COLORREF CColorButton::GetBackGroundColor(void)
{
 return(Color_BackGround);
}
COLORREF CColorButton::GetPressedBackGroundColor(void)
{
 return(Color_PressedBackGround);
}
COLORREF CColorButton::GetTextColor(void)
{
 return(Color_Text);
}
COLORREF CColorButton::GetPressedTextColor(void)
{
 return(Color_PressedText);
}
int CColorButton::GetFrameSize(void)
{
 return(FrameSize);
}


void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
 CDC* pDC=CDC::FromHandle(lpDIS->hDC);	
 UINT state=lpDIS->itemState;
 CRect focusRect, btnRect;
 focusRect.CopyRect(&lpDIS->rcItem); 
 btnRect.CopyRect(&lpDIS->rcItem);
 //получаем название кнопки
 char Text[1024];
 GetWindowText(Text,1024);
 //рисуем кнопку
 DrawFilledRect(pDC,btnRect,Color_BackGround); 
 DrawFrame(pDC,btnRect,FrameSize);
 DrawButtonText(pDC,btnRect,Text,Color_Text);
 //если курсор на кнопке
 if (state&ODS_FOCUS)
 {
  DrawFocusRect(lpDIS->hDC, (LPRECT)&focusRect);
  if (state&ODS_SELECTED)
  { 
   DrawFilledRect(pDC,btnRect,Color_PressedBackGround); 
   DrawFrame(pDC,btnRect,-1);
   CRect rectPressedBtnText=btnRect;
   rectPressedBtnText.OffsetRect(1,1);
   DrawButtonText(pDC,rectPressedBtnText,Text,Color_PressedText);
   DrawFocusRect(lpDIS->hDC,(LPRECT)&focusRect);
  }
 }
 else
 {
  //если кнопка выключена	 
  if (state&ODS_DISABLED) DrawButtonText(pDC,btnRect,Text,Color_Text);
 } 
}
void CColorButton::DrawFrame(CDC *DC,CRect R,int Inset)
{ 
 COLORREF dark,light,tlColor,brColor;
 int i,m,width;
 width=Inset;
 if (Inset<0) width=-Inset;
 for (i=0;i<width;i++)
 {
  m=255/(i+2);
  dark=PALETTERGB(m,m,m);
  m=192+(63/(i+1));
  light=PALETTERGB(m,m,m);
  if (width==1)
  {
   light=RGB(255,255,255);
   dark=RGB(128,128,128);
  }
  if (Inset<0)
  {
   tlColor=dark;
   brColor=light;
  }
  else 
  {
   tlColor=light;
   brColor=dark;
  }
  DrawLine(DC,R.left,R.top,R.right,R.top,tlColor);					     
  DrawLine(DC,R.left,R.top,R.left,R.bottom,tlColor); 				     
  if ((Inset<0) && (i==width-1) && (width>1))
  {
   DrawLine(DC,R.left+1,R.bottom-1,R.right,R.bottom-1,RGB(1,1,1));
   DrawLine(DC,R.right-1,R.top+1,R.right-1,R.bottom,RGB(1,1,1));
  }
  else
  {
   DrawLine(DC,R.left+1,R.bottom-1,R.right,R.bottom-1,brColor);
   DrawLine(DC,R.right-1,R.top+1,R.right-1,R.bottom,brColor);
  }
  InflateRect(R, -1, -1);
 }
}

void CColorButton::DrawFilledRect(CDC *DC,CRect R,COLORREF color)
{ 
 CBrush B;
 B.CreateSolidBrush(color);
 DC->FillRect(R, &B);
}
void CColorButton::DrawLine(CDC *DC,CRect EndPoints,COLORREF color)
{ 
 CPen newPen;
 newPen.CreatePen(PS_SOLID,1,color);
 CPen *oldPen=DC->SelectObject(&newPen);
 DC->MoveTo(EndPoints.left,EndPoints.top);
 DC->LineTo(EndPoints.right,EndPoints.bottom);
 DC->SelectObject(oldPen);
 newPen.DeleteObject();
}
void CColorButton::DrawLine(CDC *DC,long left,long top,long right,long bottom,COLORREF color)
{ 
 CPen newPen;
 newPen.CreatePen(PS_SOLID,1,color);
 CPen *oldPen=DC->SelectObject(&newPen);
 DC->MoveTo(left,top);
 DC->LineTo(right,bottom);
 DC->SelectObject(oldPen);
 newPen.DeleteObject();
}
void CColorButton::DrawButtonText(CDC *DC,CRect R,const char *Buf,COLORREF TextColor)
{
 COLORREF prevColor=DC->SetTextColor(TextColor);
 DC->SetBkMode(TRANSPARENT);
 DC->DrawText(Buf,strlen(Buf),R, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
 DC->SetTextColor(prevColor);
}
//-Функции обработки сообщений класса----------------------------------------
//-Прочее--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CColorButton,CButton)


///\file ULButtons.cpp
///\brief cpp файл пространства имен классов кнопок(21.07.2007)
#define _WIN32_WINNT 0x0500
#include "..\..\ULLib\Include\ULButtons.h"
#include "..\..\ULLib\Include\ULRes.h"
#include <tchar.h>

namespace ULWnds
{
	namespace ULControls
	{
		namespace ULButtons
		{
			//================================================
			//Класс кнопки
			//================================================
			CULButton::CULButton(){};
			CULButton::~CULButton(){};
			BOOL CULButton::Create(HWND hParentWnd,LPCTSTR szCaption,int x,int y,int cx,int cy,
				UINT uID,DWORD dwStyle)
			{
				m_hWnd=::CreateWindow(_T("BUTTON"),szCaption,dwStyle,
					x,y,cx,cy,hParentWnd,(HMENU)(LONG_PTR)uID,ULOther::ULGetResourceHandle(),0);
				m_hParentWnd=hParentWnd;
				//Сабклассирование
				return CULSubClass::Attach(m_hWnd);
			}

			HANDLE CULButton::SetImage(enImageType it,HANDLE hImage)
			{
				return (HANDLE)SendMessage(BM_SETIMAGE,(WPARAM)it,(LPARAM)hImage);
			}

			//================================================
			//Класс родитель для CULStyleButton и CULBitmapButton
			//================================================
			CULCustomButton::CULCustomButton():
				m_wCurState(ctUp)
			{
				
				MessageMap.AddReflectMessage<CULCustomButton>(WM_DRAWITEM,&CULCustomButton::OnDrawItem);
				MessageMap.AddMessage<CULCustomButton>(WM_MOUSEMOVE,&CULCustomButton::OnMouseMove);
				MessageMap.AddMessage<CULCustomButton>(WM_MOUSELEAVE ,&CULCustomButton::OnMouseLeave);
				MessageMap.AddMessage<CULCustomButton>(WM_SETTEXT,&CULCustomButton::OnSetText);
			};
			CULCustomButton::~CULCustomButton()
			{
			};
			LRESULT CULCustomButton::OnDrawItem(WPARAM /*wParam*/,LPARAM lParam)
			{
				DRAWITEMSTRUCT* dis=(DRAWITEMSTRUCT*)lParam;	
				ULGDI::ULDC::CULDC dc=dis->hDC;
				if(dis->itemState&ctDisable)
					dc.BitBlt(0,0,dis->rcItem.right,dis->rcItem.bottom,m_MemDC.Disable,0,0,SRCCOPY);
				else
					if((dis->itemState&ctDown)||(dis->itemState&ODS_GRAYED))
						dc.BitBlt(0,0,dis->rcItem.right,dis->rcItem.bottom,m_MemDC.Down,0,0,SRCCOPY);
					else
						if(m_wCurState&ctHiLite)
							dc.BitBlt(0,0,dis->rcItem.right,dis->rcItem.bottom,m_MemDC.HiLite,0,0,SRCCOPY);
						else
							if(dis->itemState&ctFocused)
								dc.BitBlt(0,0,dis->rcItem.right,dis->rcItem.bottom,m_MemDC.Focused,0,0,SRCCOPY);
							else
								dc.BitBlt(0,0,dis->rcItem.right,dis->rcItem.bottom,m_MemDC.Up,0,0,SRCCOPY);
				dc.Detach();
				return TRUE;
			};
			LRESULT CULCustomButton::OnMouseMove(WPARAM /*Type*/,LPARAM /*CurPos*/)
			{//изменение состояния на освещенную
				//провоцируем вызов WM_MOUSELEAVE
				TRACKMOUSEEVENT tme;
				tme.cbSize=sizeof(TRACKMOUSEEVENT);
				tme.dwFlags=TME_LEAVE;
				tme.hwndTrack=*this;
				TrackMouseEvent(&tme);
				POINT pt;
				::GetCursorPos(&pt);
				if(::WindowFromPoint(pt)==*this)
					if((m_wCurState&ctHiLite)==0)
					{
						m_wCurState|=ctHiLite;
						RedrawWindow(NULL,NULL,RDW_INVALIDATE);
					}
				return FALSE;
			};
			LRESULT CULCustomButton::OnMouseLeave(WPARAM,LPARAM)
			{//изменение состояния на неосвещенную
				m_wCurState&=~ctHiLite;
				RedrawWindow(NULL,NULL,RDW_INVALIDATE);
				return FALSE;
			};
			LRESULT CULCustomButton::OnSetText(WPARAM,LPARAM /*lParam*/)
			{//для обновления стилей
				Init();
				RedrawWindow(NULL,NULL,RDW_INVALIDATE);
				return FALSE;
			}
			//================================================
			//Класс стильной кнопки
			//================================================
			CULStyleButton::CULStyleButton():			
				m_nRadius(-1)
			{
				//установка начальных цветов
				m_ButtonColors.bcDefault=(COLORREF)-1;//прозрачный
				m_ButtonColors.bcFocused=0xffaaaa;
				m_ButtonColors.bcDisable=0xaaaaaa;
				m_ButtonColors.bcDefaultText=0x000000;
				m_ButtonColors.bcHiLiteText=0xff0000;
				m_ButtonColors.bcGradientColors.gcDownSecond=0xffffff;
				m_ButtonColors.bcGradientColors.gcDownFirst=0xb3b3b3;
				m_ButtonColors.bcGradientColors.gcUpFirst=0xffffff;
				m_ButtonColors.bcGradientColors.gcUpSecond=0xb3b3b3;
				m_ButtonColors.bcGradientDest=tagButtonColors::gdUpDown;
			};
			CULStyleButton::~CULStyleButton()
			{
			};
			BOOL CULStyleButton::CreateStyleButton(HWND hParentWnd,TCHAR* szCaption,UINT uID,int x,int y,
				int cx,int cy,HFONT hFont)
			{
				//Создание кнопки
				BOOL fRet=Create(hParentWnd,szCaption,x,y,cx,cy,uID,
					WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW|WS_TABSTOP);
				if(fRet)
				{
					m_Font=hFont;
					//Инициализация стилей
					Init();
				}
				return fRet;
			}

			BOOL CULStyleButton::Attach(HWND hWnd,HFONT hFont)
			{
				m_hParentWnd=::GetParent(hWnd);
				//Сабклассирование
				if(!CULSubClass::Attach(hWnd))
					return FALSE;
				//Инициализация стилей
				ModifyStyle(0,BS_OWNERDRAW|WS_CLIPCHILDREN);
				m_Font=hFont;
				Init();
				return TRUE;
			}
			void CULStyleButton::SetButtonColors(tagButtonColors ButtonColors)
			{
				//Установка новых цветов стилей
				m_ButtonColors=ButtonColors;
				//Переинициализация стилей
				Init();
				RedrawWindow(NULL,NULL,RDW_INVALIDATE);
			}
			void CULStyleButton::GetButtonColors(tagButtonColors* ButtonColors)
			{
				*ButtonColors=m_ButtonColors;
			}
			void CULStyleButton::Init()
			{
				RECT rcButton;
				GetClientRect(&rcButton);
				TCHAR szCaption[MAX_PATH];
				int nChars=GetWindowText(szCaption,MAX_PATH);
				if(m_nRadius==-1)
					m_nRadius=((rcButton.right<rcButton.bottom)?
						rcButton.right:rcButton.bottom)/2;
				// создание региона в виде прямоугольника с закругленными краями

				ULGDI::ULGObjs::CULRgn Rgn;
				Rgn.CreateRoundRectRgn(rcButton.left, rcButton.top, rcButton.right,
					rcButton.bottom,m_nRadius, m_nRadius);
				SetWindowRgn(Rgn, TRUE);
				Rgn.DeleteRgn();
				//=====Инициализация изображения для отпущенной и подсвеченой кнопки=====
				ULGDI::ULDC::CULClientDC DeskTopDC(NULL);
				m_MemDC.Up.CreateCompatibleDC(DeskTopDC);
				m_MemDC.HiLite.CreateCompatibleDC(DeskTopDC);
				ULGDI::ULGObjs::CULBitmap MemBitmap;
				MemBitmap.CreateCompatibleBitmap(DeskTopDC,
					rcButton.right,rcButton.bottom);
				m_MemDC.Up.SelectObject((HBITMAP)MemBitmap);
				MemBitmap.CreateCompatibleBitmap(DeskTopDC,
					rcButton.right,rcButton.bottom);
				m_MemDC.HiLite.SelectObject((HBITMAP)MemBitmap);
				//инциализация градиентной заливки
				TRIVERTEX tv[2];
				GRADIENT_RECT gr;
				tv[0].x=0;
				tv[0].y=0;
				tv[0].Red=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst)<<8;
				tv[0].Green=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst>>8)<<8;
				tv[0].Blue=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst>>16)<<8;
				tv[0].Alpha=0;
				tv[1].x=rcButton.right;
				tv[1].y=rcButton.bottom;
				tv[1].Red=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond)<<8;
				tv[1].Green=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond>>8)<<8;
				tv[1].Blue=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond>>16)<<8;
				tv[1].Alpha=0;
				gr.UpperLeft  = 0;
				gr.LowerRight = 1;
				//градиентная заливка
				GradientFill(m_MemDC.Up,tv,2,&gr,1,m_ButtonColors.bcGradientDest);
				m_MemDC.HiLite.BitBlt(0,0,rcButton.right,rcButton.bottom,m_MemDC.Up,0,0,SRCCOPY);
				if(m_Font.m_hFont!=NULL)
				{
					m_MemDC.Up.SelectObject(m_Font);
					m_MemDC.Disable.SelectObject(m_Font);
					m_MemDC.Down.SelectObject(m_Font);
					m_MemDC.Focused.SelectObject(m_Font);
					m_MemDC.HiLite.SelectObject(m_Font);
					m_MemDC.Up.SelectObject(m_Font);
				}
				//отрисовка заголовка для отпущеной кнопки
				m_MemDC.Up.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				m_MemDC.Up.SetTextColor(m_ButtonColors.bcDefaultText);
				SIZE szText;
				m_MemDC.Up.GetTextExtentPoint(szCaption,nChars,&szText);
				m_MemDC.Up.TextOut((rcButton.right-szText.cx)/2,
					(rcButton.bottom-szText.cy)/2-1,szCaption,nChars);
				ULGDI::ULGObjs::CULRgn FrRgn;
				if(m_ButtonColors.bcDefault!=-1)
				{
					FrRgn.CreateRoundRectRgn(rcButton.left, rcButton.top, rcButton.right,
						rcButton.bottom,m_nRadius, m_nRadius);
					ULGDI::ULGObjs::CULBrush brDefault;
					brDefault.CreateSolidBrush(m_ButtonColors.bcDefault);
					m_MemDC.Up.FrameRgn(Rgn,brDefault,1,1);
					FrRgn.DeleteRgn();
				}
				//отрисовка заголовка для подсвеченой кнопки
				m_MemDC.HiLite.SetTextColor(m_ButtonColors.bcHiLiteText);
				m_MemDC.HiLite.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				m_MemDC.HiLite.TextOut((rcButton.right-szText.cx)/2,
					(rcButton.bottom-szText.cy)/2-1,szCaption,nChars);
				//=====Инициализация изображения для нажатой кнопки=====
				m_MemDC.Down.CreateCompatibleDC(DeskTopDC);
				MemBitmap.CreateCompatibleBitmap(DeskTopDC,
					rcButton.right,rcButton.bottom);
				m_MemDC.Down.SelectObject((HBITMAP)MemBitmap);
				//инциализация градиентной заливки
				tv[0].x=0;
				tv[0].y=0;
				tv[0].Red=BYTE(m_ButtonColors.bcGradientColors.gcDownFirst)<<8;
				tv[0].Green=BYTE(m_ButtonColors.bcGradientColors.gcDownFirst>>8)<<8;
				tv[0].Blue=BYTE(m_ButtonColors.bcGradientColors.gcDownFirst>>16)<<8;
				tv[0].Alpha=0;
				tv[1].x=rcButton.right;
				tv[1].y=rcButton.bottom;
				tv[1].Red=BYTE(m_ButtonColors.bcGradientColors.gcDownSecond)<<8;
				tv[1].Green=BYTE(m_ButtonColors.bcGradientColors.gcDownSecond>>8)<<8;
				tv[1].Blue=BYTE(m_ButtonColors.bcGradientColors.gcDownSecond>>16)<<8;
				tv[1].Alpha=0;
				gr.UpperLeft  = 0;
				gr.LowerRight = 1;
				//градиентная заливка
				m_MemDC.Down.GradientFill(tv,2,&gr,1,m_ButtonColors.bcGradientDest);
				ULGDI::ULGObjs::CULPen Pen;
				Pen.CreatePen(PS_INSIDEFRAME,1,m_ButtonColors.bcFocused);
				Pen.m_hPen=(HPEN)m_MemDC.Down.SelectObject(Pen);//запоминаем перо
				//отрисовка рамки фокуса
				FrRgn.CreateRoundRectRgn(rcButton.left, rcButton.top, rcButton.right, 
					rcButton.bottom,m_nRadius, m_nRadius);
				ULGDI::ULGObjs::CULBrush brFocus;
				brFocus.CreateSolidBrush(m_ButtonColors.bcFocused);
				m_MemDC.Down.FrameRgn(FrRgn,brFocus,1,1);			
				Pen.m_hPen=(HPEN)m_MemDC.Down.SelectObject(Pen);//возвращаем перео
				Pen.DeletePen();
				FrRgn.DeleteRgn();
				//отрисовка заголовка
				m_MemDC.Down.SetTextColor(m_ButtonColors.bcHiLiteText);
				m_MemDC.Down.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				m_MemDC.Down.TextOut((rcButton.right-szText.cx)/2,
					(rcButton.bottom-szText.cy)/2-1,szCaption,nChars);
				//=====Инициализация изображения для фокусированной кнопки=====
				m_MemDC.Focused.CreateCompatibleDC(DeskTopDC);
				MemBitmap.CreateCompatibleBitmap(DeskTopDC,
					rcButton.right,rcButton.bottom);
				m_MemDC.Focused.SelectObject((HBITMAP)MemBitmap);
				//инциализация градиентной заливки
				tv[0].x=0;
				tv[0].y=0;
				tv[0].Red=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst)<<8;
				tv[0].Green=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst>>8)<<8;
				tv[0].Blue=BYTE(m_ButtonColors.bcGradientColors.gcUpFirst>>16)<<8;
				tv[0].Alpha=0;
				tv[1].x=rcButton.right;
				tv[1].y=rcButton.bottom;
				tv[1].Red=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond)<<8;
				tv[1].Green=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond>>8)<<8;
				tv[1].Blue=BYTE(m_ButtonColors.bcGradientColors.gcUpSecond>>16)<<8;
				tv[1].Alpha=0;
				gr.UpperLeft  = 0;
				gr.LowerRight = 1;
				//градиентная заливка
				m_MemDC.Focused.GradientFill(tv,2,&gr,1,m_ButtonColors.bcGradientDest);
				if(m_Font.m_hFont!=NULL)
					m_MemDC.Focused.SelectObject(m_Font);
				Pen.CreatePen(PS_INSIDEFRAME,1,m_ButtonColors.bcFocused);
				Pen.m_hPen=(HPEN)m_MemDC.Focused.SelectObject(Pen);
				//отрисовка рамки фокуса
				FrRgn.CreateRoundRectRgn(rcButton.left, rcButton.top, rcButton.right,
					rcButton.bottom,m_nRadius, m_nRadius);
				m_MemDC.Focused.FrameRgn(FrRgn,::CreateSolidBrush(m_ButtonColors.bcFocused),1,1);
				Pen.m_hPen=(HPEN)m_MemDC.Focused.SelectObject(Pen);
				Pen.DeletePen();
				FrRgn.DeleteRgn();
				//отрисовка заголовка
				m_MemDC.Focused.SetTextColor(m_ButtonColors.bcHiLiteText);
				m_MemDC.Focused.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				m_MemDC.Focused.TextOut((rcButton.right-szText.cx)/2,
					(rcButton.bottom-szText.cy)/2-1,szCaption,nChars);
				//=====Инициализация изображения для disable кнопки=====
				m_MemDC.Disable.CreateCompatibleDC(DeskTopDC);
				MemBitmap.CreateCompatibleBitmap(DeskTopDC,
					rcButton.right,rcButton.bottom);
				m_MemDC.Disable.SelectObject((HBITMAP)MemBitmap);
				ULGDI::ULGObjs::CULBrush brDisable;
				brDisable.CreateSolidBrush(m_ButtonColors.bcDisable);
				brDisable.m_hBrush=(HBRUSH)m_MemDC.Disable.SelectObject(brDisable);//запоминаем старую кисть
				Pen.CreatePen(PS_INSIDEFRAME,2,m_ButtonColors.bcDisable);
				Pen.m_hPen=(HPEN)m_MemDC.Disable.SelectObject(Pen);//запоминаем старое перо
				Pen.DeletePen();
				m_MemDC.Disable.Rectangle(rcButton.left,rcButton.top,
					rcButton.right-1,rcButton.bottom-1);
				if(m_Font.m_hFont!=NULL)
					m_Font.m_hFont=(HFONT)m_MemDC.Disable.SelectObject(m_Font);//запоминаем старый шрифт
				//отрисовка заголовка
				m_MemDC.Disable.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				m_MemDC.Disable.SetTextColor(m_ButtonColors.bcDefaultText);
				m_MemDC.Disable.TextOut((rcButton.right-szText.cx)/2,
					(rcButton.bottom-szText.cy)/2-1,szCaption,nChars);
				brDisable.m_hBrush=(HBRUSH)m_MemDC.Disable.SelectObject(brDisable);//возвращаем перо
				Pen.m_hPen=(HPEN)m_MemDC.Disable.SelectObject(Pen);//возвращаем кисть
				if(m_Font.m_hFont!=NULL)
					m_Font.m_hFont=(HFONT)m_MemDC.Disable.SelectObject(m_Font);//возвращаем шрифт
				MemBitmap.Detach();
			}
			int CULStyleButton::GetRadius()
			{
				return m_nRadius;
			};
			void CULStyleButton::SetRadius(int nRadius)
			{
				m_nRadius=nRadius;
				Init();
			};
			//================================================
			//Класс bitmap кнопки
			//================================================
			CULBitmapButton::CULBitmapButton()
			{
				m_ButtonBitmaps.bbDisable=NULL;
				m_ButtonBitmaps.bbDown=NULL;
				m_ButtonBitmaps.bbFocused=NULL;
				m_ButtonBitmaps.bbHiLite=NULL;
				m_ButtonBitmaps.bbMask=NULL;
				m_ButtonBitmaps.bbUp=NULL;
			};
			CULBitmapButton::~CULBitmapButton()
			{
			};

			LRESULT CULBitmapButton::OnSetText(WPARAM,LPARAM /*lParam*/)
			{//перегрузка чтоб повторно не вызвать Init
				return FALSE;
			}

			BOOL CULBitmapButton::CreateBitmapButton(HWND hParentWnd,int x,int y,UINT uID,
				WORD wUpID,WORD wDownID,WORD wFocusedID,WORD wHiLiteID,WORD wDisableID,WORD wMaskID)
			{
				return CreateBitmapButton(hParentWnd,x,y,uID,
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wUpID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wDownID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wFocusedID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wHiLiteID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wDisableID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wMaskID)));
			};

			BOOL CULBitmapButton::CreateBitmapButton(HWND hParentWnd,int x,int y,UINT uID,
				HBITMAP hUp,HBITMAP hDown,HBITMAP hFocused,HBITMAP hHiLite,HBITMAP hDisable,HBITMAP hMask)
			{
				m_hParentWnd=hParentWnd;
				//сокранение хендлов изображений
				m_ButtonBitmaps.bbUp=hUp;
				m_ButtonBitmaps.bbDown=hDown;
				m_ButtonBitmaps.bbFocused=hFocused;
				m_ButtonBitmaps.bbHiLite=hHiLite;
				m_ButtonBitmaps.bbDisable=hDisable;
				m_ButtonBitmaps.bbMask=hMask;
				//получение размера изображения
				BITMAP bmp; 
				GetObject(m_ButtonBitmaps.bbUp, sizeof(BITMAP), (LPSTR)&bmp);
				//Создание кнопки
				BOOL fRet=Create(hParentWnd,NULL,x,y,bmp.bmWidth,bmp.bmHeight,uID,
					WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW|WS_TABSTOP);
				if(fRet)
				{
					//Инициализация стилей
					Init();
				}
				return fRet;
			};


			BOOL CULBitmapButton::Attach(HWND hWnd,
				WORD wUpID,WORD wDownID,WORD wFocusedID,WORD wHiLiteID,WORD wDisableID,WORD wMaskID)
			{
				return Attach(hWnd,
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wUpID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wDownID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wFocusedID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wHiLiteID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wDisableID)),
					::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(wMaskID)));
			};

			BOOL CULBitmapButton::Attach(HWND hWnd,
				HBITMAP hUp,HBITMAP hDown,HBITMAP hFocused,HBITMAP hHiLite,HBITMAP hDisable,HBITMAP hMask)
			{
				m_hParentWnd=::GetParent(hWnd);
				//сокранение хендлов изображений
				m_ButtonBitmaps.bbUp=hUp;
				m_ButtonBitmaps.bbDown=hDown;
				m_ButtonBitmaps.bbFocused=hFocused;
				m_ButtonBitmaps.bbHiLite=hHiLite;
				m_ButtonBitmaps.bbDisable=hDisable;
				m_ButtonBitmaps.bbMask=hMask;
				//сабклассирование
				if(!CULSubClass::Attach(hWnd))
					return FALSE;
				//получение размера изображения
				BITMAP bmp; 
				GetObject(m_ButtonBitmaps.bbUp, sizeof(BITMAP), (LPSTR)&bmp);
				SetWindowPos(0,0,0,bmp.bmWidth,bmp.bmHeight,SWP_NOMOVE|SWP_NOZORDER);
				//изменение стиля кнопки
				ModifyStyle(0,BS_OWNERDRAW|WS_CLIPCHILDREN);
				//инициализация контекстов и региона
				Init();
				return TRUE;
			};

			void CULBitmapButton::Init()
			{
				ULGDI::ULDC::CULDC tmpDC;
				ULGDI::ULDC::CULClientDC dc(*this);
				tmpDC.CreateCompatibleDC(dc);
				RECT rcClient;
				GetClientRect(&rcClient);
				//инициализация контекстов
				//инциализация отпущенной кнопки
				m_MemDC.Up.CreateCompatibleDC(dc);
				ULGDI::ULGObjs::CULBitmap Bmp;
				Bmp.CreateCompatibleBitmap(dc,rcClient.right,rcClient.bottom);
				m_MemDC.Up.SelectObject(Bmp);
				HBITMAP hOldBmp=(HBITMAP)tmpDC.SelectObject(m_ButtonBitmaps.bbUp);
				m_MemDC.Up.BitBlt(0,0,rcClient.right,rcClient.bottom,tmpDC,0,0,SRCCOPY);
				//инциализация нажатой кнопки
				m_MemDC.Down.CreateCompatibleDC(dc);
				Bmp.CreateCompatibleBitmap(dc,rcClient.right,rcClient.bottom);
				m_MemDC.Down.SelectObject(Bmp);
				tmpDC.SelectObject(m_ButtonBitmaps.bbDown);
				m_MemDC.Down.BitBlt(0,0,rcClient.right,rcClient.bottom,tmpDC,0,0,SRCCOPY);
				//инциализация фокусированной кнопки
				m_MemDC.Focused.CreateCompatibleDC(dc);
				Bmp.CreateCompatibleBitmap(dc,rcClient.right,rcClient.bottom);
				m_MemDC.Focused.SelectObject(Bmp);
				tmpDC.SelectObject(m_ButtonBitmaps.bbFocused);
				m_MemDC.Focused.BitBlt(0,0,rcClient.right,rcClient.bottom,tmpDC,0,0,SRCCOPY);
				//инциализация подсвеченной кнопки
				m_MemDC.HiLite.CreateCompatibleDC(dc);
				Bmp.CreateCompatibleBitmap(dc,rcClient.right,rcClient.bottom);
				m_MemDC.HiLite.SelectObject(Bmp);
				tmpDC.SelectObject(m_ButtonBitmaps.bbHiLite);
				m_MemDC.HiLite.BitBlt(0,0,rcClient.right,rcClient.bottom,tmpDC,0,0,SRCCOPY);
				//инциализация отключенной кнопки
				m_MemDC.Disable.CreateCompatibleDC(dc);
				Bmp.CreateCompatibleBitmap(dc,rcClient.right,rcClient.bottom);
				m_MemDC.Disable.SelectObject(Bmp);
				tmpDC.SelectObject(m_ButtonBitmaps.bbDisable);
				m_MemDC.Disable.BitBlt(0,0,rcClient.right,rcClient.bottom,tmpDC,0,0,SRCCOPY);
				tmpDC.SelectObject(hOldBmp);

				//создание региона по маске
				if(m_ButtonBitmaps.bbMask==NULL)
					return;
				ULGDI::ULGObjs::CULRgn Rgn;
				Rgn.CreateBitmapRgn(m_ButtonBitmaps.bbMask);
				//подключаем созданный регион к окну
				SetWindowRgn(Rgn,TRUE);
				::DeleteObject(m_ButtonBitmaps.bbUp);
				::DeleteObject(m_ButtonBitmaps.bbDown);
				::DeleteObject(m_ButtonBitmaps.bbFocused);
				::DeleteObject(m_ButtonBitmaps.bbHiLite);
				::DeleteObject(m_ButtonBitmaps.bbDisable);
				::DeleteObject(m_ButtonBitmaps.bbMask);
			}
			//================================================
			//Класс гиперссыка
			//================================================
			//================================================
			//Конструктор
			//================================================
			CULHyperLink::CULHyperLink():
				m_fClicking(false),
				m_fVisited(false)
			{
				m_nCurType=tDefault;
				m_pdwTypes[tDefault]=sNone|0xff0000;
				m_pdwTypes[tHiLite]	=sUnderLine|0xffaaaa;
				m_pdwTypes[tVisited]=sNone|0x800080;
				m_pdwTypes[tDown]	=sUnderLine|sItalic|sBold|0xaa0000;
				MessageMap.AddReflectMessage<CULHyperLink>(WM_DRAWITEM,&CULHyperLink::OnDrawItem);
				MessageMap.AddMessage<CULHyperLink>(WM_LBUTTONDOWN,&CULHyperLink::OnLButtonDown);
				MessageMap.AddMessage<CULHyperLink>(WM_LBUTTONUP,&CULHyperLink::OnLButtonUp);
				MessageMap.AddMessage<CULHyperLink>(WM_MOUSEMOVE,&CULHyperLink::OnMouseMove);
				MessageMap.AddMessage<CULHyperLink>(WM_MOUSELEAVE,&CULHyperLink::OnMouseLeave);
			};
			//================================================
			//Деструктор
			//================================================
			CULHyperLink::~CULHyperLink(){};
			//================================================
			//Функция выполняет инциализацию ссылки
			//================================================
			BOOL CULHyperLink::Init()
			{
				//изменение курсора на руку
				HCURSOR hCursor=::LoadCursor(NULL, IDC_HAND);
				SetClassLong(GCL_HCURSOR,(LONG)(LONG_PTR)hCursor);
				//сабклассирование
				BOOL fRet=CULSubClass::Attach(*this);
				ModifyStyle(0,SS_OWNERDRAW);			
	//			AutoSize(GetDC());
				return fRet;
			};
			//================================================
			//Функция для создания гиперссылки
			//hParentWnd - хендл родительского окна
			//x,y - координата кнопки
			//szName - отображаемое имя
			//szURL - URL адресс
			//uID - ID кнопки
			//если szURL равен NULL, то при нажатии на ссылку 
			//будет посылаться родитель WM_COMMAND с указанием
			//BM_CLICKED,ID и HWND контрола
			//================================================
			BOOL CULHyperLink::CreateHyperLink(HWND hParentWnd,int x,int y,LPCTSTR szName,LPCTSTR szURL,UINT uID)
			{
				m_hParentWnd=hParentWnd;
				if(szURL)
					m_strURL=szURL;
				//создание статика
				m_hWnd=::CreateWindowEx(WS_EX_TRANSPARENT,_T("STATIC"),szName,WS_CHILD|WS_CLIPCHILDREN|WS_TABSTOP|
					WS_VISIBLE|SS_NOTIFY|SS_OWNERDRAW,x,y,1,1,
					hParentWnd,(HMENU)(LONG_PTR)uID,ULOther::ULGetResourceHandle(),0);
				//инициализация гиперссылки
				return Init();
			};
			//================================================
			//Функция для создания гиперссылки на основе уже созданного статика
			//hWnd - хендл статика
			//szURL - URL адресс
			//если szURL равен NULL, то при нажатии на ссылку 
			//будет посылаться родитель WM_COMMAND с указанием
			//BM_CLICKED,ID и HWND контрола
			//================================================
			BOOL CULHyperLink::Attach(HWND hWnd,LPCTSTR szURL)
			{
				m_hParentWnd=::GetParent(hWnd);
				m_strURL=szURL;
				m_hWnd=hWnd;
				//инициализация гиперссылки
				return Init();
			};
			//================================================
			//Функция изменяет размер статика под размер надписи
			//================================================
			void CULHyperLink::AutoSize(HDC hDC)
			{
				ULGDI::ULDC::CULDC dc=hDC;
				//создание "загруженного" шрифта
				LOGFONT lf;
				if(m_Font.GetLogFont(&lf)==0)
					GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);
				//Изменияем шрифт в зависимости от состояния
				lf.lfUnderline	= (m_pdwTypes[m_nCurType]&sUnderLine)?TRUE:FALSE;
				lf.lfItalic		= (m_pdwTypes[m_nCurType]&sItalic)?TRUE:FALSE;
				lf.lfWeight		= (m_pdwTypes[m_nCurType]&sBold)?FW_BOLD:FW_NORMAL;
				if(m_Font.m_hFont!=NULL)
					m_Font.DeleteFont();
				m_Font.CreateFontIndirect(&lf);
				//запоминаем старый шрифт
				m_Font.m_hFont=(HFONT)dc.SelectObject((HFONT)m_Font);
				//Забираем текст
				TCHAR szName[MAX_PATH];
				GetWindowText(szName,MAX_PATH);
				//измеряем размер текста
				SIZE szText;
				dc.GetTextExtentPoint(szName,(int)_tcslen(szName),&szText);
				//подгонка ширины при стиле sItalic
				//хбз почему, но szText.cx возвращается на 2 меньше чем надо
				szText.cx+=(m_pdwTypes[m_nCurType]&sItalic)?szText.cy/6:0;
				//изменияем размер контрола
				SetWindowPos(0,0,0,szText.cx,szText.cy,SWP_NOMOVE|SWP_NOZORDER);
				dc.Detach();
			};
			//================================================
			//Обработчик WM_DRAWITEM
			//================================================
			LRESULT CULHyperLink::OnDrawItem(WPARAM,LPARAM lParam)
			{
				DRAWITEMSTRUCT* dis=(DRAWITEMSTRUCT*)lParam;
				SendMessage(WM_ERASEBKGND,WPARAM(dis->hDC));

				//получаем стиль кнопки
				DWORD dwStyle   = GetWindowLong(GWL_STYLE);
				//создание стили отображаемого текста
				DWORD dwDTStyle = DT_SINGLELINE;
				if(dwStyle & SS_CENTER)      dwDTStyle |= DT_CENTER;
				if(dwStyle & SS_RIGHT)       dwDTStyle |= DT_RIGHT;
				if(dwStyle & SS_CENTERIMAGE) dwDTStyle |= DT_VCENTER;
				ULGDI::ULDC::CULDC dc=dis->hDC;
				dc.SetTextColor(m_pdwTypes[m_nCurType]&0x00ffffff);
				dc.SetBkColor(GetSysColor(COLOR_3DFACE));			
				dc.SetBkMode(ULGDI::ULDC::CULDC::bmTRANSPARENT);
				//взятие текста
				TCHAR szWinText[MAX_PATH];
				GetWindowText(szWinText, sizeof(szWinText));
				//отрисовка текста
				AutoSize(dc);
				RECT rcHLink;
				GetClientRect(&rcHLink);
				dc.DrawText(szWinText, -1, &rcHLink, dwDTStyle);
				//возвращаем старый шрифт
				m_Font.m_hFont=(HFONT)dc.SelectObject(m_Font);
				dc.Detach();
				return TRUE;
			};
			//================================================
			//Обработчик WM_LBUTTONDOWN
			//================================================
			LRESULT CULHyperLink::OnLButtonDown(WPARAM,LPARAM)
			{
				//фиксируем нажатие
				m_fClicking=true;
				m_nCurType=tDown;
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				return FALSE;
			}
			//================================================
			//Обработчик WM_LBUTTONUP
			//================================================
			LRESULT CULHyperLink::OnLButtonUp(WPARAM,LPARAM)
			{
				//проверяем нажатие
				if(m_fClicking)
				{
					m_fClicking=false;
					//устанавливаем новый тип
					m_nCurType=tVisited;
					m_fVisited=true;
					InvalidateRect();
					UpdateWindow();
					//выполняем переход по ссылке
					if(m_strURL.GetLen())
						::ShellExecute(NULL,_T("open"),m_strURL,NULL,NULL,SW_SHOWNORMAL);
					else
						::SendMessage(m_hParentWnd,WM_COMMAND,
						WPARAM(GetWindowLong(GWL_ID)),LPARAM(m_hWnd));
				}
				return FALSE;
			}
			LRESULT CULHyperLink::OnMouseMove(WPARAM wParam,LPARAM)
			{
				//провоцируем вызов WM_MOUSELEAVE
				TRACKMOUSEEVENT tme;
				tme.cbSize=sizeof(TRACKMOUSEEVENT);
				tme.dwFlags=TME_LEAVE;
				tme.hwndTrack=*this;
				::TrackMouseEvent(&tme);
				if((wParam&MK_LBUTTON)==0)
					m_nCurType=tHiLite;
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				return FALSE;
			};
			LRESULT CULHyperLink::OnMouseLeave(WPARAM,LPARAM)
			{
				m_nCurType=(BYTE)((m_fVisited)?tVisited:tDefault);
				SetWindowPos(0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_DRAWFRAME);
				return FALSE;
			};
			//================================================
			//Функция для определения новых стилей и цветов
			//================================================
			BOOL CULHyperLink::SetStyles(enType Type,enStyles Styles,COLORREF Color)
			{
				if((Type<tDefault)||(Type>tDown))
					return FALSE;
				m_pdwTypes[Type]=Styles&0xff000000;
				m_pdwTypes[Type]|=Color&0x00ffffff;
				return TRUE;
			}
			//================================================
			//Функция для установки шрифта ссылки
			//================================================
			void CULHyperLink::SetFont(HFONT hFont)
			{
				m_Font=hFont;
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			};
		}
	}
}

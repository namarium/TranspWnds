///\file ULSkinDlg.cpp
///\brief файл реализации класса скин-диалогого окна(01.09.2007)
#ifndef _WIN32_WINNT		
	#define _WIN32_WINNT 0x0500
#endif	
#include "..\..\ULLib\Include\ulskindlg.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		CULSkinDlg::CULSkinDlg():
			CULDlg(),
			m_hBGBitmap(NULL)
		{
			MessageMap.AddMessage<CULSkinDlg>(WM_ERASEBKGND,&CULSkinDlg::OnEraseBkGnd);
		};
		CULSkinDlg::~CULSkinDlg(){};
		LRESULT CULSkinDlg::OnInitDialog(WPARAM wParam,LPARAM lParam)
		{
			ModifyStyle(WS_BORDER|DS_3DLOOK|WS_DLGFRAME,0);
			//проверка версии
			OSVERSIONINFOEX osvi;
			DWORDLONG dwlConditionMask = 0;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			osvi.dwMajorVersion = 5;
			VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, 
				VER_GREATER_EQUAL );
			BOOL  fRet=VerifyVersionInfo(
				&osvi, 
				VER_MAJORVERSION,
				dwlConditionMask);
		#if _WIN32_WINNT<0x0500	
			const WS_EX_LAYERED=0;
		#endif
			DWORD dwExLayered;
			if(fRet)
				dwExLayered=WS_EX_LAYERED;
			else
				dwExLayered=0;
			ModifyStyleEx((DWORD)-1,/*WS_EX_CONTROLPARENT|*/dwExLayered);
		#if _WIN32_WINNT>=0x0501	
	//			SetLayeredWindowAttributes(*this,0,255,LWA_ALPHA);
		#endif	

			//позиционирование
			if(m_hBGBitmap!=NULL)
			{
				int horScrSize;
				int verScrSize;
				int x,y;
				RECT rect;
				GetWindowRect(&rect);
				HDC hDC=::GetDC(NULL);
				horScrSize=::GetDeviceCaps(hDC,HORZRES);
				verScrSize=::GetDeviceCaps(hDC,VERTRES);
				ReleaseDC(NULL,hDC);
				x=(horScrSize-m_SizeDlg.cx)/2;
				y=(verScrSize-m_SizeDlg.cy)/2;
				MoveWindow(x,y,m_SizeDlg.cx,m_SizeDlg.cy,FALSE);
				return FALSE;
			}
			else
				return CULDlg::OnInitDialog(wParam,lParam);

		};

		LRESULT CULSkinDlg::OnEraseBkGnd(WPARAM wParam,LPARAM)
		{
			ULGDI::ULDC::CULDC dc((HDC&)wParam);
			RECT rect;
			GetWindowRect(&rect);
			if(m_hBGBitmap!=NULL)
				dc.BitBlt(0,0,m_SizeDlg.cx,m_SizeDlg.cy,m_MemDC,0,0,SRCCOPY);
			dc.Detach();
			return TRUE;
		}

		BOOL CULSkinDlg::SetBitmapRgn(HBITMAP hRgnBitmap)
		{
			//создание региона по маске
			if(hRgnBitmap==NULL)
				return FALSE;
			ULGDI::ULGObjs::CULRgn Rgn;
			Rgn.CreateBitmapRgn(hRgnBitmap);
			//подключаем созданный регион к окну
			SetWindowRgn(Rgn,TRUE);
			return TRUE;
		}
		void CULSkinDlg::SetBitmapBG(HBITMAP hBGBitmap)
		{
			m_hBGBitmap=hBGBitmap;
			ULGDI::ULDC::CULWindowDC dc(NULL);
			m_MemDC.CreateCompatibleDC(dc.m_hDC);
			m_MemDC.SelectObject((HBITMAP)hBGBitmap);
			ULGDI::ULGObjs::CULBitmap bmp(hBGBitmap);
			BITMAP bitmap;
			bmp.GetBitmap(&bitmap);
			m_SizeDlg.cx=bitmap.bmWidth;
			m_SizeDlg.cy=bitmap.bmHeight;
			InvalidateRect(NULL,TRUE);
		}
	}
}

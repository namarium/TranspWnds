///\file ULStatusBar.cpp
///\brief cpp файл класса статусбара(21.07.2007)
#include "..\..\ULLib\Include\ULStatusbar.h"
#include "..\..\ULLib\Include\ULOther.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULBars
	{
		CULStatusBar::CULStatusBar():CULSubClass(),
			m_nParts(0)
		{
		}

		CULStatusBar::CULStatusBar(CULStatusBar& statusBar):CULSubClass(statusBar),
			m_nParts(statusBar.m_nParts)
		{
		}

		CULStatusBar::~CULStatusBar()
		{
		}
		void CULStatusBar::operator=(CULStatusBar& statusBar)
		{
			m_nParts=statusBar.m_nParts;
			ULWnds::CULSubClass::operator=(statusBar);
		}

		HWND CULStatusBar::Create(HWND hParentWnd,short nStatusID,BOOL fGrip)
		{
			m_hParentWnd=hParentWnd;
			InitCommonControls();
			m_hWnd = CreateWindowEx(0,STATUSCLASSNAME,         
				(LPCTSTR) NULL,((fGrip)?SBARS_SIZEGRIP:0) |         
				SBT_TOOLTIPS |WS_CHILD|WS_VISIBLE,     
				0, 0, 0, 0,	hParentWnd,	(HMENU) nStatusID,       
				ULOther::ULGetResourceHandle(),NULL);
			return (Attach(m_hWnd))?m_hWnd:NULL;
		}

		BOOL CULStatusBar::AddPart(int Width,LPCTSTR szToolTip)
		{
			int* lpParts;
			BOOL fRet=FALSE;	
			lpParts = new int[m_nParts+1];
			fRet=(BOOL)SendMessage(SB_GETPARTS, 
				(WPARAM) m_nParts,
				(LPARAM)lpParts);
			if(fRet)
			{
				m_nParts++;
				if(m_nParts==1)
					lpParts[m_nParts-1]=Width;
				else
					lpParts[m_nParts-1]=lpParts[m_nParts-2]+Width;
				fRet=(BOOL)SendMessage(SB_SETPARTS, 
					(WPARAM) m_nParts,
					(LPARAM)lpParts);
				if(fRet)
					SendMessage(SB_SETTIPTEXT,WPARAM(m_nParts-1),LPARAM(szToolTip));
			}
			delete[] lpParts;
			return fRet;
		} 



		BOOL CULStatusBar::DeleteAll()
		{
			BOOL fRet=FALSE;
			int DelPart[1]={0};
			fRet=(BOOL)SendMessage(SB_SETPARTS, 
				(WPARAM) 1,
				(LPARAM)DelPart);
			SendMessage(SB_SETICON,WPARAM(0),LPARAM(0));
			SendMessage(SB_SETTEXT,WPARAM(0),LPARAM(0));
			SendMessage(SB_SETTIPTEXT,WPARAM(0),LPARAM(0));
			if(!fRet)
				return FALSE;
			return TRUE;
		};

		BOOL CULStatusBar::SetIcon(int nPart, HICON hIcon)
		{
			return (SendMessage(SB_SETICON,WPARAM(nPart),LPARAM(hIcon))!=0)
				?TRUE:FALSE;
		}

		BOOL CULStatusBar::SetIcon(int nPart, short uiID)
		{
			HICON hIcon=::LoadIcon(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(uiID));
			return (SendMessage(SB_SETICON,WPARAM(nPart),LPARAM(hIcon))!=0)
				?TRUE:FALSE;
		}

		BOOL CULStatusBar::SetText(int nPart, LPCTSTR szText)
		{
			return (BOOL)SendMessage(SB_SETTEXT,WPARAM(nPart),LPARAM(szText));
		}

		BOOL CULStatusBar::SetToolTipText(int nPart, LPCTSTR szText)
		{
			return (BOOL)SendMessage(SB_SETTIPTEXT,WPARAM(nPart),LPARAM(szText));
		}

		void CULStatusBar::AutoSize()
		{
			RECT rcChild,rcParent;
			::GetClientRect(m_hParentWnd,&rcParent);
			GetClientRect(&rcChild);
			//==========Изменение размеров первой секции==============================
			BOOL fRet=FALSE;
			int MaxWidth=0;
			if(m_nParts==0)
				return;
			ULOther::CULArr<int> lpParts(m_nParts);
			fRet=(BOOL)SendMessage(SB_GETPARTS, 
				(WPARAM) m_nParts,(LPARAM)(int*)lpParts);
			if((!fRet)||(lpParts==NULL))
				return;
			if(m_nParts==1)
				lpParts[0]=rcParent.right-rcParent.left;
			else
			{
				MaxWidth+=lpParts[m_nParts-1]-lpParts[0];				
				for(int i=m_nParts-1;i>0;i--)
					lpParts[i]-=lpParts[i-1];
				lpParts[0]=rcParent.right-rcParent.left-MaxWidth;
				if(lpParts[0]<0)
					lpParts[0]=0;
				for(int i=1;i<signed(m_nParts);i++)
					lpParts[i]+=lpParts[i-1];
			}
			fRet=(BOOL)SendMessage(SB_SETPARTS, 
				(WPARAM) m_nParts,(LPARAM)(int*)lpParts);

			MoveWindow(0,rcParent.bottom-rcChild.bottom,
				rcParent.right,rcParent.bottom,FALSE);
		}
	}
}

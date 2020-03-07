///\file ULReBar.cpp
///\brief cpp файл класса ребара(11.08.2007)

#ifndef _WIN32_IE
	#define _WIN32_IE 0x0500
#endif
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif
#include "..\..\ULLib\Include\ULRebar.h"


#include "..\..\ULLib\Include\ULToolbar.h"
#include "..\..\ULLib\Include\ULFrameWnd.h"
#include "..\..\ULLib\Include\ULStates.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULBars
	{
		CULRebar::CULRebar():CULSubClass()
		{
			MessageMap.AddReflectNotify<CULRebar>(RBN_CHEVRONPUSHED,&CULRebar::OnChevronPushed);
		}

		CULRebar::CULRebar(CULRebar& rebar):CULSubClass(rebar),
			m_afFlag(rebar.m_afFlag)
		{
			MessageMap.AddReflectNotify<CULRebar>(RBN_CHEVRONPUSHED,&CULRebar::OnChevronPushed);
		}

		CULRebar::~CULRebar()
		{
		}

		void CULRebar::operator=(CULRebar& rebar)
		{
			m_afFlag=rebar.m_afFlag;
			ULWnds::CULSubClass::operator=(rebar);
		}

		BOOL CULRebar::Create(HWND hParentWnd,DWORD dwID,enAlignFlags afFlag,DWORD dwStyle)
		{
			REBARINFO     rbi;
			INITCOMMONCONTROLSEX icex;
			HINSTANCE hinst=ULOther::ULGetResourceHandle();
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC   = ICC_COOL_CLASSES;
			InitCommonControlsEx(&icex);
			m_hWnd = CreateWindowEx(0/*WS_EX_TOOLWINDOW*/,
									REBARCLASSNAME,
									NULL,
									dwStyle|afFlag,
									0,0,500,500,
									hParentWnd,
									(HMENU)dwID,
									hinst,
									NULL);
			m_afFlag=afFlag;
			if(!m_hWnd)
				return NULL;
			rbi.cbSize = sizeof(REBARINFO);  
			rbi.fMask  =  RBIM_IMAGELIST;
			rbi.himl   = (HIMAGELIST)NULL;
//			if(!SendMessage(RB_SETBARINFO, 0, (LPARAM)&rbi))
//				return FALSE;
			return Attach(m_hWnd);
		};

		BOOL CULRebar::InsertBand(int nInto,HWND hClientWnd,TCHAR* szName, HBITMAP hBitmap,
								WORD wID,SIZE* pszClient,DWORD dwStyle)
		{
			REBARBANDINFO rbBand={0};
			rbBand.cbSize = sizeof(REBARBANDINFO);  
			rbBand.fMask = RBBIM_STYLE|RBBIM_CHILDSIZE|RBBIM_IDEALSIZE/*|RBBIM_SIZE*/;
			rbBand.fMask |=((rbBand.lpText=szName)!=NULL)?RBBIM_TEXT:0;
			rbBand.fMask |=((rbBand.hbmBack=hBitmap)!=NULL)?RBBIM_BACKGROUND:0;
			rbBand.fMask |=((rbBand.wID=wID)!=NULL)?RBBIM_ID:0;
			rbBand.fStyle =dwStyle;
			RECT rect;
			if(pszClient==NULL)
				::GetWindowRect(hClientWnd,&rect);
			else
				::SetRect(&rect,0,0,pszClient->cx,pszClient->cy);
			rbBand.fMask|=((rbBand.hwndChild=hClientWnd)!=NULL)?RBBIM_CHILD:0;
			if((m_afFlag==afTop)||(m_afFlag==afBottom))
			{
				rbBand.cxMinChild = rect.right-rect.left+((dwStyle&RBBS_CHILDEDGE)?2*::GetSystemMetrics(SM_CXEDGE):0);
				rbBand.cyMinChild = rect.bottom-rect.top+((dwStyle&RBBS_CHILDEDGE)?2*::GetSystemMetrics(SM_CYEDGE):0);
			}
			else
			{
				rbBand.cxMinChild = rect.bottom-rect.top+2*::GetSystemMetrics(SM_CYEDGE);
				rbBand.cyMinChild = rect.right-rect.left+2*::GetSystemMetrics(SM_CXEDGE);
			}
			rbBand.cxIdeal=rbBand.cxMinChild;
			return (SendMessage(RB_INSERTBAND, (WPARAM)nInto, (LPARAM)&rbBand)!=NULL);
		}

		BOOL CULRebar::InsertBand(int nInto,HWND hClientWnd, TCHAR* szName, 
								COLORREF clrFore, COLORREF clrBack,
								WORD wID,SIZE* pszClient,DWORD dwStyle)
		{
			REBARBANDINFO rbBand={0};
			rbBand.cbSize = sizeof(REBARBANDINFO);  // Required
			rbBand.fMask = RBBIM_STYLE|RBBIM_CHILD|RBBIM_IDEALSIZE|RBBIM_CHILDSIZE|/*RBBIM_SIZE|*/RBBIM_BACKGROUND;
			rbBand.fMask |=((rbBand.lpText=szName)!=NULL)?RBBIM_TEXT:0;
			rbBand.fMask |=((rbBand.wID=wID)!=NULL)?RBBIM_ID:0;
			rbBand.fMask |=((rbBand.clrFore=clrFore)!=NULL)?RBBIM_COLORS:0;
			rbBand.fMask |=((rbBand.clrBack=clrBack)!=NULL)?RBBIM_COLORS:0;
			rbBand.fStyle =dwStyle;
			RECT rect;
			if(pszClient==NULL)
				::GetWindowRect(hClientWnd,&rect);
			else
				::SetRect(&rect,0,0,pszClient->cx,pszClient->cy);
			rbBand.hwndChild  = hClientWnd;
			if((m_afFlag==afTop)||(m_afFlag==afBottom))
			{
				rbBand.cxMinChild = rect.right-rect.left+2*::GetSystemMetrics(SM_CXEDGE);
				rbBand.cyMinChild = rect.bottom-rect.top+2*::GetSystemMetrics(SM_CYEDGE);
			}
			else
			{
				rbBand.cxMinChild = rect.bottom-rect.top+2*::GetSystemMetrics(SM_CYEDGE);
				rbBand.cyMinChild = rect.right-rect.left+2*::GetSystemMetrics(SM_CXEDGE);
			}
			rbBand.cxIdeal=rbBand.cxMinChild;
			return (SendMessage(RB_INSERTBAND, (WPARAM)nInto, (LPARAM)&rbBand)!=NULL);
		}

		BOOL CULRebar::DeleteBand(int numBand)
		{
			return (0!=SendMessage((UINT) RB_DELETEBAND,WPARAM(numBand),0)); 
		}

		void CULRebar::AutoSize()
		{
			RECT rect;
			GetClientRect(&rect);
			SendMessage(WM_SIZE,  SIZE_RESTORED,
				MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top));
		};

		LRESULT CULRebar::OnChevronPushed(LPARAM lParam)
		{
			LPNMREBARCHEVRON nmrc=(LPNMREBARCHEVRON)lParam;
			ULWnds::ULBars::CULToolBar* pToolBar=(ULWnds::ULBars::CULToolBar*)nmrc->lParam;
			if(!pToolBar)
				return 0;

			class CULChevronDownWnd:
				public ULWnds::ULFrames::CULFrameWnd
			{
			protected:
				ULWnds::ULBars::CULToolBar* m_pToolBar;
				ULWnds::ULBars::CULToolBar m_ToolBar;
				RECT m_rcVisibleBtns;
				HWND m_hwndOwner;
			public:
				CULChevronDownWnd(ULWnds::ULBars::CULToolBar* pToolBar,RECT rcVisibleBtns,HWND hwndOwner):
					ULWnds::ULFrames::CULFrameWnd(),
					m_pToolBar(pToolBar),
					m_rcVisibleBtns(rcVisibleBtns),
					m_hwndOwner(hwndOwner)
				{
				}
				virtual LRESULT OnCreate(WPARAM wParam,LPARAM lParam)
				{
					RECT rcTBWidth;
					UINT nVisiblePos=0;
					for(;nVisiblePos<m_pToolBar->GetButtonCount();++nVisiblePos)
					{
						m_pToolBar->GetItemRect(nVisiblePos,&rcTBWidth);
						if(rcTBWidth.right>m_rcVisibleBtns.right)
							break;
					}

					LONG lStyle=m_pToolBar->GetWindowLong(GWL_STYLE);
					SIZE szBtn=m_pToolBar->GetButtonSize();
					SIZE szBmp=m_pToolBar->GetBitmapSize();
					ASSERT(m_ToolBar.Create(*this,0,szBtn.cx,szBtn.cy,szBmp.cx,szBmp.cy,
						ULWnds::ULBars::CULToolBar::afNon|ULWnds::ULBars::CULToolBar::afTop,lStyle));

					HIMAGELIST	hHot=m_pToolBar->GetImageList();
					HIMAGELIST	hImageList=ImageList_Duplicate(hHot);
					m_ToolBar.SetImageList(0,hImageList);

					for(;nVisiblePos<m_pToolBar->GetButtonCount();++nVisiblePos)
					{
						TBBUTTON tbb;				
						m_pToolBar->GetButton(nVisiblePos,&tbb);
						m_ToolBar.AddButton(tbb.idCommand,tbb.fsState|TBSTATE_WRAP ,tbb.fsStyle,(TCHAR*)tbb.iString,(TCHAR*)tbb.iString,tbb.iBitmap);
					}
					m_ToolBar.AutoSize();

					POINT pt={m_rcVisibleBtns.right,m_rcVisibleBtns.bottom};
					::ClientToScreen(m_hwndOwner,&pt);
					int nWindth=0;
					int nHeight=0;
					for(UINT i=0;i<m_ToolBar.GetButtonCount();++i)
					{
						m_ToolBar.GetItemRect(i,&rcTBWidth);
						if(rcTBWidth.right>nWindth)
							nWindth=rcTBWidth.right;
						nHeight=rcTBWidth.bottom;
					}
					MoveWindow(pt.x-nWindth+2,pt.y,nWindth+6,nHeight+6,TRUE);

					return ULWnds::ULFrames::CULFrameWnd::OnCreate(wParam,lParam);
				}
				virtual LRESULT OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
				{
					switch(uMsg)
					{
					case WM_COMMAND:
						::PostMessage(m_hwndOwner,uMsg,wParam,lParam);
						break;
					case WM_KILLFOCUS:
						DestroyWindow();
						break;
					case WM_KEYDOWN:
						if(wParam==VK_ESCAPE)
							DestroyWindow();
						break;
					}
					return ULWnds::ULFrames::CULFrameWnd::OnMessage(uMsg,wParam,lParam);
				}
			}wndChevronDown(pToolBar,nmrc->rc,*this);	

			wndChevronDown.Create(_T("ChevronDownWnd"),0,0,0,COLOR_BACKGROUND,WS_VISIBLE|WS_POPUP|WS_DLGFRAME);
			MSG msg;
			while(::GetMessage(&msg,0,0,0)&&wndChevronDown.IsWindow())
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			return 0;
		}
	}
}

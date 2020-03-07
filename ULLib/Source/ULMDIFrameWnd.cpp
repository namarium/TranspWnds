///\file ULMDIFrameWnd.cpp
///\brief cpp файл класса главного окна архетектуры MDI(2006)
#include "..\..\ULLib\Include\ULMDIFrameWnd.h"
#include "..\..\ULLib\Include\ULResDef.h"

namespace ULWnds
{
	namespace ULFrames
	{
		CULMDIFrameWnd::CULMDIFrameWnd(void):CULFrameWnd(),
			m_dwClientStyle(0),
			m_dwClientStyleEx(0),
			m_idFirstChild(0),
			m_idSubMenu(0)
		{
			MessageMap.AddMessage<CULMDIFrameWnd>(WM_CREATE,&CULMDIFrameWnd::OnCreate);
			MessageMap.AddMessage<CULMDIFrameWnd>(WM_SIZE,&CULMDIFrameWnd::OnSize);
			MessageMap.AddMessage<CULMDIFrameWnd>(ULWndCtrls::CULWndCtrlList::MDIM_GETCLIENT,&CULMDIFrameWnd::OnGetMDIClient);
			MessageMap.AddCommand<CULMDIFrameWnd>(ID_WINDOW_CASCADE,&CULMDIFrameWnd::OnWindowCascade);
			MessageMap.AddCommand<CULMDIFrameWnd>(ID_WINDOW_ARRANGE,&CULMDIFrameWnd::OnWindowArrange);
			MessageMap.AddCommand<CULMDIFrameWnd>(ID_WINDOW_TILE_HORZ,&CULMDIFrameWnd::OnWindowTileHorz);
			MessageMap.AddCommand<CULMDIFrameWnd>(ID_WINDOW_TILE_VERT,&CULMDIFrameWnd::OnWindowTileVert);
		}

		CULMDIFrameWnd::CULMDIFrameWnd(CULMDIFrameWnd& FrameWnd)
			:CULFrameWnd(FrameWnd),
			m_dwClientStyle(FrameWnd.m_dwClientStyle),
			m_dwClientStyleEx(FrameWnd.m_dwClientStyleEx),
			m_idFirstChild(FrameWnd.m_idFirstChild),
			m_idSubMenu(FrameWnd.m_idSubMenu)
		{
			m_pMDIClientWnd=new CULMDIClientWnd(*FrameWnd.m_pMDIClientWnd);
		}

		CULMDIFrameWnd::~CULMDIFrameWnd(void)
		{
			delete m_pMDIClientWnd;
		}

		void CULMDIFrameWnd::operator = (CULMDIFrameWnd& FrameWnd)
		{
			m_dwClientStyle=FrameWnd.m_dwClientStyle;
			m_dwClientStyleEx=FrameWnd.m_dwClientStyleEx;
			m_idFirstChild=FrameWnd.m_idFirstChild;
			m_idSubMenu=FrameWnd.m_idSubMenu;
			m_pMDIClientWnd=new CULMDIClientWnd(*FrameWnd.m_pMDIClientWnd);
			ULWnds::CULWnd::operator=(FrameWnd);
		}

		HWND CULMDIFrameWnd::Create(TCHAR* szWindowName,
								short MenuID,
								short IconID,
								short IconSmID,
								short BGColor,
								UINT idSubMenu,
								DWORD dwStyle,
								DWORD dwStyleEx,
								UINT idFirstChild,
								DWORD dwClientStyle,
								DWORD dwClientStyleEx)
		{
			m_dwClientStyle=dwClientStyle;
			m_dwClientStyleEx=dwClientStyleEx;
			m_idFirstChild=idFirstChild;
			m_idSubMenu=idSubMenu;

			m_pMDIClientWnd=new CULMDIClientWnd;

			CULFrameWnd::Create(szWindowName,MenuID,IconID,IconSmID,BGColor,
				dwStyle,dwStyleEx);

			return m_hWnd;
		}

		CULMDIClientWnd* CULMDIFrameWnd::ReplaceMDIClient(CULMDIClientWnd* pMDIClientWnd)
		{
			CULMDIClientWnd* pOldMDIClientWnd=m_pMDIClientWnd;
			m_pMDIClientWnd=pMDIClientWnd;
			return pOldMDIClientWnd;
		};

		LRESULT CULMDIFrameWnd::OnCreate(WPARAM /*wParam*/,LPARAM)
		{
			//создаём контейнер для вспомогательных контролов
			m_WndCtrlList.Create(*this);
			//создаём клиентское окно
			m_pMDIClientWnd->Create(*this,m_idSubMenu,m_idFirstChild,
				m_dwClientStyle,m_dwClientStyleEx);
			return FALSE;
		}

		LRESULT CULMDIFrameWnd::OnSize(WPARAM,LPARAM)
		{
			return TRUE;
		};

		void CULMDIFrameWnd::OnWindowCascade(WORD /*ID*/,HWND /*hControl*/)
		{
			m_pMDIClientWnd->PostMessage(WM_MDICASCADE, 0, 0);
		}

		void CULMDIFrameWnd::OnWindowArrange(WORD /*ID*/,HWND /*hControl*/)
		{
			m_pMDIClientWnd->PostMessage(WM_MDIICONARRANGE, 0, 0);
		}

		void CULMDIFrameWnd::OnWindowTileHorz(WORD /*ID*/,HWND /*hControl*/)
		{
			m_pMDIClientWnd->PostMessage(WM_MDITILE, MDITILE_HORIZONTAL, 0);
		}

		void CULMDIFrameWnd::OnWindowTileVert(WORD /*ID*/,HWND /*hControl*/)
		{
			m_pMDIClientWnd->PostMessage(WM_MDITILE, MDITILE_VERTICAL, 0);
		}
	}
}

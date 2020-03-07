///\file FolderDlg.cpp
///\brief фаил реализации для класса выбора директории
#include "..\..\ULLib\Include\ULFolderDlg.h"

namespace ULWnds
{
	namespace ULDlgs
	{
		CULFolderDlg::CULFolderDlg(HWND hParentWnd,UINT ulFlags):
			CULDlg()
		{
			m_bi.ulFlags=ulFlags;//|BIF_VALIDATE;
			::ZeroMemory((LPVOID)&m_bi,sizeof(BROWSEINFO));
			m_bi.hwndOwner=hParentWnd;
			m_bi.pszDisplayName=new TCHAR[32565];
			m_bi.lpfn=BrowseCallbackProc;
			m_bi.lParam=(LPARAM)this;
		}
		CULFolderDlg::~CULFolderDlg()
		{
			delete[] m_bi.pszDisplayName;
		}
		int CULFolderDlg::CreateModal()
		{
			LPITEMIDLIST lpil;
			if((lpil=SHBrowseForFolder(&m_bi))!=NULL)
			{
				SHGetPathFromIDList(lpil,m_bi.pszDisplayName);
				if (lpil)
				{
					LPMALLOC lpMalloc;
					if(::SHGetMalloc(&lpMalloc) == NOERROR)             
					{
						lpMalloc->Free(lpil);
						lpMalloc->Release();
					}
				}
				return IDOK;
			}
			return IDCANCEL;
		}
		LPCTSTR CULFolderDlg::GetPath()
		{
			return m_bi.pszDisplayName;
		}
		BOOL CULFolderDlg::GetPath(LPTSTR lpszPath,WORD nLen)
		{
			if(lpszPath==NULL)
				return FALSE;
			_tcscpy_s(lpszPath,nLen,m_bi.pszDisplayName);
			return TRUE;
		}
		//===========================================
		int CALLBACK CULFolderDlg::BrowseCallbackProc(HWND hDlg,UINT uMsg,LPARAM /*lParam*/,LPARAM lpData)
    {
        CULFolderDlg* lpDlg=((CULFolderDlg*)lpData);
        if(uMsg==BFFM_INITIALIZED)
        {
					if(lpData)
					{
						lpDlg->m_hWnd=hDlg;
						lpDlg->OnInitialized();
					}
					::SetWindowLong(hDlg,GWL_USERDATA,(LONG)(LONG_PTR)lpData);
					((CULWnd*)lpDlg)->m_lpSubClassWndProc=(WNDPROC)(LONG_PTR)::SetWindowLong(hDlg,GWL_WNDPROC,(LONG)(LONG_PTR)WndProc);
        }
        return 0;
    };

		LRESULT CULFolderDlg::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			CULWnd* pULWnd=(CULFolderDlg*)(LONG_PTR)FromHandle(hWnd);

			LRESULT lRes=pULWnd->OnMessage(uMsg,wParam,lParam);
			if(lRes)
				return lRes;

			if(uMsg==WM_COMMAND)
			{
				BOOL fRet=pULWnd->MessageMap.ProcessCommand(pULWnd,wParam,lParam);
				if(fRet)
					return fRet;
			}
			else
				if(uMsg==WM_NOTIFY)
				{
					BOOL fRet=pULWnd->MessageMap.ProcessNotify(pULWnd,(NMHDR*)lParam);
					if(fRet)
						return fRet;
				}
			BOOL fRet=pULWnd->MessageMap.ProcessMessage(pULWnd,uMsg,wParam,lParam);
			if(fRet)
				return fRet;
			return ::CallWindowProc(pULWnd->m_lpSubClassWndProc,hWnd,uMsg,wParam,lParam);
		}
		//====================================
		void CULFolderDlg::OnInitialized()
		{
		}
		LRESULT CULFolderDlg::OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			if(uMsg==WM_COMMAND)
			{
				if(wParam==IDOK)
				{
					OnOK((WORD)wParam,(HWND)lParam);
					return ::CallWindowProc(m_lpSubClassWndProc,*this,WM_COMMAND,wParam,lParam);
				}
				if(wParam==IDCANCEL)
				{
					OnCancel((WORD)wParam,(HWND)lParam);
					return ::CallWindowProc(m_lpSubClassWndProc,*this,WM_COMMAND,wParam,lParam);
				}
			}
			return 0;
		};
		void CULFolderDlg::OnOK(WORD,HWND)
		{
		}
		void CULFolderDlg::OnCancel(WORD,HWND)
		{
		}
	}
}

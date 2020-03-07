///\file ULDlg.cpp
///\brief cpp файл класса диалогого окна(2006)
#include "..\..\ULLib\Include\ULDlg.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULDlgs
	{
		CULDlg::CULDlg(short idIcon):
			CULWnd(),
			m_fModal(FALSE),
			m_idTempl(0),
			m_idIcon(idIcon)
		{
			MessageMap.AddMessage<CULDlg>(WM_INITDIALOG,&CULDlg::OnInitDialog);
			MessageMap.AddMessage<CULDlg>(WM_DESTROY,&CULDlg::OnDestroy);
			MessageMap.AddCommand<CULDlg>(IDOK,&CULDlg::OnOK);
			MessageMap.AddCommand<CULDlg>(IDCANCEL,&CULDlg::OnCancel);
		}

		CULDlg::~CULDlg(void)
		{
		}

		LRESULT CULDlg::WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
		{
			CULWnd *pULWnd=NULL;
			switch(uMsg)
			{
			case WM_INITDIALOG:
				{
					::SetWindowLong (hWnd, GWL_USERDATA, (LONG)lParam);
					pULWnd = (CULWnd *) lParam; // получим указатель на класс
					pULWnd->m_hWnd=hWnd;
					break;
				}
			default:
				pULWnd = FromHandle(hWnd);
			}

			if(pULWnd!=NULL)
			{
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
			}
			return FALSE;
		}

		int CULDlg::CreateModal(short idTempl, HWND hParentWnd)
		{
			m_idTempl=idTempl;
			m_hParentWnd=hParentWnd;
			m_fModal=TRUE;
			return (int)DialogBoxParam(ULOther::ULGetResourceHandle(),
				(LPCTSTR)m_idTempl, 
				m_hParentWnd,
				(DLGPROC)WndProc,
				LPARAM(this));
		}

		HWND CULDlg::Create(short idTempl,HWND hParentWnd)
		{
			m_idTempl=idTempl;
			m_hParentWnd=hParentWnd;
			m_fModal=FALSE;
			m_hWnd=CreateDialogParam(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(m_idTempl), m_hParentWnd, (DLGPROC)WndProc,LPARAM(this));
			return m_hWnd;
		};

		LRESULT CULDlg::OnInitDialog(WPARAM,LPARAM /*lParam*/)
		{
			if(m_idIcon!=0)
			{
				SendMessage(WM_SETICON,
							WPARAM(ICON_SMALL),
							(LPARAM)LoadIcon(ULOther::ULGetResourceHandle(), (LPCTSTR)m_idIcon));
			}
			RECT rect;
			int nHorScrSize;
			int nVerScrSize;
			int x,y,w,h;
			GetWindowRect(&rect);
			HDC hDC=::GetDC(NULL);
			nHorScrSize=::GetDeviceCaps(hDC,HORZRES);
			nVerScrSize=::GetDeviceCaps(hDC,VERTRES);
			ReleaseDC(NULL,hDC);
			w=rect.right-rect.left;
			h=rect.bottom-rect.top;
			x=(nHorScrSize-w)/2;
			y=(nVerScrSize-h)/2;
			MoveWindow(x,y,w,h,FALSE);
			return FALSE;
		}

		LRESULT CULDlg::OnDestroy(WPARAM /*wParam*/,LPARAM /*lParam*/)
		{
			::EnableWindow(m_hParentWnd,TRUE);
			::SetActiveWindow(m_hParentWnd);
			::UpdateWindow(m_hParentWnd);
			return FALSE;
		}

		void CULDlg::OnOK(WORD /*id*/,HWND /*hControl*/)
		{
			if(IsModal())
				::EndDialog(m_hWnd,TRUE);
			else
				DestroyWindow();
		}

		void CULDlg::OnCancel(WORD /*id*/,HWND /*hControl*/)
		{
			if(IsModal())
				::EndDialog(m_hWnd,FALSE);
			else
				DestroyWindow();
		}
	}
}

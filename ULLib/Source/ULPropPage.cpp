///\file ULPropPage.cpp
///\brief cpp файл класса root окна(21.07.2007)
#include "..\..\ULLib\Include\ULPropPage.h"
#include "..\..\ULLib\Include\ULPropSheet.h"
#include "..\..\ULLib\Include\ULRes.h"
#include <uxtheme.h>
#pragma comment(lib,"UxTheme.lib")
namespace ULWnds
{
	namespace ULDlgs
	{
		CULPropPage::CULPropPage():
			CULDlg()
		{
			MessageMap.AddMessage<CULPropPage>(WM_NOTIFY,&CULPropPage::OnNotify);
			MessageMap.AddMessage<CULPropPage>(WM_INITDIALOG,&CULPropPage::OnInitDialog);
		};

		CULPropPage::~CULPropPage()
		{
		};

		LRESULT CULPropPage::WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
		{
			CULWnd *pULWnd=NULL;
			switch(uMsg)
			{
			case WM_INITDIALOG:
				{
					// получим указатель на класс
					pULWnd =(CULWnd*)((LPPROPSHEETPAGE) lParam)->lParam;
					::SetWindowLong (hWnd, GWL_USERDATA, (LONG)(LONG_PTR)pULWnd);
					pULWnd->m_hWnd=hWnd;
					break;
				}
			default:
				{
				pULWnd = FromHandle(hWnd);
				}
			};
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
				return pULWnd->MessageMap.ProcessMessage(pULWnd,uMsg,wParam,lParam);
			}
			return FALSE;
		};

		HPROPSHEETPAGE CULPropPage::Create(short TemplID, 
										LPCTSTR szTitle,
										LPCTSTR szSubTitle, 
										HICON hIcon,
										BOOL fHelp)
		{
			PROPSHEETPAGE psp;
			psp.dwSize = sizeof(psp);
			psp.hInstance = ULOther::ULGetResourceHandle();
			psp.lParam = (LPARAM)this;
			psp.pszTemplate = MAKEINTRESOURCE(TemplID);
			psp.dwFlags=PSP_DEFAULT;
			if(fHelp)
				psp.dwFlags=PSP_HASHELP;
			if(szTitle!=NULL)
			{
				psp.pszHeaderTitle = szTitle;
				psp.dwFlags|=PSP_USEHEADERTITLE;
				psp.pszTitle=szTitle;
				psp.dwFlags|=PSP_USETITLE;
			}
			if(szSubTitle!=NULL)
			{
				psp.pszHeaderSubTitle = szSubTitle;
				psp.dwFlags|=PSP_USEHEADERSUBTITLE;
			}
			if(hIcon!=NULL)
			{
				psp.hIcon= hIcon;
				psp.dwFlags|=PSP_USEHICON;
			}
			if(psp.dwFlags==PSP_DEFAULT)
				psp.dwFlags|=PSP_HIDEHEADER;
				psp.pfnDlgProc = (DLGPROC)WndProc;
			return CreatePropertySheetPage(&psp);
		}

		void CULPropPage::DoChange()
		{
			::SendMessage(GetParent(),PSM_CHANGED,0,0);
		}
		void CULPropPage::DoUnChange()
		{
			::SendMessage(GetParent(),PSM_UNCHANGED,0,0);
		}
		
		LRESULT CULPropPage::OnInitDialog(WPARAM /*wParam*/,LPARAM /*lParam*/)
		{
//			ULWnds::ULDlgs::CULPropSheet* ps=(ULWnds::ULDlgs::CULPropSheet*)FromHandle(GetParent());
			//if(ps&&!ps->IsWizard())
//				EnableThemeDialogTexture(*this,ETDT_USETABTEXTURE);
			return 0;
		}

		LRESULT CULPropPage::OnNotify(WPARAM,LPARAM lParam)
		{
			LPNMHDR pnmh = (LPNMHDR)lParam;
			switch(pnmh->code)
			{
			case PSN_SETACTIVE:	return OnSetActive(PSWIZB_NEXT|PSWIZB_BACK);
			case PSN_KILLACTIVE:return OnKillActivate(FALSE);
			case PSN_WIZNEXT:	return OnWizNext(0);//-1
			case PSN_WIZFINISH:	return OnWizFinish(FALSE);
			case PSN_RESET:		return OnReset(BOOL(((LPPSHNOTIFY) lParam)->lParam));
			case PSN_APPLY:		return OnApply(PSNRET_NOERROR);
			case PSN_HELP:		return OnHelp(((NMHDR*)lParam)->hwndFrom);
			}

			return FALSE;
		};

		LRESULT CULPropPage::OnSetActive(DWORD dwFlags)
		{
			::PostMessage(GetParent(),PSM_SETWIZBUTTONS,0,(LPARAM)dwFlags);
			return TRUE;
		}

		LRESULT CULPropPage::OnKillActivate(BOOL fProblem)
		{
			SetWindowLong(DWL_MSGRESULT, fProblem);
			return TRUE;
		}

		LRESULT CULPropPage::OnWizNext(short nResult)
		{
			SetWindowLong(DWL_MSGRESULT, nResult);
			return TRUE;
		};

		LRESULT CULPropPage::OnWizFinish(BOOL fFinish)
		{
			SetWindowLong(DWL_MSGRESULT, fFinish);
		//	::DestroyWindow(GetParent());
			return TRUE;
		}

		LRESULT CULPropPage::OnReset(BOOL /*bXorCancel*/)
		{
		//	SetWindowLong( DWL_MSGRESULT, -1);
		//	::DestroyWindow(GetParent());
			return FALSE;
		}

		LRESULT CULPropPage::OnApply(BYTE nReturn)
		{
			SetWindowLong(DWL_MSGRESULT,nReturn);
			return TRUE;
		};

		LRESULT CULPropPage::OnHelp(HWND /*hPropSheet*/)
		{
			return TRUE;
		}
	}
}

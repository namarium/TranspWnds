///\file ULCommDlgs.cpp
///\brief cpp файл классов стандартных диалоговых окон(02.01.2007)
#include "..\\include\\ULCommDlgs.h"
#include "..\\include\\ULRes.h"
#include "..\\include\\ULGObjs.h"


namespace ULWnds
{
	namespace ULDlgs
	{
		namespace ULCommDlgs
		{
			//==================CULCommDlg=======================================
			LRESULT CULCommDlg::WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
			{
				CULWnd *pULWnd=NULL;
				switch(uMsg)
				{
				case WM_INITDIALOG:
					{
						LPCHOOSECOLOR lpсс=(CHOOSECOLOR*)lParam;
						if(lpсс->lStructSize==sizeof(CHOOSECOLOR))
						{
							::SetWindowLong(hWnd,GWL_USERDATA,(LONG)lpсс->lCustData);
							pULWnd=(CULWnd*)lpсс->lCustData;//получим указатель на класс
						}
						LPOPENFILENAME lpofn=(LPOPENFILENAME)lParam;
						if(lpofn->lStructSize==sizeof(OPENFILENAME))
						{
							::SetWindowLong(hWnd,GWL_USERDATA,(LONG)lpofn->lCustData);
							pULWnd = (CULWnd *) lpofn->lCustData; // получим указатель на класс
						}
						LPCHOOSEFONT lpcf=(LPCHOOSEFONT)lParam;
						if(lpcf->lStructSize==sizeof(CHOOSEFONT))
						{
							::SetWindowLong(hWnd,GWL_USERDATA,(LONG)lpcf->lCustData);
							pULWnd=(CULWnd*)lpcf->lCustData; // получим указатель на класс
						}
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
			CULCommDlg::CULCommDlg():
					ULWnds::ULDlgs::CULDlg()
			{
				MessageMap.ClearAll();
			};
			//==================CULColorDlg=======================================
			CULColorDlg::CULColorDlg(COLORREF clrInit,
					DWORD dwFlags):
					CULCommDlg()				
			{
				ZeroMemory(&m_cc, sizeof(m_cc));
				m_cc.lStructSize=sizeof(m_cc);
				m_cc.hInstance=(HWND)ULOther::ULGetResourceHandle();
				m_cc.Flags=dwFlags;

				ULGDI::ULGObjs::CULPalette pal;
				pal.m_hPalette=(HPALETTE)GetStockObject(DEFAULT_PALETTE);
				PALETTEENTRY pe;
				for(int i=0;i<16;++i)
				{
					pal.GetPaletteEntries(i,1,&pe);
					m_acrCustClr[i]=RGB(pe.peRed,pe.peGreen,pe.peBlue);
				}
				pal.Detach();

				m_cc.lpCustColors=m_acrCustClr;
				m_cc.rgbResult=clrInit;
				m_cc.lpfnHook=(LPCCHOOKPROC)CULCommDlg::WndProc;
				m_cc.lCustData=(LPARAM)this;
			};
			CULColorDlg::~CULColorDlg()
			{
			};

			int CULColorDlg::CreateModal(short idTempl, HWND hParentWnd)
			{
				m_cc.lpTemplateName=MAKEINTRESOURCE(idTempl);
				m_cc.hwndOwner=hParentWnd;
				m_fModal=TRUE;
				return ChooseColor(&m_cc);
			};
			//==================CULFileDialog=======================================

			CULFileDlg::CULFileDlg(BOOL fOpenFileDlg,
					LPCTSTR lpszDefExt,
					LPCTSTR lpszFileName,
					DWORD dwFlags,
					LPCTSTR lpszFilter,
					DWORD dwFilterIndex):
				CULCommDlg()
			{
				m_fOpenFileDlg=fOpenFileDlg;
				ZeroMemory(&m_ofn, sizeof(m_ofn));
				m_ofn.lStructSize = sizeof(m_ofn);
				m_ofn.hInstance=ULOther::ULGetResourceHandle();
				m_ofn.lpstrDefExt=lpszDefExt;
				m_ofn.nFileExtension=0;
				m_ofn.lpstrFile=const_cast<LPTSTR>(lpszFileName);
				m_ofn.Flags=dwFlags;
				m_ofn.Flags|=(lpszDefExt!=NULL)?OFN_EXTENSIONDIFFERENT:0;

				m_ofn.lpstrFilter=lpszFilter;
				m_ofn.lpstrFile=m_strFile;
				m_ofn.nMaxFile=(DWORD)m_strFile.GetSize();
				m_ofn.lpstrFileTitle=m_strFileTitle;
				m_ofn.nMaxFileTitle=(DWORD)m_strFileTitle.GetSize();
				m_ofn.nFilterIndex=dwFilterIndex;
				m_ofn.lpstrFile[0]=_T('\0');
				m_ofn.lCustData=(LPARAM)this;
				m_ofn.lpfnHook=(LPOFNHOOKPROC)CULCommDlg::WndProc;
			}

			int CULFileDlg::CreateModal(short idTempl, HWND hParentWnd)
			{
				m_ofn.lpTemplateName=MAKEINTRESOURCE(idTempl);
				m_ofn.hwndOwner=hParentWnd;
				m_fModal=TRUE;
				if(m_fOpenFileDlg)
					return GetOpenFileName(&m_ofn);
				else
					return GetSaveFileName(&m_ofn);
			}

			LPCTSTR CULFileDlg::SetDefExt(LPCTSTR lpszDefExt)
			{
				LPCTSTR lpszOld=m_ofn.lpstrDefExt;
				m_ofn.lpstrDefExt=lpszDefExt;
				return lpszOld;
			}
			//==================CULFontDialog=======================================
			CULFontDlg::CULFontDlg(DWORD dwFlags):
				CULCommDlg()
			{
				::ZeroMemory(&m_cf,sizeof(m_cf));
				::ZeroMemory(&m_lf,sizeof(m_lf));
				m_cf.lStructSize=sizeof(m_cf);
				m_cf.lpLogFont=&m_lf;
				m_cf.rgbColors=0;
				m_cf.Flags=dwFlags;
				m_cf.lpfnHook=(LPCFHOOKPROC)CULCommDlg::WndProc;
				m_cf.lCustData=(LPARAM)this;
			}

			int CULFontDlg::CreateModal(short idTempl,HWND hParentWnd)
			{
				m_cf.lpTemplateName=MAKEINTRESOURCE(idTempl);
				m_cf.hwndOwner=hParentWnd;
				return ::ChooseFont(&m_cf);
			}

			BOOL CULFontDlg::GetLogFont(LOGFONT* plf)
			{
				if(!plf)
					return FALSE;
				memcpy(plf,&m_lf,sizeof(LOGFONT));
				return TRUE;
			}

			void CULFontDlg::SetLogFont(LOGFONT& lf)
			{
				m_cf.Flags|=CF_INITTOLOGFONTSTRUCT;
				memcpy(&m_lf,&lf,sizeof(LOGFONT));
			}

			void CULFontDlg::SetFontColor(COLORREF clrFont)
			{
				m_cf.Flags|=CF_EFFECTS;
				m_cf.rgbColors=clrFont;
			}

			COLORREF CULFontDlg::GetFontColor()
			{
				return m_cf.rgbColors;
			}

		}
	}
}

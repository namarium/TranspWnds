///\file ULToolBar.cpp
///\brief cpp файл класса тулбара(11.08.2007)
#include "..\..\ULLib\Include\ULToolbar.h"
#include "..\..\ULLib\Include\ULMenu.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULBars
	{
		CULToolBar::CULToolBar(void):CULSubClass(),
			m_dxBitmap(0),
			m_dyBitmap(0)
		{
			MessageMap.AddMessage<CULToolBar>(WM_WINDOWPOSCHANGING,&CULToolBar::OnWindowPosChanging);
			MessageMap.AddReflectNotify<CULToolBar>(TBN_DROPDOWN,&CULToolBar::OnDropDown);
		}
		CULToolBar::CULToolBar(CULToolBar& toolBar):CULSubClass(toolBar),
			m_dxBitmap(toolBar.m_dxBitmap),
			m_dyBitmap(toolBar.m_dyBitmap),
			m_TBToolTip(toolBar.m_TBToolTip),
			m_pInfoButtons(toolBar.m_pInfoButtons),
			m_afFlag(toolBar.m_afFlag),
			m_tbStyle(toolBar.m_tbStyle)
		{
		}

		CULToolBar::~CULToolBar(void)
		{
		}

		void CULToolBar::operator=(CULToolBar& toolBar)
		{
			m_dxBitmap=toolBar.m_dxBitmap;
			m_dyBitmap=toolBar.m_dyBitmap;
			m_TBToolTip=toolBar.m_TBToolTip;
			m_pInfoButtons=toolBar.m_pInfoButtons;
			m_afFlag=toolBar.m_afFlag;
			m_tbStyle=toolBar.m_tbStyle;
			ULWnds::CULSubClass::operator=(toolBar);
		}

		HWND CULToolBar::Create(HWND hParentWnd,
						UINT tbID,//ID тулбара
						int dxButton,//ширина кнопки
						int dyButton,//высота кнопки
						int dxBitmap,//ширина картинки для кнопки
						int dyBitmap,//высота картинки для кнопки
						UINT afFlag,
						DWORD tbStyle,
						DWORD tbExStyle)
		{
			m_tbStyle=tbStyle;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC  = ICC_BAR_CLASSES;
			InitCommonControlsEx(&icex);
		//==================================
			m_dxBitmap=dxBitmap;
			m_dyBitmap=dyBitmap;
			m_hParentWnd=hParentWnd;
			m_afFlag=afFlag;
			tbStyle|=afFlag;

			m_hWnd = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, 
				tbStyle, 0, 0, dxButton, dyButton, hParentWnd, 
				(HMENU)(INT_PTR)tbID, ULOther::ULGetResourceHandle(), NULL); 
			if(m_hWnd!=NULL)
			{
				SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0); 
				SendMessage(TB_SETEXTENDEDSTYLE , 0, tbExStyle ); 
			}
			//сабклассим тултип
			m_TBToolTip.m_pULToolBar=this;		
			m_TBToolTip.Attach((HWND)SendMessage(TB_GETTOOLTIPS));
			//сабклассируем тулбар
			return (Attach(m_hWnd))?m_hWnd:NULL;
		};

		int CULToolBar::AddButton(int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						LPCTSTR szTTip,
						LPCTSTR pszStr,
						int nBitmap,
						UINT_PTR nID,
						HINSTANCE hInst,
						HMENU hMenu)//хендл контекстного меню для кнопки(если btnStyle==BTNS_DROPDOWN)
		{
		//========================добавление в массив m_InfoButtons===============
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()+1);
			if((btnStyle&BTNS_SEP)==0)
			{
				if((btnStyle&BTNS_DROPDOWN)!=0)
					m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=hMenu;
				else
					m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip=szTTip;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=idCommand;
			}
			else
			{
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip[0]=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=0;
			}
		//=========================================================================	
			btnState|=(m_afFlag&CCS_VERT)?TBSTATE_WRAP:0;
			TBBUTTON tbButtonsAdd={nBitmap, idCommand, 
				btnState,
				btnStyle,
		#if defined(_WIN32) | defined(_WIN64)
				{0},
		#endif
				/*dwData*/0, (INT_PTR)pszStr};
			TBADDBITMAP tb;
			int  stdidx;
			tb.hInst=(nID>IDB_HIST_LARGE_COLOR)?ULOther::ULGetResourceHandle():hInst;
			tb.nID = nID;

			if(nID>IDB_HIST_LARGE_COLOR)
			{
				HIMAGELIST himl=(HIMAGELIST)SendMessage(TB_GETIMAGELIST,0,0);
				stdidx=ImageList_AddMasked(himl,(HBITMAP)nID,RGB(192,192,192));
				SendMessage(TB_SETIMAGELIST,0,LPARAM(himl));
			}
			else
			{
				stdidx = (int)SendMessage(TB_ADDBITMAP, nBitmap, (LPARAM)&tb);
			}
			tbButtonsAdd.iBitmap +=stdidx;

			SendMessage(TB_SETBUTTONSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			return (BOOL)SendMessage (TB_ADDBUTTONS,1,(LPARAM) &tbButtonsAdd);
		};

		int CULToolBar::AddButton(int idCommand,//ID контрола(кнопки)
						BYTE btnState,//состояние кнопки
						BYTE btnStyle,//стиль кнопки
						LPCTSTR szTTip,//тултип для кнопки
						LPCTSTR pszStr,//подпись для кнопки
						int nBitmap,//номер картинки для кнопки тулбара 
						HBITMAP hBitmap,//ID ресурса для картинки
						HMENU hMenu)//ID контекстного меню для кнопки(если btnStyle==BTNS_DROPDOWN)
		{
			return AddButton(idCommand,btnState,btnStyle,szTTip,pszStr,nBitmap,(INT_PTR)hBitmap,
				NULL,hMenu);
		}

		int CULToolBar::InsertButton(int nInto,//куда вставить
						int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						LPCTSTR szTTip,
						LPCTSTR pszStr,
						int nBitmap,
						UINT_PTR nID,
						HINSTANCE hInst,
						HMENU hMenu)
		{
		//========================вставка в массив m_InfoButtons===============
			ULOther::CULArr<tagInfoButtons> tmppInfoButtons(m_pInfoButtons);
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()+1);
			if((btnStyle&BTNS_SEP)==0)
			{
				if((btnStyle&BTNS_DROPDOWN)!=0)
					m_pInfoButtons[nInto].hMenu=hMenu;
				m_pInfoButtons[nInto].szToolTip=szTTip;
				m_pInfoButtons[nInto].idCommand=idCommand;
			}
			else
			{
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip[0]=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=0;
			}
			for(size_t i=0;i<(m_pInfoButtons.GetSize()-nInto-1);++i)
				m_pInfoButtons[i+nInto+1]=tmppInfoButtons[i+nInto];
		//===========================================================
			btnState|=(m_afFlag&CCS_VERT)?TBSTATE_WRAP:0;
			TBBUTTON tbButtonsAdd={nBitmap, idCommand, 
				btnState,
				btnStyle,
		#if defined(_WIN32) | defined(_WIN64)
				{0},
		#endif
				/*dwData*/0, (INT_PTR)pszStr};
			if((btnStyle&BTNS_SEP)==0)
			{
				TBADDBITMAP tb;
				int  stdidx;
				tb.hInst=(nID>IDB_HIST_LARGE_COLOR)?ULOther::ULGetResourceHandle():hInst;
				tb.nID = nID;

				if(nID>IDB_HIST_LARGE_COLOR)
				{
					HIMAGELIST himl=(HIMAGELIST)SendMessage(TB_GETIMAGELIST,0,0);
					stdidx=ImageList_AddMasked(himl,(HBITMAP)nID,RGB(192,192,192));
					SendMessage(TB_SETIMAGELIST,0,LPARAM(himl));
				}
				else
				{
					stdidx = (int)SendMessage (TB_ADDBITMAP, nBitmap, (LPARAM)&tb);
				}
				tbButtonsAdd.iBitmap +=stdidx;
			}
			SendMessage(TB_SETBUTTONSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_INSERTBUTTON,nInto,LPARAM(&tbButtonsAdd));
			return 0;
		};

		int CULToolBar::InsertButton(int nInto,//куда вставить
						int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						LPCTSTR szTTip,
						LPCTSTR pszStr,
						int nBitmap,//номер картинки для кнопки тулбара 
						HBITMAP hBitmap,//ID ресурса для картинки
						HMENU hMenu)
		{
			return InsertButton(nInto,idCommand,btnState,btnStyle,szTTip,pszStr,nBitmap,
				(INT_PTR)hBitmap,NULL,hMenu);
		}

		int CULToolBar::DeleteButton(int nButton)
		{
		//========================удаление из массива m_InfoButtons===============
			ULOther::CULArr<tagInfoButtons> tmppInfoButtons(m_pInfoButtons.GetSize());
			for(unsigned int i=0;i<m_pInfoButtons.GetSize();++i)
			{
				tmppInfoButtons,m_pInfoButtons[i].hMenu=m_pInfoButtons[i].hMenu;
				tmppInfoButtons,m_pInfoButtons[i].idCommand=m_pInfoButtons[i].idCommand;
				tmppInfoButtons,m_pInfoButtons[i].szToolTip=m_pInfoButtons[i].szToolTip;
			}	
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()-1);
			int counter=0;
			for(int i=0;i<signed(m_pInfoButtons.GetSize()+1);i++)
				if(i!=nButton)
				{
					m_pInfoButtons,m_pInfoButtons[counter].hMenu=tmppInfoButtons[i].hMenu;
					m_pInfoButtons,m_pInfoButtons[counter].idCommand=tmppInfoButtons[i].idCommand;
					m_pInfoButtons,m_pInfoButtons[counter].szToolTip=tmppInfoButtons[i].szToolTip;
					counter++;
				};
		//=========================================================================	
			SendMessage(TB_DELETEBUTTON,nButton,0);
			return 0;
		};


		void CULToolBar::AutoSize()//вставить в обработчик WM_SIZE окна родителя
		{
			if((m_afFlag&CCS_VERT)!=CCS_VERT)
				SendMessage(TB_AUTOSIZE); 
		}

		BOOL CULToolBar::EnableButton(int idButton,BOOL fCheck)
		{
			return (BOOL)SendMessage(TB_ENABLEBUTTON,(WPARAM)idButton,(LPARAM)fCheck);
		}

		BOOL CULToolBar::CheckButton(int idButton,BOOL fCheck)
		{
			return (BOOL)SendMessage(TB_CHECKBUTTON,(WPARAM)idButton,(LPARAM)fCheck);
		}

		DWORD CULToolBar::SetExtendedStyle(DWORD dwExStyle)
		{
			return(DWORD)SendMessage(TB_SETEXTENDEDSTYLE,NULL,(LPARAM)dwExStyle);
		}

		DWORD CULToolBar::GetExtendedStyle()
		{
			return(DWORD)SendMessage(TB_GETEXTENDEDSTYLE,NULL,NULL);
		}

		SIZE CULToolBar::GetButtonSize()
		{
			DWORD dwSize=(DWORD)SendMessage(TB_GETBUTTONSIZE);
			SIZE szRet={LOWORD(dwSize),HIWORD(dwSize)};
			return szRet;
		}

		SIZE CULToolBar::GetBitmapSize()
		{
			SIZE szRet={m_dxBitmap,m_dyBitmap};
			return szRet;
		}

		HIMAGELIST CULToolBar::GetImageList()
		{
			return (HIMAGELIST)SendMessage(TB_GETIMAGELIST);
		}

		HIMAGELIST CULToolBar::SetImageList(int iImageID,HIMAGELIST himlNew)
		{
			return (HIMAGELIST)SendMessage(TB_SETIMAGELIST,(WPARAM)iImageID,(LPARAM)himlNew);
		}

		LRESULT CULToolBar::OnWindowPosChanging(WPARAM,LPARAM lParam)
		{
			if((m_tbStyle&TBSTYLE_AUTOSIZE)==0)
			{
				WINDOWPOS* pWP=(WINDOWPOS*)lParam;
				RECT rcTBWidth;
				SendMessage(TB_GETITEMRECT,
					GetButtonCount()-1,(LPARAM)&rcTBWidth);
				if((pWP->cx>rcTBWidth.right))
						pWP->cx=rcTBWidth.right;
			}
			return FALSE;
		}

		LRESULT CULToolBar::OnDropDown(LPARAM lParam)
		{
			#define lpnm   ((LPNMHDR)lParam)
			#define lpnmTB ((LPNMTOOLBAR)lParam)
			RECT rc;
			TPMPARAMS tpm;
			::SendMessage(lpnmTB->hdr.hwndFrom, TB_GETRECT,
				(WPARAM)lpnmTB->iItem, (LPARAM)&rc);
				MapWindowPoints(lpnmTB->hdr.hwndFrom,
				HWND_DESKTOP, (LPPOINT)&rc, 2);                         
			tpm.cbSize = sizeof(TPMPARAMS);
			tpm.rcExclude = rc;
			for(unsigned int i=0;i<m_pInfoButtons.GetSize();i++)
				if(lpnmTB->iItem==(signed)m_pInfoButtons[i].idCommand)
				{
					ULControls::CULMenu PopupMenu(m_pInfoButtons[i].hMenu);
					POINT pt={rc.left, rc.bottom};
					PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,               
						pt, m_hParentWnd, &tpm); 	
					PopupMenu.Detach();
				}
			return TBDDRET_DEFAULT;
		}
		CULToolBar::CTBToolTip::CTBToolTip():ULControls::CULToolTip()
		{
			MessageMap.AddReflectNotify<CTBToolTip>(TTN_GETDISPINFO,&CTBToolTip::OnGetDispInfo);
		};

		CULToolBar::CTBToolTip::~CTBToolTip(){};
		LONG CULToolBar::CTBToolTip::OnGetDispInfo(LPARAM lParam)
		{
			#define lpnm   ((LPNMHDR)lParam)
			#define lpttt ((LPTOOLTIPTEXT)lParam)
			lpttt->hinst = ULOther::ULGetResourceHandle(); 
			UINT_PTR idButton = lpttt->hdr.idFrom; 
			for(unsigned int i=0;i<m_pULToolBar->m_pInfoButtons.GetSize();i++)
				if(idButton==m_pULToolBar->m_pInfoButtons[i].idCommand)
				{
					lpttt->lpszText = m_pULToolBar->m_pInfoButtons[i].szToolTip;
					break;
				}
			return TRUE;
		};
	}
}

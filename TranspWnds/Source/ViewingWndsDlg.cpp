/*
 * Copyright © 2009-2011 Bombizov Alexandr
 * This file is part of TranspWnds.
 *
 * TranspWnds is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TranspWnds is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with TranspWnds.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _WIN32_WINNT 0x0500
#include "Hook.h"

#include "ViewingWndsDlg.h"
#include "resource.h"
#include <sstream>

CViewingWndsDlg::CViewingWndsDlg(void):
	CULDlg()
{
	MessageMap.AddCommand<CViewingWndsDlg>(ciTransparent,&CViewingWndsDlg::OnTransparent);
	MessageMap.AddCommand<CViewingWndsDlg>(ciTransparentLevel,&CViewingWndsDlg::OnTransparentLevelComboSelenOk,CBN_SELENDOK);
	MessageMap.AddCommand<CViewingWndsDlg>(ciTopMost,&CViewingWndsDlg::OnTopMost);
	MessageMap.AddCommand<CViewingWndsDlg>(ciHideCaption,&CViewingWndsDlg::OnHideCaption);
}

CViewingWndsDlg::~CViewingWndsDlg(void)
{
}

LRESULT CViewingWndsDlg::OnInitDialog(WPARAM w,LPARAM l)
{
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,FALSE);
	//#define CS_DROPSHADOW       0x00020000
	SetClassLong(GCL_STYLE,(LONG)GetClassLong(GCL_STYLE)|0x00020000);
	m_lpSubClassWndProc=NULL;
	//================================================================
	ASSERT(m_listctrlViewingWnds.Attach(GetDlgItem(IDC_LIST_VIEWING_WINDOWS)));
	m_listctrlViewingWnds.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_listctrlViewingWnds.InsertColumn(1,CULStrTable(IDS_VIEWING_LIST_COLUMN_TRANSPARENT),LVCFMT_LEFT);
	m_listctrlViewingWnds.InsertColumn(2,CULStrTable(IDS_VIEWING_LIST_COLUMN_TRANSPARENT_LEVEL),LVCFMT_LEFT);
	m_listctrlViewingWnds.InsertColumn(3,CULStrTable(IDS_VIEWING_LIST_COLUMN_TOPMOST),LVCFMT_LEFT);
	m_listctrlViewingWnds.InsertColumn(4,CULStrTable(IDS_VIEWING_LIST_COLUMN_HIDECAPTION),LVCFMT_LEFT);
	m_listctrlViewingWnds.InsertColumn(0,CULStrTable(IDS_VIEWING_LIST_COLUMN_CAPTION),LVCFMT_LEFT);

	m_listctrlViewingWnds.SetColumnWidth(0,142);
	m_listctrlViewingWnds.SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
	m_listctrlViewingWnds.SetColumnWidth(2,LVSCW_AUTOSIZE_USEHEADER);
	m_listctrlViewingWnds.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
	int nColWidth=0;
	for(int i=0;i<4;++i)
		nColWidth+=m_listctrlViewingWnds.GetColumnWidth(i);
	RECT rc;
	GetClientRect(&rc);
	m_listctrlViewingWnds.SetColumnWidth(4,rc.right-nColWidth-30);
	InitList();
	return CULDlg::OnInitDialog(w,l);
}


namespace ULWnds
{
	namespace ULControls
	{
		namespace ULListCtrlEx
		{
//#pragma message(__FILE__":избавиться от IOM_GETITEMDATA")
			const int IOM_GETITEMDATA=WM_APP+1;
			//=====================================================================
			///\class CItemCheckBox
			///\brief класс чекбокса объекта для итема
			class CItemCheckBox:
				public IItemObject		
			{	
			protected:
				///\brief хендл темы ХР
				HTHEME m_hTheme;
			protected:
				class CButton:
					public ULButtons::CULButton
				{
				protected:
					DWORD m_dwData;
				public:
					CButton();
					void SetItemData(DWORD dwData);
				protected:
					virtual LRESULT OnKillFocus(WPARAM,LPARAM);
					virtual LRESULT OnGetItemData(WPARAM,LPARAM);

				};
			public:
				///\brief служит для отлова потери фокуса у объекта
				CButton m_Button;
			public:
				///\brief Деструктор
				~CItemCheckBox();
				///\brief фунция создания объекта
				///\param hParentWnd - хэндл родительского окна, обычно это хендл контейнера CULListCtrlEx
				///\param uID - Идентификатор контрола
				///\param pszStr - начальный текст в кнопке
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr);
				///\brief устанавливает/снимает флажок
				///\param fCheck - флаг установки
				void SetCheck(BOOL fCheck);
				///\brief возвращает состояние чекбокса
				///\return состояние чекбокса
				BOOL GetCheck();

				void SetItemData(DWORD dwData);

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/);
				virtual void OnClick(RECT& rc);
				virtual bool GetRect(RECT* /*prc*/){return false;}
				virtual void Clean();
				virtual void OnThemeChanged();
			};
			//=====================================================================
			CItemCheckBox::CButton::CButton():
				ULButtons::CULButton(),
				m_dwData(NULL)
			{
				MessageMap.AddMessage<CButton>(WM_KILLFOCUS,&CButton::OnKillFocus);
				MessageMap.AddMessage<CButton>(ULListCtrlEx::IOM_GETITEMDATA,&CButton::OnGetItemData);
			}

			void CItemCheckBox::CButton::SetItemData(DWORD dwData)
			{
				m_dwData=dwData;
			}

			LRESULT CItemCheckBox::CButton::OnKillFocus(WPARAM,LPARAM)
			{
				ShowWindow(SW_HIDE);
				return FALSE;
			}
			LRESULT CItemCheckBox::CButton::OnGetItemData(WPARAM,LPARAM)
			{
				return (LRESULT)m_dwData;
			}

			CItemCheckBox::~CItemCheckBox()
			{
				m_Button.DestroyWindow();

				if(m_hTheme)
					CloseThemeData(m_hTheme);
			}
			BOOL CItemCheckBox::Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr)
			{
				if(!m_Button.Create(hParentWnd,pszStr,0,0,0,0,uID,
					WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|WS_TABSTOP|
					BS_CHECKBOX|BS_CENTER|BS_AUTOCHECKBOX))
					return FALSE;
				m_Button.ModifyStyleEx(0,WS_EX_TRANSPARENT);
				m_Button.ShowWindow(SW_HIDE);
				m_Button.SetFont((HFONT)::SendMessage(hParentWnd,WM_GETFONT,0,0),FALSE);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_Button,L"BUTTON");
				else
					m_hTheme=NULL;
				return TRUE;
			}
			void CItemCheckBox::SetCheck(BOOL fCheck)
			{
				m_Button.SetCheck((fCheck)?
					ULWnds::ULControls::ULButtons::CULButton::csChecked:
					ULWnds::ULControls::ULButtons::CULButton::csUnchecked);
			}
			BOOL CItemCheckBox::GetCheck()
			{
				return (m_Button.GetCheck()==ULWnds::ULControls::ULButtons::CULButton::csChecked);
			}

			void CItemCheckBox::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				RECT rectBtn={
					rc.left+(rc.right-rc.left-::GetSystemMetrics(SM_CXMENUCHECK))/2,
					rc.top+(rc.bottom-rc.top-::GetSystemMetrics(SM_CYMENUCHECK))/2,
					rc.left+(rc.right-rc.left+::GetSystemMetrics(SM_CXMENUCHECK))/2,
					rc.top+(rc.bottom-rc.top+::GetSystemMetrics(SM_CYMENUCHECK))/2,
				};

				if(m_Button.IsWindowVisible()||!m_Button.IsWindowEnabled())
				{
					if(rc.top>10)
						m_Button.ShowWindow(SW_SHOW);
					m_Button.MoveWindow(rectBtn.left,rectBtn.top,
						rectBtn.right-rectBtn.left,
						rectBtn.bottom-rectBtn.top,TRUE);
				}
				else
				{

					if(m_hTheme&&IsThemeActive())
						DrawThemeBackground(m_hTheme,*pDC,BP_CHECKBOX,
						((m_Button.GetCheck())?CBS_CHECKEDNORMAL:CBS_UNCHECKEDNORMAL),
						&rectBtn,NULL);
					else
						DrawFrameControl(*pDC,&rectBtn,DFC_BUTTON|DFCS_BUTTONCHECK,
							((m_Button.GetCheck())?DFCS_CHECKED:0)|DFCS_BUTTONCHECK);
				}
			}
			void CItemCheckBox::Clean()
			{
				m_Button.SetWindowText(_T(""));
			}
			void CItemCheckBox::OnClick(RECT& rc)
			{
				if(rc.top>10)
					m_Button.ShowWindow(SW_SHOW);
				m_Button.MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top-1,TRUE);
				m_Button.SetFocus();
				m_Button.SetActiveWindow();
			}
			void CItemCheckBox::OnLButtonDown(RECT& rc)
			{
				if(!m_Button.IsWindowEnabled())
					return;
				if(rc.top>10)
					m_Button.ShowWindow(SW_SHOW);
				m_Button.MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top-1,TRUE);
				m_Button.SetFocus();
				m_Button.SetActiveWindow();
				m_Button.SetState(TRUE);
			}
			void CItemCheckBox::OnThemeChanged()
			{				
				if(m_hTheme)
					CloseThemeData(m_hTheme);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_Button,L"BUTTON");
				else
					m_hTheme=NULL;
			}
			void CItemCheckBox::SetItemData(DWORD dwData)
			{
				m_Button.SetItemData(dwData);
			}
			//=====================================================================

		}
	}
}

void CViewingWndsDlg::InitList()
{
	for(std::map<HWND,CHook::WNDINFO>::iterator i=CHook::GetHook()->m_mapWndInfo.begin();
		i!=CHook::GetHook()->m_mapWndInfo.end();i++)
		InsertItem(i->first);
}

void CViewingWndsDlg::InsertItem(HWND hwndItem)
{
	std::map<HWND,CHook::WNDINFO>::iterator i=CHook::GetHook()->m_mapWndInfo.find(hwndItem);

	bool fExist=false;
	int nFindItem=0;
	for(;nFindItem<m_listctrlViewingWnds.GetItemCount();++nFindItem)
	{
		ULListCtrlEx::CItemCheckBox* pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)m_listctrlViewingWnds.GetItemObject(nFindItem,1);
		HWND hwndTmp=(HWND)pItemCheckBox->m_Button.SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
		if(hwndItem==hwndTmp)
		{
			fExist=true;
			break;
		}
	}

	if(::IsWindow(hwndItem)&&!fExist)
	{
		TCHAR szCaption[255];
		::GetWindowText(hwndItem,szCaption,sizeof(szCaption)/sizeof(szCaption[0]));

		int nItem=m_listctrlViewingWnds.InsertItem(
			m_listctrlViewingWnds.GetItemCount(),szCaption);
		ULListCtrlEx::CItemText* pItemText=(ULListCtrlEx::CItemText*)
			m_listctrlViewingWnds.GetItemObject(nItem,0);
		pItemText->SetIcon(GetWindowIcon(i->first));
		pItemText->SetText(szCaption);

		ULListCtrlEx::CItemCheckBox* pItemCheckBox=new ULListCtrlEx::CItemCheckBox;
		pItemCheckBox->Create(m_listctrlViewingWnds,ciTransparent,NULL);
		pItemCheckBox->SetCheck(i->second.fAlpha);
		pItemCheckBox->SetItemData((DWORD)(LONG_PTR)const_cast<HWND>(i->first));
		m_listctrlViewingWnds.SetItemObject(nItem,1,pItemCheckBox);

		{		
			ULListCtrlEx::CItemComboBox* pItemComboBox=new ULListCtrlEx::CItemComboBox;
			pItemComboBox->Create(m_listctrlViewingWnds,ciTransparentLevel,TRUE);

			for(int ii=(100-int(100*double(CHook::GetHook()->m_bMinTranspVal)/255 + 0.5));
				ii>=10;ii-=int(100*double(CHook::GetHook()->m_bTranspStep)/255) + 0.5)
			{
				std::basic_stringstream<TCHAR>ss;
				ss<<ii;
				TCHAR szValAlpha[10];
				ss>>szValAlpha;
				int nItem=pItemComboBox->m_ComboBox.AddString(szValAlpha);
				pItemComboBox->m_ComboBox.SetItemData(nItem,(DWORD)ii);					
			}
			pItemComboBox->m_ComboBox.AddString(_T("0"));
			if(i->second.fAlpha)
			{
				pItemComboBox->m_ComboBox.SetCurSel(
					i->second.bAlpha/CHook::GetHook()->m_bTranspStep);
			}
			else
				pItemComboBox->m_ComboBox.EnableWindow(FALSE);
			m_listctrlViewingWnds.SetItemObject(nItem,2,pItemComboBox);
		}

		pItemCheckBox=new ULListCtrlEx::CItemCheckBox;
		pItemCheckBox->Create(m_listctrlViewingWnds,ciTopMost,NULL);
		pItemCheckBox->SetItemData((DWORD)(LONG_PTR)const_cast<HWND>(i->first));
		pItemCheckBox->SetCheck(i->second.fTopMost);
		m_listctrlViewingWnds.SetItemObject(nItem,3,pItemCheckBox);

		pItemCheckBox=new ULListCtrlEx::CItemCheckBox;
		pItemCheckBox->Create(m_listctrlViewingWnds,ciHideCaption,NULL);
		pItemCheckBox->SetItemData((DWORD)(LONG_PTR)const_cast<HWND>(i->first));
		pItemCheckBox->SetCheck(i->second.dwStyle);
		m_listctrlViewingWnds.SetItemObject(nItem,4,pItemCheckBox);	
	}
	if(fExist)
	{
		TCHAR szCaption[255];
		::GetWindowText(hwndItem,szCaption,sizeof(szCaption)/sizeof(szCaption[0]));

		ULListCtrlEx::CItemText* pItemText=(ULListCtrlEx::CItemText*)
			m_listctrlViewingWnds.GetItemObject(nFindItem,0);
		pItemText->SetIcon(GetWindowIcon(i->first));
		pItemText->SetText(szCaption);

		ULListCtrlEx::CItemCheckBox* pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)
			m_listctrlViewingWnds.GetItemObject(nFindItem,1);
		pItemCheckBox->SetCheck(i->second.fAlpha);

		pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)
			m_listctrlViewingWnds.GetItemObject(nFindItem,3);
		pItemCheckBox->SetCheck(i->second.fTopMost);

		pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)
			m_listctrlViewingWnds.GetItemObject(nFindItem,4);
		pItemCheckBox->SetCheck(i->second.dwStyle);
		m_listctrlViewingWnds.InvalidateRect();
	}
}

void CViewingWndsDlg::DeleteItem(HWND hwndItem)
{
	for(int i=0;i<m_listctrlViewingWnds.GetItemCount();++i)
	{
		ULListCtrlEx::CItemCheckBox* pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)m_listctrlViewingWnds.GetItemObject(i,1);
		HWND hwndTmp=(HWND)pItemCheckBox->m_Button.SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
		if(hwndItem==hwndTmp)
		{
			m_listctrlViewingWnds.DeleteItem(i);
			break;
		}
	}
}

void CViewingWndsDlg::OnTransparent(WORD,HWND hWnd)
{
	CULButton* pButton=(CULButton*)CULWnd::FromHandle(hWnd);
	HWND hwndManaged=(HWND)pButton->SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
	if(!::IsWindow(hwndManaged))
		return;
	CHook::GetHook()->m_mapWndInfo[hwndManaged].fAlpha=(BOOL)pButton->GetCheck();

	int nItem=0;
	for(;nItem<m_listctrlViewingWnds.GetItemCount();++nItem)
	{
		ULListCtrlEx::CItemCheckBox* pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)m_listctrlViewingWnds.GetItemObject(nItem,1);
		HWND hwndTmp=(HWND)pItemCheckBox->m_Button.SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
		if(hwndManaged==hwndTmp)
			break;
	}
	ULListCtrlEx::CItemComboBox* pItemComboBox=(ULListCtrlEx::CItemComboBox*)m_listctrlViewingWnds.GetItemObject(nItem,2);
	if(!pItemComboBox)
		return;

	pItemComboBox->m_ComboBox.EnableWindow(CHook::GetHook()->m_mapWndInfo[hwndManaged].fAlpha);

	if(CHook::GetHook()->m_mapWndInfo[hwndManaged].fAlpha)
	{
		if(CHook::GetHook()->m_mapWndInfo[hwndManaged].bAlpha==0)
			CHook::GetHook()->m_mapWndInfo[hwndManaged].bAlpha=255;
		::SetWindowLong(hwndManaged,GWL_EXSTYLE,::GetWindowLong(hwndManaged,GWL_EXSTYLE)|WS_EX_LAYERED);
		::SetLayeredWindowAttributes(hwndManaged,0,CHook::GetHook()->m_mapWndInfo[hwndManaged].bAlpha,LWA_ALPHA);
	}
	else
	{
		::SetLayeredWindowAttributes(hwndManaged,0,255,LWA_ALPHA);
		::SetWindowLong(hwndManaged,GWL_EXSTYLE,::GetWindowLong(hwndManaged,GWL_EXSTYLE)&~WS_EX_LAYERED);
		CHook::GetHook()->m_mapWndInfo[hwndManaged].fAlpha=FALSE;
		//::RedrawWindow(hwndManaged,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
	}
}

void CViewingWndsDlg::OnTransparentLevelComboSelenOk(WORD,HWND hwndCombo)
{
	int nItem=0;
	BYTE bAlpha=0;
	for(;nItem<m_listctrlViewingWnds.GetItemCount();++nItem)
	{
		ULListCtrlEx::CItemComboBox* pItemComboBox=(ULListCtrlEx::CItemComboBox*)m_listctrlViewingWnds.GetItemObject(nItem,2);
		if(hwndCombo==pItemComboBox->m_ComboBox)
		{
			bAlpha=(BYTE)pItemComboBox->m_ComboBox.GetItemData(
				pItemComboBox->m_ComboBox.GetCurSel());
			break;
		}
	}
	ULListCtrlEx::CItemCheckBox* pItemCheckBox=(ULListCtrlEx::CItemCheckBox*)m_listctrlViewingWnds.GetItemObject(nItem,1);
	HWND hwndManaged=(HWND)pItemCheckBox->m_Button.SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);

	CHook::GetHook()->m_mapWndInfo[hwndManaged].bAlpha=
		255-int(255*double(bAlpha)/100);
	::SetLayeredWindowAttributes(hwndManaged,0,CHook::GetHook()->m_mapWndInfo[hwndManaged].bAlpha,LWA_ALPHA);	

}

void CViewingWndsDlg::OnTopMost(WORD,HWND hWnd)
{
	CULButton* pButton=(CULButton*)CULWnd::FromHandle(hWnd);
	HWND hwndManaged=(HWND)pButton->SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
	if(!::IsWindow(hwndManaged))
		return;
	CHook::GetHook()->m_mapWndInfo[hwndManaged].fTopMost=(BOOL)pButton->GetCheck();
	if(CHook::GetHook()->m_mapWndInfo[hwndManaged].fTopMost)
		::SetWindowPos(hwndManaged,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	else
		::SetWindowPos(hwndManaged,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}
void CViewingWndsDlg::OnHideCaption(WORD,HWND hWnd)
{
	CULButton* pButton=(CULButton*)CULWnd::FromHandle(hWnd);
	HWND hwndManaged=(HWND)pButton->SendMessage(ULListCtrlEx::IOM_GETITEMDATA,0,0);
	if(!::IsWindow(hwndManaged))
		return;
	BOOL fHideCaption=(BOOL)pButton->GetCheck();
	if(fHideCaption)
	{
		if((::GetWindowLong(hwndManaged,GWL_STYLE)&WS_CAPTION)==WS_CAPTION)
		{
			CHook::GetHook()->m_mapWndInfo[hwndManaged].dwStyle=(DWORD)::GetWindowLong(hwndManaged,GWL_STYLE);
			::SetWindowLong(hwndManaged,GWL_STYLE,
				::GetWindowLong(hwndManaged,GWL_STYLE)&~WS_CAPTION);
			::SetWindowPos(hwndManaged,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
		}
	}
	else
	{
		::SetWindowLong(hwndManaged,GWL_STYLE,
			::GetWindowLong(hwndManaged,GWL_STYLE)|CHook::GetHook()->m_mapWndInfo[hwndManaged].dwStyle);
		::SetWindowPos(hwndManaged,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
		CHook::GetHook()->m_mapWndInfo[hwndManaged].dwStyle=0;
	}
}
void CViewingWndsDlg::OnCancel(WORD,HWND)
{
	CULDlg::OnCancel(0,0);
}

LRESULT CViewingWndsDlg::OnDestroy(WPARAM,LPARAM)
{
	return CULDlg::OnDestroy(0,0);
}

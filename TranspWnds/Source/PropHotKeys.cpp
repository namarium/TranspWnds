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
#include "PropHotKeys.h"
#include "resource.h"
CPropHotKeys::CPropHotKeys(void):
	CULPropPage()
{
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORSTATIC,&CPropHotKeys::OnCtlColor);
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORBTN,&CPropHotKeys::OnCtlColor);
}

CPropHotKeys::~CPropHotKeys(void)
{
}

LRESULT CPropHotKeys::OnInitDialog(WPARAM w,LPARAM l)
{
	//инициализируем горячие клавиши изменения прозрачности
	{
		SendDlgItemMessage(IDC_CHECK_TRANSP_ALT,BM_SETCHECK,
			m_arHotkey[hkoTransp].m_fAlt,0);
		SendDlgItemMessage(IDC_CHECK_TRANSP_CTRL,BM_SETCHECK,
			m_arHotkey[hkoTransp].m_fCtrl,0);
		SendDlgItemMessage(IDC_CHECK_TRANSP_SHIFT,BM_SETCHECK,
			m_arHotkey[hkoTransp].m_fShift,0);
		SendDlgItemMessage(IDC_CHECK_TRANSP_WIN,BM_SETCHECK,
			m_arHotkey[hkoTransp].m_fWin,0);
	}
	//инициализируем горячие клавиши для топмост
	{
		SendDlgItemMessage(IDC_CHECK_TOPMOST_ALT,BM_SETCHECK,
			m_arHotkey[hkoTopMost].m_fAlt,0);
		SendDlgItemMessage(IDC_CHECK_TOPMOST_CTRL,BM_SETCHECK,
			m_arHotkey[hkoTopMost].m_fCtrl,0);
		SendDlgItemMessage(IDC_CHECK_TOPMOST_SHIFT,BM_SETCHECK,
			m_arHotkey[hkoTopMost].m_fShift,0);
		SendDlgItemMessage(IDC_CHECK_TOPMOST_WIN,BM_SETCHECK,
			m_arHotkey[hkoTopMost].m_fWin,0);
		CULComboBox cbTopMost;
		cbTopMost.Attach(GetDlgItem(IDC_COMBO_TOPMOST_MSG));
		int nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_COMBO_LBTN));
		cbTopMost.SetItemData(nItem,WM_LBUTTONDOWN);
		nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_COMBO_RBTN));
		cbTopMost.SetItemData(nItem,WM_RBUTTONDOWN);
		nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_COMBO_MBTN));
		cbTopMost.SetItemData(nItem,WM_MBUTTONDOWN);
		for(int i=0;i<cbTopMost.GetCount();++i)
			if(m_arHotkey[hkoTopMost].IsMsg(0,cbTopMost.GetItemData(i)))
			{
				cbTopMost.SetCurSel(i);
				break;
			}
		cbTopMost.Detach();
	}
	//инициализируем горячие клавиши для move
	{
		SendDlgItemMessage(IDC_CHECK_MOVE_ALT,BM_SETCHECK,
			m_arHotkey[hkoMoveWnd].m_fAlt,0);
		SendDlgItemMessage(IDC_CHECK_MOVE_CTRL,BM_SETCHECK,
			m_arHotkey[hkoMoveWnd].m_fCtrl,0);
		SendDlgItemMessage(IDC_CHECK_MOVE_SHIFT,BM_SETCHECK,
			m_arHotkey[hkoMoveWnd].m_fShift,0);
		SendDlgItemMessage(IDC_CHECK_MOVE_WIN,BM_SETCHECK,
			m_arHotkey[hkoMoveWnd].m_fWin,0);
		CULComboBox cbMove;
		cbMove.Attach(GetDlgItem(IDC_COMBO_MOVE_MSG));
		int nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_COMBO_LBTN));
		cbMove.SetItemData(nItem,WM_LBUTTONDOWN);
		nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_COMBO_RBTN));
		cbMove.SetItemData(nItem,WM_RBUTTONDOWN);
		nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_COMBO_MBTN));
		cbMove.SetItemData(nItem,WM_MBUTTONDOWN);
		for(int i=0;i<cbMove.GetCount();++i)
			if(m_arHotkey[hkoMoveWnd].IsMsg(0,cbMove.GetItemData(i)))
			{
				cbMove.SetCurSel(i);
				break;
			}
		cbMove.Detach();
	}
	//инициализируем горячие клавиши для size
	{
		SendDlgItemMessage(IDC_CHECK_SIZE_ALT,BM_SETCHECK,
			m_arHotkey[hkoSizeWnd].m_fAlt,0);
		SendDlgItemMessage(IDC_CHECK_SIZE_CTRL,BM_SETCHECK,
			m_arHotkey[hkoSizeWnd].m_fCtrl,0);
		SendDlgItemMessage(IDC_CHECK_SIZE_SHIFT,BM_SETCHECK,
			m_arHotkey[hkoSizeWnd].m_fShift,0);
		SendDlgItemMessage(IDC_CHECK_SIZE_WIN,BM_SETCHECK,
			m_arHotkey[hkoSizeWnd].m_fWin,0);
		CULComboBox cbSize;
		cbSize.Attach(GetDlgItem(IDC_COMBO_SIZE_MSG));
		int nItem=cbSize.AddString(CULStrTable(IDS_OPTOINS_COMBO_LBTN));
		cbSize.SetItemData(nItem,WM_LBUTTONDOWN);
		nItem=cbSize.AddString(CULStrTable(IDS_OPTOINS_COMBO_RBTN));
		cbSize.SetItemData(nItem,WM_RBUTTONDOWN);
		nItem=cbSize.AddString(CULStrTable(IDS_OPTOINS_COMBO_MBTN));
		cbSize.SetItemData(nItem,WM_MBUTTONDOWN);
		for(int i=0;i<cbSize.GetCount();++i)
			if(m_arHotkey[hkoSizeWnd].IsMsg(0,cbSize.GetItemData(i)))
			{
				cbSize.SetCurSel(i);
				break;
			}
		cbSize.Detach();
	}	
	//инициализируем горячие клавиши для ToggleCaption
	{
		SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_ALT,BM_SETCHECK,
			m_arHotkey[hkoToggleCaption].m_fAlt,0);
		SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_CTRL,BM_SETCHECK,
			m_arHotkey[hkoToggleCaption].m_fCtrl,0);
		SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_SHIFT,BM_SETCHECK,
			m_arHotkey[hkoToggleCaption].m_fShift,0);
		SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_WIN,BM_SETCHECK,
			m_arHotkey[hkoToggleCaption].m_fWin,0);
		CULComboBox cbToggleCaption;
		cbToggleCaption.Attach(GetDlgItem(IDC_COMBO_TOGGLECAPTION_MSG));
		int nItem=cbToggleCaption.AddString(CULStrTable(IDS_OPTOINS_COMBO_LBTN));
		cbToggleCaption.SetItemData(nItem,WM_LBUTTONDOWN);
		nItem=cbToggleCaption.AddString(CULStrTable(IDS_OPTOINS_COMBO_RBTN));
		cbToggleCaption.SetItemData(nItem,WM_RBUTTONDOWN);
		nItem=cbToggleCaption.AddString(CULStrTable(IDS_OPTOINS_COMBO_MBTN));
		cbToggleCaption.SetItemData(nItem,WM_MBUTTONDOWN);
		for(int i=0;i<cbToggleCaption.GetCount();++i)
			if(m_arHotkey[hkoToggleCaption].IsMsg(0,cbToggleCaption.GetItemData(i)))
			{
				cbToggleCaption.SetCurSel(i);
				break;
			}
		cbToggleCaption.Detach();
	}
	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropHotKeys::OnCtlColor(WPARAM w,LPARAM l)
{
	if(::GetDlgCtrlID((HWND)l)==(short)-1)
	{
		TCHAR szClass[50];
		if(::GetClassName((HWND)l,szClass,50))
			if(_tcsicmp(szClass,_T("BUTTON"))==0)
				return 0;
	}
	HDC hDC=(HDC)w;
	::SetBkMode(hDC,TRANSPARENT);
	return (LRESULT)::GetStockObject(NULL_BRUSH);
}

LRESULT CPropHotKeys::OnApply(BYTE nReturn)
{
	//возвращаем значение горячих клавиш изменения прозрачности
	{
		m_arHotkey[hkoTransp].m_fAlt=(SendDlgItemMessage(IDC_CHECK_TRANSP_ALT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTransp].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_TRANSP_CTRL,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTransp].m_fShift=(SendDlgItemMessage(IDC_CHECK_TRANSP_SHIFT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTransp].m_fWin=(SendDlgItemMessage(IDC_CHECK_TRANSP_WIN,
			BM_GETCHECK,0,0)!=0);
	}
	//возвращаем значение горячих клавиш для топмост
	{
		m_arHotkey[hkoTopMost].m_fAlt=(SendDlgItemMessage(IDC_CHECK_TOPMOST_ALT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTopMost].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_TOPMOST_CTRL,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTopMost].m_fShift=(SendDlgItemMessage(IDC_CHECK_TOPMOST_SHIFT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoTopMost].m_fWin=(SendDlgItemMessage(IDC_CHECK_TOPMOST_WIN,
			BM_GETCHECK,0,0)!=0);
		CULComboBox cbTopMost;
		cbTopMost.Attach(GetDlgItem(IDC_COMBO_TOPMOST_MSG));
		m_arHotkey[hkoTopMost].m_uMsg[0]=cbTopMost.GetItemData(cbTopMost.GetCurSel());
		cbTopMost.Detach();
	}
	//возвращаем значение горячих клавиш для move
	{
		m_arHotkey[hkoMoveWnd].m_fAlt=(SendDlgItemMessage(IDC_CHECK_MOVE_ALT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoMoveWnd].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_MOVE_CTRL,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoMoveWnd].m_fShift=(SendDlgItemMessage(IDC_CHECK_MOVE_SHIFT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoMoveWnd].m_fWin=(SendDlgItemMessage(IDC_CHECK_MOVE_WIN,
			BM_GETCHECK,0,0)!=0);
		CULComboBox cbMove;
		cbMove.Attach(GetDlgItem(IDC_COMBO_MOVE_MSG));
		m_arHotkey[hkoMoveWnd].m_uMsg[0]=cbMove.GetItemData(cbMove.GetCurSel());
		switch(m_arHotkey[hkoMoveWnd].m_uMsg[0])
		{
		case WM_LBUTTONDOWN:
			m_arHotkey[hkoMoveWnd].IsMsg(2,WM_LBUTTONUP);
			break;
		case WM_MBUTTONDOWN:
			m_arHotkey[hkoMoveWnd].IsMsg(2,WM_MBUTTONUP);
			break;
		case WM_RBUTTONDOWN:
			m_arHotkey[hkoMoveWnd].IsMsg(2,WM_RBUTTONUP);
			break;
		}
		cbMove.Detach();
	}
	//возвращаем значение горячих клавиш для size
	{
		m_arHotkey[hkoSizeWnd].m_fAlt=(SendDlgItemMessage(IDC_CHECK_SIZE_ALT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoSizeWnd].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_SIZE_CTRL,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoSizeWnd].m_fShift=(SendDlgItemMessage(IDC_CHECK_SIZE_SHIFT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoSizeWnd].m_fWin=(SendDlgItemMessage(IDC_CHECK_SIZE_WIN,
			BM_GETCHECK,0,0)!=0);
		CULComboBox cbSize;
		cbSize.Attach(GetDlgItem(IDC_COMBO_SIZE_MSG));
		m_arHotkey[hkoSizeWnd].m_uMsg[0]=cbSize.GetItemData(cbSize.GetCurSel());
		switch(m_arHotkey[hkoSizeWnd].m_uMsg[0])
		{
		case WM_LBUTTONDOWN:
			m_arHotkey[hkoSizeWnd].IsMsg(2,WM_LBUTTONUP);
			break;
		case WM_MBUTTONDOWN:
			m_arHotkey[hkoSizeWnd].IsMsg(2,WM_MBUTTONUP);
			break;
		case WM_RBUTTONDOWN:
			m_arHotkey[hkoSizeWnd].IsMsg(2,WM_RBUTTONUP);
			break;
		}
		cbSize.Detach();
	}
	//возвращаем значение горячих клавиш для Toggle Caption
	{
		m_arHotkey[hkoToggleCaption].m_fAlt=(SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_ALT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoToggleCaption].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_CTRL,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoToggleCaption].m_fShift=(SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_SHIFT,
			BM_GETCHECK,0,0)!=0);
		m_arHotkey[hkoToggleCaption].m_fWin=(SendDlgItemMessage(IDC_CHECK_TOGGLECAPTION_WIN,
			BM_GETCHECK,0,0)!=0);
		CULComboBox cbToggleCaption;
		cbToggleCaption.Attach(GetDlgItem(IDC_COMBO_TOGGLECAPTION_MSG));
		m_arHotkey[hkoToggleCaption].m_uMsg[0]=cbToggleCaption.GetItemData(cbToggleCaption.GetCurSel());
		cbToggleCaption.Detach();
	}
	return CULPropPage::OnApply(nReturn);
}

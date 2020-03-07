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
#include "PropSystem.h"
#include "resource.h"
#include "UpdateProgressDlg.h"

CPropSystem::CPropSystem(void):
	CULPropPage()	
{
	MessageMap.AddCommand<CPropSystem>(IDC_BUTTON_CHECKNOW,&CPropSystem::OnBtnCheckNow);

	MessageMap.AddCommand<CPropSystem>(IDC_RADIO_SIZE_PROGRAM,&CPropSystem::OnRadioSizeMethod);
	MessageMap.AddCommand<CPropSystem>(IDC_RADIO_SIZE_SYSTEM,&CPropSystem::OnRadioSizeMethod);
}

CPropSystem::~CPropSystem(void)
{
}

LRESULT CPropSystem::OnInitDialog(WPARAM w,LPARAM l)
{
	SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_SETCHECK,(WPARAM)m_fAutoRun,0);	

	switch(m_UpdateType)
	{
	case utNever:
		SendDlgItemMessage(IDC_RADIO_NEVER,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utAtStart:
		SendDlgItemMessage(IDC_RADIO_ATSTART,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utEvery24:
		SendDlgItemMessage(IDC_RADIO_EVERY24HOURS,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utAtStartEvery24:
		SendDlgItemMessage(IDC_RADIO_ATSTARTANDEVERY24HOURS,BM_SETCHECK,(WPARAM)1,0);	
		break;
	}

	switch(CHook::GetHook()->m_MoveMethod)
	{
	case msmProgram:
		SendDlgItemMessage(IDC_RADIO_MOVE_PROGRAM,BM_SETCHECK,(WPARAM)1,0);
		break;
	case msmSystem:
		SendDlgItemMessage(IDC_RADIO_MOVE_SYSTEM,BM_SETCHECK,(WPARAM)1,0);
		break;
	}

	m_comboSizeMethodBy.Attach(GetDlgItem(IDC_COMBO_SIZE_METHOD));
	m_comboSizeMethodBy.AddString(CULStrTable(IDS_SIZE_METHOD_BY_CORNER));
	m_comboSizeMethodBy.AddString(CULStrTable(IDS_SIZE_METHOD_BY_BORDER));
	m_comboSizeMethodBy.SetCurSel((int)CHook::GetHook()->m_SizeMethodBy);

	switch(CHook::GetHook()->m_SizeMethod)
	{
	case msmProgram:
		SendDlgItemMessage(IDC_RADIO_SIZE_PROGRAM,BM_SETCHECK,(WPARAM)1,0);
		m_comboSizeMethodBy.EnableWindow(FALSE);
		break;
	case msmSystem:
		SendDlgItemMessage(IDC_RADIO_SIZE_SYSTEM,BM_SETCHECK,(WPARAM)1,0);		
		break;
	}


	m_hyperlinkSendMsg.Attach(GetDlgItem(IDC_STATIC_SENDMSG),_T("mailto:unclelab@gmail.com"));


	m_hyperlinkSendMsg.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,
		ULWnds::ULControls::ULButtons::CULHyperLink::sNone,RGB(0,0,255));
	m_hyperlinkSendMsg.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,
		ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine,RGB(0,0,255));
	m_hyperlinkSendMsg.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(64,0,255));
	m_hyperlinkSendMsg.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tVisited,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(128,0,255));



	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropSystem::OnApply(BYTE nReturn)
{
	m_fAutoRun=(BOOL)SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_GETCHECK,0,0);	
	if(SendDlgItemMessage(IDC_RADIO_NEVER,BM_GETCHECK,0,0))
		m_UpdateType=utNever;
	else
		if(SendDlgItemMessage(IDC_RADIO_ATSTART,BM_GETCHECK,0,0))
			m_UpdateType=utAtStart;
		else
			if(SendDlgItemMessage(IDC_RADIO_EVERY24HOURS,BM_GETCHECK,0,0))
				m_UpdateType=utEvery24;
			else
				if(SendDlgItemMessage(IDC_RADIO_ATSTARTANDEVERY24HOURS,BM_GETCHECK,0,0))
					m_UpdateType=utAtStartEvery24;

	
	if(SendDlgItemMessage(IDC_RADIO_MOVE_PROGRAM,BM_GETCHECK,0,0))
		CHook::GetHook()->m_MoveMethod=msmProgram;
	else
		if(SendDlgItemMessage(IDC_RADIO_MOVE_SYSTEM,BM_GETCHECK,0,0))
			CHook::GetHook()->m_MoveMethod=msmSystem;

	if(SendDlgItemMessage(IDC_RADIO_SIZE_PROGRAM,BM_GETCHECK,0,0))
		CHook::GetHook()->m_SizeMethod=msmProgram;
	else
		if(SendDlgItemMessage(IDC_RADIO_SIZE_SYSTEM,BM_GETCHECK,0,0))
			CHook::GetHook()->m_SizeMethod=msmSystem;

	CHook::GetHook()->m_SizeMethodBy=(enSizeMethodBy)m_comboSizeMethodBy.GetCurSel();

	return CULPropPage::OnApply(nReturn);
}

void CPropSystem::OnBtnCheckNow(WORD,HWND)
{
	CUpdateProgressDlg dlg;
	dlg.CreateModal(IDD_DIALOG_UPDATEPROGRESS,*this);
}

void CPropSystem::OnRadioSizeMethod(WORD,HWND)
{
	if(SendDlgItemMessage(IDC_RADIO_SIZE_PROGRAM,BM_GETCHECK,0,0))
		m_comboSizeMethodBy.EnableWindow(FALSE);
	else
		m_comboSizeMethodBy.EnableWindow(TRUE);
}

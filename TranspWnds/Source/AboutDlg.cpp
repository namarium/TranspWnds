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
#include "AboutDlg.h"
#include "resource.h"

CAboutDlg::CAboutDlg(void):
	CULDlg()
{
}

CAboutDlg::~CAboutDlg(void)
{
}

LRESULT CAboutDlg::OnInitDialog(WPARAM w,LPARAM l)
{
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,FALSE);

	m_hlHomePage.CreateHyperLink(*this,115,47,_T("http://transpwnds.googlecode.com"),
		_T("http://transpwnds.googlecode.com"),0);
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,
		ULWnds::ULControls::ULButtons::CULHyperLink::sNone,RGB(0,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,
		ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine,RGB(0,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(64,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tVisited,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(128,0,255));
	return CULDlg::OnInitDialog(w,l);
}

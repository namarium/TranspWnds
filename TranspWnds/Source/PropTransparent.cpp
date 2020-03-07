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
#include "PropTransparent.h"
#include "resource.h"

CPropTransparent::CPropTransparent(void):
	CULPropPage(),
	m_nMinTransparentLevel(0),
	m_nTransparentLevelStep(0)
{
	MessageMap.AddMessage<CPropTransparent>(WM_HSCROLL,&CPropTransparent::OnHScroll);
}

CPropTransparent::~CPropTransparent(void)
{
}

LRESULT CPropTransparent::OnInitDialog(WPARAM w,LPARAM l)
{
	m_sliderMinTranspLevel.Attach(GetDlgItem(IDC_SLIDER_MIN_TRANSP_LEVEL));
	m_sliderMinTranspLevel.ModifyStyle(TBS_NOTICKS,TBS_FIXEDLENGTH|TBS_AUTOTICKS);

	m_sliderTranspLevelStep.Attach(GetDlgItem(IDC_SLIDER_TRANSP_LEVEL_STEP));
	m_sliderTranspLevelStep.ModifyStyle(TBS_NOTICKS,TBS_FIXEDLENGTH|TBS_AUTOTICKS);

	m_sliderMinTranspLevel.SetLineSize(5);
	m_sliderMinTranspLevel.SetPageSize(10);
	m_sliderMinTranspLevel.SetThumbLenght(15);

	m_sliderTranspLevelStep.SetThumbLenght(15);


	m_sliderMinTranspLevel.SetRange(10,90);
	m_sliderMinTranspLevel.SetPos(m_nMinTransparentLevel);

	m_sliderMinTranspLevel.SetTicFreq(10);

	m_sliderTranspLevelStep.SetRange(1,m_nMinTransparentLevel/2);
	m_sliderTranspLevelStep.SetTicFreq(((m_nMinTransparentLevel/2)>20)?2:1);
	m_sliderTranspLevelStep.SetPos(m_nTransparentLevelStep);

	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropTransparent::OnHScroll(WPARAM wParam,LPARAM lParam)
{
	if(GetDlgItem(IDC_SLIDER_MIN_TRANSP_LEVEL)==(HWND)lParam)
	{
		if(LOWORD(wParam)==TB_ENDTRACK)
		{
			m_nMinTransparentLevel=m_sliderMinTranspLevel.GetPos();
			m_nTransparentLevelStep=m_sliderTranspLevelStep.GetPos();
			m_sliderTranspLevelStep.SetRange(1,m_nMinTransparentLevel/2);
			m_sliderTranspLevelStep.SetTicFreq(((m_nMinTransparentLevel/2)>20)?2:1);
			m_sliderTranspLevelStep.SetPos(m_nTransparentLevelStep);
		}
	}
	if(GetDlgItem(IDC_SLIDER_TRANSP_LEVEL_STEP)==(HWND)lParam)
		if(LOWORD(wParam)==TB_ENDTRACK)
			m_nTransparentLevelStep=m_sliderTranspLevelStep.GetPos();
	return 0;
}

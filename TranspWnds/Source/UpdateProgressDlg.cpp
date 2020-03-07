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
#include "UpdateProgressDlg.h"
#include "Updater.h"
#include "resource.h"
#include "WorkWnd.h"
#include <sstream>

CUpdateProgressDlg::CUpdateProgressDlg(void):
	CULDlg()
{
	MessageMap.AddMessage<CUpdateProgressDlg>(UDM_NOTIFY,&CUpdateProgressDlg::OnUpdateNotify);
	MessageMap.AddMessage<CUpdateProgressDlg>(WM_SHOWWINDOW,&CUpdateProgressDlg::OnShowWindow);
}

CUpdateProgressDlg::~CUpdateProgressDlg(void)
{
}
LRESULT CUpdateProgressDlg::OnInitDialog(WPARAM w,LPARAM l)
{
	m_listUpdateLog.Attach(GetDlgItem(IDC_LIST_UPDATELOG));
/*
	m_hyperlinkDownloadPage.Attach(GetDlgItem(IDC_STATIC_DOWNLOADPAGE),
		CULStrTable(IDS_DOWNLOADPAGE));
	*/	
	{
		::ShowWindow(GetDlgItem(IDC_STATIC_DOWNLOADPAGE),SW_HIDE);
	}

	m_hyperlinkDownloadPage.CreateHyperLink(*this,40,140,CULStrTable(IDS_DOWNLOAD_NOTIFY),
		CULStrTable(IDS_DOWNLOADPAGE),0);

	m_hyperlinkDownloadPage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,
		ULWnds::ULControls::ULButtons::CULHyperLink::sNone,RGB(0,0,255));
	m_hyperlinkDownloadPage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,
		ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine,RGB(0,0,255));
	m_hyperlinkDownloadPage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(64,0,255));
	m_hyperlinkDownloadPage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tVisited,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(128,0,255));


	return CULDlg::OnInitDialog(w,l);
}

LRESULT CUpdateProgressDlg::OnShowWindow(WPARAM wParam,LPARAM)
{
	if(wParam)
		CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::UDM_CHECKFORUPDATE,
		(WPARAM)m_hWnd,(LPARAM)UDM_NOTIFY);

	return 0;
}

LRESULT CUpdateProgressDlg::OnUpdateNotify(WPARAM wParam,LPARAM lParam)
{
	std::basic_stringstream<TCHAR> ss;
	switch(wParam)
	{
	case CUpdater::unCurrentVersion:
		{
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_CURRENTVERSION)<<" ";
			int* pVer=(int*)lParam;
			for(int i=0;i<CUpdater::constSectionSize;++i)
			{
				ss<<pVer[i];
				if(i!=(CUpdater::constSectionSize-1))
					ss<<".";
			}
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unConnect:
		if(lParam==-1)
		{
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_CONNECT)<<"......";
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		else
		{
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_CONNECT)<<"......";
			if(lParam==TRUE)
				ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
			if(lParam==FALSE)
				ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
			if(m_listUpdateLog.GetCount()>CUpdater::unConnect)
				m_listUpdateLog.DeleteString(m_listUpdateLog.GetCount()-1);
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unRecieveData:
		if(lParam==0)
			m_listUpdateLog.AddString(CULStrTable(CULStrTable(IDS_CHECKFORUPDATE_RECEIVEDATA)));
		else
		{
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_RECEIVEDATA)<<"......"<<(int)lParam;
			ss<<" "<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_DATANAME);
			if(m_listUpdateLog.GetCount()>CUpdater::unRecieveData)
				m_listUpdateLog.DeleteString(m_listUpdateLog.GetCount()-1);
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unParseData:
		ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_PARSEDATA)<<"......";
		if(lParam==TRUE)
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
		if(lParam==FALSE)
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	case CUpdater::unLastVersion:
		{
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_LASTVERSION)<<"...... ";
			int* pVer=(int*)lParam;
			for(int i=0;i<CUpdater::constSectionSize;++i)
			{
				ss<<pVer[i];
				if(i!=(CUpdater::constSectionSize-1))
					ss<<".";
			}
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unNewVersionAvail:
		ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_NEWVERSIONAVAIL)<<"......";
		if(lParam==TRUE)
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
		if(lParam==FALSE)
			ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	case CUpdater::unError:
		ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_ERROR)<<"......";
		ss<<(LPCTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	}
	return 0;
}

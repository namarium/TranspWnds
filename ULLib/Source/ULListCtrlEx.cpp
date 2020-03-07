///\file ULListCtrlEx.cpp
///\brief фаил реализации класса расширенного лист контрола
#define _WIN32_WINNT 0x0501
#include "..\Include\ULListCtrlEx.h"
#include "..\Include\ULStates.h"

#pragma comment(lib,"UxTheme.lib")

namespace ULWnds
{
	namespace ULControls
	{
		namespace ULListCtrlEx
		{
			CULListCtrlEx::CULListCtrlEx():CULListCtrl(),
				m_nItemSel(-1),m_nSubItemSel(-1),m_nItemCount(0)
			{
				MessageMap.AddReflectNotify<CULListCtrlEx>(NM_CUSTOMDRAW,&CULListCtrlEx::OnCustomDraw);
				MessageMap.AddReflectNotify<CULListCtrlEx>(NM_CLICK,&CULListCtrlEx::OnNMClick);
				
				MessageMap.AddMessage<CULListCtrlEx>(WM_ERASEBKGND,&CULListCtrlEx::OnEraseBkGnd);
				MessageMap.AddMessage<CULListCtrlEx>(WM_VSCROLL,&CULListCtrlEx::OnVScroll);
				MessageMap.AddMessage<CULListCtrlEx>(WM_HSCROLL,&CULListCtrlEx::OnHScroll);
				MessageMap.AddMessage<CULListCtrlEx>(WM_MOUSEWHEEL,&CULListCtrlEx::OnMouseWheel);
				MessageMap.AddMessage<CULListCtrlEx>(WM_LBUTTONDOWN,&CULListCtrlEx::OnLButtonDown);
				MessageMap.AddMessage<CULListCtrlEx>(WM_SIZE,&CULListCtrlEx::OnWindowPosChanged);
				MessageMap.AddMessage<CULListCtrlEx>(WM_DESTROY,&CULListCtrlEx::OnDestroy);
				MessageMap.AddMessage<CULListCtrlEx>(WM_THEMECHANGED,&CULListCtrlEx::OnThemeChanged);

				MessageMap.AddReflectMessage<CULListCtrlEx>(WM_MEASUREITEM ,&CULListCtrlEx::OnMeasureItem);
				
				MessageMap.AddMessage<CULListCtrlEx>(LCEM_RETURNITEMSEL,&CULListCtrlEx::OnReturnItemSel);
				MessageMap.AddMessage<CULListCtrlEx>(WM_COMMAND,&CULListCtrlEx::OnCommand);
				
			}

			CULListCtrlEx::~CULListCtrlEx()
			{
				OnDestroy(0,0);
			}

			BOOL CULListCtrlEx::Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
			{
				BOOL fRet=CULListCtrl::Create(hParentWnd,uID,x,y,cx,cy,dwStyle,dwStyleEx);
				if(!fRet)
					return FALSE;
				ModifyStyle(0,LVS_OWNERDRAWFIXED);
				return TRUE;
			}
			BOOL CULListCtrlEx::Attach(HWND hWnd)
			{
				if(!CULListCtrl::Attach(hWnd))
					return FALSE;
				ModifyStyle(0,LVS_OWNERDRAWFIXED);
				RECT rc;
				GetClientRect(&rc);
				SetWindowPos(0,0,0,rc.right,rc.bottom,SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED);
				return TRUE;
			}

			int CULListCtrlEx::InsertItem(int nItem,LPCTSTR lpszItem)
			{
				nItem=CULListCtrl::InsertItem(nItem,_T(""));
				ULOther::CULArr<IItemObject*>* pArr=new ULOther::CULArr<IItemObject*>;
				CItemText* pItemText=new CItemText;
				pArr->Resize(1);
				(*pArr)[0]=pItemText;		
				pItemText->SetText(lpszItem);
				SetItemData(nItem,(DWORD_PTR)pArr);
				++m_nItemCount;
				return nItem;
			}
			BOOL CULListCtrlEx::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText)
			{
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(nItem);
				if(pArr==NULL)
					return FALSE;
				CItemText* pItemText=NULL;
				if(pArr->GetSize()>(size_t)nSubItem)
					pItemText=(CItemText*)*pArr[nSubItem];
				else
				{
					IItemObject** ppItemObject=pArr->Resize(nSubItem+1);
					pItemText=new CItemText;
					ppItemObject[nSubItem]=pItemText;
				}
				pItemText->SetText(lpszText);			
				return TRUE;
			}
			BOOL CULListCtrlEx::SetItemObject(int nItem,int nSubItem,IItemObject* pItemObject)
			{
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(nItem);
				if(pArr==NULL)
					return FALSE;
				CItemText* pItemText=NULL;
				if(pArr->GetSize()>(size_t)nSubItem)
				{
					pItemText=(CItemText*)(*pArr)[nSubItem];
					if(pItemText)
						delete pItemText;
					(*pArr)[nSubItem]=pItemObject;
				}
				else
				{
					IItemObject** ppItemObject=pArr->Resize(nSubItem+1);			
					ppItemObject[nSubItem]=pItemObject;
				}
				return TRUE;
			}
			IItemObject* CULListCtrlEx::GetItemObject(int nItem,int nSubItem)
			{
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(nItem);
				if(pArr==NULL)
					return FALSE;
				if(pArr->GetSize()>(size_t)nSubItem)
					return (*pArr)[nSubItem];
				else 
					return NULL;
			}

			BOOL CULListCtrlEx::DeleteAllItems()
			{
				for(int i=0;i<m_nItemCount;++i)
				{
					ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(i);
					for(size_t ii=0;ii<pArr->GetSize();++ii)
					{
						IItemObject* pItemObject=(*pArr)[ii];
						if(pItemObject)
							delete pItemObject;
					}
				}
				m_nItemCount=0;
				m_nItemSel=0;
				m_nSubItemSel=0;
				return CULListCtrl::DeleteAllItems();
			}
			BOOL CULListCtrlEx::DeleteItem(int nItem)
			{
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(nItem);
				BOOL fRet=CULListCtrl::DeleteItem(nItem);
				for(size_t ii=0;ii<pArr->GetSize();++ii)
				{
					IItemObject* pItemObject=(*pArr)[ii];
					if(pItemObject)
						delete pItemObject;
				}
				m_nItemCount--;
				return fRet;
			}

			BOOL CULListCtrlEx::SetItemData(int nItem,DWORD_PTR dwData)
			{
				return CULListCtrl::SetItemData(nItem,dwData);
			}

			LRESULT CULListCtrlEx::OnEraseBkGnd(WPARAM,LPARAM)
			{
				return TRUE;
			}
			LRESULT CULListCtrlEx::OnVScroll(WPARAM,LPARAM)
			{
				if(::GetParent(GetFocus())==*this)
				{
					SetFocus();
					PostMessage(LCEM_RETURNITEMSEL);
				}
				return FALSE;
			}
			LRESULT CULListCtrlEx::OnHScroll(WPARAM,LPARAM)
			{
				if(::GetParent(GetFocus())==*this)
				{
					SetFocus();
					PostMessage(LCEM_RETURNITEMSEL);
				}
				return FALSE;
			}
			LRESULT CULListCtrlEx::OnMouseWheel(WPARAM,LPARAM)
			{
				if(::GetParent(GetFocus())==*this)
				{
					SetFocus();
					PostMessage(LCEM_RETURNITEMSEL);
				}
				return FALSE;
			}
			
			LRESULT CULListCtrlEx::OnNMClick(LPARAM lParam)
			{
				LPNMITEMACTIVATE lpnmitem=(LPNMITEMACTIVATE)lParam;
				
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(lpnmitem->iItem);
				if(pArr==NULL)
					return FALSE;
				if(pArr->GetSize()<=(size_t)lpnmitem->iSubItem)
					return FALSE;
				IItemObject* pItemObject=(IItemObject*)(*pArr)[lpnmitem->iSubItem];
				if(pItemObject)
				{
					RECT rc={0};
					if(lpnmitem->iSubItem==0)
						GetItemRect(lpnmitem->iItem,&rc,LVIR_LABEL);
					else
					{
						GetSubItemRect(lpnmitem->iItem,lpnmitem->iSubItem,LVIR_LABEL,&rc);
						rc.left+=2;
					}
					m_nItemSel=lpnmitem->iItem;
					m_nSubItemSel=lpnmitem->iSubItem;
					pItemObject->OnClick(rc);
				}
				return FALSE;
			}

			LRESULT CULListCtrlEx::OnLButtonDown(WPARAM,LPARAM lParam)
			{				
				LVHITTESTINFO lvhi={0};
				lvhi.pt.x=LOWORD(lParam);
				lvhi.pt.y=HIWORD(lParam);
				lvhi.flags=LVHT_BELOW;
				SubItemHitTest(&lvhi);
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(lvhi.iItem);
				if(pArr==NULL)
					return FALSE;
				if(pArr->GetSize()<=(size_t)lvhi.iSubItem)
					return FALSE;
				IItemObject* pItemObject=(IItemObject*)(*pArr)[lvhi.iSubItem];
				if(pItemObject)
				{
					RECT rc={0};
					if(lvhi.iSubItem==0)
						GetItemRect(lvhi.iItem,&rc,LVIR_LABEL);
					else
					{
						GetSubItemRect(lvhi.iItem,lvhi.iSubItem,LVIR_LABEL,&rc);
						rc.left+=2;
					}
					m_nItemSel=lvhi.iItem;
					m_nSubItemSel=lvhi.iSubItem;
					pItemObject->OnLButtonDown(rc);
				}
				return FALSE;
			}

			LRESULT CULListCtrlEx::OnMeasureItem(WPARAM,LPARAM lParam)
			{
				LPMEASUREITEMSTRUCT lpMeasureItem = (LPMEASUREITEMSTRUCT) lParam;
				lpMeasureItem->itemHeight=19;//GetSystemMetrics()
				return FALSE;
			}

			LRESULT CULListCtrlEx::OnWindowPosChanged(WPARAM,LPARAM)
			{
				InvalidateRect();
				return FALSE;
			}

			LRESULT CULListCtrlEx::OnCustomDraw(LPARAM lParam)
			{
				LPNMLVCUSTOMDRAW lpNMCustomDraw=(LPNMLVCUSTOMDRAW)lParam;
				ULGDI::ULDC::CULDC dc(lpNMCustomDraw->nmcd.hdc);

				LRESULT lRes=0;
				switch(lpNMCustomDraw->nmcd.dwDrawStage)
				 {
					case CDDS_PREPAINT:
						{
							lRes=CDRF_NOTIFYSUBITEMDRAW;
							
							RECT rc;
							GetClientRect(&rc);
							ULGDI::ULGObjs::CULBrush br;
							br.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
							dc.FillRect(&rc,br);			
							
							break;
						}
					case CDDS_ITEMPREPAINT:
					case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
					default:
						{
							lRes=CDRF_SKIPDEFAULT;
							
							int iRow=(int)lpNMCustomDraw->nmcd.dwItemSpec;
							int nSubItemCount=GetSubItemCount();
							for(int iCol=0;iCol<nSubItemCount;++iCol)
							{
								RECT rc={0};
								if(iCol==0)
									GetItemRect(iRow,&rc,LVIR_LABEL);
								else
								{
									GetSubItemRect(iRow,iCol,LVIR_LABEL,&rc);
									rc.left+=2;
								}
								ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)lpNMCustomDraw->nmcd.lItemlParam;
								if(iCol<(int)pArr->GetSize())
								{
									IItemObject* pItemObject=(IItemObject*)(*pArr)[iCol];
									if(pItemObject)
										pItemObject->Draw(&dc,rc);
								}
							}
							break;
					}
		//			default:
			//		 lRes=CDRF_DODEFAULT;
				 }

				dc.Detach();
				return lRes;
			}

			LRESULT CULListCtrlEx::OnReturnItemSel(WPARAM,LPARAM)
			{
				ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(m_nItemSel);
				if(pArr==NULL)
					return FALSE;
				if(pArr->GetSize()<=(size_t)m_nSubItemSel)
					return FALSE;
				IItemObject* pItemObject=(IItemObject*)(*pArr)[m_nSubItemSel];
				if(pItemObject)
				{
					RECT rc={0};
					if(m_nSubItemSel==0)
						GetItemRect(m_nItemSel,&rc,LVIR_LABEL);
					else
					{
						GetSubItemRect(m_nItemSel,m_nSubItemSel,LVIR_LABEL,&rc);
						rc.left+=2;
					}
					pItemObject->OnClick(rc);
				}
				return TRUE;
			}
			LRESULT CULListCtrlEx::OnCommand(WPARAM wParam,LPARAM lParam)
			{
				::SendMessage(GetParent(),WM_COMMAND,wParam,lParam);
				return TRUE;
			}
			LRESULT CULListCtrlEx::OnDestroy(WPARAM,LPARAM)
			{
				DeleteAllItems();
				return FALSE;
			}

			LRESULT CULListCtrlEx::OnThemeChanged(WPARAM,LPARAM)
			{
				for(int i=0;i<GetItemCount();++i)
				{
					ULOther::CULArr<IItemObject*>* pArr=(ULOther::CULArr<IItemObject*>*)GetItemData(i);
					if(!pArr)
						continue;
					for(size_t ii=0;ii<pArr->GetSize();++ii)
						(*pArr)[ii]->OnThemeChanged();
				}
				return 0;
			}
			//================================================================
			CItemText::CItemText():
				m_hIcon(NULL){}
			void CItemText::SetText(LPCTSTR pszText)
			{
				m_strText=pszText;
			}
			LPCTSTR CItemText::GetText()
			{
				return m_strText;
			}
			void CItemText::SetIcon(HICON hIcon)
			{
				m_hIcon=hIcon;
			}
			void CItemText::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				if(m_hIcon)
				{
					RECT rcIcon=rc;
//					rcIcon.left+=(rcIcon.right-rcIcon.left+::GetSystemMetrics(SM_CXSMICON))/2;
//					rcIcon.top+=(rcIcon.bottom-rcIcon.top+::GetSystemMetrics(SM_CYSMICON))/2;
					rcIcon.top+=2;
					pDC->DrawIconEx(rcIcon.left,rcIcon.top,m_hIcon,
						::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),
						NULL,NULL,DI_NORMAL);
				}
				
				rc.top+=3;
				rc.left+=3;
				if(m_hIcon)
					rc.left+=::GetSystemMetrics(SM_CXSMICON);
				pDC->DrawText(m_strText,m_strText.GetLen(),&rc,DT_END_ELLIPSIS|DT_LEFT);
			}
			void CItemText::Clean()
			{
				m_strText=_T("");
			}
			//================================================================
			CItemEdit::CEdit::CEdit():CULEdit()
			{
				MessageMap.AddMessage<CEdit>(WM_KILLFOCUS,&CEdit::OnKillFocus);
				MessageMap.AddMessage<CEdit>(WM_ACTIVATE,&CEdit::OnActivate);
			}
			LRESULT CItemEdit::CEdit::OnKillFocus(WPARAM,LPARAM)
			{
				ShowWindow(SW_HIDE);
				return FALSE;
			}
			LRESULT CItemEdit::CEdit::OnActivate(WPARAM wParam,LPARAM)
			{
				if(wParam==WA_INACTIVE)
					ShowWindow(SW_HIDE);
				return FALSE;
			}
			CItemEdit::~CItemEdit()
			{
				m_Edit.DestroyWindow();
			}
			BOOL CItemEdit::Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr)
			{
				if(!m_Edit.Create(hParentWnd,pszStr,uID,0,0,0,0,WS_CHILD|ES_LEFT|WS_TABSTOP|ES_AUTOHSCROLL,0))
					return FALSE;
				m_Edit.SetFont((HFONT)::SendMessage(hParentWnd,WM_GETFONT,0,0),FALSE);
				return TRUE;
			}
			void CItemEdit::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				if(m_Edit.IsWindowVisible())
				{
					if(rc.top>10)
						m_Edit.ShowWindow(SW_SHOW);
					m_Edit.MoveWindow(rc.left,rc.top+3,rc.right-rc.left,rc.bottom-rc.top-5,FALSE);
				}
				else
				{
					ULOther::CULStr m_strText(m_Edit.GetWindowTextLength()+1);
					m_Edit.GetWindowText(m_strText,m_Edit.GetWindowTextLength()+1);			
					rc.top+=3;
					rc.left+=3;
					pDC->DrawText(m_strText,m_strText.GetLen(),&rc, DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_LEFT);
				}
			}
			void CItemEdit::Clean()
			{
				m_Edit.SetWindowText(_T(""));
			}
			void CItemEdit::OnClick(RECT& rc)
			{
				if(rc.top>10)
					m_Edit.ShowWindow(SW_SHOW);
				m_Edit.MoveWindow(rc.left,rc.top+3,rc.right-rc.left,rc.bottom-rc.top-5,TRUE);
				m_Edit.SetFocus();
				m_Edit.SetActiveWindow();
				m_Edit.SetSel(-2,-2);
			}
			//=====================================================
				CItemComboBox::CComboBox::CComboBox():CULComboBox()
				{
					MessageMap.AddMessage<CComboBox>(WM_KILLFOCUS,&CComboBox::OnKillFocus);
				}
				LRESULT CItemComboBox::CComboBox::OnKillFocus(WPARAM wParam,LPARAM)
				{
					if(::GetParent((HWND)wParam)!=*this)
						ShowWindow(SW_HIDE);
					return FALSE;
				}

				CItemComboBox::CEdit::CEdit():CULEdit()
				{
					MessageMap.AddMessage<CEdit>(WM_KILLFOCUS,&CEdit::OnKillFocus);
				}
				LRESULT CItemComboBox::CEdit::OnKillFocus(WPARAM wParam,LPARAM)
				{
					if(::GetParent((HWND)wParam)!=*this)
						::ShowWindow(GetParent(),SW_HIDE);				
					return FALSE;
				}
			CItemComboBox::~CItemComboBox()
			{
				m_Edit.Detach();
				m_ComboBox.DestroyWindow();
				if(m_hTheme)
					CloseThemeData(m_hTheme);
			}
			BOOL CItemComboBox::Create(HWND hParentWnd,UINT uID,BOOL fDropDownList)
			{
				ASSERT(m_ComboBox.Create(hParentWnd,uID,0,0,0,200,WS_CHILD|WS_TABSTOP|CBS_HASSTRINGS|
					((fDropDownList)?CBS_DROPDOWNLIST:CBS_DROPDOWN)|WS_VSCROLL,0));			
				m_ComboBox.SetFont((HFONT)::SendMessage(hParentWnd,WM_GETFONT,0,0),FALSE);
				m_ComboBox.ModifyStyleEx((DWORD)-1,0);

				if(!fDropDownList)
				{
					COMBOBOXINFO cbi={0};
					cbi.cbSize=sizeof(COMBOBOXINFO);					
					m_ComboBox.GetComboBoxInfo(&cbi);
					ASSERT(m_Edit.Attach(cbi.hwndItem));
				}
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_ComboBox,L"SCROLLBAR");
				else
					m_hTheme=NULL;
				return TRUE;
			}
			void CItemComboBox::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				if(m_ComboBox.IsWindowVisible())
				{
					if(rc.top>10)
						m_ComboBox.ShowWindow(SW_SHOW);
//					m_ComboBox.MoveWindow(rc.left,rc.top-1,rc.right-rc.left,rc.bottom-rc.top+1,FALSE);
				}
				else
				{
					ULOther::CULStr m_strText(m_ComboBox.GetWindowTextLength()+1);
					m_ComboBox.GetWindowText(m_strText,m_ComboBox.GetWindowTextLength()+1);			
					rc.top+=3;
					rc.left+=3;
					pDC->DrawText(m_strText,m_strText.GetLen(),&rc, DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_LEFT);
					rc.top-=3;
					RECT rectBtn={rc.right-(rc.bottom-rc.top)+1,rc.top+1,rc.right-2,rc.bottom-2};
					if(m_hTheme&&IsThemeActive())
						DrawThemeBackground(m_hTheme ,*pDC, SBP_ARROWBTN, ((1)?ABS_DOWNNORMAL:PBS_PRESSED), &rectBtn, NULL);
					else
						DrawFrameControl(*pDC,&rectBtn, DFC_SCROLL,DFCS_SCROLLCOMBOBOX);
				}
			}
			void CItemComboBox::Clean()
			{
				m_ComboBox.ResetContent();
			}

			void CItemComboBox::OnClick(RECT& rc)
			{
				if(!m_ComboBox.IsWindowEnabled())
					return;
				if(rc.top>10)
					m_ComboBox.ShowWindow(SW_SHOW);
				m_ComboBox.MoveWindow(rc.left,rc.top-1,rc.right-rc.left,rc.bottom-rc.top-2,TRUE);
				m_ComboBox.SetFocus();
				m_ComboBox.SetActiveWindow();
				LONG lStyle=m_ComboBox.GetWindowLong(GWL_STYLE);
				if((lStyle&CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST)
					m_ComboBox.ShowDropDown(TRUE);
				else
					if((lStyle&CBS_DROPDOWN)==CBS_DROPDOWN)
					{				
						RECT rectBtn={rc.right-(rc.bottom-rc.top)+1,rc.top+1,rc.right-2,rc.bottom-2};
						POINT pt={0};
						GetCursorPos(&pt);
						ScreenToClient(m_ComboBox.GetParent(),&pt);
						if(PtInRect(&rectBtn,pt))
							m_ComboBox.ShowDropDown(TRUE);
					}
					
			}
			void CItemComboBox::OnLButtonDown(RECT& rc)
			{
				if(!m_ComboBox.IsWindowEnabled())
					return;
				if(rc.top>10)
					m_ComboBox.ShowWindow(SW_SHOW);
				m_ComboBox.MoveWindow(rc.left,rc.top-1,rc.right-rc.left,rc.bottom-rc.top-2,TRUE);
				m_ComboBox.SetFocus();
				m_ComboBox.SetActiveWindow();
			}

			bool CItemComboBox::GetRect(RECT* prc)
			{
				return (m_ComboBox.GetClientRect(prc)==TRUE);
			}
			void CItemComboBox::OnThemeChanged()
			{				
				if(m_hTheme)
					CloseThemeData(m_hTheme);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_ComboBox,L"SCROLLBAR");
				else
					m_hTheme=NULL;
			}
		//================================================
				CItemDateTimePicker::CDateTimePicker::CDateTimePicker():CULDateTimePicker()
				{
					MessageMap.AddMessage<CDateTimePicker>(WM_KILLFOCUS,&CDateTimePicker::OnKillFocus);
				}
				LRESULT CItemDateTimePicker::CDateTimePicker::OnKillFocus(WPARAM,LPARAM)
				{
					ShowWindow(SW_HIDE);
					return FALSE;
				}
			CItemDateTimePicker::~CItemDateTimePicker()
			{
				m_DateTimePicker.DestroyWindow();
				if(m_hTheme)
					CloseThemeData(m_hTheme);
			}
			BOOL CItemDateTimePicker::Create(HWND hParentWnd,WORD uID)
			{
				ASSERT(m_DateTimePicker.Create(hParentWnd,uID,0,0,0,0,WS_CHILD|DTS_RIGHTALIGN|DTS_APPCANPARSE|WS_TABSTOP ));
				m_DateTimePicker.ModifyStyleEx((DWORD)-1,0);
				m_DateTimePicker.ModifyStyleEx(0,WS_EX_STATICEDGE);
				m_DateTimePicker.ModifyStyle(WS_BORDER|DTS_SHOWNONE,0);
				m_DateTimePicker.SetFont((HFONT)::SendMessage(hParentWnd,WM_GETFONT,0,0),FALSE);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_DateTimePicker,L"SCROLLBAR");
				else
					m_hTheme=NULL;
				return TRUE;
			}

			void CItemDateTimePicker::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				if(m_DateTimePicker.IsWindowVisible())
				{
					if(rc.top>10)
						m_DateTimePicker.ShowWindow(SW_SHOW);
					m_DateTimePicker.MoveWindow(rc.left,rc.top,rc.right-rc.left-1,rc.bottom-rc.top-1,FALSE);
				}
				else
				{
					ULOther::CULStr m_strText(m_DateTimePicker.GetWindowTextLength()+1);
					m_DateTimePicker.GetWindowText(m_strText,m_DateTimePicker.GetWindowTextLength()+1);			
					rc.top+=3;
					rc.left+=3;
					pDC->DrawText(m_strText,m_strText.GetLen(),&rc, DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_LEFT);
					rc.top-=3;
					RECT rectBtn={rc.right-(rc.bottom-rc.top)+1,rc.top+1,rc.right-2,rc.bottom-2};
					if(m_hTheme&&IsThemeActive())
						DrawThemeBackground(m_hTheme ,*pDC, SBP_ARROWBTN, ((1)?ABS_DOWNNORMAL:PBS_PRESSED), &rectBtn, NULL);
					else
						DrawFrameControl(*pDC,&rectBtn, DFC_SCROLL,DFCS_SCROLLCOMBOBOX);
				}
			}

			void CItemDateTimePicker::OnLButtonDown(RECT& /*rc*/)
			{
			}

			void CItemDateTimePicker::OnClick(RECT& rc)
			{
				if(m_DateTimePicker.IsWindowVisible())
					return;
				if(rc.top>10)
					m_DateTimePicker.ShowWindow(SW_SHOW);
				m_DateTimePicker.MoveWindow(rc.left,rc.top,rc.right-rc.left-1,rc.bottom-rc.top-1,TRUE);
				m_DateTimePicker.SetFocus();
				m_DateTimePicker.SetActiveWindow();
			}

			void CItemDateTimePicker::Clean()
			{
			}
			bool CItemDateTimePicker::GetRect(RECT* prc)
			{
				return (m_DateTimePicker.GetClientRect(prc)==TRUE);;
			}
			void CItemDateTimePicker::OnThemeChanged()
			{				
				if(m_hTheme)
					CloseThemeData(m_hTheme);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_DateTimePicker,L"SCROLLBAR");
				else
					m_hTheme=NULL;
			}

			//================================================================
					CItemButton::CButton::CButton():ULButtons::CULButton()
					{
						MessageMap.AddMessage<CButton>(WM_KILLFOCUS,&CButton::OnKillFocus);
					}
					LRESULT CItemButton::CButton::OnKillFocus(WPARAM,LPARAM)
					{
						ShowWindow(SW_HIDE);
						return FALSE;
					}
			CItemButton::~CItemButton()
			{
				m_Button.DestroyWindow();
				if(m_hTheme)
					CloseThemeData(m_hTheme);
			}
			BOOL CItemButton::Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr)
			{
				if(!m_Button.Create(hParentWnd,pszStr,0,0,0,0,uID))
					return FALSE;
				m_Button.ShowWindow(SW_HIDE);
				m_Button.SetFont((HFONT)::SendMessage(hParentWnd,WM_GETFONT,0,0),FALSE);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_Button,L"BUTTON");
				else
					m_hTheme=NULL;
				return TRUE;
			}
			void CItemButton::Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)
			{
				if(m_Button.IsWindowVisible()||!m_Button.IsWindowEnabled())
				{
					if(rc.top>10)
						m_Button.ShowWindow(SW_SHOW);
					m_Button.MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top-1,TRUE);
				}
				else
				{
					RECT rectBtn={rc.left,rc.top,rc.right,rc.bottom-1};

					if(m_hTheme&&IsThemeActive())
						DrawThemeBackground(m_hTheme ,*pDC, BP_PUSHBUTTON, PBS_DEFAULTED, &rectBtn, NULL);
					else
						DrawFrameControl(*pDC,&rectBtn,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_INACTIVE);


					ULOther::CULStr m_strText(m_Button.GetWindowTextLength()+1);
					m_Button.GetWindowText(m_strText,m_Button.GetWindowTextLength()+1);			
					rc.top+=2;
					rc.left+=2;
					rc.bottom-=1;
#ifdef UNICODE
					if(m_hTheme&&IsThemeActive())
						DrawThemeText(m_hTheme, *pDC, BP_PUSHBUTTON, PBS_DEFAULTED, m_strText, -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, &rectBtn);
					else
						pDC->DrawText(m_strText,m_strText.GetLen(),&rc, DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_CENTER);
#else
#pragma message(__FUNCTION__"переписать для ANSII")
#endif
				}
			}
			void CItemButton::Clean()
			{
				m_Button.SetWindowText(_T(""));
			}
			void CItemButton::OnClick(RECT& rc)
			{
				if(rc.top>10)
					m_Button.ShowWindow(SW_SHOW);
				m_Button.MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top-1,TRUE);
				m_Button.SetFocus();
				m_Button.SetActiveWindow();
			}
			void CItemButton::OnLButtonDown(RECT& rc)
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
			void CItemButton::OnThemeChanged()
			{				
				if(m_hTheme)
					CloseThemeData(m_hTheme);
				if(IsThemeActive())
					m_hTheme = OpenThemeData(m_Button,L"BUTTON");
				else
					m_hTheme=NULL;
			}
			//=====================================================

		}
	}
}

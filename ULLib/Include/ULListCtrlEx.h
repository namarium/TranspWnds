///\file ULListCtrlEx.h 
///\brief фаил объявления класса расширенного лист контрола(BETA)
#ifndef _ULLISTCTRL_EX_H__
#define _ULLISTCTRL_EX_H__
#include "ULControls.h"
#include "commctrl.h"
#include "ULGDI.h"
#include "ULOther.h"
#include "ULDateTimePicker.h"
#include "ULButtons.h"
#include <uxtheme.h>
//#include <tmschema.h>
#include <Vsstyle.h>

namespace ULWnds
{
	namespace ULControls
	{
		///\brief пространство имен расширенного лист контрола
		namespace ULListCtrlEx
		{
			///\brief интерфейс объекта помещаемого в ячейку расхиренного листконтрола
			struct IItemObject
			{
				///\brief деструктор
				virtual ~IItemObject(){};
				///\brief функция отрисовки объекта
				///\param pDC - указатель на класс контекста, в который производится отрисовка
				///\param rc - область ячейки, в который производится отрисовка
				///\note когда объект не активен, происходит только отрисовка его содержимого, \n
				///\ а сам он должен быть скрыт
				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc)=0;
				///\brief сообщение, посылаемое при клике по ячейке
				///\param rc - область ячейки
				///\note при потере фокуса объект должен стать не видимым и в последующем только производить \n
				///\ свою отрисовку в функции Draw
				virtual void OnClick(RECT& rc)=0;
				///\brief сообщение, посылаемое при нажатии по ячейке
				///\param rc - область ячейки
				virtual void OnLButtonDown(RECT& rc)=0;
				///\brief функция для получения размера объекта
				///\param prc - указательна ну структуру с размером объекта
				virtual bool GetRect(RECT* prc)=0;
				///\brief сбрасывает содержимое объекта
				virtual void Clean()=0;	
				///\brief сообщение посылаемое итему при смене темы windows
				virtual void OnThemeChanged()=0;
			};
			///\class CULListCtrlEx
			///\brief класс расширенного лист контрола
			class CULListCtrlEx:
				public ULWnds::ULControls::CULListCtrl
			{
			private:
				///\brief выбранные итем/сабитем
				int m_nItemSel,m_nSubItemSel;
				///\brief перечисление внутренних сообхений
				///\param LCEM_RETURNITEMSEL - идентификатор сообщения, посылаемого \n
				///\ самому себе при скролировании листа. Это необходимо, для того чтобы востановить\n
				///\ активность итема после перерисовки.
				enum enLCEM
				{
					LCEM_RETURNITEMSEL=WM_APP+1
				};
				///\brief 
				int m_nItemCount;
			public:
				///\brief Конструктор
				CULListCtrlEx();
				///\brief Деструктор
				~CULListCtrlEx();
				///\brief создаёт расширенный листконтрол
				///\param hParentWnd - хэндл родительского окна
				///\param uID - Идентификатор контрола
				///\param x,y - координаты окна
				///\param cx,cy - размеры окна
				///\param dwStyle - стиль окна
				///\param dwStyleEx - расширенный стиль окна		
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx);
				///\brief Фунцкия осуществляющая подмену оконных функций
				///\param hWnd
				BOOL Attach(HWND hWnd);
				///\brief вставляет итем 
				///\param nItem - позиция вставки
				///\param lpszItem - строка
				///\return номер вставленного итема
				int InsertItem(int nItem,LPCTSTR lpszItem);
				///\brief устанавливает текст в нужную позицию
				///\param nItem - № итема для вставки
				///\param nSubItem - № сабитема для вставки
				///\param lpszText - текст для вставки
				///\return TRUE в случае успеха
				BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);
				///\brief устанавливает текст в нужную позицию
				///\param nItem - № итема для вставки
				///\param nSubItem - № сабитема для вставки
				///\param pItemObject - указатель на объкт размещаемый в ячейке
				///\return TRUE в случае успеха
				BOOL SetItemObject(int nItem,int nSubItem,IItemObject* pItemObject);
				///\brief Функция для получения объекта из ячейки
				///\param nItem - № итема для вставки
				///\param nSubItem - № сабитема для вставки
				///\return pItemObject - запрошеный указатель на объкт размещаемый в ячейке \n
				///\в случае успеха, иначе NULL
				IItemObject* GetItemObject(int nItem,int nSubItem);
				///\brief удаляет все итемы в листконтроле
				///\return TRUE в случае успеха
				BOOL DeleteAllItems();
				///\brief удаляет выбранную позицию
				///\param nItem - позиция для удаления
				///\return TRUE в случае успеха
				BOOL DeleteItem(int nItem);
			protected:
				///\brief перегрузка CULListCtrl::SetItemData
				BOOL SetItemData(int nItem,DWORD_PTR dwData);
			protected:
				///\brief обработчик сообщения самотрисовки
				virtual LRESULT OnCustomDraw(LPARAM);
				///\brief обработчк сообщения выставления высоты(в реализации этого контрола)
				virtual LRESULT OnMeasureItem(WPARAM,LPARAM);
				///\brief обработчк сообщения окончания позиционирования
				virtual LRESULT OnWindowPosChanged(WPARAM,LPARAM);
				///\brief обработчк сообщения очистки фона
				virtual LRESULT OnEraseBkGnd(WPARAM,LPARAM);
				///\brief обработчк сообщения скролирования по вертикали
				virtual LRESULT OnVScroll(WPARAM,LPARAM);
				///\brief обработчк сообщения скролирония по горизонтали
				virtual LRESULT OnHScroll(WPARAM,LPARAM);
				///\brief обработчк сообщения вращения колеса мыши
				virtual LRESULT OnMouseWheel(WPARAM,LPARAM);
				///\brief обработчк сообщения нажатия на левую кнопку мыши
				virtual LRESULT OnLButtonDown(WPARAM,LPARAM);
				///\brief обработчк сообщения клика по итему
				virtual LRESULT OnNMClick(LPARAM lParam);
				///\brief обработчк сообщения LCEM_RETURNITEMSEL
				virtual LRESULT OnReturnItemSel(WPARAM,LPARAM);
				///\brief обработчк сообщения WM_COMMAND для пересылки сообщения родителю
				virtual LRESULT OnCommand(WPARAM,LPARAM);
				///\brief обработчк сообщения WM_DESTROY
				virtual LRESULT OnDestroy(WPARAM,LPARAM);
				///\brief обработчк сообщения WM_THEMECHANGED
				virtual LRESULT OnThemeChanged(WPARAM,LPARAM);
			};
			//=====================================================================
			///\class CItemText
			///\brief класс обычного тектового объекта для итема
			class CItemText:
				public IItemObject
			{
				///\brief объект для хранения текстовой строки
				ULOther::CULStr m_strText;
				///\brief хендл иконки
				 HICON m_hIcon;
			public:
				///\brief конструктор
				CItemText();
				///\brief функция для установки текста в объект
				///\param pszText - буфер с текстом
				void SetText(LPCTSTR pszText);
				///\brief функция для получения текстовой строки
				LPCTSTR GetText();
				///\brief устанавливает иконку слева от текста
				///\param hIcon - хендл иконки
				void SetIcon(HICON hIcon);

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/){}
				virtual void OnClick(RECT& /*rc*/){}
				virtual bool GetRect(RECT* /*prc*/){return false;}
				virtual void Clean();
				virtual void OnThemeChanged(){};
			};
			//=====================================================================
			///\class CItemEdit
			///\brief класс эдита объекта для итема
			class CItemEdit:
				public IItemObject		
			{	
			public:
				class CEdit:
					public CULEdit
				{
				public:
					CEdit();
					virtual LRESULT OnKillFocus(WPARAM,LPARAM);
					virtual LRESULT OnActivate(WPARAM wParam,LPARAM);
				};
				///\brief служит для отлова потери фокуса у объекта и обращения к самому Edit
				CEdit m_Edit;
			public:
				///\brief фунция создания объекта
				///\param hParentWnd - хэндл родительского окна, обычно это хендл контейнера CULListCtrlEx
				///\param uID - Идентификатор контрола
				///\param pszStr - начальный текст в итеме
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr);
				///\brief деструктор
				~CItemEdit();

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/){}
				virtual void OnClick(RECT& rc);
				virtual bool GetRect(RECT* /*prc*/){return false;}
				virtual void Clean();
				virtual void OnThemeChanged(){};
			};
			//=====================================================================
			///\class CItemComboBox
			///\brief класс комбобокса объекта для итема
			class CItemComboBox:
				public IItemObject		
			{	
			protected:
				///\brief хендл темы ХР
				HTHEME m_hTheme;
			protected:
				class CComboBox:
					public CULComboBox
				{
				public:
					CComboBox();
					virtual LRESULT OnKillFocus(WPARAM,LPARAM);
				};
				class CEdit:
					public CULEdit
				{
				public:
					CEdit();
				protected:
					virtual LRESULT OnKillFocus(WPARAM,LPARAM);
				};
			public:
				///\brief служит для отлова потери фокуса у объекта и обращения к самому ComboBox
				CComboBox m_ComboBox;
			protected:
				///\brief служит для отлова потери фокуса у объекта
				CEdit m_Edit;
			public:
				///\brief Деструктор
				~CItemComboBox();
				///\brief фунция создания объекта
				///\param hParentWnd - хэндл родительского окна, обычно это хендл контейнера CULListCtrlEx
				///\param uID - Идентификатор контрола
				///\param fDropDownList - флаг для создания TRUE(со статиком)/FALSE(с эдитом)
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,UINT uID,BOOL fDropDownList);

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/);
				virtual void OnClick(RECT& rc);
				virtual bool GetRect(RECT* prc);
				virtual void Clean();
				virtual void OnThemeChanged();
			};
			//=====================================================================
			///\class CItemDateTimePicker
			///\brief класс контрола времени даты объекта для итема
			class CItemDateTimePicker:
				public IItemObject
			{
			protected:
				///\brief хендл темы ХР
				HTHEME m_hTheme;
			public:
				class CDateTimePicker:
					public CULDateTimePicker
				{
				public:
					CDateTimePicker();
				protected:
					virtual LRESULT OnKillFocus(WPARAM wParam,LPARAM);
				};
				///\brief служит для отлова потери фокуса у объекта и обращения к самому DateTimePicker
				CDateTimePicker m_DateTimePicker;
			public:
				///\brief Деструктор
				~CItemDateTimePicker();
				///\brief фунция создания объекта
				///\param hParentWnd - хэндл родительского окна, обычно это хендл контейнера CULListCtrlEx
				///\param uID - Идентификатор контрола
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,WORD uID);

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/);
				virtual void OnClick(RECT& rc);
				virtual void Clean();
				virtual bool GetRect(RECT* prc);
				virtual void OnThemeChanged();
			};
			//=====================================================================
			///\class CItemButton
			///\brief класс кнопки объекта для итема
			class CItemButton:
				public IItemObject		
			{	
			protected:
				///\brief хендл темы ХР
				HTHEME m_hTheme;
			public:
				class CButton:
					public ULButtons::CULButton
				{
				public:
					CButton();
					virtual LRESULT OnKillFocus(WPARAM,LPARAM);
				};
				///\brief служит для отлова потери фокуса у объекта
				CButton m_Button;
			public:
				///\brief Деструктор
				~CItemButton();
				///\brief фунция создания объекта
				///\param hParentWnd - хэндл родительского окна, обычно это хендл контейнера CULListCtrlEx
				///\param uID - Идентификатор контрола
				///\param pszStr - начальный текст в кнопке
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,UINT uID,LPCTSTR pszStr);

				virtual void Draw(ULGDI::ULDC::CULDC* pDC,RECT& rc);
				virtual void OnLButtonDown(RECT& /*rc*/);
				virtual void OnClick(RECT& rc);
				virtual bool GetRect(RECT* /*prc*/){return false;}
				virtual void Clean();
				virtual void OnThemeChanged();
			};
		}
	}
}

#endif //_ULLISTCTRL_EX_H__

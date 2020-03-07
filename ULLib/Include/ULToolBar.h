///\file ULToolBar.h
///\brief Заголовочный файл класса тулбара(12.10.2007)
#pragma once
#ifndef __UL_ULTOOLBAR_H__
#define __UL_ULTOOLBAR_H__

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include "ULOther.h"
#include "ULSubClass.h"
#include "ULControls.h"
namespace ULWnds
{
	namespace ULBars
	{
		///\class CULToolBar
		///\brief Класс тулбара(12.10.2007)
		class CULToolBar: public CULSubClass
		{
		private:
			///\class CTBToolTip
			///\brief  класс для подсказки, чтоб выполнить обработку TTN_GETDISPINFO
			class CTBToolTip:public ULControls::CULToolTip
			{			
			public:
				///\brief конструктор
				CTBToolTip();
				///\brief деструктор
				virtual ~CTBToolTip();
				///\brief Указатель на текущий класс тулбара
				CULToolBar* m_pULToolBar;
			protected:
				///\brief Обработчик TTN_GETDISPINFO
				virtual LONG OnGetDispInfo(LPARAM);
			};
			///\brief Объект класса подсказаки
			CTBToolTip m_TBToolTip;
		protected:
			///\brief Cтруктура данных, которая несет необходимую информацию о кнопках
			///\param szToolTip - Тект подсказки
			///\param idCommand - ID команды
			///\param hMenu - хендл контекстного меню для 
			///	кнопки(если btnStyle==BTNS_DROPDOWN)
			struct tagInfoButtons
			{
				ULOther::CULStr szToolTip;
				UINT idCommand;	
				HMENU hMenu;
				tagInfoButtons& operator=(tagInfoButtons& InfoButtons)
				{
					szToolTip=InfoButtons.szToolTip;
					idCommand=InfoButtons.idCommand;
					hMenu=InfoButtons.hMenu;
					return *this;
				}
			};
			///\brief массив, который несет необходимую информацию о кнопках
			ULOther::CULArr<tagInfoButtons> m_pInfoButtons;
			///\brief Флаг,указывающий как размещен тулбар по краям родителя
			UINT m_afFlag;
			///\brief Ширина рисунка для кнопки
			int m_dxBitmap;
			///\brief Высота рисунка для кнопки
			int m_dyBitmap;
			///\brief m_tbStyle - стиль
			DWORD m_tbStyle;
		public:
			///\brief Конструктор
			CULToolBar(void);
			///\brief Конструктор копирования
			CULToolBar(CULToolBar& toolBar);
			///\brief Деструктор
			virtual ~CULToolBar(void);
			///\brief оператор копирования
			void operator=(CULToolBar&);
			///\brief Набор флагов для размещения тулбара по краям родителя
			///\param afTop - по верхнему краю
			///\param afBottom - по нижнему краю
			///\param afLeft - по левому краю
			///\param afRight - по правому краю
			///\param afNon - отстутствует соответствие
			enum enAlignFlags
			{
				afTop=CCS_TOP,				
				afBottom=CCS_BOTTOM,		
				afLeft=CCS_LEFT,			
				afRight=CCS_RIGHT,			
				afNon=CCS_NOPARENTALIGN		
			};
			///\brief Функция создания тулбара
			///\param hParentWndхэндл - родительского окна
			///\param tbID -ID тулбара
			///\param dxButton - ширина кнопок
			///\param dyButton - высота кнопок
			///\param dxBitmap - ширина картинок для кнопок
			///\param dyBitmap - высота картинок для кнопок
			///\param afFlag - положение относительно родителя
			///\param tbStyle - стиль
			///\param tbExStyle - расширенный стиль
			HWND Create(HWND hParentWnd,		
						UINT tbID,				
						int dxButton,			
						int dyButton,			
						int dxBitmap,			
						int dyBitmap,			
						UINT afFlag=afTop,	
						DWORD tbStyle=WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|
						TBSTYLE_AUTOSIZE|CCS_NODIVIDER,
						DWORD tbExStyle=TBSTYLE_EX_DRAWDDARROWS);
			///\brief Функция добавляет кнопку на тулбар
			///\param idCommand - ID контрола(кнопки)
			///\param btnState - состояние кнопки
			///\param btnStyle - стиль кнопки
			///\param szTTip - подсказка для кнопки
			///\param pszStr - подпись для кнопки(с TBSTYLE_LIST)
			///\param nBitmap - номер картинки для кнопки тулбара с лева направо и ресурса nID
			///\param nID - ID ресурса для картинки
			///\param hInst - HINSTANCE модуля ресурса
			///\param hMenu - ID контекстного меню для кнопки, если (btnStyle&BTNS_DROPDOWN)==BTNS_DROPDOWN)
			int AddButton(int idCommand,					
						BYTE btnState,						
						BYTE btnStyle,						
						LPCTSTR szTTip,						
						LPCTSTR pszStr,					
						int nBitmap,						
						UINT_PTR nID=IDB_STD_SMALL_COLOR,	
						HINSTANCE hInst=HINST_COMMCTRL,		
						HMENU hMenu=NULL);						
			///\brief Функция добавляет кнопку на тулбар
			///\param idCommand - ID контрола(кнопки)
			///\param btnState - состояние кнопки
			///\param btnStyle - стиль кнопки
			///\param szTTip - подсказка для кнопки
			///\param pszStr - подпись для кнопки(с TBSTYLE_LIST)
			///\param nBitmap - номер картинки для кнопки тулбара с лева направо и ресурса nID
			///\param hBitmap - хендл картинки
			///\param hMenu - ID контекстного меню для кнопки, если (btnStyle&BTNS_DROPDOWN)==BTNS_DROPDOWN)
			int AddButton(int idCommand,	
						BYTE btnState,		
						BYTE btnStyle,		
						LPCTSTR szTTip,		
						LPCTSTR pszStr,
						int nBitmap,		
						HBITMAP hBitmap,	
						HMENU hMenu=NULL);	  	
			///\brief Функция вставляет кнопку в тулбар
			///\param nInto - позиция вставки
			///\param idCommand - ID контрола(кнопки)
			///\param btnState - состояние кнопки
			///\param btnStyle - стиль кнопки
			///\param szTTip - подсказка для кнопки
			///\param pszStr - подпись для кнопки(с TBSTYLE_LIST)
			///\param nBitmap - номер картинки для кнопки тулбара с лева направо и ресурса nID
			///\param nID - ID ресурса для картинки
			///\param hInst - HINSTANCE модуля ресурса
			///\param hMenu - ID контекстного меню для кнопки, если (btnStyle&BTNS_DROPDOWN)==BTNS_DROPDOWN)
			int InsertButton(int nInto,		
						int idCommand,		
						BYTE btnState,		
						BYTE btnStyle,		
						LPCTSTR szTTip,		
						LPCTSTR pszStr,
						int nBitmap,  		
						UINT_PTR nID=IDB_STD_SMALL_COLOR,
						HINSTANCE hInst=HINST_COMMCTRL,	 
						HMENU hMenu=NULL);					 
			///\brief Функция вставляет кнопку в тулбар
			///\param nInto - позиция вставки
			///\param idCommand - ID контрола(кнопки)
			///\param btnState - состояние кнопки
			///\param btnStyle - стиль кнопки
			///\param szTTip - подсказка для кнопки
			///\param pszStr - подпись для кнопки(с TBSTYLE_LIST)
			///\param nBitmap - номер картинки для кнопки тулбара с лева направо и ресурса nID
			///\param hBitmap - хендл картинки
			///\param hMenu - ID контекстного меню для кнопки, если (btnStyle&BTNS_DROPDOWN)==BTNS_DROPDOWN)
			int InsertButton(int nInto,
						int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						LPCTSTR szTTip,
						LPCTSTR pszStr,
						int nBitmap,
						HBITMAP hBitmap,
						HMENU hMenu=NULL);
			///\brief Функция удаляет указанную кнопку
			int DeleteButton(int nButton/*!<Номер кнопки*/);
			/*!\brief Изменяет размер тулбара по ширине или высоте родителя
			в зависимости от местоположения
			с подгонкой ширины нулевой секции.
			Вставить в обработчик WM_SIZE окна родителя */
			void AutoSize();
			///\brief Функция возвращает колличество кнопок на тулбаре
			inline unsigned int GetButtonCount()
				{return (unsigned int)m_pInfoButtons.GetSize();}
			///\brief получение размера кнопки
			///\param nItem - номер итема
			///\param lpRect - указатель на структура с размером
			///\return TRUE  в случае успеха
      BOOL GetItemRect(int nItem,RECT* lpRect)
			  {return (BOOL)SendMessage(TB_GETITEMRECT,(WPARAM)nItem,(LPARAM)lpRect);}
			///\brief получает информацию о кнопке
			///\param nButton - номер кнопки
			///\param lpButton - указатель на структуру с информацией о кнопке
			///\return TRUE  в случае успеха
			BOOL GetButton(int nButton,LPTBBUTTON lpButton)
				{return (BOOL)SendMessage(TB_GETBUTTON,(WPARAM)nButton,(LPARAM)lpButton);}
			///\brief активирует/деактивирует кнопку
			///\param idButton - командный идентификатор кнопки
			///\param fEnable - флаг 
			///\return TRUE  в случае успеха
			BOOL EnableButton(int idButton,BOOL fEnable);
			///\brief нажимает/отжимает кнопку
			///\param idButton - командный идентификатор кнопки
			///\param fCheck - флаг 
			///\return TRUE  в случае успеха
			BOOL CheckButton(int idButton,BOOL fCheck);
			///\brief устанавливает расширенный стиль
			///\param dwExStyle - сам стиль
			///\return предыдущий стиль
			DWORD SetExtendedStyle(DWORD dwExStyle);
			///\brief возвращает расширенный стиль
			///\return стиль
			DWORD GetExtendedStyle();
			///\brief возвращает ширину и высоту кнопок в пикселях
			///\return структура с размером
			SIZE GetButtonSize();
			///\brief возвращает ширину и высоту рисунка в пикселях
			///\return структура с размером
			SIZE GetBitmapSize();
			///\brief Возвращает imagelist тулбара
			///\return imagelist. NULL в случае ошибки
			HIMAGELIST GetImageList();
			///\brief Заменяет imagelist тулбара
			///\param iImageID - индекс imagelist
			///\param himlNew - хендл imagelist
			///\return предыдущий imagelist
			HIMAGELIST SetImageList(int iImageID,HIMAGELIST himlNew);
		protected:
			///\brief Функция обработчик WM_WINDOWPOSCHANGING
			virtual LRESULT OnWindowPosChanging(WPARAM,LPARAM);
			///\brief Обработчик TBN_DROPDOWN
			virtual LRESULT OnDropDown(LPARAM);			
		};
	}
}
#endif //__UL_ULTOOLBAR_H__

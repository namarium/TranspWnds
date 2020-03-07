///\file ULStatusBar.h
///\brief Заголовочный файл класса статусбара(12.10.2007)
#pragma once
#ifndef __UL_ULSTATUSBAR_H__
#define __UL_ULSTATUSBAR_H__

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include "ULSubClass.h"
#include "ULControls.h"
namespace ULWnds
{
	namespace ULBars
	{
		///\class CULStatusBar
		///\brief Класс статусбара(12.10.2007)
		class CULStatusBar: public CULSubClass
		{
			///\brief Колличество секций на статусбаре
			UINT m_nParts;
		public:
			///\brief Конструктор
			CULStatusBar();
			///\brief Конструктор копирования
			CULStatusBar(CULStatusBar& statusBar);
			///\brief Деструктор
			virtual ~CULStatusBar();
			///\brief оператор копирования
			void operator=(CULStatusBar&);
			///\brief Функция создания статусбара
			///\param hParentWnd-хендл родителя для статусбара
			///\param nStatusID - ID статусбара
			///\param fGrip-разметка на статусбаре для изменения размеров
			HWND Create(HWND hParentWnd,short nStatusID,BOOL fGrip=TRUE);
			///\brief Добавление информационной секции на статусбар
			///\param Width - ширина секции
			///\param szToolTip - при наведении мышью подсказка, если 
			///	текст весь не уместился в секцию
			BOOL AddPart(int Width,LPCTSTR szToolTip);
			//Убирает все информационные секции со статусбара
			BOOL DeleteAll();
			///\brief Устанавливает иконку в информационную секцию
			///\param nPart - номер секции
			///\param hIcon - хендл иконки
			BOOL SetIcon(int nPart, HICON hIcon);
			///\brief Устанавливает иконку в информационную секцию
			///\param nPart - номер секции
			///\param uiID - ID иконки из ресурсов
			BOOL SetIcon(int nPart, short uiID);
			///\brief Устанавливает текст в информационную секцию
			///\param nPart - номер секции
			///\param szText - сам текст :)
			BOOL SetText(int nPart, LPCTSTR szText);
			///\brief Устанавливает текст подсказки в информационную секцию
			///\param nPart - номер секции
			///\param szText - сам текст :)
			BOOL SetToolTipText(int nPart, LPCTSTR szText);
			///\brief Изменяет размер сатусбара по ширине родителя
			///	с подгонкой ширины нулевой секции.
			///	Вставить в обработчик WM_SIZE окна родителя 
			void AutoSize();
		};
	}
}
#endif //__UL_ULSTATUSBAR_H__

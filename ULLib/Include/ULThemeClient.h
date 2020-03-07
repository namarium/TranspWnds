///\file ULThemeClient.h
///\brief Заголовочный файл класса оконной темы(06.08.2008)
#pragma once 
#ifndef _ULTHEMECLIENT_H__
#define _ULTHEMECLIENT_H__
#include "ULWnd.h"
#include "ULGDI.h"
#include "ULButtons.h"
namespace ULWnds
{
	///\class CULThemeClient
	///\brief класс оконной темы(06.08.2008)
	class CULThemeClient
	{
	private:
		///\brief Функция окна
		static LRESULT ClientWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
		///\brief Функция окна необходимая для сабклассирования дочерними контролами
		WNDPROC m_lpSubClassWndProc;
	protected:
		///\brief указатель на объект класса окна владельца
		CULWnd* m_pClientWnd;
		///\brief  контекст в памяти для двойной буферизации
		ULGDI::ULDC::CULDC m_MemDC;
		///\brief внутренний цвет
		COLORREF m_clrInner;
		///\brief внешний цвет
		COLORREF m_clrOuter;
		///\brief радиус скругления
		int m_nRadius;
		///\brief гиперссылка для кнопки закрыть (Х)
		ULWnds::ULControls::ULButtons::CULHyperLink m_hlX;
		///\brief гиперссылка для кнопки свернуть (_)
		ULWnds::ULControls::ULButtons::CULHyperLink m_hl_;
		///\brief идентификатор для кнопки свернуть
		enum {CTM_MINIMIZE=IDCANCEL+1};
	protected:		
		///\brief отрисовывает в контекст в памяти верхний левый угол
		void CreateLeftUpCorner();
		///\brief отрисовывает в контекст в памяти верхний правый угол
		void CreateRightUpCorner();
		///\brief отрисовывает в контекст в памяти нижний левый угол
		void CreateLeftDownCorner();
		///\brief отрисовывает в контекст в памяти верхний праавый угол
		void CreateRightDownCorner();
		///\brief отрисовывает в контекст в памяти границы
		void CreateBorder();
	public: 
		///\brief Конструктор
		CULThemeClient();
		///\brief Конструктор копирования 
		CULThemeClient(CULThemeClient& ThemeClient);
		///\brief Конструктор. создает тему
		///\param hClientWnd - окно владелец темы
		CULThemeClient(HWND hClientWnd);
		///\brief Оператор копирования
		virtual void operator = (CULThemeClient& ThemeClient);
		///\brief деструктор
		virtual ~CULThemeClient();
	//================================================
		///\brief Создает тему
		///\param hClientWnd - окно владелец темы
		///\return TRUE  в случае успеха
		BOOL Create(HWND hClientWnd);
		///\brief обработчик события очистки фона
		///\param dc - объект контейнер контекста
		///\return TRUE  в случае успеха
		virtual BOOL OnEraseBkGndClient(ULGDI::ULDC::CULDC& dc);
		///\brief обработчик события создания региона
		///\return TRUE  в случае успеха
		virtual BOOL OnCreateRgn();
		///\brief обработчик события создания фона
		///\return TRUE  в случае успеха
		virtual BOOL OnCreateBkGnd();
		///\brief устанавливает значения цветов
		///\param clrInner внутренний цвет
		///\param clrOuter внешний цвет
		void SetColors(COLORREF clrInner,COLORREF clrOuter);
		///\brief устанавливает значения цветов
		///\param nRadius радиус 
		///\param внешний цвет
		void SetRadius(int nRadius);
	};
}
#endif _ULTHEMECLIENT_H__

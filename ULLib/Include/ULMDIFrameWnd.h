///\file ULMDIFrameWnd.h
///\brief Заголовочный файл класса главного окна архетектуры MDI(2006)
#pragma once
#ifndef __UL_ULMDIFRAMEWND_H__
#define __UL_ULMDIFRAMEWND_H__

#include "ULFrameWnd.h"
#include "UlWndCtrlList.h"
#include "ULMDIClientWnd.h"
namespace ULWnds
{
	namespace ULFrames
	{
		///\class CULMDIFrameWnd 
		///\brief Класс главного окна архетектуры MDI(2006)
		class CULMDIFrameWnd :
			public CULFrameWnd
		{
		protected:
			///\briefСтиль окна-клиента
			DWORD m_dwClientStyle;
			///brief Расширеный стиль окна-клиента
			DWORD m_dwClientStyleEx;
			///\brief ID первого дочернего окна
			UINT m_idFirstChild;
			///\brief ID подменю, в котором будут отмечаться открытые дочерние окна
			UINT m_idSubMenu;
			///\brief Контейнер для плавающих панелек. 
			///	Все вспомогательные контролы, такие как тулбар, статусбар...
			///	размещаемые в MDI должны быть унаследованы от CULWndCtrl 
			///	и добавлены в m_WndCtrlList.
			///	Это необходимо свободной клиентской области главного окна 
			///	для позиционирования MDICLIENT окна.
			ULWndCtrls::CULWndCtrlList m_WndCtrlList;
			///\brief Объект MDI-клиентского окна
			CULMDIClientWnd* m_pMDIClientWnd;
		public:
			///\brief Конструктор
			CULMDIFrameWnd(void);
			///\brief Конструктор копирования
			CULMDIFrameWnd(CULMDIFrameWnd&);
			///\brief Деструктор
			virtual ~CULMDIFrameWnd(void);
			///\brief Оператор копирования
			virtual void operator = (CULMDIFrameWnd&);
			///\brief Функция создания главного окна
			///\param szWindowName - заголовок окна
			///\param MenuID - ID меню из ресурсов
			///\param IconID - ID иконки из ресурсов
			///\param IconSmID - ID маленькой иконки из ресурсов
			///\param BGColor - цвет клиентской области
			///\param idSubMenu - номер подменю главного меню окна,
			///	в котором будут отображаться открытые окна
			///\param dwStyle - стиль окна
			///\param dwStyleEx - расширенный стиль окна
			///\param idFirstChild - айди первого дочернего окна
			///\param dwClientStyle - стиль клинтского окна 
			///\param dwClientStyleEx - расширенный стиль клинтского окна
			HWND Create(TCHAR* szWindowName,
						short MenuID, 
						short IconID, 
						short IconSmID, 
						short BGColor,
						UINT idSubMenu,
						DWORD dwStyle=WS_OVERLAPPEDWINDOW |	WS_CLIPCHILDREN,
						DWORD dwStyleEx=WS_EX_APPWINDOW,
						UINT idFirstChild=1000,
						DWORD dwClientStyle=WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
						DWORD dwClientStyleEx=0);
			///\brief Дефолтовая функция окна
			inline virtual LRESULT DefWindowProc(UINT uMsg , WPARAM wParam , LPARAM lParam)
			{
				return ::DefFrameProc(*this,
					((m_pMDIClientWnd!=NULL)?m_pMDIClientWnd->m_hWnd:NULL),uMsg,wParam,lParam);
			};
			///\brief Функция для подмены класса клиентского окна
			///\param pMDIClientWnd указатель на новый объект клиентского окна
			///\return TRUE в случае успеха
			CULMDIClientWnd* ReplaceMDIClient(CULMDIClientWnd* pMDIClientWnd);
			///\brief Обработчик WM_CREATE
		protected:
			virtual LRESULT OnCreate(WPARAM /*wParam*/,LPARAM);
			///\brief Обработчик WM_SIZE
			virtual LRESULT OnSize(WPARAM,LPARAM lParam);
			///\brief Обработчик CULWndCtrlList::MDIM_GETCLIENT
			virtual LRESULT OnGetMDIClient(WPARAM,LPARAM)
				{return (BOOL)(LONG_PTR)m_pMDIClientWnd->m_hWnd;};	
			///\brief Обработчик срабатывания контрола с ID=ID_WINDOW_CASCADE
			///	раствляет окна каскадом
			virtual void OnWindowCascade(WORD ID,HWND hControl);
			///\brief Обработчик срабатывания контрола с ID=ID_WINDOW_ARRANGE
			///	упорядочивает окна
			virtual void OnWindowArrange(WORD ID,HWND hControl);
			///\brief Обработчик срабатывания контрола с ID=ID_WINDOW_TILE_HORZ
			///	раствляет окна горизотально
			virtual void OnWindowTileHorz(WORD ID,HWND hControl);
			///\brief Обработчик срабатывания контрола с ID=ID_WINDOW_TILE_VERT
			///	раствляет окна вертикально
			virtual void OnWindowTileVert(WORD ID,HWND hControl);
		};
	}
}
#endif//__UL_ULMDIFRAMEWND_H__

///\file ULWndCtrlList.h
///\brief Заголовочный файл класса контейнера для плавающих панелек(03.08.2007)
#pragma once
#ifndef __UL_ULCTRLWNDLIST_H__
#define __UL_ULCTRLWNDLIST_H__
#include "ULWndCtrl.h"
#include "ULOther.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		///\class CULWndCtrlList
		///\brief Класс - контейнер для плавающих панелек(03.08.2007)
		class CULWndCtrlList
		{
		private:
			///\brief Функция необходимаю для сабклассирования глвного окна
			static LRESULT SubClassParentProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
			///\brief Родная оконная функция главного окна
			static WNDPROC lpOldParentProc;
			///\brief Флаг необходимый для того чтоб сделать пнельки активными
			static BOOL m_fActive;
			///\brief Массив указателей для плавающих панелей
			static ULOther::CULArr<CULWndCtrl*> m_arWndCtrl;
		public:
			///\brief Сообщение, посылаемое MDI фрэйму, что получить MDI киент
			enum enToMDIFrameMessage{MDIM_GETCLIENT=WM_APP+1001};
			///\brief Координаты свободной клиетской области
			static RECT m_rcParentClientFreeRect;
		public:
			///\brief Конструктор 
			CULWndCtrlList();
			///\brief Конструктор копирования
			CULWndCtrlList(const CULWndCtrlList& WndCtrlList);
			///\brief Деструктор 
			~CULWndCtrlList();
			///\brief оператор копирования
			void operator=(const CULWndCtrlList& WndCtrlList);
			///\brief Функция созданий коньейнера панелек
			///\param hOwnerWnd - хендл окна родителя(носиеля)
			///\return TRUE в случае успеха
			BOOL Create(HWND hOwnerWnd);
			///\brief Функция добавления в контейнер новой панельки
			///\param pULWndCtrl - указатель на класс плавающей панельки
			///\return TRUE в случае успеха
			BOOL AddWndCtrl(CULWndCtrl* pULWndCtrl);
		};
	}
}
#endif //__UL_ULCTRLWNDLIST_H__

///\file ULMDIClientWnd.h
///\brief Заголовочный файл класса клиентского окна архетектуры MDI(26.07.2007)
#pragma once
#ifndef __UL_ULMDICLIENTWND_H__
#define __UL_ULMDICLIENTWND_H__

#include "ULSubClass.h"
#include "ULOther.h"
#include "ULMDIChildWnd.h"
namespace ULWnds
{
	namespace ULFrames
	{
		///\class CULMDIClientWnd
		///\brief Класс клиентского окна архетектуры MDI(26.07.2007)
		class CULMDIClientWnd:
			public CULSubClass
		{
		private:
			///\brief массив дочерних окон
			ULOther::CULArr<CULMDIChildWnd*> m_pMDIChilds;
		public:
			///\brief Конструктор
			CULMDIClientWnd(void);
			///\brief Конструктор копирования
			CULMDIClientWnd(CULMDIClientWnd& MDIClientWnd);
			///\brief Деструктор
			virtual ~CULMDIClientWnd(void);
			///\brief Оператор копирования
			virtual void operator = (CULMDIClientWnd&);
			///\brief Функция создания клиентского окна
			///\param hParentWnd - хендл родителя
			///\param idSubMenu - номер подменю главного меню окна,в котором будут отображаться открытые окна
			///\param idFirstChild - айди первого дочернего окна
			///\param dwClientStyle - стиль клинтского окна 
			///\param dwClientStyleEx - расширенный стиль клинтского окна
			///\return хендл созданного окна
			HWND Create(HWND hParentWnd,
				UINT idSubMenu,
				UINT idFirstChild,
				DWORD dwClientStyle,
				DWORD dwClientStyleEx);
			///\brief Функция добавляет чайлд в массив указателей MDI-чайлдов
			///\param pULMDIChildWnd - указатель на MDI-чалд
			///\return колличество чайлдов
			int AddChild(CULMDIChildWnd* pULMDIChildWnd);
			///\brief возвращает колличество дочерних окон
			///\return число дочерних окон
			inline int GetChildCount()
				{return (int)m_pMDIChilds.GetSize();}
		protected:
			///\Обработчик сообщения(CLM_DESTROYCHILD), которое чайлд посылает при закрытии для удаления из m_pMDIChilds
			///\param wParam - HWND чайлда
			///\return TRUE в случае успеха
			virtual LRESULT OnDestroyChild(WPARAM wParam,LPARAM);
		};
	}
}
#endif//__UL_ULMDICLIENTWND_H__

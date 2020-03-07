///\file ULSubClass.h
///\brief Заголовочный файл класса для сабклассирования контролов(12.10.2007)
#pragma once
#ifndef __SUBCLASS__H_
#define __SUBCLASS__H_
#include "ULWnd.h"
namespace ULWnds
{
	///\class CULSubClass
	///\brief Класс для сабклассирования контролов(12.10.2007)
	class CULSubClass:public CULWnd
	{
	private:
		///\brief Старая функция окна
		WNDPROC m_lpOldWndProc;
	protected:
		///\brief Новая функция окна
		static LRESULT WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
		///\brief Новая функция окна родителя
		static LRESULT ParentWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
		///\brief Фунцкия вызываемая перед сабклассированием
		virtual void PreSubClass();
	public:
		///\brief Конструктор
		CULSubClass();
		///\brief Конструктор копирования
		CULSubClass(CULSubClass& SubClass);
		///\brief Деструктор
		virtual ~CULSubClass();
		///\brief Оператор копирования
		virtual void operator = (CULSubClass&);
		///\brief Фунцкия осуществляющая подмену оконных функций
		///\param hWnd - хендл окна, которое будет привязано к классу
		virtual BOOL Attach(HWND hWnd);
		///\brief Получает ID дочернего контрола
		///\return ID контрола
		inline int GetDlgCtrlID()
			{return ::GetDlgCtrlID(*this);}
		///\brief Вызов функции окна
		inline virtual LRESULT CallWindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
    {return ((m_lpOldWndProc)?::CallWindowProc(m_lpOldWndProc,*this, uMsg, wParam, lParam):0);}	
	};
}
#endif //__SUBCLASS__H_

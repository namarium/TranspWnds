///\file ULWndCtrl.h
///\brief Заголовочный файл класса окна типа тулвиндоу(21.07.2007)
#pragma once
#ifndef __UL_ULCTRLWND_H__
#define __UL_ULCTRLWND_H__
#include "ULWnd.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		///\class CULWndCtrl
		///\brief Класс окна типа тулвиндоу, которое может прилепляться 
		///	к краям клиентской области окна родителя(21.07.2007)
		class CULWndCtrl:public CULWnd
		{
		private:
			///\brief Стили окна
			///\param twsPopupStyles,twsPopupExStyles - для плавающего окна
			///\param twsChildStyles,twsChildExStyles - для прилипшего окна
			enum enCtrlWndStyles
			{
				twsPopupStyles	=WS_POPUP | WS_CLIPCHILDREN |
					WS_CLIPSIBLINGS | WS_SYSMENU | WS_CAPTION /*| WS_THICKFRAME*/,
				twsPopupExStyles=WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE,
				twsChildStyles	=WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				twsChildExStyles	=0
			};
			///\brief Флаг перетаскивания в текущий момент
			BOOL	m_fDragging;
			///\brief Флаг нажатой VK_CONTROL в текущий момент
			BOOL	m_fControl;
			BOOL	m_fOldDrawDocked;
			BOOL	m_fOldControl;
			POINT	m_ptPos;
			SIZE	m_FrameSize;
			RECT	m_rcBorderDock;
			///\brief Позиция плавающей панельки
			RECT	m_rcBorderFloat;
			RECT	m_rcOldRect;
			///\brief Стили панели
			UINT	m_dwDockingStyle;
		//////////////////////////
			///\brief Флаг движения мыши с зацепленной панелью
			BOOL	m_fMouseMoved;
			///\brief Текущая позиция курсора
			POINT	m_ptCursorPos;
			///\brief Начальное расположение панельки
			RECT	m_rcStart;
			///\brief Начальная позиция курсора
			POINT	m_ptOldCursorPos;
		protected:
			///\brief Флаг состояния прилип/плавает
			BOOL	m_fDocked;
			///\brief Флаг состояния к каком краю прилип
			UINT	m_dwDockedState;
			///\brief Отступ необходимый для прилипания к краю окна родителя
			int		m_nDockedSize;
			///\brief Размер панели в плавающем состоянии
			SIZE	m_FloatingSize;
		public:
			///\brief Сообщение, которое посылается окну при "закрытии" панели
			enum enToParentMessage{CWM_CLOSE=WM_APP+1};
			///\brief Стили панели 
			///\param dsBorderTop границца сверху				
			///\param dsBorderBottom границца снизу
			///\param dsBorderLeft границца слева
			///\param dsBorderRight границца справа
			///\param dsDrawGripperDocked держатель в стыкованном состоянии
			///\param dsDrawGripperFloating держатель в плавающем состоянии
			///\param dsForceDock всегда пристыкован
			///\param dsForceFloat всегда плавает			
			///\param dsResizeble изменяемый размер		
			///\param dsNoResize неизменяемый размер		
			///\param dsUseBorders	использовать границы		
			///\param dsNoSetFocus	не захватывает фокус
			///\param dsNoDestroy	"неубиваемый"
			///\param dsAllowDockLeft пристыковывается слева
			///\param dsAllowDockRight	пристыковывается с права
			///\param dsAllowDockTop пристыковывается сверху
			///\param dsAllowDockBottom пристыковывается снизу
			///\param dsAllowDockAll везде пристыковывается
			enum enDockingStyles
			{
				dsBorderTop				=0x0001,
				dsBorderBottom			=0x0002,
				dsBorderLeft			=0x0004,
				dsBorderRight			=0x0008,
				dsDrawGripperDocked		=0x0010,
				dsDrawGripperFloating	=0x0020,
				dsForceDock				=0x0040,
				dsForceFloat			=0x0080,
				dsResizeble				=0x0100,
				dsNoResize				=0x0200,
				dsUseBorders			=0x1000,
				dsNoSetFocus			=0x2000,
				dsNoDestroy				=0x4000,
				dsAllowDockLeft			=0x10000,
				dsAllowDockRight		=0x20000,
				dsAllowDockTop			=0x40000,
				dsAllowDockBottom		=0x80000,
				dsAllowDockAll			=dsAllowDockLeft|dsAllowDockRight|
										dsAllowDockTop|dsAllowDockBottom
			};
			///\brief Конструктор 
			CULWndCtrl();
			///\brief Конструктор копирования
			CULWndCtrl(CULWndCtrl&);
			///\brief Деструктор 
			virtual ~CULWndCtrl();
			///\brief оператор копирования
			void operator=(CULWndCtrl&);
			///\brief Создаёт панель
			///\param szWndName - заголовок окна
			///\param x,y - координаты окна(в плавающем состоянии)
			///\param cx,cy - размер окна(в плавающем состоянии)
			///\param hParentWnd - хендл родительского окна
			///\param dwDockedState - назначение стыковки на момент создания 
			///\param fDocked - состояние стыковки на момент создания
			///\param dwDockingStyles - стили панели
			///\return Хэндл созданного окна
			HWND Create(TCHAR* szWndName,
				int x,int y,int cx,int cy,
				HWND hParentWnd,
				DWORD dwDockedState,
				BOOL fDocked=TRUE,
				DWORD dwDockingStyles=dsUseBorders|
							dsDrawGripperDocked|
							dsDrawGripperFloating|
							dsBorderTop|dsBorderLeft|
							dsBorderBottom|dsBorderRight|
							dsAllowDockAll);
			///\brief Функция изменияет стили прилипшей панели на стили
			///	плавающей панели и наоброт
			void TogglePopupStyle();
			///\brief Функция изменияет режим  прилипшей панели на стили
			///	плавающей панели и наоборот
			void ToggleDockingMode();
			///\brief Функция для отрисовки контура при перетаскивании окна
			///\param rect - координаты прямоугольника отрисовки
			///\param fDocked флаг указывающий вид прямоугольника от положения панели
			void DrawXorFrame(RECT *rect, BOOL fDocked);
			///\brief Функция если вернула не 0, то значит потенциальный
			///	край клиентской области для прилипания
			///\param rcDrag прямоугольник для прилипания 
			///\param ptCursorPos координаты курсора, проверяемые на прилипание 
			///\return если не 0, то позицию по enDockingStyles
			UINT IsDockable(RECT* rcDrag,POINT ptCursorPos);
			///\brief Функция необходима для пересчета координат для плавающей панели
			void CalcFloatingRect();
			///\brief Функция оcуществляет отрисовку области, необходимой
			///	для того чтоб зацепиться мышкой при перетаскивании
			void DrawGripper(HDC hDC,RECT rcClient);
			///\brief Посылает событие для перепозиционирования всех 
			///	панелей на родительском окне
			void ResizeAllWndCtrls(void);
			///\brief Возвращает сотояние панели (прилип/плавает)
			///\return TRUE/FALSE - прилип/плавает
			inline BOOL IsDocked()
				{return m_fDocked;}
			///\brief Возвращает размер стыковочной области панели 
			///\return размер стыковочной области панели 
			inline int GetDockedSize()
				{return m_nDockedSize;}
			///\brief Возвращает положение панели 
			///\return положение панели 
			inline UINT GetDockedState()
				{return m_dwDockedState;}
		protected:
			///\brief Функции обработчик WM_NCLBUTTONDBLCLK
			virtual LRESULT OnNCLButtonDblClick(WPARAM,LPARAM);
			///\brief Функции обработчик WM_WINDOWPOSCHANGED
			virtual LRESULT OnWindowPosChanged(WPARAM,LPARAM);
			///\brief Функции обработчик WM_NCLBUTTONDOWN
			virtual LRESULT OnNCLButtonDown(WPARAM,LPARAM);
			///\brief Функции обработчик WM_MOUSEMOVE
			virtual LRESULT OnMouseMove(WPARAM,LPARAM);
			///\brief Функции обработчик WM_LBUTTONUP
			virtual LRESULT OnLButtonUp(WPARAM,LPARAM);
			///\brief Функции обработчик WM_GETMINMAXINFO
			virtual LRESULT OnGetMinMaxInfo(WPARAM,LPARAM);
			///\brief Функции обработчик WM_SETTINGCHANGE
			virtual LRESULT OnEraseBkGnd(WPARAM,LPARAM);
			///\brief Функции обработчик WM_ERASEBKGND
			virtual LRESULT OnSettingChange(WPARAM,LPARAM);
			///\brief Функции обработчик WM_NCHITTEST
			virtual LRESULT OnNCHitTest(WPARAM Type,LPARAM CurPos);
			///\brief Функции обработчик WM_CLOSE
			virtual LRESULT OnClose(WPARAM,LPARAM);
		};
	}
}
#endif //__UL_ULCTRLWND_H__

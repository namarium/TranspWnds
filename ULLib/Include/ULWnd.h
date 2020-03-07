///\file ULWnd.h
///\brief Заголовочный файл класса root окна(24.10.2007)
#pragma once
#ifndef __ULWND_H__
#define __ULWND_H__

#include <windows.h>
#include "ULMessageMap.inl"
#include "ULProperty.h"

namespace ULWnds
{
	///\class CULWnd
	///\brief Класс root для всех дочерних окон(24.10.2007)
	class CULWnd
	{
	protected:
		LPCTSTR m_szDefClassName;
	protected:
		///\brief Функция окна
		static LRESULT WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
/*
		///\brief хендлер Set для свойства m_propText
		///\param pstrText - указатель на буфер текста
		void SetText(ULOther::CULStr* pstrText);
		///\brief хендлер Get для свойства m_propText
		///\param pstrText - указатель на буфер текста
		void GetText(ULOther::CULStr* pstrText);
*/
		///\brief хендлер Set для свойства propStyle
		///\param pdwStyle - указатель на стиль
		void SetStyle(DWORD* pdwStyle);
		///\brief хендлер Get для свойства propStyle
		///\param pdwStyle - указатель на стиль
		void GetStyle(DWORD* pdwStyle);
		///\brief хендлер Set для свойства propStyleEx
		///\param pdwStyleEx - указатель на стиль
		void SetStyleEx(DWORD* pdwStyleEx);
		///\brief хендлер Get для свойства propStyleEx
		///\param pdwStyleEx - указатель на стиль
		void GetStyleEx(DWORD* pdwStyleEx);

		///\brief хендлер Set для свойства propLeft
		///\param pnLeft - указатель на позицию
		void SetLeft(int* pnLeft);
		///\brief хендлер Get для свойства propLeft
		///\param pnLeft - указатель на позицию
		void GetLeft(int* pnLeft);
		///\brief хендлер Set для свойства propTop
		///\param pnTop - указатель на позицию
		void SetTop(int* pnTop);
		///\brief хендлер Get для свойства propTop
		///\param pnTop - указатель на позицию
		void GetTop(int* pnTop);
		///\brief хендлер Set для свойства propWidth
		///\param pdwWidth - указатель на ширину
		void SetWidth(DWORD* pdwWidth);
		///\brief хендлер Get для свойства propWidth
		///\param pdwWidth - указатель на ширину
		void GetWidth(DWORD* pdwWidth);
		///\brief хендлер Set для свойства propHeight
		///\param pdwHeght - указатель на высоту
		void SetHeight(DWORD* pdwHeght);
		///\brief хендлер Get для свойства propHeight
		///\param pdwHeght - указатель на высоту
		void GetHeight(DWORD* pdwHeght);
		///\brief хендлер Set для свойства propVisible
		///\param pfVisible - указатель на флаг
		void SetVisible(BOOL* pfVisible);
		///\brief хендлер Get для свойства propVisible
		///\param pfVisible - указатель на флаг
		void GetVisible(BOOL* pfVisible);
		///\brief хендлер Set для свойства propEnable
		///\param pfEnable - указатель на флаг
		void SetEnable(BOOL* pfEnable);
		///\brief хендлер Get для свойства propEnable
		///\param pfEnable - указатель на флаг
		void GetEnable(BOOL* pfEnable);
	public:
		///\brief Конструктор
		CULWnd(void);
		///\brief Конструктор копирования
		CULWnd(CULWnd& Wnd);
		///\brief Конструктор 
		CULWnd(HWND hwnd);
		///\brief Десруктор
		virtual ~CULWnd(void);
		///\brief Объект класса добавления и обработки сообщений
		///сообщения добавляются в конструкторе класса
		CULMessageMap<CULWnd> MessageMap;
		///\brief Функция для получения класса окна по его хендлу,
		///если окно создовалось при помощи этого класса,
		///т.к.в обработчике WM_CREATE или WM_INITDIALOG в GWL_USERDATA помещается this
		///\param hWnd Хэндл окна
		///\return указатель на класс окна
		static CULWnd* FromHandle(HWND hWnd)
			{return (CULWnd *)(LONG_PTR)::GetWindowLong(hWnd, GWL_USERDATA);}
		///\brief Функция окна необходимая для сабклассирования дочерними контролами
		WNDPROC m_lpSubClassWndProc;
		///\brief функция вызываемая перед обработкой сообщения
		///\необходима инициализация GetApp()->MessageMap.InitializePreTranslateMessage (рекомендуется вызывать в конструкторе)
		///\после того как функция будет не нужна, её необходимо освободить
		///\ GetApp()->MessageMap.UnInitializePreTranslateMessage (рекомендуется вызывать в деструкторе)
		///\param pMsg - указатель на структуру несущую информацию о сообщении
		///\return TRUE для продолжении обработкии сообщения, FALSE для прекращения обработки сообщения
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		///\brief хэндл текущего окна 
		HWND m_hWnd;
		///\brief хэндл родительского окна
		HWND m_hParentWnd;
		///\brief Для подстановки объекта класса там где требуется 
		///только хендл окна
		inline operator HWND() const{return m_hWnd;}
		///\brief Оператор копирования
		virtual void operator = (CULWnd&);
	//==============================================================
		///\brief создает окно
		///\param lpClassName - класс окна.Если lpClassName равен NULL\n
		///\ то регистрируеться класс поумолчанию ULWNDCLASS
		///\param lpWindowName - имя окна
		///\param dwStyle - стиль окна
		///\param x,y,nWidth,nHeight - координаты и размеры окна
		///\param hWndParent - хендл роддительского окна
		///\param hMenu - хендл меню. для чайлдовых это идентификатор
		///\return TRUE в случае успеха
		BOOL Create(LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,
			int x,int y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu);
		///\brief создает окно
		///\param dwExStyle - расширенный стиль окна
		///\param lpClassName - класс окна.Если lpClassName равен NULL\n
		///\ то регистрируеться класс поумолчанию ULWNDCLASS
		///\param lpWindowName - имя окна
		///\param dwStyle - стиль окна
		///\param x,y,nWidth,nHeight - координаты и размеры окна
		///\param hWndParent - хендл роддительского окна
		///\param hMenu - хендл меню. для чайлдовых это идентификатор
		///\return TRUE в случае успеха
		BOOL CreateEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,
			int x,int y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu);
		///\brief регистрирует класс окна поумолчанию
		///\return TRUE в случае успеха
		BOOL RegisterDefClass();
		///\brief Дефолтовая функция окна
		virtual LRESULT DefWindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
		///\brief Вызов функции окна
		virtual LRESULT CallWindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
		///\brief Функция вызывающаяся из WndProc по приходу сообщения
		///\return если возвращает не ноль, то пропускается обработка этого сообщения другими обработчиками
		virtual LRESULT OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
		///\brief Получение текста окна
		///\param lpString - строка
		///\param nMaxCount - максимальная лдина строки
		///\return колличество записанных символов в lpString
		int GetWindowText(LPTSTR lpString,int nMaxCount);
		///\brief Получение длины текста окна
		///\return колличество символов в окне
		int GetWindowTextLength();
		///\brief Установка текста в окно
		///\param lpString - Текст
		///\return TRUE в случае успеха
		BOOL SetWindowText(LPCTSTR lpString);
		///\brief отправка сообщения в окно
		///\param Msg - Сообщение
		///\param wParam - Параметр 1
		///\param lParam - Параметр 2
		///\return ответ обработчика сообщения
		LRESULT SendMessage(UINT Msg,WPARAM wParam=0,LPARAM lParam=0);
		///\brief постановка сообщения в окна в конец очереди цикла сообщений
		///\param Msg - Сообщение
		///\param wParam - Параметр 1
		///\param lParam - Параметр 2
		///\return TRUE в случае успеха
		BOOL PostMessage(UINT Msg,WPARAM wParam=0,LPARAM lParam=0);
		///\brief Установка фокуса
		///\return возвращает фокус предыдущего окна
		HWND SetFocus();
		///\brief Уничтожает окно
		///\return TRUE в случае успеха
		BOOL DestroyWindow();
		///\brief Делает окно активным
		///\return Пердыдущее активное окно
		HWND SetActiveWindow();
		///\brief Помещает окно на передний план и делает его активным
		///\return TRUE в случае успеха
		BOOL SetForegroundWindow();
		///\brief Возвращает родителя
		///\return Хэндл родителя
		HWND GetParent();
		///\brief Устанавливает нового родителя
		///\return Хэндл предыдущего родителя
		HWND SetParent(HWND hWndNewParent);
		///\brief Изменяет состояние видимости окна
		///\param nCmdShow - Состояние
		///\return TRUE в случае предыдущей видимости, иначе FALSE
		BOOL ShowWindow(int nCmdShow);
		///\brief Обновляет окно
		///\return TRUE в случае предыдущей видимости, иначе FALSE
		BOOL UpdateWindow();
		///\brief Изменяет позицию окна
		///\param hWndInsertAfter - положение окна по Z координате
		///возможны HWND_BOTTOM,HWND_NOTOPMOST,HWND_TOP,HWND_TOPMOST
		///\param X,Y - Координаты
		///\param cx,cy - Размеры
		///\param uFlags - флаг отображения
		///\return TRUE в случае успеха
		BOOL SetWindowPos(HWND hWndInsertAfter,int X,int Y,int cx,int cy,UINT uFlags);
		///\brief "Изменяет multiple-window – position structure"
		///\param hWinPosInfo - "Handle to a multiple-window – position structure"
		///	содержащая информацию об одной или более структур
		///\param hWndInsertAfter - положение окна по Z координате
		///возможны HWND_BOTTOM,HWND_NOTOPMOST,HWND_TOP,HWND_TOPMOST
		///\param x,y - Координаты
		///\param cx,cy - Размеры
		///\param uFlags - флаг отображения
		///\return "Изменяет multiple-window – position structure"
		HDWP DeferWindowPos(HDWP hWinPosInfo,HWND hWndInsertAfter,
			int x,int y,int cx,int cy,UINT uFlags);
		///\brief Перемецает окно
		///\param X,Y - Координаты
		///\param nWidth,nHeight - Размеры
		///\param fRepaint - флаг наличия перерисовки после перемещения
		///\return TRUE в случае успеха
		BOOL MoveWindow(int X,int Y,int nWidth,int nHeight,BOOL fRepaint);
		///\brief Возвращает инфрмацию об окне
		///\param nIndex - ID поля возвращаемой информации
		///\param запрашиваемая информация
		LONG GetWindowLong(int nIndex);
		///\brief устанавливает новое значение в поле окна
		///\param nIndex- ID поля 
		///\param dwNewLong - новое значение
		///\return предыдущее значение
		LONG SetWindowLong(int nIndex,LONG dwNewLong);
		///\brief Модифицирует стиль окна
		///\param dwRemStyle - Убираемый стиль
		///\param dwAddStyle - Устанавливаемый стиль
		///\param uFlags - Флаг обновления окна
		///\return TRUE в случае успеха
		BOOL ModifyStyle(DWORD dwRemStyle,DWORD dwAddStyle,UINT uFlags=SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		///\brief Модифицирует расширенный стиль окна
		///\param dwRemStyleEx - Убираемый стиль
		///\param dwAddStyleEx - Устанавливаемый стиль
		///\param uFlags - Флаг обновления окна
		///\return TRUE в случае успеха
		BOOL ModifyStyleEx(DWORD dwRemStyleEx,DWORD dwAddStyleEx,UINT uFlags=SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		///\brief Функция для получения размера клиентской области
		///\param lpRect - Указатель на получаемый размер
		///\return TRUE в случае успеха
		BOOL GetClientRect(LPRECT lpRect);
		///\brief Функция для получения расположения окна
		///\param lpRect - Указатель на получаемое расположение
		///\return TRUE в случае успеха
		BOOL GetWindowRect(LPRECT lpRect);
		///\brief Устанавливает шрифт окна
		///\param hFont - Хэндл шрифта
		///\param fRedraw - Перерисует себя если TRUE
		void SetFont(HFONT hFont,BOOL fRedraw);
		///\brief Возвращает шрифт окна
		HFONT GetFont();		
		///\brief Обновляет указанный прямоугольник на окне
		///\param lpRect - прямоугольник для обновления, если NULL, то обновиться всё окно
		///\param fErase - при TRUE стерает содержимое
		///\return TRUE в случае успеха
		BOOL InvalidateRect(CONST RECT *lpRect=NULL,BOOL fErase=FALSE);
		///\brief Выводит диалоговое окно
		///\param lpText - тект
		///\param lpCaption - заголовок
		///\param uType - тип окна
		///\return  
		///	IDABORT Abort button was selected. 
		///	IDCANCEL Cancel button was selected. 
		///	IDCONTINUE Continue button was selected. 
		///	IDIGNORE Ignore button was selected. 
		///	IDNO No button was selected. 
		///	IDOK OK button was selected. 
		///	IDRETRY Retry button was selected. 
		///	IDTRYAGAIN Try Again button was selected. 
		///	IDYES Yes button was selected. 
		int MessageBox(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
		///\brief Возвращает контекст окна
		///\return контекст окна
		HDC GetDC();
		///\brief Задаёт регион для окна
		///\param hRgn - хэндл региона
		///\param fRedraw - флаг перерисовки после установки региона
		///\return TRUE в случае успеха
		BOOL SetWindowRgn(HRGN hRgn,BOOL fRedraw);
		///\brief Возвращает регион для окна
		///\param hRgn - хэндл возвращаемого региона
		///\return 
		///	NULLREGION The region is empty. 
		///	SIMPLEREGION The region is a single rectangle. 
		///	COMPLEXREGION The region is more than one rectangle. 
		///	ERROR The specified window does not have a region, 
		///	or an error occurred while attempting to return the region.  
		int GetWindowRgn(HRGN hRgn);
		///\brief Переносит окно на вперёд по Z координате
		///\return TRUE в случае успеха
		BOOL BringWindowToTop();
		///\brief Переводит координату относительно текущего окна в абсолютную координату экрана
		///\param lpPoint - указатель на координату
		///\return TRUE в случае успеха
		BOOL ScreenToClient(LPPOINT lpPoint);
		///\brief Осуществляет перерисовку окна
		///\param lprcUpdate - перерисовываемый прямоугольник. Если NULL, то перерисовывается весь прямоугольник.
		///\param hrgnUpdate - перерисовываемый регион. Если NULL, то перерисовывается весь регион.
		///\param flags - флпг перерисовки
		///\return TRUE в случае успеха
		BOOL RedrawWindow(CONST RECT *lprcUpdate,HRGN hrgnUpdate,UINT flags);
		///brief активирует/дезактивирует перерисовку окна
		///\param fRedraw - Флаг активации
		///\return TRUE в случае успеха
		BOOL SetRedraw(BOOL fRedraw);
		///\brief "включает/выключает" окно
		///\param fEnable - флаг включения/выключения
		///\return TRUE в случае успеха
		BOOL EnableWindow(BOOL fEnable);
		///\brief Установка таймера для окна
		///\param nIDEvent - ID таймера, используется для изменения времени срабатывания
		///\param uElapse - время срабатывания в мс
		///\return ID таймера
		UINT_PTR SetTimer(UINT_PTR nIDEvent,UINT uElapse);
		///\brief Уничтожает выбранный таймер
		///\param uIDEvent - ID таймера, который вернул SetTimer
		///\return TRUE в случае успеха
		BOOL KillTimer(UINT_PTR uIDEvent);
		///\brief Устанавливает меню для окна
		///\param hMenu - хендл меню
		///\return TRUE в случае успеха
		BOOL SetMenu(HMENU hMenu);
		///\brief Получает хендл на дочерний контрол
		///\param nIDDlgItem - ID контрола
		///\return Хендл контроа
		HWND GetDlgItem(int nIDDlgItem);
		///\brief Проверяет видимость окна
		///\return TRUE если видимо
		BOOL IsWindowVisible();
		///\brief Проверяет доступность окна
		///\return TRUE если видимо
		BOOL IsWindowEnabled();
		///\brief Определяет существует ли окно
		///\return TRUE если существует
		BOOL IsWindow();
		///\brief Переводит координаты клиента в экранные
		///\param lpPoint - структура с координатой
		///\return FALSE в случае неудачи
		BOOL ClientToScreen(LPPOINT lpPoint);
		///\brief Устанавливает иконку на окно
		///\param hIcon - хендл иконки
		///\param fBig - если TRUE, то будет установлена большая иконка, иначе маленькая
		///\return хендл предыдущей установленной иконки в случае успеха, иначе NULL
		HICON SetIcon(HICON hIcon,BOOL fBig);
		///\brief Утанавливает прозрачность окна по а-каналу и/или по цветовому ключу
		///\param crKey - цветовой ключ, по по которому окно будет прозрачно
		///\param bAlpha - альфа-канал
		///\param  dwFlags - критерий работы функции по ключу(LWA_COLORKEY)
		///	и/или по альфаканалу(LWA_ALPHA)
		///\return FALSE в случае неудачи
	#if(_WIN32_WINNT >= 0x0500)
		BOOL SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
	#endif
		///\brief отсоединяет класс окна от хендла
		///\return хендл окна
		HWND Detach();
		///\brief Функция заносит/изменяет свойство в списке свойств окна
		///\param lpString - имя свойства
		///\param hData - даннык
		///\return TRUE в случае успеха
		BOOL SetProp(LPTSTR lpString,HANDLE hData);
		///\brief Функция получает свойство из списка свойств окна
		///\param lpString - имя свойства
		///\return данные
		HANDLE GetProp(LPTSTR lpString);
		///\brief сворачивает окно
		///\return TRUE в слуыае успеха
		BOOL CloseWindow();
		///\brief возвращает меню окна
		///\return меню окна
		HMENU GetMenu();
		///\brief возвращает запрашиваемое окно
		///\param uCmd - параметр запроса
		///\return хендл запрашиваемого окна
		HWND GetWindow(UINT uCmd);
		///\brief функция для изменения полей класса окна
		///\param nIndex - индекс поля
		///\param dwNewLong - значение поля
		///\return предыдущее значение
		ULONG_PTR SetClassLongPtr(int nIndex,LONG dwNewLong);
		///\brief функция для получения полей класса окна
		///\param nIndex - индекс поля
		///\return значение поля
		ULONG_PTR GetClassLongPtr(LONG nIndex);
		///\brief функция выполняет захват фокуса мыши
		///\return предыдущее окно, имеющее фокус
		HWND SetCapture();
/*
		///\brief свойство установки/получения текста
		ULProperty::CULProperty<ULOther::CULStr,CULWnd,
			&CULWnd::SetText,
			&CULWnd::GetText>	propText;
*/
		///\brief свойство установки/получения стилья
		ULProperty::CULProperty<DWORD,CULWnd,
			&CULWnd::SetStyle,
			&CULWnd::GetStyle> propStyle;
		///\brief свойство установки/получения расширенного стиля
		ULProperty::CULProperty<DWORD,CULWnd,
			&CULWnd::SetStyleEx,
			&CULWnd::GetStyleEx> propStyleEx;
		///\brief свойство установки/получения левой  координаты окна
		ULProperty::CULProperty<int,CULWnd,
			&CULWnd::SetLeft,
			&CULWnd::GetLeft> propLeft;
		///\brief свойство установки/получения верхней координаты окна
		ULProperty::CULProperty<int,CULWnd,
			&CULWnd::SetTop,
			&CULWnd::GetTop> propTop;
		///\brief свойство установки/получения ширины окна
		ULProperty::CULProperty<DWORD,CULWnd,
			&CULWnd::SetWidth,
			&CULWnd::GetWidth> propWidth;
		///\brief свойство установки/получения высоты окна
		ULProperty::CULProperty<DWORD,CULWnd,
			&CULWnd::SetHeight,
			&CULWnd::GetHeight> propHeight;
		///\brief свойство установки/получения видимости окна
		ULProperty::CULProperty<BOOL,CULWnd,
			&CULWnd::SetVisible,
			&CULWnd::GetVisible> propVisible;
		///\brief свойство установки/получения включенности окна
		ULProperty::CULProperty<BOOL,CULWnd,
			&CULWnd::SetEnable,
			&CULWnd::GetEnable> propEnable;
	};
	///\brief функция для получения иконки указанного окна
	///\param hWnd - хендл окна
	///\return хендл иконки
	HICON GetWindowIcon(HWND hWnd);
}
#endif //__ULWND_H__

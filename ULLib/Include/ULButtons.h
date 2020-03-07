///\file ULButtons.h
///\brief Заголовочный файл пространства имен классов кнопок(15.10.2007)
#pragma once
#ifndef __BUTTONS__H_
#define __BUTTONS__H_
#include "ULSubClass.h"
#include "ULGDI.h"
#pragma comment(lib,"Msimg32.lib")

namespace ULWnds
{
	namespace ULControls
	{
		///\namespace ULButtons
		///\brief Пространство имен классов кнопок(15.10.2007)
		namespace ULButtons
		{
			///\class CULButton
			///\brief Класс кнопки(08.09.2007)
			class CULButton:public CULSubClass
			{
			public:
				///\brief Состояния кнопки
				enum enCheckState
				{
					//для GetCheck и GetState
					csChecked		=BST_CHECKED,
					csIndeterminate	=BST_INDETERMINATE,
					csUnchecked		=BST_UNCHECKED,
					//для GetState
					csFocus			=BST_FOCUS,
					csPushed		=BST_PUSHED,
					csXBZ			=0x0003
				};
				enum enImageType
				{
					itBitmap	=IMAGE_BITMAP,
					itIcon		=IMAGE_ICON
				};
			public:
				CULButton();
				virtual ~CULButton();
				///\brief Функция для создания кнопки
				///\param szCaption - надпись на кнопке
				///\param x,y - координата кнопки
				///\param cx,cy - размер кнопки
				///\param hParentWnd - хендл родительского окна
				///\param uID - ID кнопки
				///\param dwStyle- стиль
				///\return TRUE в случае успеха
				BOOL Create(HWND hParentWnd,LPCTSTR szCaption,int x,int y,int cx,int cy,
					UINT uID,DWORD dwStyle=WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|
					BS_PUSHBUTTON|WS_TABSTOP);
			//==================inline function============================
				///\brief Кликает кнопку
				inline void Click()
					{SendMessage(BM_CLICK);}
				///\brief Возвращает положение кнопки
				///\return положение типа enCheckState
				inline enCheckState GetCheck()
					{return (enCheckState)SendMessage(BM_GETCHECK);}
				///\brief Устанавливает положение кнопки
				///\param cs - положение типа enCheckState
				inline void SetCheck(enCheckState cs)
					{SendMessage(BM_SETCHECK,(WPARAM)cs);}
				///\brief Возвращает сотояние кнопки
				///\return состояние типа enCheckState
				inline enCheckState GetState()
					{return (enCheckState)SendMessage(BM_GETSTATE);}
				///\brief Устанавливает состояние подсветки кнопки
				///\param fHighLite - состояние подсветки
				inline void SetState(BOOL fHighLite)
					{SendMessage(BM_SETSTATE,(WPARAM)fHighLite);}
				///\brief Возвращает картинку с кнопки
				///\param it - тип картинки enImageType
				///\return Хендл на картинку, если нет, то NULL
				inline HANDLE GetImage(enImageType it)
					{return (HANDLE)SendMessage(BM_GETIMAGE,(WPARAM)it);}
				///\brief Устанавливает картинку на кнопку
				///\param it - тип картинки enImageType
				///\param hImage - Хендл картинки
				///\return Хендл на предыдущую картинку, если нет, то NULL
				HANDLE SetImage(enImageType it,HANDLE hImage);
				///\brief Устанавливает стиль кнопки
				///\param dwStyle - стиль
				///\param fRedraw - Флаг перерисовки
				inline void SetStyle(DWORD dwStyle,BOOL fRedraw)
					{SendMessage(BM_SETSTYLE,(WPARAM)dwStyle,(LPARAM)fRedraw);}
			};
			///\class CULCustomButton
			///\brief Класс родитель для CULStyleButton и CULBitmapButton
			///(не использовать для чекбокса и радиобатона)(15.10.2007)
			class CULCustomButton:public CULButton
			{
			protected:
				///\brief структура, содержащая контексты для двойной буферизации
				///\param Up - для обычного состояния кнопки
				///\param Down - для нажатой кнопки
				///\param Focused - для фокусированной кнопки
				///\param Disable - для отключенной кнопки
				struct tagMemDC
				{
					ULGDI::ULDC::CULDC Up;
					ULGDI::ULDC::CULDC Down;
					ULGDI::ULDC::CULDC Focused;
					ULGDI::ULDC::CULDC HiLite;
					ULGDI::ULDC::CULDC Disable;
				}m_MemDC;
				///\brief Перечеслимый тип состояний кнопки
				///\param ctUp - для обычного состояния кнопки
				///\param ctDown - для нажатой кнопки
				///\param ctFocused - для фокусированной кнопки
				///\param ctDisable - для отключенной кнопки
				///\param ctHiLite - для подсвеченной кнопки при наведении мыши
				enum enCurrentState
				{
					ctUp			=ODS_DEFAULT,
					ctDown			=ODS_SELECTED,
					ctFocused		=ODS_FOCUS,
					ctDisable		=ODS_DISABLED,
#if(WINVER >= 0x0500)
					ctHiLite		=ODS_HOTLIGHT
#endif
				};
				///\brief Переменная хранящая в себе текущее состояние кнопки
				WORD m_wCurState;
			public:
				///\brief Конструктор
				CULCustomButton();
				///\brief Деструктор
				virtual ~CULCustomButton();
			protected:
				///\brief Функция для инициализации кнопки
				virtual void Init()=0;
				///\brief Обработчик WM_DRAWITEM(для отрисовки кнопки)
				virtual LRESULT OnDrawItem(WPARAM wParam,LPARAM);
				///\brief Обработчик WM_MOUSEMOVE(изменение состояния на освещенную)
				virtual LRESULT OnMouseMove(WPARAM Type,LPARAM CurPos);
				///\brief Обработчик WM_MOUSELEAVE(изменение состояния на неосвещенную)
				virtual LRESULT OnMouseLeave(WPARAM,LPARAM);
				///\brief Обработчик WM_SETTEXT(для обновления стилей)
				virtual LRESULT OnSetText(WPARAM,LPARAM);		
			};
			///\class CULStyleButton
			///\brief Класс стильной кнопки(15.10.2007)
			///(не использовать для чекбокса и радиобатона)
			class CULStyleButton:public CULCustomButton
			{
			public:
				///\brief Структура цветов кнопки
				///\param bcDefault - цвет рамки в отпущенной кнопке
				///\param bcHiLiteText - цвет шрифта в фокусированной, подсвеченой и нажатой кнопке
				///\param bcDefaultText - цвет шрифта в отпущенной кнопке
				///\param bcFocused - цвет шрифта и рамки фокусированной кнопки
				///\param bcDisable - цвет отключенной кнопки 
				///\param bcGradientColors - цветов градиента
				///\param bcGradientDest - направление градиента
				struct tagButtonColors
				{
					COLORREF bcDefault;
					COLORREF bcFocused;
					COLORREF bcHiLiteText;
					COLORREF bcDefaultText;
					COLORREF bcDisable;
					///\brief Структура цветов при градиенте кнопки
					///\param gcUpFirst,gcUpSecond - начальный,конечный цвет кнопки	в обычном состоянии
					///\param gcDownFirst,gcDownSecond - начальный,конечный цвет нажатой кнопки
					struct tagGradientColors
					{
						COLORREF gcUpFirst;
						COLORREF gcUpSecond;
						COLORREF gcDownFirst;
						COLORREF gcDownSecond;
					}bcGradientColors;
					///\brief Перечисление направлений градиента
					///\param gdUpDown - сверху вниз
					///\param gdLeftRight - слева направо
					enum enGradients
					{
						gdUpDown		=GRADIENT_FILL_RECT_V,
						gdLeftRight		=GRADIENT_FILL_RECT_H
					};
					enGradients bcGradientDest;
				};
			protected:
				///\brief Цвета кнопки
				tagButtonColors m_ButtonColors;
				///\brief Шрифт надписи на кнопке
				ULGDI::ULGObjs::CULFont m_Font;
				///\brief Радиус скругления углов кнопки
				int m_nRadius;
			public:
				///\brief Конструктор
				CULStyleButton();
				///\brief Деструктор
				virtual ~CULStyleButton();
				///\brief Функция для создания кнопки
				///\param hParentWnd - хендл родительского окна
				///\param szCaption - надпись на кнопке
				///\param uID - ID кнопки
				///\param x,y - координата кнопки
				///\param cx,cy - размер кнопки
				///\param hFont - шрифт надписи на кнопке
				///\return TRUE в случае успеха
				BOOL CreateStyleButton(HWND hParentWnd,TCHAR* szCaption,UINT uID,int x,int y,
					int cx,int cy,HFONT hFont=NULL);
				///\brief Функция для стильной кнопки из уже созданной
				///\param hWnd - созданной кнопки
				///\param hFont - шрифт надписи на кнопке
				///\return TRUE в случае успеха
				virtual BOOL Attach(HWND hWnd,HFONT hFont=NULL);
				///\brief Функция установки цветов стильной кнопки 
				void SetButtonColors(tagButtonColors ButtonColors);
				///\brief Функция для получения цветов стильной кнопки 
				void GetButtonColors(tagButtonColors* ButtonColors);
				///\brief Функция возвращает радиус изгиба угла стильной кнопки 
				///\return радиус изгиба угла стильной кнопки 
				int GetRadius();
				///\brief Функция для установки радиуса изгиба угла стильной кнопки 
				void SetRadius(int nRadius);
			protected:
				///\brief Функция производит инициализацию стильной кнопки 
				virtual void Init();
			};
			///\class CULBitmapButton
			///\brief Класс bitmap кнопки(15.10.2007)
			///(не использовать для чекбокса и радиобатона)
			class CULBitmapButton:public CULCustomButton
			{
			protected:
				///\brief Функция для инициализации контекстов и региона
				virtual void Init();
			protected:
				///\brief Структура для хранения хендлов картинок кнопки
				struct tagButtonBitmaps
				{
					HBITMAP bbUp;
					HBITMAP bbDown;
					HBITMAP bbFocused;
					HBITMAP bbHiLite;
					HBITMAP bbDisable;
					HBITMAP bbMask;
				}m_ButtonBitmaps;
			public:
				///\brief Конструктор
				CULBitmapButton();
				///\brief Деструктор
				virtual ~CULBitmapButton();
				///\brief Функция для создания кнопки
				///\param hParentWnd - хендл родительского окна
				///\param uID - ID кнопки
				///\param x,y - координата кнопки
				///\param wUpID - ID картинки кнопки в обычном состоянии
				///\param wDownID - ID картинки нажатой кнопки 
				///\param wFocusedID - ID картинки фокусированной кнопки 
				///\param wHiLiteID - ID картинки подсвеченой кнопки 
				///\param wDisableID - ID картинки отключенной кнопки 
				///\param wMaskID - ID картинки маски для региона кнопки 
				///\return TRUE в случае успеха
				BOOL CreateBitmapButton(HWND hParentWnd,int x,int y,UINT uID,
					WORD wUpID,WORD wDownID,WORD wFocusedID,WORD wHiLiteID,WORD wDisableID,WORD wMaskID);
				///\brief Функция для создания кнопки
				///\param hParentWnd - хендл родительского окна
				///\param uID - ID кнопки
				///\param x,y - координата кнопки
				///\param hUp - хендл картинки кнопки в обычном состоянии
				///\param hDown - хендл картинки нажатой кнопки 
				///\param hFocused - хендл картинки фокусированной кнопки 
				///\param hHiLite - хендл картинки подсвеченой кнопки 
				///\param hDisable - хендл картинки отключенной кнопки 
				///\param hMask - хендл картинки маски для региона кнопки 
				///\return TRUE в случае успеха
				BOOL CreateBitmapButton(HWND hParentWnd,int x,int y,UINT uID,
					HBITMAP hUp,HBITMAP hDown,HBITMAP hFocused,HBITMAP hHiLite,HBITMAP hDisable,HBITMAP hMask);
				///\brief Функция для создания bitmap кнопки на основе уже созданной
				///\param hWnd - хендл кнопки
				///\param wUpID - ID картинки кнопки в обычном состоянии
				///\param wDownID - ID картинки нажатой кнопки 
				///\param wFocusedID - ID картинки фокусированной кнопки 
				///\param wHiLiteID - ID картинки подсвеченой кнопки 
				///\param wDisableID - ID картинки отключенной кнопки 
				///\param wMaskID - ID картинки маски для региона кнопки 
				///\return TRUE в случае успеха
				virtual BOOL Attach(HWND hWnd,
					WORD wUpID,WORD wDownID,WORD wFocusedID,WORD wHiLiteID,WORD wDisableID,WORD wMaskID);
				///\brief Функция для создания bitmap кнопки на основе уже созданной
				///\param hWnd - хендл кнопки
				///\param hUp - хендл картинки кнопки в обычном состоянии
				///\param hDown - хендл картинки нажатой кнопки 
				///\param hFocused - хендл картинки фокусированной кнопки 
				///\param hHiLite - хендл картинки подсвеченой кнопки 
				///\param hDisable - хендл картинки отключенной кнопки 
				///\param hMask - хендл картинки маски для региона кнопки 
				///\return TRUE в случае успеха
				virtual BOOL Attach(HWND hWnd,
					HBITMAP hUp,HBITMAP hDown,HBITMAP hFocused,HBITMAP hHiLite,HBITMAP hDisable,HBITMAP hMask);
			protected:
				///\brief перегрузка, чтоб повторно не вызвать Init()
				virtual LRESULT OnSetText(WPARAM,LPARAM);		
			};
			///\class CULHyperLink
			///\brief Класс гиперссыка(17.07.2007)
			class CULHyperLink:public CULSubClass
			{
			public:
				///\brief Виды состояний кнопки
				///\param tDefault по умолчанию
				///\param tHiLite при наведении мышкой
				///\param tVisited посещенная
				///\param tDown при нажатии мышкой
				enum enType
				{
					tDefault	=0,
					tHiLite		=1,
					tVisited	=2,
					tDown		=3
				};
				///\brief Колличество типов
				static const int constTypeNum=4;
				///\brief Стили шрифта
				///\param sNone - обычный
				///\param sItalic - наклонный
				///\param sUnderLine - подчеркнутый
				///\param sBold - жирный
				enum enStyles
				{
					sNone		=0,
					sItalic		=0x01000000,
					sUnderLine	=0x02000000,
					sBold		=0x04000000
				};
				///\brief Текущий шрифт
				ULGDI::ULGObjs::CULFont m_Font;
			protected:
				///\brief Флаг отвечающий за нажитие ссылки
				bool m_fClicking;
				///\brief Флаг отвечающий за посещение ссылки
				bool m_fVisited;
				///\brief Номер текущего типа
				BYTE m_nCurType;
				///\brief Массив типов
				DWORD m_pdwTypes[constTypeNum];
				///\brief Адресс ссылки
				ULOther::CULStr m_strURL;
			protected:
				///\brief Функция выполняет инциализацию ссылки
				///\return TRUE в случае успеха
				BOOL Init();
				///\brief Функция изменяет размер статика под размер надписи
				///\param hDC хендл контекста
				void AutoSize(HDC hDC);
			public:
				///\brief Конструктор
				CULHyperLink();
				///\brief Деструктор
				virtual ~CULHyperLink();
				///\brief Функция для создания гиперссылки.
				///	Если szURL равен NULL, то при нажатии на ссылку 
				///	будет посылаться родителю WM_COMMAND с указанием
				///	BM_CLICKED,ID и HWND контрола.
				///\param hParentWnd - хендл родительского окна
				///\param x,y - координата кнопки
				///\param szName - отображаемое имя
				///\param szURL - URL адресс
				///\param uID - ID кнопки
				///\return TRUE в случае успеха
				BOOL CreateHyperLink(HWND hParentWnd,int x,int y,LPCTSTR szName,LPCTSTR szURL,UINT uID);
				///\brief Функция для создания гиперссылки на основе уже созданного статика
				///	Если szURL равен NULL, то при нажатии на ссылку 
				///	будет посылаться родителю WM_COMMAND с указанием
				///	BM_CLICKED,ID и HWND контрола.
				///\param hWnd - хендл статика
				///\param szURL - URL адресс
				///\return TRUE в случае успеха
				BOOL Attach(HWND hWnd,LPCTSTR szURL);
				///\brief Функция для определения новых стилей и цветов
				BOOL SetStyles(enType Type,enStyles Styles,COLORREF Color);
				///\brief Функция для установки шрифта ссылки
				void SetFont(HFONT hFont);
			protected:
				///\brief Обработчик WM_DRAWITEM
				virtual LRESULT OnDrawItem(WPARAM,LPARAM);
				///\brief Обработчик WM_LBUTTONDOWN
				virtual LRESULT OnLButtonDown(WPARAM,LPARAM);
				///\brief Обработчик WM_LBUTTONUP
				virtual LRESULT OnLButtonUp(WPARAM,LPARAM);
				///\brief Обработчик WM_MOUSEMOVE
				virtual LRESULT OnMouseMove(WPARAM,LPARAM);
				///\brief Обработчик WM_MOUSELEAVE
				virtual LRESULT OnMouseLeave(WPARAM,LPARAM);
			};
		}
	}
}
#endif //__BUTTONS__H_

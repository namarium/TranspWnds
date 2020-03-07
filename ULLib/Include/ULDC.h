///\file ULDC.h
///\brief Заголовочный файл классов контекстов(08.10.2007)
#include <windows.h>
#pragma once
#ifndef __ULDC__H_
#define __ULDC__H_
namespace ULGDI
{
	///\namespace ULGDI::ULDC
	///\brief пространство имён классов контекстов(08.10.2007)
	namespace ULDC
	{
		///\class CULDC
		///\brief Класс контекста
		class CULDC
		{
		public:
			///\brief контекст
			HDC m_hDC;
			///\brief окно владелец
			HWND m_hWndOwner;
			///\brief режим фона
			///\param bmOPAQUE - заливка будет производиться текущий BkColor(SetBkColor)
			///\param bmTRANSPARENT - прозрачный фон
			enum enBkMode
			{
				bmOPAQUE		=OPAQUE,
				bmTRANSPARENT	=TRANSPARENT	
			};
		public:
			///\brief Конструктор
			CULDC():m_hDC(NULL),m_hWndOwner(NULL){};
			///\brief Конструктор копирования
			CULDC(HDC& hDC):m_hDC(hDC),m_hWndOwner(NULL){}
			///\brief Конструктор копирования
			CULDC(CULDC& DC):m_hDC(DC),m_hWndOwner(NULL){}
			///\brief Деструктор
			virtual ~CULDC(){DeleteDC();}
			///\brief Сохраняетвыбранную область контекста в память
			///\param pszFile - путь к фаилу
			///\param rcSize - область подлежащая заиси в фаил
			///\param dwComp - тип компрессии (BI_RGB,BI_RLE8,BI_RLE4,BI_BITFIELDS,BI_JPEG,BI_PNG)
			///\param wBitCount - битность сохраняемого изображения
			///\return TRUE в случае успеха, иначе FALSE
			BOOL ULGDI::ULDC::CULDC::CreateBMPFile(LPCTSTR pszFile, RECT rcSize, DWORD dwComp, WORD wBitCount);
		//==================inline functions============================
			///\brief Функция создания контекста(никогда не пользовался :D)
			inline BOOL CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice,
				LPCTSTR lpszOutput,CONST DEVMODE* lpInitData)
				{return ((m_hDC=::CreateDC(lpszDriver,lpszDevice,lpszOutput,lpInitData))!=NULL);}
			///\brief Создает совместимый контекст на основе входнрого
			///\param hDC - входной контекст
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateCompatibleDC(HDC hDC)
				{return ((m_hDC=::CreateCompatibleDC(hDC))!=NULL);}
			///\brief Для подстановки объекта класса там где требуется 
			///	только хендл DC
			inline operator HDC() const{return m_hDC;};
			inline BOOL BitBlt(int nXDest,int nYDest,int nWidth,int nHeight,
				HDC hdcSrc,int nXSrc,int nYSrc,DWORD dwRop)
			{return ::BitBlt(*this,nXDest,nYDest,nWidth,nHeight,
				hdcSrc,nXSrc,nYSrc,dwRop);};
			///\brief устанавливает цвет текста
			///\param crColor - цвет
			///\return предыдущий цвет
			inline COLORREF SetTextColor(COLORREF crColor)
				{return ::SetTextColor(*this,crColor);}
			///\brief Возвращает цвет текста
			///\return цвет
			inline COLORREF GetTextColor()
				{return ::GetTextColor(*this);}			 
			///\brief устанавливает цвет фона текста
			///\param crColor - цвет
			///\return предыдущий цвет
			inline COLORREF SetBkColor(COLORREF crColor)
				{return ::SetBkColor(*this,crColor);};
			///\brief устанавливает режим отрисовки текста
			///\param bm - режим
			///\return предыдущий цвет
			inline int SetBkMode(enBkMode bm)
				{return ::SetBkMode(*this,bm);};
			///\brief отрисовывает текст
			///\param lpString - текст
			///\param nCount - длина текста
			///\param lpRect - прямоугольник в котором отрисовывать
			///\param uFormat - формат отрисовки(по МСДН)
			///\return в случае успеза вернёт высоту текста в логическихединицах, иначе 0
			inline int DrawText(LPCTSTR lpString,int nCount,LPRECT lpRect,UINT uFormat)
				{return ::DrawText(*this,lpString,nCount,lpRect,uFormat);};
			///\brief для получения размера текста в контексте при заданном шрифте
			///\param lpString - строка
			///\param cbString - длина строки
			///\param lpSize - размер текста
			///\return TRUE в случае успеха
			inline BOOL GetTextExtentPoint(LPCTSTR lpString,int cbString,LPSIZE lpSize)
				{return ::GetTextExtentPoint(*this,lpString,cbString,lpSize);};
			///\brief выбирает инструмент рисования в контекст
			///\param hgdiobj - инструмент рисования
			///\return предыдущий инструмент рисования
			inline HGDIOBJ SelectObject(HGDIOBJ hgdiobj)
				{return ::SelectObject(*this,hgdiobj);};
			///\brief удаляет контекст
			///\return TRUE в случае успеха
			inline BOOL DeleteDC()
				{if(m_hDC!=NULL)return ::DeleteDC(*this);else return FALSE;}
			///\brief отсоединяет контекст от класса
			///\return контекст
			inline HDC Detach()
				{HDC hRetDC=m_hDC;m_hDC=NULL;return hRetDC;}
			///\brief Рисует прямоугольник
			///\param nLeftRect,nTopRect,nRightRect,nBottomRect - координаты
			///\return TRUE в случае успеха
			inline BOOL Rectangle(int nLeftRect,int nTopRect,int nRightRect,int nBottomRect)
				{return ::Rectangle(*this,nLeftRect,nTopRect,nRightRect,nBottomRect);}
			///\brief Рисует текст
			///\param nXStart,nYStart - координаты
			///\param nXStart,nYStart - координаты
			///\param lpString - строка
			///\param cbString - длина строки
			///\return TRUE в случае успеха
			inline BOOL TextOut(int nXStart,int nYStart,LPCTSTR lpString,int cbString)
				{return ::TextOut(*this,nXStart,nYStart,lpString,cbString);}
			///\brief Рисует контур по заданному региону
			///\param hrgn - регион
			///\param hbr - кисть, которой будет выполнена отрисовка
			///\param nWidth,nHeight - размеры региона
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL FrameRgn(HRGN hrgn,HBRUSH hbr,int nWidth,int nHeight)
				{return ::FrameRgn(*this,hrgn,hbr,nWidth,nHeight);};
			///\brief выполняет градиентную закраску заданной области
			///\param pVertex - массив TRIVERTEX структур
			///\param dwNumVertex - число TRIVERTEX структур в массиве pVertex
			///\param pMesh - массив GRADIENT_TRIANGLE структур для режима треугольника
			///	или массив GRADIENT_RECT структур для режима прямоугольника
			///\param dwNumMesh - число элементов в pMesh
			///\param dwMode - режим
			///	GRADIENT_FILL_RECT_H горизонтальный градиент
			///	GRADIENT_FILL_RECT_V вертикальный градиент
			///	GRADIENT_FILL_TRIANGLE градиент по треугольнку(не пробовал)
			inline BOOL GradientFill(PTRIVERTEX pVertex,ULONG dwNumVertex,PVOID pMesh,ULONG dwNumMesh,ULONG dwMode)
				{return ::GradientFill(*this,pVertex,dwNumVertex,pMesh,dwNumMesh,dwMode);}; 
			///\brief возвращает информацию о контексте
			///\param index - ID поля
			///\return значение
			virtual int GetDeviceCaps(int index)
				{return ::GetDeviceCaps(*this,index);}
			///\brief Возвращает выбранный графический объект
			///\param type - тип объекта
			///\return графический объект в слечае удачи, иначе NULL
			inline HGDIOBJ GetCurrentObject(UINT type)
				{return ::GetCurrentObject(*this,type);}
			///\brief рисует линию начиная с текущей точки
			///\param nXEnd, nYEnd - координаты конечной точки
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL LineTo(int nXEnd,int nYEnd)
				{return ::LineTo(*this,nXEnd,nYEnd);}
			///\brief устанавливает значение текущей точки
			///\param X, Y - Координата новой устанавливаемой точки
			///\param lpPoint - Координата предыдущей точки
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL MoveTo(int X,int Y,LPPOINT lpPoint=NULL)
				{return ::MoveToEx(*this,X,Y,lpPoint);}
			///\brief Копирует биты в заданый буфер, преобразуя их определенным образом
			///\param hbmp - хендл на битмап
			///\param uStartScan - номер первой линии для копирования
			///\param cScanLines - число копируемых линий
			///\param lpvBits - выходной буфер для скопированных данных
			///\param lpbi - информация о битмапе
			///\param uUsage -цветовой формат(DIB_PAL_COLORS,DIB_RGB_COLORS)
			///\return число скопированных линий в буфер
			inline int GetDIBits(HBITMAP hbmp, UINT uStartScan,UINT cScanLines,LPVOID lpvBits,LPBITMAPINFO lpbi,UINT uUsage)
				{return ::GetDIBits(*this,hbmp, uStartScan,cScanLines,lpvBits,lpbi,uUsage);	}
			///\brief ресует закрашеный прямоугольник
			///\param lprc - указатель на структуру прямоугольника
			///\param hbr - хендл кисти, которой будет осуществлено закрашивание
			///\return TRUE в случае успеха
			inline int FillRect(CONST RECT *lprc,HBRUSH hbr)
				{return (0!=::FillRect(*this,lprc,hbr));};
			///\brief ресует закрашеный прямоугольник
			///\param hrgn - хендл закрашиваемого региона
			///\param hbr - хендл кисти, которой будет осуществлено закрашивание
			///\return TRUE в случае успеха
			inline int FillRgn(HRGN hrgn,HBRUSH hbr)
				{return (0!=::FillRgn(*this,hrgn,hbr));};
			///\brief функция сохраняет текущее состояние контекста
			///\return TRUE в случае успеха
			BOOL SaveDC();
			///\brief функция востанавливает сохранённое состояние контекста
			///\param nSavedDC - состояние для востановления
			///\return TRUE в случае успеха
			BOOL RestoreDC(int nSavedDC);
			///\brief функция рисует полигон
			///\param lpPoints - указатель на массив точек полигона
			///\param nCount - число точек в массиве
			///\return TRUE в случае успеха
			BOOL Polygon(CONST POINT *lpPoints,int nCount);
			///\brief функция для получения цвета по указанной координате
			///\param nXPos,nYPos - координата
			///\return цвет в случае успеха, иначе CLR_INVALID
			COLORREF GetPixel(int nXPos,int nYPos);
			///\brief функция для установке цвета по указанной координате
			///\param nXPos,nYPos - координата
			///\param crColor - устанавливаемый цвет 
			///\return TRUE в случае успеха
			BOOL SetPixel(int nXPos,int nYPos,COLORREF crColor);
			///\brief функция рисует иконку
			///\param X,Y - координаты верхнего левого угла иконки
			///\param hIcon - хендл иконки
			///\return TRUE в случае успеха
			BOOL DrawIcon(int X,int Y,HICON hIcon);
			///\brief функция рисует иконку
			///\param xLeft,yTop - координаты верхнего левого угла иконки
			///\param hIcon - хендл иконки
			///\param cxWidth,cyWidth - размеры иконки
			///\param istepIfAniCur - индекс фрейма, если иконка - анимированный курсор
			///\param hbrFlickerFreeDraw - хендл на кисть, может быть NULL
			///\param diFlags - флаг отрисовки
			///\return TRUE в случае успеха
			BOOL DrawIconEx(int xLeft,int yTop,HICON hIcon,int cxWidth,int cyWidth,
				UINT istepIfAniCur,HBRUSH hbrFlickerFreeDraw,UINT diFlags);
			///\brief функция устанавливает режим комбинирования \n
			/// рисоввания кистью или карандашом с текущим уже \n
			/// нарисованным изображением
			///\param fnDrawMode режим комбинирования
			///\return в случае успеха вернет предыдущий режим комбинирования, \n
			/// в случае неудачи вернёт 0
			int SetROP2(int fnDrawMode);
	};
		///\class CULWindowDC
		///\brief Класс контекста окна(10.09.2007)
		class CULWindowDC:public CULDC
		{
		public:
			///\Конструктор
			inline explicit CULWindowDC(HWND hWndOwner):CULDC()
			{
					m_hWndOwner=hWndOwner;
					m_hDC=::GetWindowDC(hWndOwner);
			};
			///\brief деструктор
			~CULWindowDC(){if(m_hDC!=NULL)::ReleaseDC(m_hWndOwner,*this);m_hDC=NULL;};
		};
		///\class CULClientDC
		///\brief Класс контекста клиентской области окна(10.09.2007)
		class CULClientDC:public CULDC
		{
		public:
			///\Конструктор
			inline explicit CULClientDC(HWND hWndOwner):CULDC()
				{
					m_hWndOwner=hWndOwner;
					m_hDC=::GetDC(hWndOwner);
				};
			///\brief деструктор
			~CULClientDC(){if(m_hDC!=NULL)::ReleaseDC(m_hWndOwner,*this);m_hDC=NULL;};
		};
		///\class CULPaintDC
		///\brief Класс контекста клиентской области окна(10.09.2007)
		///применяется только в обработчиках WM_PAINT
		class CULPaintDC:public CULDC
		{
		public:
			PAINTSTRUCT m_PaintStruct;
		public:
			///\Конструктор
			inline explicit CULPaintDC(HWND hWndOwner):CULDC()
				{	
					m_hWndOwner=hWndOwner;
					m_hDC=::BeginPaint(hWndOwner,&m_PaintStruct);
				};
			///\brief деструктор
			~CULPaintDC(){::EndPaint(m_hWndOwner,&m_PaintStruct);m_hDC=NULL;};
		};
	}
}
#endif//__ULDC__H_

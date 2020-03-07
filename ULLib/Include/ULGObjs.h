///\file ULGObjs.h
///\brief Заголовочный файл классов графических объектов(15.10.2007)
#include <windows.h>
#include <commctrl.h>
#pragma once
#ifndef __ULGOBJS__H_
#define __ULGOBJS__H_
namespace ULGDI
{
	///\namespace ULGDI::ULGObjs
	///\brief пространство имён классов графических объектов(15.10.2007)
	namespace ULGObjs
	{
		///\class CULBitmap
		///\brief Класс битового изображения(15.10.2007)
		class CULBitmap
		{
		public:
			///\brief Хэндл битового изображения
			HBITMAP m_hBitmap;
		public:
			///\brief Конструктор
			CULBitmap():m_hBitmap(NULL){}
			///\brief Конструктор копирования
			CULBitmap(HBITMAP hBitmap):m_hBitmap(hBitmap){}
			///\brief Деструктор
			virtual ~CULBitmap(){DeleteBitmap();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HBITMAP
			inline operator HBITMAP() const{return m_hBitmap;}
			///\brief Создаёт битовое изображение
			///\param nWidth, nHeight - размер изображения
			///\param cPlanes - число цветовых уровней
			///\param cBitsPerPel - число бит на точку
			///\param lpvBits - битовый массив
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateBitmap(int nWidth,int nHeight,UINT cPlanes,UINT cBitsPerPel,CONST VOID *lpvBits);
			///\brief Создаёт совместимый с контекстом битовый рисунок
			///\param hDC - контекст
			///\param nWidth,nHeight - размер создаваемого рисунка
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateCompatibleBitmap(HDC hDC,int nWidth,int nHeight);
			///\brief создаёт битовый рисункой с индивидуальной цветовой картой
			///\param hInstance - инстанс модуля, в котором лежит ресурс
			///\param idBitmap - идентификатор ресурса
			///\param wFlags - либо 0 либо CMB_MASKED, чтоб была взята маска битмапа(вроде)
			///\param lpColorMap - массив цветов, которые/на которые надо заменить
			///\param iNumMaps - число элементов в массиве
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateMappedBitmap(HINSTANCE hInstance,INT_PTR idBitmap,UINT wFlags,
				LPCOLORMAP lpColorMap,int iNumMaps);
			///brief Загружает битовый рисунок из ресурса или из фаила
			///\param hInstance - Хендл модуля, из которого загружается битовый рисунок 
			///\param lpBitmapName - Имя фаила или ресурса
			///\return TRUE в случае успеха, иначе FALSE
			BOOL LoadBitmap(HINSTANCE hInstance,LPCTSTR lpBitmapName);
			///brief Загружает битовый рисунок из ресурса
			///\param hInstance - Хендл модуля, из которого загружается битовый рисунок 
			///\param nIDRes - ID ресурса
			///\return TRUE в случае успеха, иначе FALSE
			BOOL LoadBitmap(HINSTANCE hInstance,int nIDRes);
			///\brief Возвращает структуру битового избражения
			///\param pBmp - указатель на структуру битового избражения
			///\return колличество байт скопированных в буфер
			///	если pBmp NULL то вернет требуемое коллчиство байт
			int GetBitmap(BITMAP* pBmp);
			///\brief см. GetBitmap
			int GetBitmap(DIBSECTION* pDib);
			///\brief Возвращает битовый массив изображения
			///\param cbBuffer - длина буфера
			///\param lpvBits - Битовый массив
			///\return в случае успеха число скопированных байт, иначе 0
			LONG GetBitmapBits(LONG cbBuffer,LPVOID lpvBits);
			///\brief Удаляет битовое изображение
			///\return TRUE в случае успеха, иначе FALSE
			BOOL DeleteBitmap();
			///\brief отсоединяет битовое изображение от класса
			///\return хендл на битовое изображение 
			HBITMAP Detach();
		};
		///\class CULRgn
		///\brief Класс региона(15.10.2007)
		class CULRgn
		{
		public:
			///\brief Хэндл региона
			HRGN m_hRgn;
		public:
			///\brief Конструктор
			CULRgn():m_hRgn(NULL){};
			///\brief Конструктор копирования
			CULRgn(HRGN& hRgn):m_hRgn(hRgn){};
			///\brief Деструктор
			virtual ~CULRgn(){DeleteRgn();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HRGN
			inline operator HRGN() const{return m_hRgn;}
			///\brief Создаёт регион из битового изображения,
			///	граница региона определяется цветом пикселя по координате 0,0
			///\param RgnBitmap - Хэндл битового изображения
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateBitmapRgn(CULBitmap RgnBitmap);
			///\brief создаёт регион в виде прямоугольника
			///\param nLeftRect,nTopRect,nRightRect,nBottomRect - координаты углов прямоугольника
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateRectRgn(int nLeftRect,int nTopRect,int nRightRect,int nBottomRect);
			///\brief создаёт регион в виде прямоугольника со скруглёнными краями
			///\param nLeftRect,nTopRect,nRightRect,nBottomRect - координаты углов прямоугольника
			///\param nWidthEllipse,nHeightEllipse - размеры элипса
			///\return TRUE в случае успеха, иначе FALSE
			BOOL CreateRoundRectRgn(int nLeftRect,int nTopRect,int nRightRect,int nBottomRect,
				int nWidthEllipse,int nHeightEllipse);
			///\brief Комбинирует два исходных региона и помещает в текущий
			///\param hrgnSrc1,hrgnSrc2 - хендлы исходных регионов
			///\param fnCombineMode - режим комбинирования
			///	RGN_AND		Creates the intersection of the two combined regions. 
			///	RGN_COPY	Creates a copy of the region identified by hrgnSrc1. 
			///	RGN_DIFF	Combines the parts of hrgnSrc1 that are not part of hrgnSrc2. 
			///	RGN_OR		Creates the union of two combined regions. 
			///	RGN_XOR		Creates the union of two combined regions except for any overlapping areas. 
			///\return 
			///	NULLREGION The region is empty. 
			///	SIMPLEREGION The region is a single rectangle. 
			///	COMPLEXREGION The region is more than a single rectangle. 
			///	ERROR No region is created. 
			int CombineRgn(HRGN hrgnSrc1,HRGN hrgnSrc2,int fnCombineMode);
			///\brief Комбинирует текущий регион с hrgnSrc2 (см. CombineRgn)
			///\param hrgnSrc2 - второй регион
			///\param fnCombineMode - режим комбинирования см. CombineRgn
			///\return см. CombineRgn			
			int CombineRgn(HRGN hrgnSrc2,int fnCombineMode);
			///\brief Удаляет регион
			///\return TRUE в случае успеха, иначе FALSE
			BOOL DeleteRgn();
			///\brief отсоединяет регион от класса
			///\return хендл на регион
			HRGN Detach();
		};
		///\class CULPen
		///\brief Класс пера(15.10.2007) 
		class CULPen
		{
		public:
			///\brief Хэндл пера
			HPEN m_hPen;
		public:
			///\brief Конструктор
			CULPen():m_hPen(NULL){};
			///\brief Конструктор копирования
			CULPen(HPEN& hPen):m_hPen(hPen){};
			///\brief Конструктор с созданием карандаша
			CULPen(int fnPenStyle,int nWidth,COLORREF crColor);
			///\brief Деструктор
			virtual ~CULPen(){DeletePen();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HPEN
			inline operator HPEN() const{return m_hPen;}
			///\brief Создаёт перо
			///\param fnPenStyle - стиль пера
			///	PS_SOLID		The pen is solid. 
			///	PS_DASH			The pen is dashed. This style is valid only when the pen width is one or less in device units. 
			///	PS_DOT			The pen is dotted. This style is valid only when the pen width is one or less in device units. 
			///	PS_DASHDOT		The pen has alternating dashes and dots. This style is valid only when the pen width is one or less in device units. 
			///	PS_DASHDOTDOT	The pen has alternating dashes and double dots. This style is valid only when the pen width is one or less in device units. 
			///	PS_NULL			The pen is invisible. 
			///	PS_INSIDEFRAME	The pen is solid. When this pen is used in any GDI drawing function that takes a bounding rectangle, the dimensions of the figure are shrunk so that it fits entirely in the bounding rectangle, taking into account the width of the pen. This applies only to geometric pens. 			
			///\param nWidth - толщина пера
			///\param crColor - цвет пера
			///\return 	TRUE в случае успеха, иначе FALSE
			BOOL CreatePen(int fnPenStyle,int nWidth,COLORREF crColor)
				{return ((m_hPen=::CreatePen(fnPenStyle,nWidth,crColor))!=NULL);}
			///\brief Создаёт перо из структуры LOGPEN
			///\param lplgpn - указатель на переменную структуры LOGPEN
			///\return 	TRUE в случае успеха, иначе FALSE
			BOOL CreatePenIndirect(CONST LOGPEN *lplgpn)
				{return ((m_hPen=::CreatePenIndirect(lplgpn))!=NULL);}
			///\brief Создаёт перо системного типа
			///\param nIndex - индекс системного типа
			///	BLACK_PEN Black pen. 
			///	DC_PEN Windows 2000/XP: Solid pen color. The default color is white. The color can be changed by using the SetDCPenColor function. For more information, see the Remarks section. 
			///	WHITE_PEN White pen. 
			///\return TRUE в случае успеха, иначе FALSE	
			inline BOOL CreateSysPen(int nIndex)
				{return ((m_hPen=(HPEN)::GetStockObject(nIndex))!=NULL);}
			///\brief Возвращает структуру пера
			///\param pLogPen - указатель на структуру пера(LOGPEN)
			///\return колличество байт скопированных в буфер
			///	если pLogPen NULL то вернет требуемое коллчиство байт
			inline int GetLogPen(LOGPEN* pLogPen)
				{return ::GetObject((HPEN)*this,sizeof(LOGPEN),(LPSTR)pLogPen);}
			///\brief Возвращает структуру пера
			///\param pExtLogPen - указатель на структуру пера(EXTLOGPEN)
			///\return колличество байт скопированных в буфер
			///	если pExtLogPen NULL то вернет требуемое коллчиство байт
			inline int GetExtLogPen(EXTLOGPEN* pExtLogPen)
				{return ::GetObject((HPEN)*this,sizeof(EXTLOGPEN),(LPSTR)pExtLogPen);}
			///\brief Удаляет перо
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL DeletePen()
				{if(m_hPen!=NULL)return ::DeleteObject(m_hPen);else return FALSE;}
			///\brief отсоединяет перо от класса
			///\return хендл на перо
			inline HPEN Detach()
				{HPEN hRetPen=m_hPen;m_hPen=NULL;return hRetPen;}
		};
		///\class CULBrush
		///\brief Класс кисти(15.10.2007) 
		class CULBrush
		{
		public:
			///\brief Хэндл пера
			HBRUSH m_hBrush;
		public:
			///\brief Конструктор
			CULBrush():m_hBrush(NULL){};
			///\brief Конструктор копирования
			CULBrush(HBRUSH& hBrush):m_hBrush(hBrush){};
			///\brief Деструктор
			virtual ~CULBrush(){DeleteBrush();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HBRUSH
			inline operator HBRUSH() const{return m_hBrush;}
			///\brief Создаёт кисть
			///\param lplb - содержит информацию о кисти
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateBrushIndirect(CONST LOGBRUSH *lplb)
				{return ((m_hBrush=::CreateBrushIndirect(lplb))!=NULL);}
			///\brief Создаёт кисть из DIB
			///\param hglbDIBPacked - Handle to a global memory object containing a packed DIB,
			///	which consists of a BITMAPINFO structure immediately followed by an array of bytes defining the pixels of the bitmap. 
			///\param fuColorSpec - какие цвета в hglbDIBPacked
			///	DIB_PAL_COLORS A color table is provided and consists of an array of 16-bit 
			///	indexes into the logical palette of the device context into which the brush is to be selected. 
			///	DIB_RGB_COLORS A color table is provided and contains literal RGB values. 
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateDIBPatternBrush(HGLOBAL hglbDIBPacked,UINT fuColorSpec)
				{return ((m_hBrush=::CreateDIBPatternBrush(hglbDIBPacked,fuColorSpec))!=NULL);}
			///\brief Создаёт штрихованную кисть
			///\param fnStyle - стиль
			///	HS_BDIAGONAL 45-degree upward left-to-right hatch 
			///	HS_CROSS Horizontal and vertical crosshatch 
			///	HS_DIAGCROSS 45-degree crosshatch 
			///	HS_FDIAGONAL 45-degree downward left-to-right hatch 
			///	HS_HORIZONTAL Horizontal hatch 
			///	HS_VERTICAL Vertical hatch 
			///\param clrref - цвет
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateHatchBrush(int fnStyle,COLORREF clrref)
				{return ((m_hBrush=::CreateHatchBrush(fnStyle,clrref))!=NULL);}
			///\brief Создаёт кисть из битового изображения
			///\param hbmp - хендл битового изображения
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreatePatternBrush(HBITMAP hbmp)
				{return ((m_hBrush=::CreatePatternBrush(hbmp))!=NULL);}
			///\brief Создаёт сплошную кисть 
			///\param crColor - цвет
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateSolidBrush(COLORREF crColor)
				{return ((m_hBrush=::CreateSolidBrush(crColor))!=NULL);}
			///\brief Создаёт кисть системного цвета
			///\param nIndex - индекс системного цвета
			///	BLACK_BRUSH Black brush. 
			///	DKGRAY_BRUSH Dark gray brush. 
			///	DC_BRUSH Windows 2000/XP: Solid color brush. The default color is white. The color can be changed by using the SetDCBrushColor function. For more information, see the Remarks section. 
			///	GRAY_BRUSH Gray brush. 
			///	HOLLOW_BRUSH Hollow brush (equivalent to NULL_BRUSH). 
			///	LTGRAY_BRUSH Light gray brush. 
			///	NULL_BRUSH Null brush (equivalent to HOLLOW_BRUSH). 
			///	WHITE_BRUSH White brush. 
			///\return TRUE в случае успеха, иначе FALSE	
			inline BOOL CreateSysColorBrush(int nIndex)
				{return ((m_hBrush=::GetSysColorBrush(nIndex))!=NULL);}
			///\brief Возвращает структуру кисти
			///\param pLogBrush - указатель на структуру пера(LOGBRUSH)
			///\return колличество байт скопированных в буфер
			///	если pLogBrush NULL то вернет требуемое коллчиство байт
			inline int GetLogBrush(LOGBRUSH* pLogBrush)
				{return ::GetObject((HBRUSH)*this,sizeof(LOGBRUSH),(LPSTR)pLogBrush);}
			///\brief Удаляет кисть
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL DeleteBrush()
				{if(m_hBrush!=NULL)return ::DeleteObject(m_hBrush);else return FALSE;}
			///\brief отсоединяет кисть от класса
			///\return хендл на кисть
			inline HBRUSH Detach()
				{HBRUSH hRetBrush=m_hBrush;m_hBrush=NULL;return hRetBrush;}
		};
		///\class CULFont
		///\brief Класс шрифта(15.10.2007) 
		class CULFont
		{
		public:
			///\brief Хэндл шрифта
			HFONT m_hFont;
		public:
			///\brief Конструктор
			CULFont():m_hFont(NULL){};
			///\brief Конструктор копирования
			CULFont(HFONT hFont):m_hFont(hFont){};
			///\brief Деструктор
			virtual ~CULFont(){DeleteFont();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HFONT
			inline operator HFONT() const{return m_hFont;}
			///\brief Создаёт шрифт
			///\param nHeight - высота шрифта(пользуемся nHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 96);)
			///\param nWidth - средняя ширина симыола
			///\param nEscapement - angle of escapement
			///\param nOrientation - base-line orientation angle
			///\param fnWeight - вес шрифта(типа тонкий(FW_THIN),толстый(FW_BOLD))
			///\param fdwItalic - флаг наклонности
			///\param fdwUnderline - флаг подчёркивания
			///\param fdwStrikeOut - флаг зачёркивания 
			///\param fdwCharSet - кодировака
			///\param fdwOutputPrecision - output precision
			///\param fdwClipPrecision - clipping precision
			///\param fdwQuality - флаг качества
			///\param fdwPitchAndFamily - pitch and family
			///\param lpszFace - название шрифта
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateFont(int nHeight,int nWidth,int nEscapement,int nOrientation,
				int fnWeight,DWORD fdwItalic,DWORD fdwUnderline,DWORD fdwStrikeOut,
				DWORD fdwCharSet,DWORD fdwOutputPrecision,DWORD fdwClipPrecision,
				DWORD fdwQuality,DWORD fdwPitchAndFamily,LPCTSTR lpszFace)
				{return ((m_hFont=::CreateFont(nHeight,nWidth,nEscapement,nOrientation,
					fnWeight,fdwItalic,fdwUnderline,fdwStrikeOut,fdwCharSet,
					fdwOutputPrecision,fdwClipPrecision,fdwQuality,fdwPitchAndFamily,
					lpszFace))!=NULL);};
			///\brief Создаёт шрифт
			///\param lpLogFont - информация о шрифте
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateFontIndirect(const LOGFONT* lpLogFont)
				{return ((m_hFont=::CreateFontIndirect(lpLogFont))!=NULL);}
			///\brief Создаёт шрифт системного типа
			///\param nIndex - индекс системного типа
			///	ANSI_FIXED_FONT Windows fixed-pitch (monospace) system font. 
			///	ANSI_VAR_FONT Windows variable-pitch (proportional space) system font. 
			///	DEVICE_DEFAULT_FONT Windows NT/2000/XP: Device-dependent font. 
			///	DEFAULT_GUI_FONT Default font for user interface objects such as menus and dialog boxes. This is MS Sans Serif. Compare this with SYSTEM_FONT. 
			///	OEM_FIXED_FONT Original equipment manufacturer (OEM) dependent fixed-pitch (monospace) font. 
			///	SYSTEM_FONT System font. By default, the system uses the system font to draw menus, dialog box controls, and text. 
			///	SYSTEM_FIXED_FONT Fixed-pitch (monospace) system font. This stock object is provided only for compatibility with 16-bit Windows versions earlier than 3.0.  
			///\return TRUE в случае успеха, иначе FALSE	
			inline BOOL CreateSysFont(int nIndex)
				{return ((m_hFont=(HFONT)::GetStockObject(nIndex))!=NULL);}
			///\brief Возвращает структуру шрифта
			///\param pLogFont - указатель на структуру шрифта(LOGFONT)
			///\return колличество байт скопированных в буфер
			///	если pLogFont NULL то вернет требуемое коллчиство байт
			inline int GetLogFont(LOGFONT* pLogFont)
				{return ::GetObject((HFONT)*this,sizeof(LOGFONT),(LPSTR)pLogFont);}
			///\brief Удаляет шрифт
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL DeleteFont()
				{if(m_hFont!=NULL)return ::DeleteObject(m_hFont);else return FALSE;}
			///\brief отсоединяет шрифт от класса
			///\return хендл на шрифт
			inline HFONT Detach()
				{HFONT hRetFont=m_hFont;m_hFont=NULL;return hRetFont;}
		};
		///\class CULPalette
		///\brief Класс палитры(15.10.2007) 
		class CULPalette
		{
		public:
			///\brief Хэндл палитры
			HPALETTE m_hPalette;
		public:
			///\brief Конструктор
			CULPalette():m_hPalette(NULL){};
			///\brief Конструктор копирования
			CULPalette(HPALETTE& hPalette):m_hPalette(hPalette){};
			///\brief Деструктор
			virtual ~CULPalette(){DeletePalette();}
			///\brief Для подстановки объекта класса там где требуется 
			///только HPALETTE
			inline operator HPALETTE() const{return m_hPalette;}
			///\brief Создаёт полутоновую палитру на основе контекста
			///\param hDC - контекст
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreateHalftonePalette(HDC hDC)
				{return ((m_hPalette=::CreateHalftonePalette(hDC))!=NULL);}
			///\brief Создаёт палитру 
			///\param lpLogPalette - параметр содержащий информацию о палитре
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL CreatePalette(LPLOGPALETTE lpLogPalette)
				{return ((m_hPalette=::CreatePalette(lpLogPalette))!=NULL);}
			///\brief Заменяет цвета в палитре
			///\param iStartIndex - стартовый индекс
			///\param cEntries - колличество цветов
			///\param ppe - массив структур PALETTEENTRY
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL AnimatePalette(UINT iStartIndex,UINT cEntries,CONST PALETTEENTRY *ppe)
				{return ::AnimatePalette(*this,iStartIndex,cEntries,ppe);}
			///\brief Возвращает ближайший по цвету индекс палитры
			///\param crColor - цвет 
			///\return индекс палитры
			inline UINT GetNearestPaletteIndex(COLORREF crColor)
				{return ::GetNearestPaletteIndex(*this,crColor);}
			///\brief Изменяет размер палитры
			///\param nEntries - новое колличество цветов
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL ResizePalette(UINT nEntries)
				{return ::ResizePalette(*this,nEntries);}
			///\brief Возвращает массив цветов из палитры
			///\param iStartIndex - стартовый индекс
			///\param nEntries - колличество цветов
			///\param lppe - возвращаемый массив структур PALETTEENTRY
			///\return число скопированных цветов
			inline UINT GetPaletteEntries(UINT iStartIndex,UINT nEntries,LPPALETTEENTRY lppe)
				{return ::GetPaletteEntries(*this,iStartIndex,nEntries,lppe);}
			///\brief Устанаыливает массив новых цветов в палитру
			///\param iStartIndex - стартовый индекс
			///\param nEntries - колличество цветов
			///\param lppe - массив структур PALETTEENTRY
			///\return число скопированных цветов
			inline UINT SetPaletteEntries(UINT iStartIndex,UINT nEntries,LPPALETTEENTRY lppe)
				{return ::SetPaletteEntries(*this,iStartIndex,nEntries,lppe);}
			///\brief Возвращает число вхождений в палитру
			///\return число цветов в палитре
			inline int GetEntryCount()
			{	
				WORD nEntries;
				::GetObject(m_hPalette, sizeof(WORD), &nEntries); 
				return (int)nEntries; 
			}
			///\brief Удаляет палитру
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL DeletePalette()
				{if(m_hPalette!=NULL)return ::DeleteObject(m_hPalette);else return FALSE;}
			///\brief отсоединяет палитру от класса
			///\return хендл на палитру
			inline HPALETTE Detach()
				{HPALETTE hRetPalette=m_hPalette;m_hPalette=NULL;return hRetPalette;}

		};
	}
	///\brief Загружает картинку типа BMP, GIF, JPEG, ICO, WMF, и EMF:
	///\param szResource - имя ресурса или фаила
	///\param szResourceType - тип ресурса
	///\param hMod - Хендл модуля, в котором лежит ресурс
	///\return хендл битмапа, в случае ошибки NULL
	HBITMAP ULLoadImage(LPCTSTR szResource,LPCTSTR szResourceType=NULL,HINSTANCE hMod=NULL);
}
#endif//__ULGOBJS__H_

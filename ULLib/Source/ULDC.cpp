///\file ULDC.cpp
///\brief cpp файл классов контекстов(08.10.2007)
#include "..\\include\\ULGDI.h"

namespace ULGDI
{
	namespace ULDC
	{
		BOOL ULGDI::ULDC::CULDC::CreateBMPFile(LPCTSTR pszFile,RECT rcSize, DWORD dwComp, WORD wBitCount) 
		{	
			typedef struct
			{
				BITMAPINFOHEADER bmiHeader;
				RGBQUAD bmiColors[259];
			} DIBINFO;
				HANDLE hf;                 
				BITMAPFILEHEADER hdr;       
				LPBYTE lpBits;             
				DWORD dwTmp; 

				BITMAP ddbinfo = {0};
				DIBINFO bInfo = {0};

			ULGDI::ULGObjs::CULBitmap Bmp;
			Bmp.CreateCompatibleBitmap(*this,rcSize.right-rcSize.left,rcSize.bottom-rcSize.top);

			//Запись рисунка в память
			ULGDI::ULDC::CULDC MemDC;
			MemDC.CreateCompatibleDC(m_hDC);
			HGDIOBJ hOld = MemDC.SelectObject((HBITMAP)Bmp);
			MemDC.BitBlt(0,0,rcSize.right-rcSize.left,rcSize.bottom-rcSize.top,m_hDC,0,0,SRCCOPY);
			MemDC.SelectObject(hOld);
			//Получение данных по изображению
			if (!Bmp.GetBitmap(&ddbinfo))
				return FALSE;
			bInfo.bmiHeader.biSize  = sizeof(BITMAPINFOHEADER);
			bInfo.bmiHeader.biWidth = ddbinfo.bmWidth;
			bInfo.bmiHeader.biHeight = ddbinfo.bmHeight;

			bInfo.bmiHeader.biBitCount = wBitCount;
			bInfo.bmiHeader.biCompression = dwComp;

			//Используется сжатие - можно не делать  
			bInfo.bmiHeader.biPlanes = 1;
			bInfo.bmiHeader.biBitCount = wBitCount;
			bInfo.bmiHeader.biCompression = dwComp;//BI_RLE8, BI_RLE4...

			//Получение размера для сжатого рисунка из исходного
			if (!GetDIBits(Bmp, 0, ddbinfo.bmHeight, NULL, (BITMAPINFO*)&bInfo , DIB_RGB_COLORS))
				return FALSE;
			//Размер заголовка растра
			DWORD dwInfSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*259;
			//Весь растр
			DWORD dwTotalSize = dwInfSize + bInfo.bmiHeader.biSizeImage;
			lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, dwTotalSize);
			if (!lpBits)
				return FALSE;
			memset(lpBits,0,dwTotalSize);
			memcpy(lpBits,&bInfo,dwInfSize);

			//Преобразование DDB в DIB
			if (!GetDIBits(Bmp, 0, ddbinfo.bmHeight, lpBits+dwInfSize, (BITMAPINFO*)lpBits , DIB_RGB_COLORS)) 
			{
				GlobalFree((HGLOBAL)lpBits);
				return FALSE;
			}
			//Создание файла для рисунка
			hf = CreateFile(pszFile, 
				GENERIC_READ | GENERIC_WRITE, 
				(DWORD) 0, 
				NULL, 
				CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, 
				(HANDLE) NULL); 
		        
			if (hf == INVALID_HANDLE_VALUE) 
			{
				GlobalFree((HGLOBAL)lpBits);
				return FALSE;
			}
			//Структура заголовка файла
			hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
			hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + dwTotalSize);
			hdr.bfReserved1 = 0; 
			hdr.bfReserved2 = 0; 
			hdr.bfOffBits = (DWORD) (sizeof(BITMAPFILEHEADER) + dwInfSize);  

			//Запись заголовка
			if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
				(LPDWORD) &dwTmp,  NULL))
			{
				GlobalFree((HGLOBAL)lpBits);
				CloseHandle(hf);
				return FALSE;
			}
			//Запись описания растра
			if (!WriteFile(hf, (BITMAPINFO*)& bInfo, dwInfSize, (LPDWORD) &dwTmp,  NULL))
			{
				GlobalFree((HGLOBAL)lpBits);
				CloseHandle(hf);
				return FALSE;
			}
			//Запись массива битов
			if (!WriteFile(hf, (BYTE*)lpBits+dwInfSize, dwTotalSize-dwInfSize, (LPDWORD) &dwTmp,NULL)) 
			{
				GlobalFree((HGLOBAL)lpBits);
				CloseHandle(hf);
				return FALSE;
			}
			GlobalFree((HGLOBAL)lpBits);
			CloseHandle(hf);
			return TRUE;
		}

		BOOL CULDC::SaveDC()
		{
			return (::SaveDC(m_hDC)!=0);
		}

		BOOL CULDC::RestoreDC(int nSavedDC)
		{
			return (::RestoreDC(m_hDC,nSavedDC)!=0);
		}
		BOOL CULDC::Polygon(CONST POINT *lpPoints,int nCount)
		{
			return ::Polygon(m_hDC,lpPoints,nCount);
		}
		COLORREF CULDC::GetPixel(int nXPos,int nYPos)
		{
			return ::GetPixel(m_hDC,nXPos,nYPos);
		}
		BOOL CULDC::SetPixel(int nXPos,int nYPos,COLORREF crColor)
		{
			return (::SetPixel(m_hDC,nXPos,nYPos,crColor)!=CLR_INVALID);
		}
		BOOL CULDC::DrawIcon(int X,int Y,HICON hIcon)
		{
			return ::DrawIcon(*this,X,Y,hIcon);
		}
		BOOL CULDC::DrawIconEx(int xLeft,int yTop,HICON hIcon,int cxWidth,int cyWidth,
			UINT istepIfAniCur,HBRUSH hbrFlickerFreeDraw,UINT diFlags)
		{
			return ::DrawIconEx(*this,xLeft,yTop,hIcon,cxWidth,cyWidth,
			istepIfAniCur,hbrFlickerFreeDraw,diFlags);
		}
		int CULDC::SetROP2(int fnDrawMode)
		{
			return ::SetROP2(m_hDC,fnDrawMode);
		}
	}
}

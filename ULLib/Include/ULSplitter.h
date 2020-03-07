///\file ULSplitter.h 
///\brief фаил объявления класса оконного разделителя
#ifndef _ULSPLITTER_H__
#define _ULSPLITTER_H__
#include "ULWnd.h"

namespace ULWnds
{
	namespace ULControls
	{
		///\class CULSplitter
		///\brief класс контрола оконного разделителя 
		class CULSplitter:
			public ULWnds::CULWnd
		{
		private:
			///\brief позиция сплитера
			int m_nSplitterPos;
			//\brief размер сплитера
			int m_nSplitterBorder;
			///\brief предыдущая позиция сплитера
			int  m_oldPos;
			///\brief флаги режимов
			BOOL m_fMoved;
			BOOL m_fDragMode;
			///\brief флаг, указывающий на возможность изменения размеров областей
			BOOL m_fResize;
		protected:
			///\brief массив для окон, которые разделяет сплитер
			ULWnds::CULWnd* m_arPane[2];
		public:
			///\brief команды посылаемые владельцу слитера
			///\param scPaneSized - когда сплитер сайзится
			enum enSplitterCommand
			{
				scPaneSized
			};
			///\brief режимы сплитера
			///\param soHorz - горизонтальный
			///\param soVert - вертикальный
			enum enSplitterOrientation
			{
				soHorz,
				soVert
			};
			///\brief отражает текущий режим сплитера
			enSplitterOrientation m_SplitterOrientation;
		public:
			///\brief активирует возможность изменения пропорции
			void SetResize(BOOL* pfEnable);
			///\brief возвращает состояние возможности изменения пропорции
			void GetResize(BOOL* pfEnable);
		public:
			///\brief Конструктор
			CULSplitter();
			///\brief создает сплитер
			///\param hParentWnd - хендл окна родителя
			///\param wID - идентификатор контрола
			///\param x,y - координаты контрола
			///\param cx,cy - размеры
			///\param SplitterOrientation - ориентация
			///\param nSplitterPos - текущая позиция сплитера
			///\param fResize - флаг, указывающий на возможность мышью изменения размеров областей
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,WORD wID,int x,int y,int cx,int cy,enSplitterOrientation SplitterOrientation,int nSplitterPos,BOOL fResize=TRUE);
			///\brief устанавливает окно в ячейку
			///\param nPos - позиция ячейки
			///\param pwndPane - указатель на окно
			///\return TRUE в случае успеха
			BOOL SetPane(int nPos,ULWnds::CULWnd* pwndPane);
			///\brief установка позиции сплитера
			///\param nSplitterPos - позиция сплитера
			void SetSplitterPos(int nSplitterPos);
			///\brief свойство установки/получения включенности окна
			ULProperty::CULProperty<BOOL,CULSplitter,
				&CULSplitter::SetResize,
				&CULSplitter::GetResize> propResize;
		protected:
			///\brief ресайзит ячейки 
			///\param nWidth, nHeight - размер сплитера
			void SizeWindowContents(int nWidth, int nHeight);
			///\brief рисует полосу при ресайзе ячеек
			///\param hDC - контекст для отрисовки
			///\param x1,y1,width, height - координаты и размеры полоски
			void DrawXorBar(HDC hDC, int x1, int y1, int width, int height);
		protected:
			virtual LRESULT OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnLButtonDown(WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnLButtonUp(WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnMouseMove(WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnSize(WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnWindowPosChanged(WPARAM wParam,LPARAM lParam);
			virtual LRESULT OnSetCursor(WPARAM wParam,LPARAM lParam);			
		};
	}
}

#endif _ULSPLITTER_H__

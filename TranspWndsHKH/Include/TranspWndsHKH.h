/*
 * Copyright © 2009-2011 Bombizov Alexandr
 * This file is part of TranspWnds.
 *
 * TranspWnds is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TranspWnds is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with TranspWnds.  If not, see <http://www.gnu.org/licenses/>.
 */
//------------------------------------------------------------------------------------------//
///\file TranspWndsHKH.h
///\date 30.04.09 (20.22)
///\author Volodin Oleg
//------------------------------------------------------------------------------------------//
#pragma once

#ifdef TRANSP_WNDS_HKH_EXPORTS
	#define TRANSP_WNDS_HKH_API __declspec(dllexport)
#else
	#define TRANSP_WNDS_HKH_API __declspec(dllimport)
#endif

#define NWM_TOUCHPADMOUSEWHEEL WM_USER + 0x10

TRANSP_WNDS_HKH_API
BOOL SetTranspWndsHookForTouchpad(HWND hHostWnd);

TRANSP_WNDS_HKH_API
BOOL UnSetTranspWndsHookForTouchpad();

///\brief включает/отключает блокировку изменения размера выбранного окна
///\param hTargetWnd - хендл окна, размер которого блокируется
///\param szWnd - размер, который должен быть постоянным у окна
///\param fEnable - флаг включения/выключения блокировки размера
///\note данная функция вспомогательная для инструмента сворачивания окна в заголовок
TRANSP_WNDS_HKH_API void EnableBlockSize(HWND hTargetWnd,SIZE szWnd,BOOL fEnable);

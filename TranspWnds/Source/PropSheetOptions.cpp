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
#include "PropSheetOptions.h"
#include "resource.h"

CPropSheetOptions::CPropSheetOptions(void)
{
	MessageMap.AddMessage<CPropSheetOptions>(WM_INITDIALOG,&CPropSheetOptions::OnInitDialog);
}

CPropSheetOptions::~CPropSheetOptions(void)
{
}

LRESULT CPropSheetOptions::OnInitDialog(WPARAM,LPARAM)
{
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,FALSE);
	//#define CS_DROPSHADOW       0x00020000
	SetClassLong(GCL_STYLE,(LONG)GetClassLong(GCL_STYLE)|0x00020000);
	return 0;
}

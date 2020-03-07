///\file ULRes.h
///\brief фаил содержащий функции установления хендлов фаилов ресурсов
#pragma once
#ifndef _ULRES_H__
#define _ULRES_H__
#include <windows.h>
namespace ULOther
{
	///\brief устанавливает хендл модуля содержащего ресурсы
	///\return предыдущий хендл. 0 не переприсваевает
	HINSTANCE ULSetResourceHandle(HINSTANCE hNewRes);
	///\brief возвращает хендл модуля содержащего ресурсы
	///\return хендл модуля содержащего ресурсы
	HINSTANCE ULGetResourceHandle();
}
#endif //_ULRES_H__

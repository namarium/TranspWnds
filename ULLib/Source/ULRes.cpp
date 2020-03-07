///\file ULRes.cpp
///\brief фаил реализации функции установления хендлов фаилов ресурсов
#include "..\..\ULLib\Include\ULRes.h"

namespace ULOther
{
	HINSTANCE ULSetResourceHandle(HINSTANCE hNewRes)
	{
		static HINSTANCE hRes=NULL;
		if(hNewRes)
		{
			HINSTANCE hTmpRes=hRes;
			hRes=hNewRes;
			return hTmpRes;
		}
		return hRes;
	}
	HINSTANCE ULGetResourceHandle()
	{
		return ULSetResourceHandle(NULL);
	}
}

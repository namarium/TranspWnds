///\file ULMessageMap.inl
///\brief Заголовочный файл класса обработчика сообщений окна(23.10.2007)
#pragma once
#ifndef __UL_MESSAGEMAP_H__
#define __UL_MESSAGEMAP_H__
#include <windows.h>
#include "ULOther.h"
#include <assert.h>
///\class CULMessageMap
///\brief класс обработчика сообщений окна
///\param CULClass_t - класс родитель, для тех классов, \n
///которые используют CULMessageMap
///\note сообщения обычно добавляются в конструкторе класса окна \n
/// при помощи фукция Add... . \n
/// для устранения конфликтов отнаследованных клссов с родительскими \n
/// в обработчиках сообщений возможна очистка хранилищ сообщений \n
/// путём вызова соответствующих функций Clear...
template <class CULClass_t>
class CULMessageMap
{
private:
	///\brief Тип функции обработчика сообщений
	typedef LRESULT(CULClass_t::*MSGPROC)(WPARAM,LPARAM);
	///\brief Тип функции обработчика команд от контролов
	typedef void(CULClass_t::*CMDPROC)(WORD,HWND);
	///\brief Тип функции обработчика команд от комконтролов
	typedef LRESULT(CULClass_t::*NOTIFYPROC)(LPARAM);
	///\brief Тип функции обработчика команд от контролов, в самих же контролах
	typedef LRESULT(CULClass_t::*REFLECTCMDPROC)();
	///\brief Тип функции вызываемой перед обработкой сообщений
	typedef BOOL(CULClass_t::*PRETRANSLATEMESSAGEPROC)(MSG* pMsg);

	///\brief Структура, содержащая информацию об обработчике сообщения
	///\param msMsgID - ID сообщения
	///\param msMsgProc - указатель на функцию обработчик
	struct tagMessageStorage
	{
		DWORD msMsgID;			
		MSGPROC msMsgProc;
	};
	///\brief массив структур, содержащий информацию об обработчиках сообщений
	ULOther::CULArr<tagMessageStorage> m_pMessageStorage;
	///\brief массив структур, содержащий информацию обобработчиках возвращаемых сообщений
	ULOther::CULArr<tagMessageStorage> m_pReflectMessageStorage;
	///\brief Cтруктура, содержащая информацию об обработчике команды от контрола
	///\param csNotifyCode - код сообщения
	///\param csCtrlID - идентификатор контрола
	///\param csCmdProc - указатель на функцию обработчик
	struct tagCommandStorage
	{
		WORD csNotifyCode;
		WORD csCtrlID;			
		CMDPROC csCmdProc;		
	};
	///\brief массив структур, содержащий информацию об обработчиках команд от контролов
	ULOther::CULArr<tagCommandStorage> m_pCommandStorage;
	///\brief Cтруктура, содержащая информацию об обработчике команды от контрола
	///\param csNotifyCode - код сообщения
	///\param csReflectCmdProc - указатель на функцию обработчик
	struct tagReflectCommandStorage
	{
		WORD csNotifyCode;
		REFLECTCMDPROC csReflectCmdProc;		
	};
	///\brief массив структур, содержащий информацию об обработчиках команд от контролов
	ULOther::CULArr<tagReflectCommandStorage> m_pReflectCommandStorage;
	///\brief Cтруктура, содержащиая информацию об обработчике сообщения от контрола
	///\param nsNMHDR - содержит информацию о сообщении
	///\param nsNotifyProc - указатель на функцию обработчик
	struct tagNotifyStorage
	{
		NMHDR nsNMHDR;			
		NOTIFYPROC nsNotifyProc;
	};
	///\brief массив структур, содержащий информацию об обработчиках сообщений от контролов
	ULOther::CULArr<tagNotifyStorage> m_pNotifyStorage;
	///\brief массив структур, содержащий информацию об обработчиках возвращаемых сообщений от контролов
	ULOther::CULArr<tagNotifyStorage> m_pReflectNotifyStorage;
	///\brief Cтруктура, содержащиая информацию функции вызываемой перед обработкой сообщений
	///\param lpObject - указатель на объект сдержащий функцию
	///\param lpPreTranslateMessageProc - указатель на функцию
	struct tagPreTranslateMessageStorage
	{
		CULClass_t* lpObject;
		PRETRANSLATEMESSAGEPROC lpPreTranslateMessageProc;
	};
	///\brief массив структур, содержащиах информацию функции вызываемой перед обработкой сообщений
	ULOther::CULArr<tagPreTranslateMessageStorage> m_pPreTranslateMessageStorage;
public:
	///brief Конструктор
	CULMessageMap()
	{
		m_pMessageStorage.Resize(0);
		m_pReflectMessageStorage.Resize(0);
		m_pCommandStorage.Resize(0);
		m_pReflectCommandStorage.Resize(0);
		m_pNotifyStorage.Resize(0);
		m_pReflectNotifyStorage.Resize(0);
		m_pPreTranslateMessageStorage.Resize(0);
	};
	///brief Конструктор копирования
	CULMessageMap(CULMessageMap<CULClass_t>& MessageMap):
		m_pMessageStorage(MessageMap.m_pMessageStorage),
		m_pReflectMessageStorage(MessageMap.m_pReflectMessageStorage),
		m_pCommandStorage(MessageMap.m_pCommandStorage),
		m_pReflectCommandStorage(MessageMap.m_pReflectCommandStorage),
		m_pNotifyStorage(MessageMap.m_pNotifyStorage),
		m_pReflectNotifyStorage(MessageMap.m_pReflectNotifyStorage),
		m_pPreTranslateMessageStorage(MessageMap.m_pPreTranslateMessageStorage)
	{
	}
	///\brief Деструктор
	~CULMessageMap(){};
	///\brief оператор копирования
	void operator=(CULMessageMap<CULClass_t>& MessageMap)
	{
		m_pMessageStorage=MessageMap.m_pMessageStorage;
		m_pReflectMessageStorage=MessageMap.m_pReflectMessageStorage;
		m_pCommandStorage=MessageMap.m_pCommandStorage;
		m_pReflectCommandStorage=MessageMap.m_pReflectCommandStorage;
		m_pNotifyStorage=MessageMap.m_pNotifyStorage;
		m_pReflectNotifyStorage=MessageMap.m_pReflectNotifyStorage;
		m_pPreTranslateMessageStorage=MessageMap.m_pPreTranslateMessageStorage;
	}
	///\brief Функция говорит о наличии обработчиков
	///\return TRUE, если обработчики есть
	BOOL IsAllowMessage()
		{return ((m_pMessageStorage.GetSize())||
			(m_pCommandStorage.GetSize())||(m_pNotifyStorage.GetSize()));}
	///\brief Функция говорит о наличии обработчиков возвращаемых событий
	///\return TRUE, если обработчики есть
	BOOL IsAllowReflect()
		{return ((m_pReflectMessageStorage.GetSize())||
			(m_pReflectCommandStorage.GetSize())||(m_pReflectNotifyStorage.GetSize()));}
	///\brief Функция для добаления обработчиков ссобщений
	///	CULCurClass_t - класс окна, в котором происходит обработка сообщения
	///\param MsgID - ID сообщения
	///\param MsgProc - указатель на функцию обработчик
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddMessage(WORD MsgID,LRESULT(CULCurClass_t::*MsgProc)(WPARAM,LPARAM))	
	{
		if(m_pMessageStorage.Resize(m_pMessageStorage.GetSize()+1)!=NULL)
		{
			m_pMessageStorage[m_pMessageStorage.GetSize()-1].msMsgProc=reinterpret_cast<MSGPROC>(MsgProc);
			m_pMessageStorage[m_pMessageStorage.GetSize()-1].msMsgID=MsgID;
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для добаления обработчиков возвращаемых ссобщений
	///	CULCurClass_t - класс окна, в котором происходит обработка сообщения
	///\param MsgID - ID сообщения
	///\param MsgProc - указатель на функцию обработчик
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddReflectMessage(WORD MsgID,LRESULT(CULCurClass_t::*MsgProc)(WPARAM,LPARAM))	
	{
		if(m_pReflectMessageStorage.Resize(m_pReflectMessageStorage.GetSize()+1)!=NULL)
		{
			m_pReflectMessageStorage[m_pReflectMessageStorage.GetSize()-1].msMsgProc=reinterpret_cast<MSGPROC>(MsgProc);
			m_pReflectMessageStorage[m_pReflectMessageStorage.GetSize()-1].msMsgID=MsgID;
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для добаления обработчиков команд от контролов
	///	CULCurClass_t - класс окна, в котором происходит	обработка сообщения
	///\param CtrlID - идентификатор контрола
	///\param CmdProc - указатель на функцию обработчик
	///\param wNotifyCode - идентификатор сообщения от контрола
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddCommand(WORD CtrlID,void(CULCurClass_t::*CmdProc)(WORD,HWND),WORD wNotifyCode=0)	
	{
		if(m_pCommandStorage.Resize(m_pCommandStorage.GetSize()+1)!=NULL)
		{
			m_pCommandStorage[m_pCommandStorage.GetSize()-1].csCtrlID=CtrlID;
			m_pCommandStorage[m_pCommandStorage.GetSize()-1].csCmdProc=reinterpret_cast<CMDPROC>(CmdProc);
			m_pCommandStorage[m_pCommandStorage.GetSize()-1].csNotifyCode=wNotifyCode;
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для добаления обработчиков возвращаемых команд от контролов
	///	CULCurClass_t - класс окна, в котором происходит	обработка сообщения
	///\param ReflectCmdProc - указатель на функцию обработчик
	///\param wNotifyCode - идентификатор сообщения от контрола
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddReflectCommand(LRESULT(CULCurClass_t::*ReflectCmdProc)(),WORD wNotifyCode=0)	
	{
		if(m_pReflectCommandStorage.Resize(m_pReflectCommandStorage.GetSize()+1)!=NULL)
		{
			m_pReflectCommandStorage[m_pReflectCommandStorage.GetSize()-1].csReflectCmdProc=reinterpret_cast<REFLECTCMDPROC>(ReflectCmdProc);
			m_pReflectCommandStorage[m_pReflectCommandStorage.GetSize()-1].csNotifyCode=wNotifyCode;
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для добаления обработчиков сообщений от контролов
	///	CULCurClass_t - класс окна, в котором происходит	обработка сообщения
	///\param CtrlID -идентификатор контрола
	///\param NotifyCode - ID команды
	///\param NotifyProc - указатель на функцию обработчик
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddNotify(WORD CtrlID,DWORD NotifyCode, 
		LRESULT(CULCurClass_t::*NotifyProc)(LPARAM))	
	{
		if(m_pNotifyStorage.Resize(m_pNotifyStorage.GetSize()+1)!=NULL)
		{
			m_pNotifyStorage[m_pNotifyStorage.GetSize()-1].nsNMHDR.hwndFrom=NULL;
			m_pNotifyStorage[m_pNotifyStorage.GetSize()-1].nsNMHDR.idFrom=CtrlID;
			m_pNotifyStorage[m_pNotifyStorage.GetSize()-1].nsNMHDR.code=NotifyCode;
			m_pNotifyStorage[m_pNotifyStorage.GetSize()-1].nsNotifyProc=reinterpret_cast<NOTIFYPROC>(NotifyProc);
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для добаления обработчиков возвращаемых сообщений от контролов
	///	CULCurClass_t - класс окна, в котором происходит	обработка сообщения
	///\param NotifyCode - ID команды
	///\param NotifyProc - указатель на функцию обработчик
	///\return TRUE в случае успеха
	template<class CULCurClass_t>
	BOOL AddReflectNotify(DWORD NotifyCode, 
		LRESULT(CULCurClass_t::*NotifyProc)(LPARAM))	
	{
		if(m_pReflectNotifyStorage.Resize(m_pReflectNotifyStorage.GetSize()+1)!=NULL)
		{
			m_pReflectNotifyStorage[m_pReflectNotifyStorage.GetSize()-1].nsNMHDR.code=NotifyCode;
			m_pReflectNotifyStorage[m_pReflectNotifyStorage.GetSize()-1].nsNotifyProc=
				reinterpret_cast<NOTIFYPROC>(NotifyProc);
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция очистки хранилища обработчиков сообщений
	void ClearMessage()
	{
		m_pMessageStorage.Resize(0);
	}
	///\brief Функция очистки хранилища обработчиков возвращаемых сообщений
	void ClearReflectMessage()
	{
		m_pReflectMessageStorage.Resize(0);
	}
	///\brief Функция очистки хранилища обработчиков команд от контролов
	void ClearCommand()
	{
		m_pCommandStorage.Resize(0);
	}
	///\brief Функция очистки хранилища обработчиков возвращаемых команд от контролов
	void ClearReflectCommand()
	{
		m_pReflectCommandStorage.Resize(0);
	}
	///\brief Функция очистки хранилища обработчиков команд от контролов
	void ClearNotify()
	{
		m_pNotifyStorage.Resize(0);
	}
	///\brief Функция очистки хранилища обработчиков возвращаемых команд от контролов
	void ClearReflectNotify()
	{
		m_pReflectNotifyStorage.Resize(0);
	}
	///\brief функция очистки всех кранилищь обработчиков
	void ClearAll()
	{
		ClearMessage();
		ClearReflectMessage();
		ClearCommand();
		ClearReflectCommand();
		ClearNotify();
		ClearReflectNotify();
	}
	///\brief Функция для добаления функций, вызываемых перед обработкой сообщений
	///	CULCurClass_t - класс окна, содержащего функцию
	///\param lpObject - указатель на объект, содержащий функцию
	///\param lpPreTranslateMessageProc - функция
	///\return TRUE в случае успеха
	///\note функция должна вызываться в том же модуле, в \n
	///\ в котором создан объект CULApp 
	template<class CULCurClass_t>
	BOOL InitializePreTranslateMessage(CULCurClass_t* lpObject, BOOL(CULCurClass_t::*lpPreTranslateMessageProc)(MSG*))	
	{
		if(m_pPreTranslateMessageStorage.Resize(m_pPreTranslateMessageStorage.GetSize()+1)!=NULL)
		{
			m_pPreTranslateMessageStorage[m_pPreTranslateMessageStorage.GetSize()-1].lpObject=lpObject;
			m_pPreTranslateMessageStorage[m_pPreTranslateMessageStorage.GetSize()-1].lpPreTranslateMessageProc=
				reinterpret_cast<PRETRANSLATEMESSAGEPROC>(lpPreTranslateMessageProc);
			return TRUE;
		}
		else
			return FALSE;
	};
	///\brief Функция для удаления функций, вызываемых перед обработкой сообщений
	///	CULCurClass_t - класс окна, содержащего функцию
	///\param lpObject - указатель на объект, содержащий функцию
	///\param lpPreTranslateMessageProc - функция
	template<class CULCurClass_t>
	void UnInitializePreTranslateMessage(CULCurClass_t* lpObject, BOOL(CULCurClass_t::*lpPreTranslateMessageProc)(MSG*))	
	{
		size_t nFind=0;
		for(nFind=0;nFind<m_pPreTranslateMessageStorage.GetSize();++nFind)
			if((m_pPreTranslateMessageStorage[nFind].lpObject==lpObject)&&
				m_pPreTranslateMessageStorage[nFind].lpPreTranslateMessageProc==lpPreTranslateMessageProc)
				break;
		for(size_t i=nFind;i<(m_pPreTranslateMessageStorage.GetSize()-1);++i)
		{
			m_pPreTranslateMessageStorage[i].lpObject=m_pPreTranslateMessageStorage[i+1].lpObject;
			m_pPreTranslateMessageStorage[i].lpPreTranslateMessageProc=m_pPreTranslateMessageStorage[i+1].lpPreTranslateMessageProc;
		}
		assert(m_pPreTranslateMessageStorage.GetSize());
		m_pPreTranslateMessageStorage.Resize(m_pPreTranslateMessageStorage.GetSize()-1);
	};
	///\brief Функция вызова обработчиков сообщений
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param uMsg - идентификатор сообщения
	///\param wParam - параметр 1
	///\param lParam - параметр 2
	///\return возвращаемое значение обработчика
	BOOL ProcessMessage(CULClass_t* pWnd,UINT uMsg , WPARAM wParam , LPARAM lParam)
	{
		for(unsigned int i=0;i<m_pMessageStorage.GetSize();++i)
			if(m_pMessageStorage[i].msMsgID==uMsg)
				return(pWnd->*m_pMessageStorage[i].msMsgProc)(wParam,lParam);
		return FALSE;
	};
	///\brief Функция вызова обработчиков возвращаемых сообщений
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param uMsg - идентификатор сообщения
	///\param wParam - параметр 1
	///\param lParam - параметр 2
	///\return возвращаемое значение обработчика
	BOOL ProcessReflectMessage(CULClass_t* pWnd,UINT uMsg , WPARAM wParam , LPARAM lParam)
	{
		for(unsigned int i=0;i<m_pReflectMessageStorage.GetSize();++i)
			if(m_pReflectMessageStorage[i].msMsgID==uMsg)
				return(pWnd->*m_pReflectMessageStorage[i].msMsgProc)(wParam,lParam);
		return FALSE;
	};
	///\brief Функция вызова обработчиков команд от контролов
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param wParam - параметр 1
	///\param lParam - параметр 2
	///\return TRUE, если есть обработчик
	BOOL ProcessCommand(CULClass_t* pWnd,WPARAM wParam , LPARAM lParam)
	{
		WORD wNotifyCode=HIWORD(wParam);
		WORD wID=LOWORD(wParam);
		HWND hControl=HWND(lParam);
		for(unsigned int i=0;i<m_pCommandStorage.GetSize();++i)
			if((m_pCommandStorage[i].csCtrlID==wID)&&
				((m_pCommandStorage[i].csNotifyCode==wNotifyCode)||
				((m_pCommandStorage[i].csNotifyCode==0)&&(wNotifyCode==1))))//для акселератора
			{
				(pWnd->*m_pCommandStorage[i].csCmdProc)(wID,hControl);
				return TRUE;
			}
		return FALSE;
	};
	///\brief Функция вызова обработчиков возвращаемых команд от контролов
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param wNotifyCode - идентификатор сообщения от контрола
	///\return TRUE, если есть обработчик
	BOOL ProcessReflectCommand(CULClass_t* pWnd,WORD wNotifyCode)
	{
		for(unsigned int i=0;i<m_pReflectCommandStorage.GetSize();++i)
			if(m_pReflectCommandStorage[i].csNotifyCode==wNotifyCode)
				return(pWnd->*m_pReflectCommandStorage[i].csReflectCmdProc)();
		return FALSE;
	};
	///\brief Функция вызова обработчиков команд от контролов
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param pNMHDR - указатель на структуру NMHDR
	///\return TRUE, если есть обработчик
	BOOL ProcessNotify(CULClass_t* pWnd,NMHDR* pNMHDR)
	{
		for(unsigned int i=0;i<m_pNotifyStorage.GetSize();++i)
			if((m_pNotifyStorage[i].nsNMHDR.idFrom==pNMHDR->idFrom)&&
				(m_pNotifyStorage[i].nsNMHDR.code==pNMHDR->code))
			{
				::SetWindowLong(pWnd->m_hWnd,DWL_MSGRESULT,(pWnd->*m_pNotifyStorage[i].nsNotifyProc)(LPARAM(pNMHDR)));
				return TRUE;
			}
		return FALSE;
	};
	///\brief Функция вызова обработчиков возвращаемых команд от контролов
	///\param pWnd - указатель на класс, к которому принадлежит WndProc
	///\param pNMHDR - указатель на структуру NMHDR
	///\param pfRet - возвращаемое значение от обработчика сообщения
	///\return TRUE, если есть обработчик
	BOOL ProcessReflectNotify(CULClass_t* pWnd,NMHDR* pNMHDR,BOOL *pfRet)
	{
		for(unsigned int i=0;i<m_pReflectNotifyStorage.GetSize();++i)
			if(m_pReflectNotifyStorage[i].nsNMHDR.code==pNMHDR->code)
			{
				*pfRet=(pWnd->*m_pReflectNotifyStorage[i].nsNotifyProc)(LPARAM(pNMHDR));
				//::SetWindowLong(pWnd->m_hWnd,DWL_MSGRESULT,*pfRet);
				return TRUE;
			}
		return FALSE;
	};

	///\brief Функция вызова функций, вызываемых перед обработкой сообщений
	///\param pMsg - указатель на экземпляр структуры, несущей информацию о сообщении
	///\return FALSE(код фозврата функции Pre...) ведет к пропусканию обработки текущего сообщения
	BOOL ProcessPreTranslateMessage(MSG* pMsg)
	{
		for(unsigned int i=0;i<m_pPreTranslateMessageStorage.GetSize();++i)
			if(!(m_pPreTranslateMessageStorage[i].lpObject->*m_pPreTranslateMessageStorage[i].lpPreTranslateMessageProc)(pMsg))
				return FALSE;
		return TRUE;
	};
};
#endif //__UL_MESSAGEMAP_H__

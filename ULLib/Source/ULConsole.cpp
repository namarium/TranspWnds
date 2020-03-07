///\file ULConsole.cpp
///\brief cpp файл консоли
#include "..\\include\\ULConsole.h"
#include <stdio.h>
#include "..\\include\\ULPtr.inl"

//Конструктор (с привязкой к потокам)
CULConsole::CULConsole()
{
	//Получение указателей на стандартный ввод и вывод
	m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
}

CULConsole::CULConsole(CULConsole& Console):
	m_hStdout(Console.m_hStdout),
	m_hStdin(Console.m_hStdin)
{

}

CULConsole::~CULConsole()
{
};

void CULConsole::operator=(CULConsole& Console)
{
	m_hStdout=Console.m_hStdout;
	m_hStdin=Console.m_hStdin;
}
//Очистка экрана
void CULConsole::Clear()
{
	system("cls");
}

//Задержка
void CULConsole::Pause()
{
	system("pause");
}

//Получение заголовка окна
LPTSTR CULConsole::GetTitle(LPTSTR pszTitle,WORD wSizeOfBuf)
{
	//Получение заголовка окна
	GetConsoleTitle(pszTitle, wSizeOfBuf);
	//Преобразование кодировки
//	OemToCharA(szBuf, szBuf);
	//Возврат строки
	return pszTitle;
}

//Установка заголовка окна
void CULConsole::SetTitle(LPTSTR pszTitle)
{
	//Установка заголовка окна
	SetConsoleTitle(pszTitle);
}

//Получение цвета текста
enConsoleColor CULConsole::GetColor()
{
	//Информация о консоли
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	//Получение информации о консоли
	GetConsoleScreenBufferInfo(m_hStdout, &csbiInfo);
	//Возврат цвета
	return (enConsoleColor)(csbiInfo.wAttributes % 16);
}

//Установка цвета текста
void CULConsole::SetColor(enConsoleColor Color)
{
	SetConsoleTextAttribute(m_hStdout, (WORD)Color + (WORD)GetBgColor() * 16);
}

//Получение цвета фона
enConsoleColor CULConsole::GetBgColor()
{
	//Информация о консоли
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	//Получение информации о консоли
	GetConsoleScreenBufferInfo(m_hStdout, &csbiInfo);
	//Возврат цвета
	return (enConsoleColor)(csbiInfo.wAttributes / 16);
}

//Установка цвета фона
void CULConsole::SetBgColor(enConsoleColor Color)
{
	SetConsoleTextAttribute(m_hStdout, (WORD)Color * 16 + (WORD)GetColor());
}

//Вывод символа
void CULConsole::Write(TCHAR c)
{
	_puttc(c,stdout);
}

//Вывод строки (с преобразованием кодировки)
void CULConsole::Write(LPTSTR pszStr)
{
	//Буфер
	ULOther::CULStrT<char> szBuf;
	//Преобразование кодировки
	CharToOem(pszStr, szBuf);
	puts(szBuf);
}

//Переход на новую строку
void CULConsole::WriteLine()
{
	_puttc(_T('\n'),stdout);
}

//Вывод строки (с преобразованием кодировки и переходом на новую строку)
void CULConsole::WriteLine(LPTSTR pszStr)
{
	//Буфер	
	ULOther::CULStrT<char> szBuf;
	//Преобразование кодировки
	CharToOem(pszStr, szBuf);
	puts(szBuf);
	putc('\n',stdout);
}
//Получение позиции курсора
COORD CULConsole::GetCursorPos()
{
	//Информация о консоли
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	//Получение информации о консоли
	GetConsoleScreenBufferInfo(m_hStdout, &csbiInfo);
	return csbiInfo.dwCursorPosition;
}

//Установка позиции курсора
void CULConsole::SetCursorPos(short X, short Y)
{
	//Координаты курсора
	COORD p;
	p.X = X;
	p.Y = Y;
	SetConsoleCursorPosition(m_hStdout, p);
}

//Получение размеров окна (в символах)
SIZE CULConsole::GetWindowSize()
{
	//Информация о консоли
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	//Получение информации о консоли
	GetConsoleScreenBufferInfo(m_hStdout, &csbiInfo);
	SIZE szRet={csbiInfo.dwMaximumWindowSize.X,csbiInfo.dwMaximumWindowSize.Y};
	return szRet;
}

//Чтение кода нажатой клавиши (с ожиданием)
TCHAR CULConsole::ReadKey()
{
	//Количество собылий
	DWORD n;
	//Данные о вводе
	INPUT_RECORD record;
	//Символ
	TCHAR c;
	//Цикл ожидания нажатия клвиши
	for ( ; ; )
	{
		//Если невозможно прочесть событие
		if (!ReadConsoleInput(m_hStdin, &record, 1L, &n ) || (n == 0L))
		{
			//Установка признака конца файла и выход
			c = (TCHAR)EOF;
			break;
		}
		//Если нажата клавиша
		if ((record.EventType == KEY_EVENT) && record.Event.KeyEvent.bKeyDown)
			//Если нажата символьная клавиша, выход
			if ((c = static_cast<TCHAR>(record.Event.KeyEvent.uChar.AsciiChar))!=NULL)
				break;
	}
	//Возврат символа
	return c;
}

//Чтение строки (не пустой)
LPTSTR CULConsole::ReadLine(LPTSTR pszBuf,WORD wSizeOfBuf)
{ 
	//Буфер
	do
		//Чтение строки
		_getts_s(pszBuf,wSizeOfBuf);
	//Пока строка пустая
	while (_tcslen(pszBuf) < 1);
	//Возврат текста
	return pszBuf;
}

//Чтение целого числа (со знаком)
long CULConsole::ReadLong()
{
	//Значение
	long nVal;
	//Текст ошибки
	LPTSTR pszErr;
	//Буфер
	ULOther::CULStr szBuf;
	//Ввод и преобразование
	nVal = _tcstol(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr, 10);
	//Если имеется текст ошибки
	while (_tcslen(pszErr) > 0)
	{
		//Повтор ввода
		Write(_T("Ожидается целое число: "));
		nVal = _tcstol(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr, 10);
	}
	//Возврат значения
	return nVal;
}

//Чтение целого числа (без знака)
unsigned long CULConsole::ReadULong()
{
	//Значение
	long nVal;
	//Текст ошибки
	TCHAR* pszErr;
	//Буфер
	ULOther::CULStr szBuf;
	//Ввод и преобразование
	nVal = _tcstol(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr, 10);
	//Если имеется текст ошибки или значение < 0
	while (_tcslen(pszErr) > 0 || nVal < 0)
	{
		//Повтор ввода
		Write(_T("Ожидается целое неотрицательное число: "));
		nVal = _tcstol(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr, 10);
	}
	//Возврат значения
	return static_cast<unsigned long>(nVal);
}

//Чтение действительного числа
double CULConsole::ReadDouble()
{
	//Значение
	double dblVal;
	//Текст ошибки
	LPTSTR pszErr;
	//Буфер
	ULOther::CULStr szBuf;
	//Ввод и преобразование
	dblVal = _tcstod(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr);
	//Если имеется текст ошибки
	while (_tcslen(pszErr) > 0)
	{
		//Повтор ввода
		Write(_T("Ожидается вещественное число: "));
		dblVal = _tcstod(ReadLine(szBuf,(WORD)szBuf.GetSize()), &pszErr);
	}
	//Возврат значения
	return dblVal;
}

int CULConsole::Format(LPTSTR szFormat,...)
{
	va_list va;
	va_start( va, szFormat );	
	int nRet=_vftprintf(stdout,szFormat,va);
	va_end( va );
	return nRet;
}

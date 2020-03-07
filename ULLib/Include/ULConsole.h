///\file ULConsole.h
///\brief Заголовочный файл консоли
#pragma once
#include <windows.h>
#include <tchar.h>
///\brief Цвета в консоли
enum enConsoleColor
{
	clBlack		= 0,
	clNavy		= 1,
	clGreen		= 2,
	clTeal		= 3,
	clBrown		= 4,
	clPurple	= 5,
	clOlive		= 6,
	clGray		= 7,
	clSilver	= 8,
	clBlue		= 9,
	clLime		= 10,
	clCyan		= 11,
	clRed		= 12,
	clMagneta	= 13,
	clYellow	= 14,
	clWhite		= 15,
};
///\class CULConsole 
///\brief Класс для работы с консолью
class CULConsole
{
private:
	///\brief Указатель на стандатрный вывод
	HANDLE m_hStdout;
	///\brief Указатель на стандатрный ввод
	HANDLE m_hStdin;
public:
	///\brief Конструктор
	CULConsole();
	///\brief Конструктор копирования
	CULConsole(CULConsole&);
	///\brief Деструктор
	virtual ~CULConsole();
	///\brief оператор копирования
	void operator=(CULConsole&);
	///\brief Очистка экрана
	void Clear();
	///\brief Задержка
	void Pause();
	///\brief Получение заголовка окна
	///\param pszTitle - буфер для заголовка
	///\param wSizeOfBuf - размер буфера для заголовка
	///\return указатель на буфр заголовка (pszTitle)
	TCHAR* GetTitle(LPTSTR pszTitle,WORD wSizeOfBuf);
	///\brief Установка заголовка окна
	///\param pszTitle - буфер
	void SetTitle(LPTSTR pszTitle);
	///\brief Получение цвета текста
	///\return цвет текста
	enConsoleColor GetColor();
	///\brief Установка цвета текста
	///\param Color - цвет теста
	void SetColor(enConsoleColor Color);
	///\brief Получение цвета фона
	///\param цвет фона
	enConsoleColor GetBgColor();
	///\brief Установка цвета фона
	///\param  Color цвет фона
	void SetBgColor(enConsoleColor Color);
	///\brief Получение позиции курсора
	///\return позиция курсора
	COORD GetCursorPos();
	///\brief Установка позиции курсора
	///\param X,Y - позиция курсора
	void SetCursorPos(short X, short Y);
	///\brief Получение размеров окна (в символах)
	///\return структура размера	
	SIZE GetWindowSize();
	///\brief Вывод символа
	///\param c - символ
	void Write(TCHAR c);
	///\brief Вывод строки (с преобразованием кодировки)
	///\param pszStr - буфер
	void Write(LPTSTR pszStr);
	///\brief Переход на новую строку
	void WriteLine();
	///\brief Вывод строки (с преобразованием кодировки и переходом на новую строку)
	///\param pszStr - буфер
	void WriteLine(LPTSTR pszStr);
	///\brief Чтение строки (не пустой)
	///\param pszBuf - буфер,в который будет произведено чтение
	///\param wSizeOfBuf - размер буфера (pszBuf)
	///\return указатель на прочитанную строку (pszBuf)
	LPTSTR ReadLine(LPTSTR pszBuf,WORD wSizeOfBuf);
	///\brief Чтение целого числа (со знаком)
	///\return целое число (со знаком)
	long ReadLong();
	///\brief Чтение целого числа (без знака)
	///\return целое число (без знака)
	unsigned long ReadULong();
	///\brief Чтение действительного числа
	///\return действительное число
	double ReadDouble();
	///\brief Чтение кода нажатой клавиши (с ожиданием)
	///\return код нажатой клавиши
	TCHAR ReadKey();
	///\brief Выводит форматированный текст
	///\param szFormat - Формат вывода
	///\param ... - набор параметров
	///\return число выведенных символов
	int Format(LPTSTR szFormat,...);
};

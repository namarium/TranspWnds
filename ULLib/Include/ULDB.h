///\file ULDB.h
///\brief Заголовочный файл классов баз данных
//=======В ОТЛАДКЕ!!!!============================
#pragma once

#include<windows.h>
#include<sqlext.h>
#include <tchar.h>
#include "ULOther.h"
///\namespace ULDB
///\brief Пространство имен классов баз данных(26.08.2007)
namespace ULDB
{
	///\brief Класс ODBC базы данных(20.08.2007)
	class CULODBC
	{
	public:
		SQLHDBC m_hDBC;
	protected:
		SQLHENV m_hEnv;
		SQLHSTMT m_hStmt;

		struct tagColData
		{
			tagColData():DataPtr(NULL){}
			virtual ~tagColData()
			{
				if( DataPtr )
				{
					switch( FldSqlType )
					{
					case SQL_REAL:
					case SQL_FLOAT:
					case SQL_DOUBLE:
					case SQL_INTEGER:
					case SQL_SMALLINT:
					case SQL_TINYINT:
					case SQL_BIT:
						delete DataPtr;
						break;
					default:
						delete[] DataPtr;
					}
				}
			}

			SQLTCHAR    szColName[MAX_PATH + 1];
			SQLSMALLINT cbColName;
			SQLSMALLINT FldSqlType;
			SQLSMALLINT DecimalDigitsPtr;
			SQLSMALLINT NullablePtr;
			LONG StrLen_or_Ind;

			// Data should be stored here
			SQLPOINTER DataPtr;
			SQLUINTEGER cbBuffLen;
		};

		ULOther::CULArr<tagColData> m_pColData;

		BOOL Prepare();
		BOOL GetData();
		BOOL BindCol(SQLUSMALLINT nCol, tagColData &ColData);

	public:
		CULODBC();
		virtual ~CULODBC();
		BOOL Connect(LPCTSTR pcszDsn,LPCTSTR pcszUser,LPCTSTR pcszPass);
		BOOL DriverConnect(LPCTSTR pcszConnStrIn);
		BOOL ExecuteSQL(LPCTSTR pcszQueryBuff);
		int GetCollumnName(int nCol,LPTSTR pcszColName,int nLenColName);
		int GetCollumnCount();
		BOOL MoveNext();
		int GetField(int nCol,LPSTR pszField,int nLenField);
		void Close();
		BOOL IsConnected(void);
	};
};

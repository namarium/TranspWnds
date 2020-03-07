///\file ULDB.cpp
///\brief cpp файл классов баз данных
#include "..\..\ULLib\Include\uldb.h"

namespace ULDB
{
	CULODBC::CULODBC():
	  m_hDBC(NULL),
	  m_hEnv(NULL),
	  m_hStmt(NULL)
	{
		Prepare();
	};
	CULODBC::~CULODBC()
	{
		Close();
	};

	BOOL CULODBC::Prepare()
	{
		if (::SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_hEnv) 
					==SQL_ERROR)
		{
			Close();
			return FALSE;
		}
		::SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3,
			SQL_IS_INTEGER);        

		if (::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDBC) 
					==SQL_ERROR)
		{
			Close();
			return FALSE;
		}
		return TRUE;
	};

	BOOL CULODBC::Connect(LPCTSTR pcszDsn,LPCTSTR pcszUser,LPCTSTR pcszPass)
	{
		if (::SQLConnect(m_hDBC, (SQLTCHAR*)pcszDsn, SQL_NTS,
			(SQLTCHAR*)pcszUser, (SQLSMALLINT)_tcslen(pcszUser),(SQLTCHAR*)pcszPass,
			(SQLSMALLINT)_tcslen(pcszPass))==SQL_ERROR )
		{
			Close();
			return FALSE;
		}
		if (::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &m_hStmt) 
					==SQL_ERROR)
		{
			Close();
			return FALSE;
		}
		return TRUE;
	}

	BOOL CULODBC::DriverConnect(LPCTSTR pcszConnStrIn)
	{
		TCHAR szConnStrOut[256]={0};
		SQLSMALLINT cbConnStrOut=0;
		if ( SQLDriverConnect(m_hDBC,NULL,(SQLTCHAR*)pcszConnStrIn,
			(SQLSMALLINT)_tcslen(pcszConnStrIn),(SQLTCHAR*)szConnStrOut,MAX_PATH,
			&cbConnStrOut,SQL_DRIVER_COMPLETE)==SQL_ERROR )
		{
			Close();
			return FALSE;
		}
		if (::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &m_hStmt)==SQL_ERROR)
		{
			Close();
			return FALSE;
		}
		return TRUE;
	}

	BOOL CULODBC::ExecuteSQL(LPCTSTR pcszQueryBuff)
	{
		::SQLCloseCursor(m_hStmt);
		if (::SQLExecDirect(m_hStmt, (SQLTCHAR*)pcszQueryBuff, SQL_NTS) 
			==SQL_ERROR)
			return FALSE;
		else
		{
			return GetData();
		}
	}

	BOOL CULODBC::GetData()
	{
		SQLSMALLINT nCols=0;
		SQLRETURN ret=SQLNumResultCols(m_hStmt, &nCols);
		if((ret==SQL_ERROR)||(ret==SQL_INVALID_HANDLE))
		{
			Close();
			return FALSE;
		}
		if(nCols<1)
			return FALSE;

		m_pColData.Resize(nCols);
        for(SQLSMALLINT i = 0;i<nCols;i++)
        {
			// Получаем информацию о столбцах
			ret=::SQLDescribeCol(m_hStmt, i+1, m_pColData[i].szColName,
				MAX_PATH, &(m_pColData[i].cbColName), &(m_pColData[i].FldSqlType),
				&(m_pColData[i].cbBuffLen), &(m_pColData[i].DecimalDigitsPtr),
				&(m_pColData[i].NullablePtr));

			if((ret==SQL_ERROR)||(ret==SQL_INVALID_HANDLE))
			{
				Close();
				return FALSE;
			}
			// биндим столбцы
			if(!BindCol((SQLUSMALLINT)i+1, m_pColData[i]))
			{
				Close();
				return FALSE;
			}
		}
		return TRUE;
	};

	BOOL CULODBC::BindCol(SQLUSMALLINT nCol, tagColData &ColData)
	{
			SQLRETURN ret;
			SQLINTEGER BuffLen;
			SQLSMALLINT FldBindType;

			switch( ColData.FldSqlType )
			{
			case SQL_REAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
					FldBindType = SQL_C_DOUBLE;
					BuffLen = sizeof(DOUBLE);
					ColData.DataPtr = (SQLPOINTER)new DOUBLE();
			break;

			case SQL_INTEGER:
			case SQL_SMALLINT:
			case SQL_TINYINT:
			case SQL_BIT:
					FldBindType = SQL_C_SLONG;
					BuffLen = sizeof(LONG);
					ColData.DataPtr = (SQLPOINTER)new LONG();
			break;

			default:
					FldBindType = SQL_C_CHAR;
					BuffLen = ColData.cbBuffLen;
					ColData.DataPtr = (SQLPOINTER)new TCHAR[BuffLen];
			}
			ret = ::SQLBindCol(m_hStmt, nCol, FldBindType, ColData.DataPtr,
					BuffLen, &ColData.StrLen_or_Ind );

			return ( ret==SQL_ERROR || ret==SQL_INVALID_HANDLE ) ? FALSE : TRUE;
	}

	BOOL CULODBC::MoveNext()
	{
		SQLRETURN ret=::SQLFetch(m_hStmt);
		if(/*(ret==SQL_ERROR)||*/(ret==SQL_INVALID_HANDLE))
		{
			Close();
			return FALSE;
		}
		if(ret==SQL_NO_DATA)
			return FALSE;
		return TRUE;
	};

	int CULODBC::GetField(int nCol,LPSTR pszField,int nLenField)
	{ 
		if((pszField==NULL)&&(m_pColData.GetSize()!=0))
			return (int)strlen((LPSTR)(LONG_PTR)m_pColData[nCol].DataPtr);
		if((pszField==NULL)||(m_pColData.GetSize()==0))
			return 0;
		if(m_pColData[nCol].DataPtr!=0)
			strcpy_s(pszField,nLenField,(LPSTR)(LONG_PTR)m_pColData[nCol].DataPtr);
		return (int)strlen(pszField);
	};

	int CULODBC::GetCollumnName(int nCol,LPTSTR pcszColName,int nLenColName)
	{
		_tcscpy_s((LPTSTR)pcszColName,nLenColName,(LPCTSTR)m_pColData[nCol].szColName);
		return (int)_tcslen((LPTSTR)pcszColName);
	};

	int CULODBC::GetCollumnCount()
	{
		return (int)m_pColData.GetSize();
	}

	void CULODBC::Close()
	{
		if (m_hStmt!=NULL)
		{
			SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
			m_hStmt=NULL;
		}
		if (m_hDBC!=NULL)
		{
			SQLDisconnect(m_hDBC);
			SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
			m_hDBC=NULL;
		}
		if (m_hEnv!=NULL)
		{
			SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
			m_hEnv=NULL;
		}
	};
	BOOL CULODBC::IsConnected(void)
	{
		return ((m_hDBC!=NULL)&&(m_hStmt!=NULL));
	}
}


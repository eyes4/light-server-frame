/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/database/CDBSrcMySql.cpp $
* @brief MySql数据库连接类
*
* $Id: CDBSrcMySql.cpp 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#include "../include/CDBSrcMySql.h"


CDBSrcMySql::CDBSrcMySql()
	: CDBSrcBase()
{
	m_pDB = NULL;
}

CDBSrcMySql::~CDBSrcMySql()
{
    DEBUGMSG("to close mysql source \n");
}

bool CDBSrcMySql::Init()
{
	m_bDbConnected = false;
	m_QueryResult = NULL;
	try
	{
		m_pDB = mysql_init((MYSQL *)0);
	}
	catch(...)
	{
		m_pDB = NULL;
		return false;
	}
	return true;
}

void CDBSrcMySql::Uninit()
{
	if(m_bDbConnected)
		Disconnect();
}

bool CDBSrcMySql::Connect(const string &strConn)
{
    try
      {
        if (m_bDbConnected) Disconnect();
        string tmp = strConn;
		m_strConnStr = strConn;
        //connstr: username/password/dbname@ip:port
        string IP, Port, Username, Password, DBName;
        Username = CutFirstField(tmp, "/");
        Password = CutFirstField(tmp, "/");
        DBName = CutFirstField(tmp, "@");
        IP = CutFirstField(tmp, ":");
        Port = tmp;
        m_DBHandle = mysql_real_connect(m_pDB, IP.c_str(), Username.c_str(), Password.c_str(), DBName.c_str(), atoi(Port.c_str()), 0, 0);
        
        if (m_DBHandle == NULL)
        {
          m_bDbConnected = false;
          fprintf(stderr, "Error: %s\n", mysql_error(m_pDB));
		  return false;
        }
        else
        {
          m_bDbConnected = true;
          mysql_query(m_DBHandle, "SET NAMES 'gb2312';");
		  mysql_query(m_DBHandle, "SET CHARACTER_SET 'gb2312';");
		  //char value = 1;
		  //mysql_options(m_DBHandle, MYSQL_OPT_RECONNECT, (char *)&value);
        }
      }
      catch(...)
      {
        return false;
      }
      return true;
}

void CDBSrcMySql::Disconnect(void)
{
	if(m_QueryResult != NULL)
	{
		mysql_free_result(m_QueryResult);
		m_QueryResult = NULL;
	}
	mysql_close(m_DBHandle);
	m_bDbConnected = false;
}

bool CDBSrcMySql::SetQuerySql(const string &strSql)
{
	if (!m_bDbConnected)
		return false;

	if(m_QueryResult != NULL)
	{
		mysql_free_result(m_QueryResult);
		m_QueryResult = NULL;
	}
	m_strSql = strSql;
	return true;
}

bool CDBSrcMySql::Open(void)
{
	int re;
	re = mysql_query(m_DBHandle, m_strSql.c_str());

	if (re != 0)
	{
		//fprintf(stderr, "Error: %s\n",mysql_error(m_DBHandle));
		//numrows= 0;
		
		int iError = mysql_errno(m_DBHandle);
		if (iError == CR_SERVER_GONE_ERROR) //2006 server has gone away
		{
			//mysql_ping(m_DBHandle);
			//mysql_query(m_DBHandle, "set names 'gb2312';");
			Uninit();
			//Disconnect();
			Init();
			while(! Connect(m_strConnStr)) 
				Disconnect();
			re = mysql_query(m_DBHandle, m_strSql.c_str());
		}
	}

	if (re == 0)
	{
		m_QueryResult = mysql_store_result(m_DBHandle);
		m_nNumRows = (unsigned int)mysql_affected_rows(m_DBHandle);
		m_nNumFields = (unsigned int)mysql_field_count(m_DBHandle);
		m_nCurrField = 0;
		m_nCurrRow = 0;

		m_Row = mysql_fetch_row(m_QueryResult);

		return true;
	}
	return false;
}

bool CDBSrcMySql::Eof(void)
{   
	if (m_QueryResult)
		return m_Row == NULL?true:false;
	return true;
}

bool CDBSrcMySql::SetExecSql(const string &strSql)
{
	if (!m_bDbConnected)
		return false;

	if(m_QueryResult != NULL)
	{
		mysql_free_result(m_QueryResult);
		m_QueryResult = NULL;
	}
	m_strSql = strSql;
	return true;
}

bool CDBSrcMySql::ExecSql(void)
{
	int re = -1;
	re = mysql_query(m_DBHandle, m_strSql.c_str());
	if(re != 0)
	{
		int iError = mysql_errno(m_DBHandle);
		if (iError == CR_SERVER_GONE_ERROR) //2006 server has gone away
		{
			//mysql_ping(m_DBHandle);
			//mysql_query(m_DBHandle, "set names 'gb2312';");
			Uninit();
			//Disconnect();
			Init();
			while(! Connect(m_strConnStr)) 
				Disconnect();
			re = mysql_query(m_DBHandle, m_strSql.c_str());
		}
	}
	
	if (re == 0)
	{
		//mysql_commit(m_DBHandle);
		return true;
	}

	return false;
}

void CDBSrcMySql::Close(void)
{
    if(m_QueryResult != NULL)
    {
        mysql_free_result(m_QueryResult);
        m_QueryResult = NULL;
    }
}

void CDBSrcMySql::SetTextParam(char *szParamName, const string &strParam)
{
	SetParam(szParamName, strParam);
}

bool CDBSrcMySql::GetValue(int &nValue)
{
	try
	{
		*this >> nValue;
	}
	catch(...)
    {
        
        return false;
    }
	return true;
}

bool CDBSrcMySql::GetValue(long long &nValue)
{
	try
	{
		*this >> nValue;
	}
	catch(...)
    {
        
        return false;
    }
	return true;
}

bool CDBSrcMySql::GetValue(double &fValue)
{
	try
	{
		*this>>fValue;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CDBSrcMySql::GetValue(char *szValue)
{
	try
	{
		*this >> szValue;
	}
	catch(...)
    {
        return false;
    }
	return true;
}

bool CDBSrcMySql::GetValue(string &strValue)
{
	//char *Res =  m_Row[m_nCurrField];
	//m_nCurrField++;
	//return Res;
	try
	{
		//strValue.empty();
		strValue = "";
		if (m_Row[m_nCurrField] != NULL)
			strValue += (char *)m_Row[m_nCurrField];
		m_nCurrField++;
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CDBSrcMySql::GetTimeValue(time_t &tTime)
{
	try
	{
		if (m_nCurrField < m_nNumFields)
        {
 
			struct tm tm1;
			sscanf(m_Row[m_nCurrField], "%4d-%2d-%2d %2d:%2d:%2d",     
						&tm1.tm_year,
						&tm1.tm_mon,
						&tm1.tm_mday,
						&tm1.tm_hour,
						&tm1.tm_min,
						&tm1.tm_sec);
			tm1.tm_year -= 1900;
			tm1.tm_mon --;

			tm1.tm_isdst=-1;
	   
			tTime = mktime(&tm1);
			m_nCurrField ++;
			return true;
        }
        else
            return false;
	}
	catch(...)
    {
        
        return false;
    }
	return true;
}

void CDBSrcMySql::SetTimeParam(char *szParamName, time_t tTime)
{
	char szTimeStr[32];
	tm* tms = localtime(&tTime);
	string param=":";
	param += szParamName;
	string value="'";
	sprintf(szTimeStr, "%d-%.2d-%.2d %.2d:%.2d:%.2d.0", 
		tms->tm_year + 1900, tms->tm_mon + 1, tms->tm_mday,
		tms->tm_hour, tms->tm_min, tms->tm_sec);
	value = value + szTimeStr + "'";
	string_replace_word(m_strSql, param, value);
}

void CDBSrcMySql::SetNowParam(char *szParamName)
{
	string param=":";
	param += szParamName;
	string_replace_word(m_strSql, param, "now()");
}

void CDBSrcMySql::First()
{
    if (m_QueryResult != NULL)
    {
        mysql_data_seek(m_QueryResult, 0);
        m_Row = mysql_fetch_row(m_QueryResult);
        m_nCurrField = 0;
    }
}

void CDBSrcMySql::Next()
{
    if (m_QueryResult != NULL)
    {
        m_Row = mysql_fetch_row(m_QueryResult);
        m_nCurrField = 0;
    }
}


bool CDBSrcMySql::StartTransaction(void)
{
	if (m_pDB == NULL) return false;
	return mysql_autocommit(m_pDB, FALSE) == 0?true:false;
}

bool CDBSrcMySql::Commit(void)
{
	if (m_pDB == NULL) return false;
	if( mysql_commit(m_pDB) == 0)
	{
		mysql_autocommit(m_pDB, 1);
		return true;
	}
	else 
		return false;
}

bool CDBSrcMySql::Rollback(void)
{
	if (m_pDB == NULL) return false;
	if(mysql_rollback(m_pDB) == 0)
	{
		mysql_autocommit(m_pDB, 1);
		return true;
	}
	else
		return false;
}


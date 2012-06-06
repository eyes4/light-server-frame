/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBSrcMySql.h $
* @brief mysql数据库连接类
*  需要库：
*    win32: 
*    linux: 
* $Id: CDBSrcMySql.h 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CDBSRCMYSQL_H__
#define __CDBSRCMYSQL_H__



#include "common.h"
#include "strutil.h"
#include "CDBSrcBase.h"
#include "./mysql5/mysql.h"
#include "./mysql5/errmsg.h"

#define DBTYPE_MYSQL 2

class CDBSrcMySql
   : public CDBSrcBase
{
public:
    CDBSrcMySql();
    ~CDBSrcMySql();
    bool Init();
    void Uninit();
    bool Connect(const string &strConn);
    void Disconnect(void);
    bool SetQuerySql(const string &strSql);
    bool Open(void);
    bool Eof(void);
    bool SetExecSql(const string &strSql);
    bool ExecSql(void);
    void Close(void);
	void SetTextParam(char *szParamName, const string &strParam);
    bool GetValue(int &nValue);
	bool GetValue(long long &nValue);
	bool GetValue(double &fValue);
    bool GetValue(char *szValue);
	bool GetValue(string &strValue);
    bool GetTimeValue(time_t &tTime);
	void SetTimeParam(char *szParamName, time_t tTime);
	void SetNowParam(char *szParamName);
    void First();
    void Next();
	bool StartTransaction(void);
	bool Commit(void);
	bool Rollback(void);

	template <typename T>
    CDBSrcMySql & operator<<(T t)
    {
        return *this;
    }

    CDBSrcMySql & operator >>(char *result)
    {
        if (m_nCurrField < m_nNumFields && m_Row != NULL)
        {
            sprintf(result, "%s", m_Row[m_nCurrField]);
            m_nCurrField ++;
        }
        else
            result = NULL;
        return *this;
    }

    CDBSrcMySql & operator >>(int &result)
    {
        if (m_nCurrField < m_nNumFields)
        {
            result = atoi(m_Row[m_nCurrField]);
            m_nCurrField ++;
        }
        else
            result = 0;
        return *this;
    }

	CDBSrcMySql & operator >>(long long &result)
    {
        if (m_nCurrField < m_nNumFields)
        {
            result = atoi(m_Row[m_nCurrField]);
            m_nCurrField ++;
        }
        else
            result = 0;
        return *this;
    }

	CDBSrcMySql & operator >>(double &result)
	{
		if (m_nCurrField < m_nNumFields)
		{
			sscanf(m_Row[m_nCurrField],"%lf",&result);
			m_nCurrField ++;
		}
		else
			result = 0.0;
		return *this;
	}

	CDBSrcMySql & operator ++(int)
    {
        Next();
        return *this;
    }
private:
	string m_strConnStr;
    MYSQL *m_pDB;
    MYSQL *m_DBHandle;
    bool m_bDbConnected;
    MYSQL_RES *m_QueryResult;
    //string m_strSql;
    unsigned int m_nCurrRow;
    unsigned int m_nNumRows;
    size_t m_nCurrField;
    unsigned int m_nNumFields;
    MYSQL_ROW m_Row;
};

#endif

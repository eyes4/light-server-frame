/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/database/CDBSrcBase.cpp $
* @brief 数据库连接基类
*
* $Id: CDBSrcBase.cpp 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#include "../include/strutil.h"
#include "../include/CDBSrcBase.h"

CDBSrcBase::CDBSrcBase()
{
	m_strSql = "";
	m_bDbConnected = false;
}

void CDBSrcBase::SetParam(char *szParamName, int nParam)
{
	string param=":";
	param += szParamName;
	string_replace_word(m_strSql, param, IntToStr(nParam));
}

void CDBSrcBase::SetParam(char *szParamName, long long nParam)
{
	string param = ":";
	param += szParamName;
	string_replace_word(m_strSql, param, Int64ToStr(nParam));
}

void CDBSrcBase::SetParam(char *szParamName, long nParam)
{
	string param=":";
	param += szParamName;
	string_replace_word(m_strSql, param, IntToStr(nParam));
}

void CDBSrcBase::SetParam(char *szParamName, char *szParam)
{
	string param=":";
	param += szParamName;
	string value= string(szParam);
	string_replace_all(value, "'", "''");
	//value = value + szParam + "'";
	value = "'" + value + "'";
	string_replace_word(m_strSql, param, value);
}

void CDBSrcBase::SetParam(char *szParamName, const string &strParam)
{
	string param=":";
	//string_replace_all(strParam, "'", "''");
	param += szParamName;
	string value=strParam;
	string_replace_all(value, "'", "''");
	string_replace_word(m_strSql, param, "'"+value+"'");
}

const char *CDBSrcBase::GetSql(void)
{
	return m_strSql.c_str();
}

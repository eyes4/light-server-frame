/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/database/CDBSrcOracle.cpp $
* @brief Oracle数据库连接类
*
* $Id: CDBSrcOracle.cpp 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#include "../include/CDBSrcOracle.h"


CDBSrcOracle::CDBSrcOracle()
	: CDBSrcBase()
{
	m_str = "";
	m_bTranscation = false;
}

CDBSrcOracle::~CDBSrcOracle()
{
    DEBUGMSG("to close oracle source \n");
}

bool CDBSrcOracle::Init()
{
	m_bDbConnected = false;
	try
	{
		otl_connect::otl_initialize(); // initialize OCI environment
		
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void CDBSrcOracle::Uninit()
{
    Close();
    m_DB.logoff();
}

bool CDBSrcOracle::Connect(const string &strConn)
{
    try
    {
        DEBUGMSG("to connect db:%s\n", strConn.c_str());
        m_DB.rlogon(strConn.c_str(), 1);
    }
    catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
    m_bDbConnected = true;
    return true;
}

void CDBSrcOracle::Disconnect(void)
{
    m_DB.logoff();
    m_bDbConnected = false;
}

bool CDBSrcOracle::SetQuerySql(const string &strSql)
{
    m_Stream.close();
    m_strSql = strSql;
    return true;
}

bool CDBSrcOracle::Open(void)
{
    DEBUGMSG("to open: %s %d\n" , m_strSql.c_str(), &m_DB);
	try
	{
		m_Stream.open(150, m_strSql.c_str(), m_DB);
	}
    catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
    return true;
}

bool CDBSrcOracle::Eof(void)
{   
    return m_Stream.eof()==1?true:false;
}

bool CDBSrcOracle::SetExecSql(const string &strSql)
{
    m_Stream.close();
    DEBUGMSG("to set sql:%s\n", strSql.c_str());
    m_strSql = strSql;
    DEBUGMSG("set sql ok\n");
    return true;
}

bool CDBSrcOracle::ExecSql(void)
{
	try
	{
		m_Stream.open(1, m_strSql.c_str(), m_DB);
		if (m_str != "")//如果m_str不为空串，表示有长字符串字段参数需要处理
		{
			otl_long_string param(20000000);

			otl_lob_stream lob;

			memcpy(&param[0], m_str.c_str(), m_str.size());
			param.set_len(m_str.size()+1);
			m_Stream<<lob;
			lob.set_len(m_str.size()+1);
			lob<<param;
			lob.close();
			m_DB.commit();
			m_str = "";
			if (!m_bTranscation)
			{
				m_DB.auto_commit_on();
				m_Stream.set_commit(1);
			}
		}
		
	}
    catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
    return true;
}

void CDBSrcOracle::Close(void)
{
    DEBUGMSG("to close\n");
    m_Stream.close();
    DEBUGMSG("closed\n");
}

void CDBSrcOracle::SetTextParam(char *szParamName, const string &strParam)
{
	m_DB.auto_commit_off();
	m_Stream.set_commit(0);
	string sparam=":";
	sparam += szParamName;
	string_replace_word(m_strSql, sparam, "empty_clob()");
	m_strSql =m_strSql + " returning "+szParamName +" into :" +szParamName+"<clob>";
	printf("%s\n", m_strSql.c_str());
	m_str = strParam;
	
}

bool CDBSrcOracle::GetValue(int &nValue)
{
	try
	{
		m_Stream >> nValue;
	}
	catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
	return true;
}

bool CDBSrcOracle::GetValue(char *szValue)
{
	try
	{
		m_Stream >> szValue;
	}
	catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
	return true;
}

bool CDBSrcOracle::GetValue(string &strValue)
{
	try
	{
		otl_long_string f2(20000);
		otl_lob_stream lob;
		strValue = "";
		m_Stream >> lob;

		//int n=0;
		while(!lob.eof())// read while not "end-of-file" -- end of CLOB
		{ 
			//++n;
			lob>>f2; // reading a chunk of CLOB
			//cout<<"   chunk #"<<n;
			//cout<<&f2[0]<<endl;

			strValue += (char *)&f2[0];
		}
		lob.close(); 
	}
	catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
	return true;
}

bool CDBSrcOracle::GetTimeValue(time_t &tTime)
{
	try
	{
		otl_datetime f1;
		m_Stream >> f1;
		struct tm t;
		t.tm_year = f1.year;
		t.tm_mon = f1.month;
		t.tm_mday = f1.day;
		t.tm_hour = f1.hour;
		t.tm_min = f1.minute;
		t.tm_sec = f1.second;
		tTime = mktime(&t);
	}
	catch(otl_exception& p)
    {
        cout<<p.msg<<endl;
        return false;
    }
	return true;
}

void CDBSrcOracle::SetTimeParam(char *szParamName, time_t tTime)
{
	char szTimeStr[32];
	tm* tms = localtime(&tTime);
	string param=":";
	param += szParamName;
	string value="to_timestamp('";
	sprintf(szTimeStr, "%d-%.2d-%.2d %.2d:%.2d:%.2d.0", 
		tms->tm_year + 1900, tms->tm_mon + 1, tms->tm_mday,
		tms->tm_hour, tms->tm_min, tms->tm_sec);
	value = value + szTimeStr + "', 'yyyy-mm-dd hh24:mi:ss.ff')";
	string_replace_word(m_strSql, param, value);
}

void CDBSrcOracle::SetNowParam(char *szParamName)
{
	string param=":";
	param += szParamName;
	string_replace_word(m_strSql, param, "sysdate / 86400");
}

void CDBSrcOracle::First()
{
    (*m_Stream.ss)->rewind();//first();
}

void CDBSrcOracle::Next()
{
	//OTL在取记录时，指针会自动后移，为了与mysql兼容，这里忽略
    //(*m_Stream.ss)->next();
}

bool CDBSrcOracle::StartTransaction(void)
{
	/*m_Stream.close();
	m_Stream.open(50, "begin transaction;", m_DB);
	m_Stream.close();*/
	m_bTranscation = true;
	m_DB.auto_commit_off();
	m_Stream.set_commit(0);

	return true;
}

bool CDBSrcOracle::Commit(void)
{
	m_DB.commit();
	m_DB.auto_commit_on();
	m_Stream.set_commit(1);
	m_bTranscation = false;
	return true;
}

bool CDBSrcOracle::Rollback(void)
{
	m_DB.rollback();
	m_DB.auto_commit_on();
	m_Stream.set_commit(1);
	m_bTranscation = false;
	return true;
}

/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBSrcOracle.h $
* @brief oracle数据库连接类
*  需要库：
*    win32: oci.lib ociw32.lib
*    linux: libclntsh.so libnnz10.so libmysqlclient.so 
* $Id: CDBSrcOracle.h 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CDBSRCORACLE_H__
#define __CDBSRCORACLE_H__

#ifdef WIN32
#pragma   comment(lib, "oci.lib") 
#pragma   comment(lib, "ociw32.lib")   
#endif

#define OTL_ORA9I 						///< 编译 OTL 4.0/OCI9i，使用oracle9i，如要使用10g，则宏为OTL_ORA10G 					// Compile OTL 4.0/OCI10g
#define OTL_ORA TIMESTAMP				//enable Oracle 9i TIMESTAMPs [with [local] time zone]
#define OTL_STREAM_READ_ITERATOR_ON

#include "common.h"
#include "strutil.h"
#include "CDBSrcBase.h"
#include "./oracle9i/otlv4.h"

#define DBTYPE_ORA9I 1

class CDBSrcOracle
   : public CDBSrcBase
{
public:
    CDBSrcOracle();
    ~CDBSrcOracle();
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
    CDBSrcOracle & operator<<(T t)
    {
        m_Stream<<t;
        return *this;
    }
    template <typename T>
    CDBSrcOracle & operator>>(T &t)
    {
        m_Stream>>t;
        return *this;
    }
	CDBSrcOracle & operator++(int)
    {
        (*m_Stream.ss)->next();
        return *this;
    }
private:
    otl_connect m_DB; 			// connect object
    otl_stream m_Stream;
	string m_str;
	bool m_bTranscation;
};

#endif

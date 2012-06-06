/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBSrcBase.h $
* @brief ���ݿ�������
*  ʵ�������ݿ����ӡ���ѯ������ɾ���Ȼ������ܣ�δʵ��������
* $Id: CDBSrcBase.h 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CDBSOURCE_H__
#define __CDBSOURCE_H__


#include <iostream>
#include <string>
using namespace std;

/**
* @brief ���ݿ����ӻ���
*  ���ݿ����ӻ��࣬���������ݿ����Ӽ����ݷ��ʵĽӿں�����
*  ���нӿں���������������ʵ��
*/
class CDBSrcBase
{

public:
	/**
	* @brief Ĭ�ϵĹ��캯��
	*/
	CDBSrcBase();
	/**
	* @brief Ĭ�ϵ���������
	*/
    virtual ~CDBSrcBase() {};
    
	/**
	* @brief ��ʼ�����ݿ�����
	*/
    virtual bool Init(void) = 0;

	/**
	* @brief ����ʼ�����ݿ�����
	*/
    virtual void Uninit(void) = 0;
    
	/**
	* @brief ͨ�������ַ����������ݿ�
	* 
	* @param strConn ���ݿ������ַ�������ʽ�ɾ�������������壬��ͬ�����࣬��ʽ�ɲ�ͬ
	*   - oracle�����ַ����ĸ�ʽΪ �û���/����@������
	*   - mysql�����ַ�����ʽΪ �û���/����/���ݿ���@��������:�˿ں�
	*/
    virtual bool Connect(const string &strConn) = 0;
    
	/**
	* @brief �Ͽ����ݿ�����
	*/
    virtual void Disconnect(void) = 0;
    
	/**
	* @brief ����ѯ�ļ�¼�Ƿ��Ѿ�����
	*/
    virtual bool Eof(void) = 0;

    /**
	* @brief ���ò�ѯsql���
	*
	* @param strSql ���ڲ�ѯ��sql���
	* @return ִ�н��
	* @retval true ��ȷִ��
	* @retval false ʧ��
	*/
    virtual bool SetQuerySql(const string &strSql) = 0;
    
	/**
	* @brief ִ��ʹ��SetQuerySql���õ�Sql�����в�ѯ
	*
	* @return ִ�н��
	* @retval true ��ȷִ��
	* @retval false ʧ��
	*/
    virtual bool Open(void) = 0;

	/**
	* @brief �رղ�ѯ
	*/
    virtual void Close(void) = 0;
    
	/**
	* @brief ��ѯʱ���������ݼ��ĵ�һ����¼
	*/
    virtual void First(void) = 0;

    /**
	* @brief ��ѯʱ���������ݼ��е�ǰ�α����һ����¼
	*/
    virtual void Next(void) = 0;
    
	/**
	* @brief ����ִ��sql���
	* 
	* @param strSql ����ִ�е�sql���
	* @return ִ�н��
	* @retval true ��ȷִ��
	* @retval false ʧ��
	*/
    virtual bool SetExecSql(const string &strSql) = 0;
    
	/**
	* @brief ִ��ʹ��SetExecSql���õ�sql���
	*
	* @return ִ�н��
	* @retval true ��ȷִ��
	* @retval false ʧ��
	*/
    virtual bool ExecSql() = 0;

    /**
	* @brief ������������
	* 
	* @param szParamName ������
	* @param nParam ���������
	*/
    virtual void SetParam(char *szParamName, int nParam);

	/**
	* @brief ��ӳ����������
	* 
	* @param szParamName ������
	* @param nParam �����������
	*/
    virtual void SetParam(char *szParamName, long nParam);

	/**
	* @brief ��ӳ������������
	* 
	* @param szParamName ������
	* @param nParam �������������
	*/
	virtual void SetParam(char *szParamName, long long nParam);

	/**
	* @brief ����ַ��������
	* 
	* @param szParamName ������
	* @param szParam �ַ��������
	*/
    virtual void SetParam(char *szParamName, char *szParam);

	/**
	* @brief ���string�����
	* 
	* @param szParamName ������
	* @param strParam string�����
	*/
    virtual void SetParam(char *szParamName, const string &strParam);

	/**
	* @brief ���ʱ���Ͳ���
	* 
	* @param szParamName ������
	* @param tTime ʱ���Ͳ��������ڸ������ݿ��sql����У�ʱ��ı�ʾ����һ������˴˺�����Ҫ������ʵ��
	*/
    virtual void SetTimeParam(char *szParamName, time_t tTime) = 0;

	/**
	* @brief ��ӵ�ǰʱ����������������滻�ɵ�ǰʱ��now
	*
	* @param szParamName ������
	*/
	virtual void SetNowParam(char *szParamName) = 0;

	/**
	* @brief ��ӳ��ַ����������mysql:text���ֶΣ�oracle:clob���ֶΣ�
	* 
	* @param szParamName ������
	* @param strParam string�����
	*/
    virtual void SetTextParam(char *szParamName, const string &strParam) = 0;

    /*//����ַ���ת�����Ͳ���
    virtual void AppendChar2DateParam(const char *szDate, char *szFormat) = 0;
    //����ַ���תʱ���Ͳ�����oracle-timestamps  mysql-datetime)
    virtual void AppendChar2DtParam(const char *szTime, char *szFormat) = 0;
    //����Զ������ֶβ�����szNameΪ�Զ�����������nDirectΪ���򣬣�1ʱ������
    virtual void AppendAutoIncParam(const char *szName, int nDirect = 0) = 0;
    //��ӽ�ʱ��ת��Ϊ�ַ��͵��ֶ�
    virtual void AppendDt2CharFieldParam(char *szFieldName, char *szFormat, int nIncSec = 0) = 0;//Datetime to char
    //��ӵ�ǰʱ�亯��
    virtual void AppendNowFieldParam(int nDiffSec = 0) = 0;
    //����Ӵ�����
    virtual void AppendSubstrParam(const char *szFieldName, int nPos, int nLen) = 0;
    //virtual void AppendFormatDatetime(char *szFieldName, char *szFormat) = 0;*/
    
	/**
	* @brief ȡsql��ѯ���ص�������ֵ������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param nValue ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
    virtual bool GetValue(int &nValue) = 0;

	/**
	* @brief ȡsql��ѯ���صĳ�����������ֵ������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param nValue ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
    virtual bool GetValue(long long &nValue) = 0;

	/**
	* @brief ȡsql��ѯ���صĸ�����ֵ������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param fValue ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
	virtual bool GetValue(double &fValue) = 0;


	/**
	* @brief ȡsql��ѯ���ص��ַ�����ֵ������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param szValue ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
    virtual bool GetValue(char *szValue) = 0;

	/**
	* @brief ȡsql��ѯ���ص�string��ֵ(mysql text�ͣ�oracle clob��)������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param strValue ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
	virtual bool GetValue(string &strValue) = 0;

	/**
	* @brief ȡsql��ѯ���ص�ʱ����ֵ������GetValue����������ز�ѯ���õĸ��ֶ�ֵ
	* @param tTime ��ѯ����ֵ
	* @return �����Ƿ���ȷ��ȡ������ֵ
	* @retval true ��ȷ
	* @retval false ʧ��
	*/
    virtual bool GetTimeValue(time_t &tTime) = 0;

    /**
	* @brief ȡ��ǰ����ִ�е�sql���
	* @return ��ǰִ�е�sql���
	*/
    virtual const char *GetSql(void);

	/**
	* @brief ��sql����еĲ�����ֵ
	*/
	template <typename T>
		CDBSrcBase & operator<<(T t){};
	
	/**
	* @brief �����ѯ���
	*/
    template <typename T>
		CDBSrcBase & operator>>(T &t){};

	/**
	* @brief ��ѯ�õ������ݼ���¼ָ�����һλ
	*/
	virtual CDBSrcBase & operator++(int) = 0;

	/**
	* @brief ��ʼ������
	*/
	virtual bool StartTransaction(void) = 0;

	/**
	* @brief �ύ����
	*/
	virtual bool Commit(void) = 0;

	/**
	* @brief �ع�����
	*/
	virtual bool Rollback(void) = 0;
protected:
    string m_strSql;
    bool m_bDbConnected;
};

#endif

/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBSrcBase.h $
* @brief 数据库连接类
*  实现了数据库连接、查询、插入删除等基本功能，未实现事务功能
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
* @brief 数据库连接基类
*  数据库连接基类，定义了数据库连接及数据访问的接口函数，
*  所有接口函数均需在子类中实现
*/
class CDBSrcBase
{

public:
	/**
	* @brief 默认的构造函数
	*/
	CDBSrcBase();
	/**
	* @brief 默认的析构函数
	*/
    virtual ~CDBSrcBase() {};
    
	/**
	* @brief 初始化数据库连接
	*/
    virtual bool Init(void) = 0;

	/**
	* @brief 反初始化数据库连接
	*/
    virtual void Uninit(void) = 0;
    
	/**
	* @brief 通过连接字符串连接数据库
	* 
	* @param strConn 数据库连接字符串，格式由具体的子类来定义，不同的子类，格式可不同
	*   - oracle连接字符串的格式为 用户名/密码@连接名
	*   - mysql连接字符串格式为 用户名/密码/数据库名@服务器名:端口号
	*/
    virtual bool Connect(const string &strConn) = 0;
    
	/**
	* @brief 断开数据库连接
	*/
    virtual void Disconnect(void) = 0;
    
	/**
	* @brief 检查查询的记录是否已经到底
	*/
    virtual bool Eof(void) = 0;

    /**
	* @brief 设置查询sql语句
	*
	* @param strSql 用于查询的sql语句
	* @return 执行结果
	* @retval true 正确执行
	* @retval false 失败
	*/
    virtual bool SetQuerySql(const string &strSql) = 0;
    
	/**
	* @brief 执行使用SetQuerySql设置的Sql语句进行查询
	*
	* @return 执行结果
	* @retval true 正确执行
	* @retval false 失败
	*/
    virtual bool Open(void) = 0;

	/**
	* @brief 关闭查询
	*/
    virtual void Close(void) = 0;
    
	/**
	* @brief 查询时，跳到数据集的第一条记录
	*/
    virtual void First(void) = 0;

    /**
	* @brief 查询时，跳到数据集中当前游标的下一条记录
	*/
    virtual void Next(void) = 0;
    
	/**
	* @brief 设置执行sql语句
	* 
	* @param strSql 用于执行的sql语句
	* @return 执行结果
	* @retval true 正确执行
	* @retval false 失败
	*/
    virtual bool SetExecSql(const string &strSql) = 0;
    
	/**
	* @brief 执行使用SetExecSql设置的sql语句
	*
	* @return 执行结果
	* @retval true 正确执行
	* @retval false 失败
	*/
    virtual bool ExecSql() = 0;

    /**
	* @brief 添加整型类参数
	* 
	* @param szParamName 参数名
	* @param nParam 整型类参数
	*/
    virtual void SetParam(char *szParamName, int nParam);

	/**
	* @brief 添加长整型类参数
	* 
	* @param szParamName 参数名
	* @param nParam 长整型类参数
	*/
    virtual void SetParam(char *szParamName, long nParam);

	/**
	* @brief 添加长长整型类参数
	* 
	* @param szParamName 参数名
	* @param nParam 长长整型类参数
	*/
	virtual void SetParam(char *szParamName, long long nParam);

	/**
	* @brief 添加字符串类参数
	* 
	* @param szParamName 参数名
	* @param szParam 字符串类参数
	*/
    virtual void SetParam(char *szParamName, char *szParam);

	/**
	* @brief 添加string类参数
	* 
	* @param szParamName 参数名
	* @param strParam string类参数
	*/
    virtual void SetParam(char *szParamName, const string &strParam);

	/**
	* @brief 添加时间型参数
	* 
	* @param szParamName 参数名
	* @param tTime 时间型参数，由于各种数据库的sql语句中，时间的表示都不一样，因此此函数需要由子类实现
	*/
    virtual void SetTimeParam(char *szParamName, time_t tTime) = 0;

	/**
	* @brief 添加当前时间参数，即将参数替换成当前时间now
	*
	* @param szParamName 参数名
	*/
	virtual void SetNowParam(char *szParamName) = 0;

	/**
	* @brief 添加长字符串类参数（mysql:text类字段，oracle:clob类字段）
	* 
	* @param szParamName 参数名
	* @param strParam string类参数
	*/
    virtual void SetTextParam(char *szParamName, const string &strParam) = 0;

    /*//添加字符串转日期型参数
    virtual void AppendChar2DateParam(const char *szDate, char *szFormat) = 0;
    //添加字符串转时间型参数（oracle-timestamps  mysql-datetime)
    virtual void AppendChar2DtParam(const char *szTime, char *szFormat) = 0;
    //添加自动增量字段参数，szName为自动增量表名，nDirect为方向，－1时负增长
    virtual void AppendAutoIncParam(const char *szName, int nDirect = 0) = 0;
    //添加将时间转化为字符型的字段
    virtual void AppendDt2CharFieldParam(char *szFieldName, char *szFormat, int nIncSec = 0) = 0;//Datetime to char
    //添加当前时间函数
    virtual void AppendNowFieldParam(int nDiffSec = 0) = 0;
    //添加子串函数
    virtual void AppendSubstrParam(const char *szFieldName, int nPos, int nLen) = 0;
    //virtual void AppendFormatDatetime(char *szFieldName, char *szFormat) = 0;*/
    
	/**
	* @brief 取sql查询返回的数字型值，调用GetValue函数逐个返回查询所得的各字段值
	* @param nValue 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
    virtual bool GetValue(int &nValue) = 0;

	/**
	* @brief 取sql查询返回的长长型数字型值，调用GetValue函数逐个返回查询所得的各字段值
	* @param nValue 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
    virtual bool GetValue(long long &nValue) = 0;

	/**
	* @brief 取sql查询返回的浮点型值，调用GetValue函数逐个返回查询所得的各字段值
	* @param fValue 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
	virtual bool GetValue(double &fValue) = 0;


	/**
	* @brief 取sql查询返回的字符串型值，调用GetValue函数逐个返回查询所得的各字段值
	* @param szValue 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
    virtual bool GetValue(char *szValue) = 0;

	/**
	* @brief 取sql查询返回的string型值(mysql text型，oracle clob型)，调用GetValue函数逐个返回查询所得的各字段值
	* @param strValue 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
	virtual bool GetValue(string &strValue) = 0;

	/**
	* @brief 取sql查询返回的时间型值，调用GetValue函数逐个返回查询所得的各字段值
	* @param tTime 查询返回值
	* @return 返回是否正确获取到返回值
	* @retval true 正确
	* @retval false 失败
	*/
    virtual bool GetTimeValue(time_t &tTime) = 0;

    /**
	* @brief 取当前用于执行的sql语句
	* @return 当前执行的sql语句
	*/
    virtual const char *GetSql(void);

	/**
	* @brief 给sql语句中的参数赋值
	*/
	template <typename T>
		CDBSrcBase & operator<<(T t){};
	
	/**
	* @brief 输出查询结果
	*/
    template <typename T>
		CDBSrcBase & operator>>(T &t){};

	/**
	* @brief 查询得到的数据集记录指针后移一位
	*/
	virtual CDBSrcBase & operator++(int) = 0;

	/**
	* @brief 开始事务处理
	*/
	virtual bool StartTransaction(void) = 0;

	/**
	* @brief 提交事务
	*/
	virtual bool Commit(void) = 0;

	/**
	* @brief 回滚事务
	*/
	virtual bool Rollback(void) = 0;
protected:
    string m_strSql;
    bool m_bDbConnected;
};

#endif

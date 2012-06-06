/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBConnPool.h $
* @brief 数据库连接池类
*
* $Id: CDBConnPool.h 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CDBCONNPOOL_H__
#define __CDBCONNPOOL_H__

#include <set>
#include <string>
#include <vector>

#include "common_thread.h"
#include "CSingleton.h"
#include "CDBSrcBase.h"
#ifdef DB_MYSQL
#include "CDBSrcMySql.h"
#endif
#ifdef DB_ORA9I
#include "CDBSrcOracle.h"
#endif

using namespace std;

class CDBConnPoolBase
{
public:
    virtual bool Init(const string strConnStr,  unsigned int nMin=5, unsigned int nMax=200) = 0;
  virtual void Uninit(void) = 0;
    virtual CDBSrcBase *PopDBSrc(void) = 0;
    virtual void PushDBSrc(CDBSrcBase *pDBSrc) = 0;
    virtual int IdleDBSrcCount(void) = 0;
    virtual int BusyDBSrcCount(void) = 0;
    virtual ~CDBConnPoolBase(){}
};

template <class CDBSrc>
class CDBConnPool
  : public CDBConnPoolBase,
    public CSingleton<CDBConnPool<CDBSrc> >
{
public:
  /**
  * @brief 数据库连接池构造函数
  */
  CDBConnPool();

  /**
  * @brief 数据库连接池析构函数
  */
  ~CDBConnPool();

  /**
  * @brief 初始化数据库连接池
  * @param strConnStr 数据库连接字符串
  * @param nMin 数据库连接池中最小连接个数。在数据库连接池初始化时，预生成的连接个数。
  * @param nMax 数据库连接池中最大连接个数。在连接池使用过程中，当池中的连接不够时，
  *       自动新建连接，但池中所有连接（包括已使用和未使用）的总数不超过nMax
  * @return 返回初始化是否成功
  * @retval true 初始化成功
  * @retval false 初始化失败
  */
  bool Init(const string strConnStr,  unsigned int nMin, unsigned int nMax);

  /**
  * @brief 反初始化数据库连接池，用以释放资源
  */
  void Uninit(void);

  /**
  * @brief 从连接池中获取空闲的数据库连接
  * @return 数据库连接
  * @retval NULL 连接池中无空闲连接
  * @retval 非NULL 空闲连接
  */
  CDBSrc *PopDBSrc(void);

  /**
  * @brief 将使用完后的数据库连接归还到连接池中去
  * @param pDBSrc 数据库连接
  */
  void PushDBSrc(CDBSrcBase *pDBSrc);

  /**
  * @brief 获取池中空闲的数据库连接个数
  * @return 池中空闲的数据库连接个数
  */
  int IdleDBSrcCount(void);

  /**
  * @brief 获取池中正在被使用的数据库连接个数
  * @return 池中正在被使用的数据库连接个数
  */
  int BusyDBSrcCount(void);
private:
  bool m_bInited;             /**<数据库连接池是否已初始化*/
  vector<CDBSrc *> m_IdleDBSrcPool; /**<空闲连接池*/
  set<CDBSrc *> m_BusyDBSrcPool;    /**<繁忙连接池*/
  string m_strConnStr;          /**<数据库连接字符串，在新建连接或重建连接时使用*/
  unsigned int m_nMinConnNum;       /**<池中最小连接个数*/
  unsigned int m_nMaxConnNum;       /**<池中最大连接个数*/
  unsigned int m_nCurrConnNum;      /**<池中当前连接个数*/
  ThreadHandle m_hThread;         /**<建立数据库连接的工作线程句柄*/
  ThreadMutex m_Mutex;          /**<临界区*/

  /**
  * @brief 建立新的数据库连接，桥接函数，桥接到NewDBSrcConn。
  */
  static void *_NewDBSrcConn(void *context); 

  /**
  * @brief 建立新的数据库连接
  */
  void NewDBSrcConn(void);

  /**
  * @brief 声明与单件类为友元
  */
    friend class CSingleton<CDBConnPool<CDBSrc> >;
};

template <class CDBSrc>
CDBConnPool<CDBSrc>::CDBConnPool()
{
    m_bInited = false;
  InitMutex(&m_Mutex);
}

template <class CDBSrc>
CDBConnPool<CDBSrc>::~CDBConnPool()
{
    DEBUGMSG("Clear database connect pool\n");
    int i;
    for (i = 0; i < (int)m_IdleDBSrcPool.size(); i++)
    {
    ((CDBSrc *)(m_IdleDBSrcPool[i]))->Uninit();
    delete (CDBSrc *)(m_IdleDBSrcPool[i]);
    }
    m_IdleDBSrcPool.clear();
  
    typename set<CDBSrc *>::iterator BusyDBSrcPoolIter;
    for (BusyDBSrcPoolIter = m_BusyDBSrcPool.begin(); BusyDBSrcPoolIter != m_BusyDBSrcPool.end();
++BusyDBSrcPoolIter)
    {
    ((CDBSrc *)(*BusyDBSrcPoolIter))->Uninit();
    delete (CDBSrc *)(*BusyDBSrcPoolIter);
    }
    m_BusyDBSrcPool.clear();
  DestroyMutex(&m_Mutex);

}

template <class CDBSrc>
bool CDBConnPool<CDBSrc>::Init(const string strConnStr, unsigned int nMin=5, unsigned int nMax=200)
{
    if (m_bInited)
        return true;
  DEBUGMSG("Init database connect pool, connect string:%s\n" , strConnStr.c_str());
    m_strConnStr = strConnStr;
    m_nMinConnNum = nMin;
    m_nMaxConnNum = nMax;
    m_nCurrConnNum = 0;
    m_IdleDBSrcPool.reserve(nMax);
    NewDBSrcConn();
    while(m_IdleDBSrcPool.size() < nMin)
  {
        SleepMs(1000);
  }
    m_bInited = true;
    return true;
}

template <class CDBSrc>
void CDBConnPool<CDBSrc>::Uninit(void)
{
    CDBSrc  *pDBSrc = NULL;
    while (m_BusyDBSrcPool.size() > 0)
    {
        pDBSrc = static_cast<CDBSrc *> (*m_BusyDBSrcPool.begin());
        pDBSrc->Close();
        m_BusyDBSrcPool.erase(pDBSrc);
        m_IdleDBSrcPool.push_back(pDBSrc);
    }

    for(int i = 0; i < (int)m_IdleDBSrcPool.size(); i++)
    {
        pDBSrc = static_cast<CDBSrc *> (m_IdleDBSrcPool[i]);
        pDBSrc->Disconnect();
        pDBSrc->Uninit();
        delete pDBSrc;
        pDBSrc = NULL;
    }
}

template <class CDBSrc>
CDBSrc *CDBConnPool<CDBSrc>::PopDBSrc(void)
{
    EnterMutex(&m_Mutex);
    CDBSrc  *pDBSrc;
    if (m_IdleDBSrcPool.size() <= 0 && m_nCurrConnNum <= m_nMaxConnNum)
    {
        try
        {
            pDBSrc = new CDBSrc();
            pDBSrc->Init();
            DEBUGMSG("to connect db ID:%d\n", m_IdleDBSrcPool.size());
            pDBSrc->Connect(m_strConnStr);
            m_IdleDBSrcPool.push_back(pDBSrc);
            DEBUGMSG(" connected db \n");
            m_nCurrConnNum ++;
        }
        catch(...)
        {
            pDBSrc->Uninit();
            delete pDBSrc;
            pDBSrc = NULL;
      LeaveMutex(&m_Mutex);

            return NULL;
        }
    }
    if (m_IdleDBSrcPool.size() <= 0)
    {
    LeaveMutex(&m_Mutex);
        return NULL;
    }
    pDBSrc = m_IdleDBSrcPool.back();
    if (pDBSrc != NULL )
    {
        m_BusyDBSrcPool.insert(pDBSrc);
        m_IdleDBSrcPool.pop_back();
    }
  LeaveMutex(&m_Mutex);

    return (CDBSrc *)pDBSrc;
}

template <class CDBSrc>
void CDBConnPool<CDBSrc>::PushDBSrc(CDBSrcBase *pDBSrc)
{
    EnterMutex(&m_Mutex);
    //DEBUGMSG("release m_UsedConnPool:%d\n", m_BusyDBSrcPool.size());
    m_BusyDBSrcPool.erase((CDBSrc *)pDBSrc);
    //DEBUGMSG("release db connect:%d\n", m_IdleDBSrcPool.size());
    m_IdleDBSrcPool.push_back((CDBSrc *)pDBSrc);
    //    DEBUGMSG("releaseed\n");
    LeaveMutex(&m_Mutex);
}

template <class CDBSrc>
int CDBConnPool<CDBSrc>::IdleDBSrcCount(void)
{
    return (int)m_IdleDBSrcPool.size();
}

template <class CDBSrc>
int CDBConnPool<CDBSrc>::BusyDBSrcCount(void)
{
    return (int)m_BusyDBSrcPool.size();
}


template <class CDBSrc>
void *CDBConnPool<CDBSrc>::_NewDBSrcConn(void *context)
{
    CDBConnPool<CDBSrc> *pPool = static_cast<CDBConnPool<CDBSrc> *> (context);
    pPool->NewDBSrcConn();
}

template <class CDBSrc>
void CDBConnPool<CDBSrc>::NewDBSrcConn(void)
{
    CDBSrc *pDBSrc = NULL;
    while(m_IdleDBSrcPool.size() < m_nMinConnNum)
    {
        try
        {
            pDBSrc = new CDBSrc();
            if (!pDBSrc->Init()) continue;
            if (!pDBSrc->Connect(m_strConnStr)) continue;
            m_IdleDBSrcPool.push_back(pDBSrc);
            m_nCurrConnNum ++;
        }
        catch(...)
        {
            pDBSrc->Uninit();
            delete pDBSrc;
            pDBSrc = NULL;
            SleepMs(1000);
        }
    }
}

class CDBConnPoolFactory
{
  public:
    static CDBConnPoolBase *GetDBPool(int nType = 0)
    {
    static CDBConnPoolBase *pDBConnPool = NULL;
      switch(nType)
      {
	  case 0://空，只是为了防止出现编辑警告
		  break;
#ifdef DB_ORA9I
        case DBTYPE_ORA9I:
          pDBConnPool = CDBConnPool<CDBSrcOracle>::Instance();
      break;
#endif
#ifdef DB_MYSQL
        case DBTYPE_MYSQL:
          pDBConnPool = CDBConnPool<CDBSrcMySql>::Instance();
      break;
#endif
		default:
			break;
      }
    return pDBConnPool;
    }
 
};

#endif

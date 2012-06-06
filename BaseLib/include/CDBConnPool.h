/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CDBConnPool.h $
* @brief ���ݿ����ӳ���
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
  * @brief ���ݿ����ӳع��캯��
  */
  CDBConnPool();

  /**
  * @brief ���ݿ����ӳ���������
  */
  ~CDBConnPool();

  /**
  * @brief ��ʼ�����ݿ����ӳ�
  * @param strConnStr ���ݿ������ַ���
  * @param nMin ���ݿ����ӳ�����С���Ӹ����������ݿ����ӳس�ʼ��ʱ��Ԥ���ɵ����Ӹ�����
  * @param nMax ���ݿ����ӳ���������Ӹ����������ӳ�ʹ�ù����У������е����Ӳ���ʱ��
  *       �Զ��½����ӣ��������������ӣ�������ʹ�ú�δʹ�ã�������������nMax
  * @return ���س�ʼ���Ƿ�ɹ�
  * @retval true ��ʼ���ɹ�
  * @retval false ��ʼ��ʧ��
  */
  bool Init(const string strConnStr,  unsigned int nMin, unsigned int nMax);

  /**
  * @brief ����ʼ�����ݿ����ӳأ������ͷ���Դ
  */
  void Uninit(void);

  /**
  * @brief �����ӳ��л�ȡ���е����ݿ�����
  * @return ���ݿ�����
  * @retval NULL ���ӳ����޿�������
  * @retval ��NULL ��������
  */
  CDBSrc *PopDBSrc(void);

  /**
  * @brief ��ʹ���������ݿ����ӹ黹�����ӳ���ȥ
  * @param pDBSrc ���ݿ�����
  */
  void PushDBSrc(CDBSrcBase *pDBSrc);

  /**
  * @brief ��ȡ���п��е����ݿ����Ӹ���
  * @return ���п��е����ݿ����Ӹ���
  */
  int IdleDBSrcCount(void);

  /**
  * @brief ��ȡ�������ڱ�ʹ�õ����ݿ����Ӹ���
  * @return �������ڱ�ʹ�õ����ݿ����Ӹ���
  */
  int BusyDBSrcCount(void);
private:
  bool m_bInited;             /**<���ݿ����ӳ��Ƿ��ѳ�ʼ��*/
  vector<CDBSrc *> m_IdleDBSrcPool; /**<�������ӳ�*/
  set<CDBSrc *> m_BusyDBSrcPool;    /**<��æ���ӳ�*/
  string m_strConnStr;          /**<���ݿ������ַ��������½����ӻ��ؽ�����ʱʹ��*/
  unsigned int m_nMinConnNum;       /**<������С���Ӹ���*/
  unsigned int m_nMaxConnNum;       /**<����������Ӹ���*/
  unsigned int m_nCurrConnNum;      /**<���е�ǰ���Ӹ���*/
  ThreadHandle m_hThread;         /**<�������ݿ����ӵĹ����߳̾��*/
  ThreadMutex m_Mutex;          /**<�ٽ���*/

  /**
  * @brief �����µ����ݿ����ӣ��ŽӺ������Žӵ�NewDBSrcConn��
  */
  static void *_NewDBSrcConn(void *context); 

  /**
  * @brief �����µ����ݿ�����
  */
  void NewDBSrcConn(void);

  /**
  * @brief �����뵥����Ϊ��Ԫ
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
	  case 0://�գ�ֻ��Ϊ�˷�ֹ���ֱ༭����
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

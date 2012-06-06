/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CThreadPool.h $
* @brief �̳߳���
*
* $Id: CThreadPool.h 16 2008-11-18 11:36:40Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CTHREADPOOL_H__
#define __CTHREADPOOL_H__
 
#include <set>
#include <string>
#include <vector>
#include <map>
#include <iterator>

#include "common_thread.h"
#include "common_semaphore.h"
#include "CSingleton.h"

using namespace std;

/**
* @brief �����߳�״̬��wsNone-��δ������wsIdle-���С�wsBusy-����
*/
typedef enum
{
	wsNone,/**<��δ����*/
    wsIdle,/**<����*/
	wsBusy,/**<����*/
}TWorkThreadState;

template <class TTask>
class CThreadPool;

/**
* @brief �����߳���
*/
template <class TTask>
class CWorkThread
{
public:
    TTask *m_pTask; /**<�����߳�ʵ��*/

	/**
	* @brief �����߳��๹�캯��
	*
	* @param nID �����̱߳��
	* @param nTimeOut ����������ʱʱ�䣬��taskִ��ʱ�䳬��ָ����nTimeOutʱ���˳�task
	* @param pPool �̳߳�ָ��
	*/
    CWorkThread(const unsigned int nID, const unsigned int nTimeOut, CThreadPool<TTask> *pPool);
    
	/**
	* @brief �����߳�����������
	*/
    ~CWorkThread(void);

	/**
	* @brief ȡ�����̵߳ı�� 
	* @return �����̱߳��
	*/
    unsigned int GetID(void);

	/**
	* @brief ȡ�����̵߳�ͷ״̬
	* @return �����߳�״̬
	*/
    TWorkThreadState GetState(void);

	/**
	* @brief ���������߳�
	*/
    void Start(void);

	/**
	* @brief ���ѹ����߳�
	*/
    void Resume(void *pInfo);

	/**
	* @brief �������̣߳�ʹ�����̴߳�����ͣ״̬
	*/
    void Suspend(void);

	/**
	* @brief ֹͣ�����߳�
	*/
    void Stop(void);

private:
	bool m_bRun;					/**<�����߳��Ƿ����ִ��*/
    unsigned int m_nID;				/**<�����̱߳��*/
    CThreadPool<TTask> *m_pPool;	/**<�̳߳�ָ��*/
    TWorkThreadState m_ThreadState;	/**<�����̵߳�ǰ״̬*/
	SemHandle m_hSem;				/**<���ڹ����̻߳��Ѻ͹�����ź���*/
    ThreadHandle m_hThread;			/**<�����߳̾��*/
    ThreadMutex m_ThreadMutex;		/**<�����̻߳�����*/
	void *m_pInfo;					/**<�����̻߳���ʱ���ⲿ����Ĳ������������������ͣ���task����ʱ������*/
    static THREAD_FUN _ThreadProc(void *context);	/**<�����߳��ŽӺ���*/
    void ThreadProc();				/**<�����߳��߳����庯��*/
};

/**
* @brief �̳߳���
*/
template <class TTask >
    class CThreadPool
            //: public CSingleton<CThreadPool<TTask> >
{
public:
    /**
    * @brief �̳߳ع��캯��
    */
    CThreadPool();

    /**
    * @brief �̳߳���������
    */
    ~CThreadPool();

    /**
    * @brief ��ʼ���̳߳�
    * @param nMin �̳߳����߳����ٸ���
    * @param nMax �̳߳����߳�������
    * @param nTimeOut �����̳߳�ʱʱ��
    * @return ���س�ʼ�����
    * @retval true ��ʼ���ɹ�
    * @retval false ��ʼ��ʧ��
    */
    bool Init(const unsigned int nMin = 10, const unsigned int nMax = 100, const unsigned int nTimeOut=5);

	bool Uninit(void);
    /**
    * @brief ȡ�̳߳ؿ����̸߳���
    * @return �����̳߳��п��е��̸߳���
    */
    unsigned int IdleCount(void);

    /**
    * @brief ȡ�̳߳������ڹ������߳�
    * @return �����̳߳������ڹ������̸߳���
    */
    unsigned int BusyCount(void);

    /**
    * @brief ���̳߳��л�ȡһ�����е��߳�
    * @return ����һ�����е��߳�
    */
    CWorkThread<TTask> *GetIdleThread(void);

    /**
    * @brief ��ʹ�ù�����̷߳��ظ��̳߳�
    * @param nID ���ظ��̳߳ص��߳�ID
    */
    void ReleaseThread(unsigned int nID);
private:
    vector<CWorkThread<TTask> *> m_IdleThreadPool;                  /**<�̳߳��еĿ����߳�����*/
    map<unsigned int, CWorkThread<TTask> *> m_BusyThreadPool;        /**<�̳߳������ڹ������߳�map*/
    size_t m_nMin;                                        /**<�̳߳�����С�̸߳���*/
    size_t m_nMax;                                        /**<�̳߳�������̸߳���*/
    unsigned int m_nThreadCount;                                  /**<�̳߳��е�ǰ�߳�����*/
    unsigned int m_nTimeOut;                                     /**<�����̳߳�ʱʱ��*/
    bool m_bInited;                                             /**<�̳߳��Ƿ��ѱ���ʼ��*/
    ThreadMutex m_Mutex;                                       /**<�̻߳�����*/

    /**
    * @brief �����߳�
    */
    void CreateThreadProc(void);

    //friend class CSingleton<CThreadPool<TTask> >;
    friend class CWorkThread<TTask>;
};

/*********************************CWorkThread******************************************/
template <class TTask>
CWorkThread<TTask>::CWorkThread(const unsigned int nID, const unsigned int nTimeOut, CThreadPool<TTask> *pPool)
:  m_nID(nID),
m_pPool(pPool),
m_hThread(0),
m_ThreadState(wsNone)
{
	m_pTask = new TTask();
	DEBUGMSG("New Task%d\n", m_pTask);
	SemCreate(0, &m_hSem);//�����ź���
	Start();  
}

template <class TTask>
CWorkThread<TTask>::~CWorkThread(void)
{
	Stop();
	delete m_pTask;
	m_pTask = NULL;
}

template <class TTask>
unsigned int CWorkThread<TTask>::GetID(void)
{
    return m_nID;
}

template <class TTask>
TWorkThreadState CWorkThread<TTask>::GetState(void)
{
    return m_ThreadState;
}

template <class TTask>
void CWorkThread<TTask>::Start(void)
{
	m_bRun = true;
	m_hThread = ThreadCreate(_ThreadProc, this);
}

template <class TTask>
void CWorkThread<TTask>::Resume(void *pInfo)
{
    if (m_ThreadState == wsIdle)
    {
		m_pTask->SetInfo(pInfo);
		SemPost(&m_hSem);
    }
}

template <class TTask>
void CWorkThread<TTask>::Suspend(void)
{
	
}

template <class TTask>
void CWorkThread<TTask>::Stop(void)
{
    m_bRun = false;
	SemPost(&m_hSem);
	WaitForThread(m_hThread);
	CloseThreadHandle(m_hThread);
	m_hThread = 0;
}

template <class TTask>
THREAD_FUN CWorkThread<TTask>::_ThreadProc(void *context)
{
    CWorkThread<TTask> *pThread = static_cast<CWorkThread<TTask> *> (context);
    pThread->ThreadProc();
	return 0;
}

template <class TTask>
void CWorkThread<TTask>::ThreadProc()
{
    while(m_bRun)
    {
	m_ThreadState = wsIdle;
	//int sem_value;
        //sem_getvalue (&m_hSem, &sem_value);
        SemWait(&m_hSem);
		if (m_bRun == false) break;
	m_ThreadState = wsBusy;
	//DEBUGMSG("to run\n");
        m_pTask->Run(m_nID);
	//DEBUGMSG("to release thread\n");
        m_pPool->ReleaseThread(m_nID);
	//DEBUGMSG("thread released\n");
	m_ThreadState = wsIdle;

    }

	ThreadExit();
}

/*********************************CThreadPool******************************************/
template <class TTask >
CThreadPool<TTask>::CThreadPool()
{
    m_bInited = false;
    InitMutex(&m_Mutex);
}
    
template <class TTask >
CThreadPool<TTask>::~CThreadPool()
{
   // CWorkThread<TTask> *pWorkThread;
    Uninit();
    DestroyMutex(&m_Mutex);
}

template <class TTask> 
bool CThreadPool<TTask>::Init(const unsigned int nMin, const unsigned int nMax, 
							  const unsigned int nTimeOut)
{
    if (m_bInited)
        return true;
    m_nMin = nMin;
    m_nMax = nMax;
    m_nTimeOut = nTimeOut;
    m_nThreadCount = 0;
    m_IdleThreadPool.reserve(nMax);
    CreateThreadProc();
    while(m_IdleThreadPool.size() < nMin)
        SleepMs(1000);

    m_bInited = true;
    return true;
}

template <class TTask>
bool CThreadPool<TTask>::Uninit()
{
	if (!m_bInited) return true;
	for (size_t i = 0; i < (size_t)m_IdleThreadPool.size(); i++)
    {
		delete (CWorkThread<TTask> *)(m_IdleThreadPool[i]);
    }
    m_IdleThreadPool.clear();
    
    typename map<unsigned int, CWorkThread<TTask> *>::iterator BusyThreadPoolIter;
    for (BusyThreadPoolIter = m_BusyThreadPool.begin(); BusyThreadPoolIter != m_BusyThreadPool.end(); ++BusyThreadPoolIter)
    {
		delete (CWorkThread<TTask> *)(BusyThreadPoolIter->second);
    }
    m_BusyThreadPool.clear();
	m_bInited = true;
	return true;
}
    
template <class TTask> 
unsigned int CThreadPool<TTask>::IdleCount(void)
{
    return (int)m_IdleThreadPool.size();
}
  
template <class TTask> 
size_t CThreadPool<TTask>::BusyCount(void)
{
    return m_BusyThreadPool.size();
}

template <class TTask> 
CWorkThread<TTask> *CThreadPool<TTask>::GetIdleThread(void)
{
    if (m_IdleThreadPool.size() <= 0)
    {
        if (m_IdleThreadPool.size() < m_nMax)
        {
            CWorkThread<TTask> *pWorkThread = 
				new CWorkThread<TTask>(m_nThreadCount, m_nTimeOut, this);
            EnterMutex(&m_Mutex);
            m_IdleThreadPool.push_back(pWorkThread);
            LeaveMutex(&m_Mutex);
            
            m_nThreadCount++;
        }
        else
        {
            int nLoop = 3;
            while(m_IdleThreadPool.size() <= 0)
            {
                SleepMs(1000);
                nLoop --;
                if (m_IdleThreadPool.size() > 0)
                    break;
                else
				{
                    if (nLoop <= 0)
                    {
                        return NULL;
                    }
				}
            }
        }
    }

    EnterMutex(&m_Mutex);
    CWorkThread<TTask> *pThread = m_IdleThreadPool.back();
    m_BusyThreadPool[pThread->GetID()] = pThread;
    m_IdleThreadPool.pop_back();
    LeaveMutex(&m_Mutex);

    return pThread;
}

template <class TTask> 
void CThreadPool<TTask>::ReleaseThread(unsigned int nID)
{
    typename map<unsigned int, CWorkThread<TTask> *>::iterator pos = m_BusyThreadPool.find(nID);
    if (pos != m_BusyThreadPool.end())
    {
    	//DEBUGMSG("To enter mutex\n");
		EnterMutex(&m_Mutex);
		//DEBUGMSG("to push back\n");
		m_IdleThreadPool.push_back(pos->second);
		//DEBUGMSG("to earse\n");
		m_BusyThreadPool.erase(pos->first);
		//DEBUGMSG("to out mutex\n");
		LeaveMutex(&m_Mutex);
		//DEBUGMSG("out mutex\n");
    }
}

template <class TTask>
void CThreadPool<TTask>::CreateThreadProc(void)
{

    while(m_IdleThreadPool.size() < m_nMin)
    {
        try
        {
            CWorkThread<TTask> *pWorkThread =
                new CWorkThread<TTask>(m_nThreadCount, m_nTimeOut, this);
            EnterMutex(&m_Mutex);
            m_IdleThreadPool.push_back(pWorkThread);
            LeaveMutex(&m_Mutex);
            m_nThreadCount++;
        }
        catch(...)
        {
            SleepMs(1000);
        }
    }
}

#endif


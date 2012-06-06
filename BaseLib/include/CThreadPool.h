/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CThreadPool.h $
* @brief 线程池类
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
* @brief 工作线程状态，wsNone-尚未建立、wsIdle-空闲、wsBusy-工作
*/
typedef enum
{
	wsNone,/**<尚未建立*/
    wsIdle,/**<空闲*/
	wsBusy,/**<工作*/
}TWorkThreadState;

template <class TTask>
class CThreadPool;

/**
* @brief 工作线程类
*/
template <class TTask>
class CWorkThread
{
public:
    TTask *m_pTask; /**<工作线程实例*/

	/**
	* @brief 工作线程类构造函数
	*
	* @param nID 工作线程编号
	* @param nTimeOut 工作函数超时时间，当task执行时间超过指定的nTimeOut时，退出task
	* @param pPool 线程池指针
	*/
    CWorkThread(const unsigned int nID, const unsigned int nTimeOut, CThreadPool<TTask> *pPool);
    
	/**
	* @brief 工作线程类析构函数
	*/
    ~CWorkThread(void);

	/**
	* @brief 取工作线程的编号 
	* @return 工作线程编号
	*/
    unsigned int GetID(void);

	/**
	* @brief 取工作线程当头状态
	* @return 工作线程状态
	*/
    TWorkThreadState GetState(void);

	/**
	* @brief 启动工作线程
	*/
    void Start(void);

	/**
	* @brief 唤醒工作线程
	*/
    void Resume(void *pInfo);

	/**
	* @brief 挂起工作线程，使工作线程处于暂停状态
	*/
    void Suspend(void);

	/**
	* @brief 停止工作线程
	*/
    void Stop(void);

private:
	bool m_bRun;					/**<工作线程是否继续执行*/
    unsigned int m_nID;				/**<工作线程编号*/
    CThreadPool<TTask> *m_pPool;	/**<线程池指针*/
    TWorkThreadState m_ThreadState;	/**<工作线程当前状态*/
	SemHandle m_hSem;				/**<用于工作线程唤醒和挂起的信号量*/
    ThreadHandle m_hThread;			/**<工作线程句柄*/
    ThreadMutex m_ThreadMutex;		/**<工作线程互斥锁*/
	void *m_pInfo;					/**<工作线程唤醒时从外部传入的参数，可以是任意类型，在task运行时被调用*/
    static THREAD_FUN _ThreadProc(void *context);	/**<工作线程桥接函数*/
    void ThreadProc();				/**<工作线程线程主体函数*/
};

/**
* @brief 线程池类
*/
template <class TTask >
    class CThreadPool
            //: public CSingleton<CThreadPool<TTask> >
{
public:
    /**
    * @brief 线程池构造函数
    */
    CThreadPool();

    /**
    * @brief 线程池析构函数
    */
    ~CThreadPool();

    /**
    * @brief 初始化线程池
    * @param nMin 线程池中线程最少个数
    * @param nMax 线程池中线程最多个数
    * @param nTimeOut 工作线程超时时间
    * @return 返回初始化结果
    * @retval true 初始化成功
    * @retval false 初始化失败
    */
    bool Init(const unsigned int nMin = 10, const unsigned int nMax = 100, const unsigned int nTimeOut=5);

	bool Uninit(void);
    /**
    * @brief 取线程池空闲线程个数
    * @return 返回线程池中空闲的线程个数
    */
    unsigned int IdleCount(void);

    /**
    * @brief 取线程池中正在工作的线程
    * @return 返回线程池中正在工作的线程个数
    */
    unsigned int BusyCount(void);

    /**
    * @brief 从线程池中获取一个空闲的线程
    * @return 返回一个空闲的线程
    */
    CWorkThread<TTask> *GetIdleThread(void);

    /**
    * @brief 将使用过后的线程返回给线程池
    * @param nID 返回给线程池的线程ID
    */
    void ReleaseThread(unsigned int nID);
private:
    vector<CWorkThread<TTask> *> m_IdleThreadPool;                  /**<线程池中的空闲线程序列*/
    map<unsigned int, CWorkThread<TTask> *> m_BusyThreadPool;        /**<线程池中正在工作的线程map*/
    size_t m_nMin;                                        /**<线程池中最小线程个数*/
    size_t m_nMax;                                        /**<线程池中最大线程个数*/
    unsigned int m_nThreadCount;                                  /**<线程池中当前线程总数*/
    unsigned int m_nTimeOut;                                     /**<工作线程超时时间*/
    bool m_bInited;                                             /**<线程池是否已被初始化*/
    ThreadMutex m_Mutex;                                       /**<线程互斥锁*/

    /**
    * @brief 创建线程
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
	SemCreate(0, &m_hSem);//创建信号量
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


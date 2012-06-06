/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CTimerTask.h $
 *
 *  @brief 定时任务类头文件
 *
 *  $Id: CTimerTask.h 12 2008-08-21 14:07:09Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __CTIMERTASK_H__
#define __CTIMERTASK_H__

#include <queue>
#include <string>

using namespace std;

#include "common_socket.h"
#include "common_thread.h"
#include "CThreadPool.h"


template <class TTask>
class CTimerTask
{
	public:
		/**
		* @brief 定时任务类构造函数
		*/
		CTimerTask(void);
		
		/**
		* @brief 定时任务类析构函数
		*/
		~CTimerTask(void);
		
		/**
		* @brief 启动定时任务
		*
		* @param nInterval 定时间隔，以毫秒为单位
		*/
		virtual bool Start(const int nInterval);
		
		/**
		* @brief 停止定时任务
		*/
		virtual bool Stop(void);
		
		/**
		* @brief 定时触发事件，需要子类来具体实现
		*
		* @param m_pThreadPool 用于执行定时任务的线程池
		*/
		virtual void OnTimer(CThreadPool<TTask> *m_pThreadPool) = 0;
	protected:
		CThreadPool<TTask> m_ThreadPool;/**<工作线程池*/
	private:
		int m_nInterval;				/**<定时间隔（ms）*/
		ThreadHandle m_hThread;			/**<线程句柄*/
		bool m_bThreadRunning;			/**<线程是否正在执行*/
		
		/**
		* @brief 线程入口
		*
		* @param context 上下文，通常传入类的实例
		*/
		static THREAD_FUN _ThreadProc(void *context)
		{
			(reinterpret_cast <CTimerTask<TTask> *> (context))->ThreadProc();
			return 0;
		}
		
		/**
		* @brief 线程函数
		*/
		void ThreadProc();

};


template <class TTask>
CTimerTask<TTask>::CTimerTask(void)
{
	m_nInterval = 1;
}

template <class TTask>
CTimerTask<TTask>::~CTimerTask(void)
{
	
}

template <class TTask>
bool CTimerTask<TTask>::Start(const int nInterval)
{
	m_nInterval = nInterval;
	m_ThreadPool.Init(2,2,2);
	m_bThreadRunning = true;
	m_hThread = ThreadCreate(_ThreadProc, this);
	return true;
}

template <class TTask>
bool CTimerTask<TTask>::Stop(void)
{
	m_bThreadRunning = false;
	WaitForThread(m_hThread);
	CloseThreadHandle(m_hThread);
	m_hThread = NULL;
	return true;
}

template <class TTask>
void CTimerTask<TTask>::ThreadProc()
{
	while(m_bThreadRunning)
	{
		
		
		OnTimer(&m_ThreadPool);

		SleepMs(m_nInterval);
	}
}

#endif
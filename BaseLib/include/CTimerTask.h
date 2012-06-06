/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CTimerTask.h $
 *
 *  @brief ��ʱ������ͷ�ļ�
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
		* @brief ��ʱ�����๹�캯��
		*/
		CTimerTask(void);
		
		/**
		* @brief ��ʱ��������������
		*/
		~CTimerTask(void);
		
		/**
		* @brief ������ʱ����
		*
		* @param nInterval ��ʱ������Ժ���Ϊ��λ
		*/
		virtual bool Start(const int nInterval);
		
		/**
		* @brief ֹͣ��ʱ����
		*/
		virtual bool Stop(void);
		
		/**
		* @brief ��ʱ�����¼�����Ҫ����������ʵ��
		*
		* @param m_pThreadPool ����ִ�ж�ʱ������̳߳�
		*/
		virtual void OnTimer(CThreadPool<TTask> *m_pThreadPool) = 0;
	protected:
		CThreadPool<TTask> m_ThreadPool;/**<�����̳߳�*/
	private:
		int m_nInterval;				/**<��ʱ�����ms��*/
		ThreadHandle m_hThread;			/**<�߳̾��*/
		bool m_bThreadRunning;			/**<�߳��Ƿ�����ִ��*/
		
		/**
		* @brief �߳����
		*
		* @param context �����ģ�ͨ���������ʵ��
		*/
		static THREAD_FUN _ThreadProc(void *context)
		{
			(reinterpret_cast <CTimerTask<TTask> *> (context))->ThreadProc();
			return 0;
		}
		
		/**
		* @brief �̺߳���
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
/****************************************************
 *  @file    $URL$
 *
 *  @brief 并发服务器类
 *
 *  $Id$
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/CServer.h"

template <class TTask>
CServer<TTask>::CServer()//const char *szBindIP, const unsigned short usListenPort)
{
	/*sprintf(m_szBindIP, "%s", szBindIP);
	m_usListenPort = usListenPort;
	m_ThreadPool.Init();
	m_bThreadRunning = true;
	m_hThread = ThreadCreate(_ThreadProc, this);*/
}

template <class TTask>
CServer<TTask>::~CServer(void)
{
	m_bThreadRunning = false;
	WaitForThread(m_hThread);
	CloseThreadHandle(m_hThread);
	m_hThread = NULL;
}

/*template <class TTask>
bool CServer<TTask>::Start(void)
{
	SvrListen(m_szBindIP, m_usListenPort);
	return true;
}

template <class TTask>
bool CServer<TTask>::Stop(void)
{
	SvrStop();
	return true;
}

template <class TTask>
void CServer<TTask>::OnRead(SOCKET socket, void * p, char *buf, int len)
{
	TMsg *pMsg = new TMsg();
	pMsg->socket = socket;
	pMsg->msg = buf;
	pMsg->len = len;
	m_MsgQueue.push(pMsg);
}

template <class TTask>
void CServer<TTask>::OnAccept(SOCKET socket)
{
}

template <class TTask>
void CServer<TTask>::OnClose(void * p)
{
}

template <class TTask>
void CServer<TTask>::ThreadProc()
{
	TMsg *pMsg;
	while(m_bThreadRunning)
	{
		while(m_MsgQueue.size() > 0)
		{
			pMsg = m_MsgQueue.front();
			if (m_ThreadPool.IdleCount() > 0)
			{
				CWorkThread<TTask> *pThread = m_ThreadPool.GetIdleThread();
				pThread->Resume((void *)pMsg);
				m_MsgQueue.pop();
			}
			else
				SleepMs(100);

		}
	}
}*/
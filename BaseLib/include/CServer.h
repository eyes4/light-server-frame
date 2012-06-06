/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CServer.h $
 *
 *  @brief 并发服务器类头文件
 *
 *  $Id: CServer.h 13 2008-09-30 11:47:16Z gang.chen $ 
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __CSERVER_H__
#define __CSERVER_H__

#include <queue>
#include <string>

using namespace std;

#include "common_socket.h"
#include "CTcpSvrSocket.h"
#include "common_thread.h"
#include "CThreadPool.h"

/**
* @brief 服务器收到的消息结构体
*/
typedef struct _TMsg
{
	SOCKET socket;			/**<与客户端建立连接的套接字*/
	int len;				/**<消息长度*/
	char clientip[16];		/**<客户端IP地址*/
	char msg[4096];			/**<消息*/
}TMsg;

template <class TTask, class TMsgProcessor>
class CServer
	: public CTcpSvrSocket
{
	public:
		CServer(void);
		CServer(const char *szBindIP, const unsigned short usListenPort);
			//int nThreadPoolMinSize, int nThreadPoolMaxSize, int nTimeOut, char *szTail = "$");
		~CServer(void);

		bool SetBindAddr(const char *szBindIP, const unsigned short usListenPort);
		bool SetThreadPoolMaxSize(int nSize);
		bool SetThreadPoolMinSize(int nSize);
		bool SetSlienceTimeOut(int nTimeOut);//未收到消息时，多长时间断开连接，ms
		bool SetConnTimeOut(int nTimeOut);//连接后未交互时，多长时间断开连接，ms
		bool SetMsgTail(char *szTail, int nLen);

		bool Start(void);
		bool Stop(void);
		
		virtual void OnRead(SOCKET socket, const char *szClientIP, char *buf, int len);
		virtual void OnAccept(SOCKET socket, const char *szClientIP);
		virtual void OnClose(SOCKET socket, const char *szClientIP);
	protected:
		char m_szBindIP[16];
		unsigned short m_usListenPort;

		int m_nThreadPoolMinSize;
		int m_nThreadPoolMaxSize;
		char m_szTail[16];
		int m_nTailLen;
		int m_nSlienceTimeOut;
		int m_nConnTimeOut;
	private:
		TMsgProcessor *m_pMsgProcessor;
		queue<TMsg *> m_MsgQueue;
		ThreadHandle m_hThread;
		bool m_bThreadRunning;
		CThreadPool<TTask> m_ThreadPool;

		static THREAD_FUN _ThreadProc(void *context)
		{
			(reinterpret_cast <CServer<TTask, TMsgProcessor> *> (context))->ThreadProc();
			return 0;
		}

		void ThreadProc();

};

template <class TTask, class TMsgProcessor>
CServer<TTask, TMsgProcessor>::CServer(void)
{

	CServer("0.0.0.0", 0);
}

template <class TTask, class TMsgProcessor>
CServer<TTask, TMsgProcessor>::CServer(const char *szBindIP, const unsigned short usListenPort)//,
						//int nThreadPoolMinSize, int nThreadPoolMaxSize, int nTimeOut, char *szTail)
{
	sprintf(m_szBindIP, "%s", szBindIP);
	m_usListenPort = usListenPort;
	m_nThreadPoolMinSize = 10;
	m_nThreadPoolMaxSize = 30;
	m_nSlienceTimeOut = 20000;
	m_nConnTimeOut = 100;
	sprintf(m_szTail, "$");
	m_nTailLen = 1;
	m_bThreadRunning = true;
	m_hThread = ThreadCreate(_ThreadProc, this);
}

template <class TTask, class TMsgProcessor>
CServer<TTask, TMsgProcessor>::~CServer(void)
{
	m_bThreadRunning = false;
	WaitForThread(m_hThread);
	CloseThreadHandle(m_hThread);
	m_hThread = 0;
	delete m_pMsgProcessor;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetBindAddr(const char *szBindIP, const unsigned short usListenPort)
{
	if (strlen(szBindIP) >= 16) return false;
	sprintf(m_szBindIP, "%s", szBindIP);
	m_usListenPort = usListenPort;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetThreadPoolMaxSize(int nSize)
{
	m_nThreadPoolMaxSize = nSize;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetThreadPoolMinSize(int nSize)
{
	m_nThreadPoolMinSize = nSize;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetSlienceTimeOut(int nTimeOut)
{
	if (nTimeOut <= 100) return false;
	m_nSlienceTimeOut = nTimeOut;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetConnTimeOut(int nTimeOut)
{
	if (nTimeOut < 10) return false;
	m_nConnTimeOut = nTimeOut;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::SetMsgTail(char *szTail, int nLen)
{
	if (strlen(szTail) > 15 || nLen > 15) return false;
	sprintf(m_szTail,"%s", szTail);
	m_nTailLen = nLen;
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::Start(void)
{
	if (m_usListenPort == 0) return false;
	m_ThreadPool.Init(m_nThreadPoolMinSize, m_nThreadPoolMaxSize, 2);
	m_pMsgProcessor = new TMsgProcessor(m_nSlienceTimeOut, m_szTail);
	SvrListen(m_szBindIP, m_usListenPort);
	return true;
}

template <class TTask, class TMsgProcessor>
bool CServer<TTask, TMsgProcessor>::Stop(void)
{
	SvrStop();
	m_ThreadPool.Uninit();
	delete m_pMsgProcessor;
	m_pMsgProcessor = NULL;
	return true;
}

template <class TTask, class TMsgProcessor>
void CServer<TTask, TMsgProcessor>::OnRead(SOCKET socket, const char *szClientIP,  char *buf, int len)
{
	m_pMsgProcessor->OnRead(socket, szClientIP, buf, len);	
}

template <class TTask, class TMsgProcessor>
void CServer<TTask, TMsgProcessor>::OnAccept(SOCKET socket, const char *szClientIP)
{
	m_pMsgProcessor->OnAccept(socket, szClientIP);
}

template <class TTask, class TMsgProcessor>
void CServer<TTask, TMsgProcessor>::OnClose(SOCKET socket, const char *szClientIP)
{
	m_pMsgProcessor->OnClose(socket, szClientIP);
}

template <class TTask, class TMsgProcessor>
void CServer<TTask, TMsgProcessor>::ThreadProc()
{
	void *pMsg;
	while(m_bThreadRunning)
	{
		while(m_pMsgProcessor->MsgCount() > 0)
		{
	
			pMsg = m_pMsgProcessor->GetMsg();
			if (m_ThreadPool.IdleCount() > 0)
			{
				CWorkThread<TTask> *pThread = m_ThreadPool.GetIdleThread();
				//DEBUGMSG("Get FreeThread :%d\n", pThread);
				pThread->Resume(pMsg);

				m_pMsgProcessor->PopMsg();
			}
			else
			{
				m_pMsgProcessor->CheckTimeout();
				SleepMs(10);
			}

		}

		m_pMsgProcessor->CheckTimeout();
		SleepMs(10);
	}
}



#endif

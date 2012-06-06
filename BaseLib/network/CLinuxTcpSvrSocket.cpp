/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/network/CLinuxTcpSvrSocket.cpp $
 *
 *  @brief 并发服务器Socket类(linux平台)
 *
 *  $Id: CLinuxTcpSvrSocket.cpp 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#define MAXLINE 1024
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000
#include <pthread.h>
#include "common_thread.h"
#include "../include/common_socket.h"
#include "../include/CTcpSvrSocket.h"


void setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

CTcpSvrSocket::CTcpSvrSocket(void)
{
	m_workThread = false;
	m_sSocket = -1;
	m_hwThread = 0;
}

CTcpSvrSocket::~CTcpSvrSocket(void)
{

}

THREAD_FUN CTcpSvrSocket::_WorkThread(void* Param)
{
	CTcpSvrSocket * pthis = (CTcpSvrSocket *)Param;
	pthis->WorkThread();
}

void CTcpSvrSocket::WorkThread(void)
{
	//CTcpSvrSocket * pthis = (CTcpSvrSocket *)Param;
	int nfds, i, connfd, sockfd;
	struct epoll_event events[20], ev;
	struct sockaddr_in clientaddr;
	socklen_t clilen;
	ssize_t n;
	char line[MAXLINE];
	char szAddr[16];
	while(m_workThread)
	{
		nfds = epoll_wait(m_epfd, events,20,500);
		for (i = 0; i < nfds; ++i)
		{
			if (events[i].data.fd == m_sSocket)
			{
				connfd = accept(m_sSocket, (sockaddr *)&clientaddr, &clilen);

				if (connfd < 0)
				{
					perror("connfd < 0");
					continue;
				}
				setnonblocking(connfd);
				//pthis->m_MapClient[connfd] = (void*)Info;
				sprintf(szAddr, "%s", inet_ntoa(clientaddr.sin_addr));
				OnAccept(connfd, szAddr);
				ev.data.fd = connfd;
				ev.events = EPOLLIN;
				epoll_ctl(m_epfd, EPOLL_CTL_ADD, connfd, &ev);
				
			}
			else if (events[i].events & EPOLLIN)
			{
				if ((sockfd = events[i].data.fd) < 0) continue;
				if ((n = read(sockfd, line, MAXLINE)) < 0)
				{
					if (errno == ECONNRESET)
					{
						close(sockfd);
						events[i].data.fd = -1;
					}
					else
					{
						close(sockfd);
						events[i].data.fd = -1;
					}
					//ev.data.fd = sockfd;
					//ev.events = EPOLLOUT;
					//epoll_ctl(m_epfd, EPOLL_CTL_MOD, sockfd, &ev);
				}	
				else if (n == 0)
				{
					close(sockfd);
					events[i].data.fd = -1;
				}
				else
				{
					
					//void * re;
					struct sockaddr_in server;   
					socklen_t len = sizeof(server);   
					getpeername(sockfd,(sockaddr*)&server,&len); 
					//saddr=inet_ntoa(server.sin_addr);
					sprintf(szAddr, "%s", inet_ntoa(server.sin_addr));
					OnRead(sockfd, szAddr, line, n);
				}
				
			}
		}
	}
	//return 0;
}

bool CTcpSvrSocket::SvrListen(const char *szBindIP, const unsigned short usListenPort)
{
	
	struct epoll_event ev;
	m_epfd = epoll_create(40000);
	if (strcmp(szBindIP, "") == 0 || strcmp(szBindIP, "0.0.0.0"))
	{
		m_sSocket = SocketBindAll(SOCK_STREAM, usListenPort);
	}
	else
	{
		m_sSocket = SocketBind(SOCK_STREAM, (char *)szBindIP, usListenPort);
	}
	if (m_sSocket < 0) return false;
	setnonblocking(m_sSocket);
	//设置与要处理的事件相关的文件描述符
	ev.data.fd = m_sSocket;
	//设置要处理的事件类型
	ev.events=EPOLLIN|EPOLLET;
	//注册epoll事件
	epoll_ctl(m_epfd, EPOLL_CTL_ADD,m_sSocket,&ev);
	if (!SocketListen(m_sSocket))
	{
		SocketClose(m_sSocket);
		return false;
	}
	
	
	this->m_workThread = true;
	m_hwThread = ThreadCreate(_WorkThread, this);
	//m_hwThread = CreateThread(NULL, 0, WorkThread, (LPVOID)this, 0, &m_wthreadID); //   创建工作线程，用来处理完成端口消息的
	return true;

}

void CTcpSvrSocket::SvrStop(void)
{

//	if (m_sSocket >= 0)
//	{
//		SocketClose(m_sSocket);
//		m_sSocket = -1;
//	}
//
//	map<SOCKET, void*>::iterator ClientIter;
//	for (ClientIter = m_MapClient.begin(); ClientIter != m_MapClient.end();++ClientIter)
//	{
//		//DEBUGMSG("to delete pFrontDevInfo\n");   
//		OVERLAPPEDPLUS *olp = (OVERLAPPEDPLUS*)(ClientIter->second);
//		closesocket(olp->s);
//		delete olp;
//	}
//	m_MapClient.clear();
//
//	if (m_haThread != NULL)
//	{
//		m_acceptThread = false;
//		WaitForSingleObject((HANDLE)m_haThread, INFINITE);
//		CloseHandle((HANDLE)m_haThread);
//		m_haThread = NULL;
//	}
//	if (m_hwThread != NULL)
//	{
//		m_workThread = false;
//		WaitForSingleObject((HANDLE)m_hwThread, INFINITE);
//		CloseHandle((HANDLE)m_hwThread);
//		m_hwThread = NULL;
//	}
}


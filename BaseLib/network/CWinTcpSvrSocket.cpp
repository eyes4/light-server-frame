/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/network/CWinTcpSvrSocket.cpp $
 *
 *  @brief 并发服务器Socket类(windows平台)
 *
 *  $Id: CWinTcpSvrSocket.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include <iostream>
#include <fstream>
#include "../include/common_socket.h"
#include "../include/CTcpSvrSocket.h"
using namespace std;
CTcpSvrSocket::CTcpSvrSocket(void)
{
	m_workThread = false;
	m_acceptThread = false;
	m_sSocket = -1;
	m_hIocp = NULL;
	m_haThread = NULL;
	m_hwThread = NULL;
	m_bInited = SocketInit();

}

CTcpSvrSocket::~CTcpSvrSocket(void)
{
	SocketUninit();

}

bool CTcpSvrSocket::SvrListen(const char *szBindIP, const unsigned short usListenPort)
{
	if (!m_bInited) return false;
	if (strcmp(szBindIP, "") == 0 || strcmp(szBindIP, "0.0.0.0"))
	{
		m_sSocket = SocketBindAll(SOCK_STREAM, usListenPort);
	}
	else
	{
		m_sSocket = SocketBind(SOCK_STREAM, (char *)szBindIP, usListenPort);
	}
	if (m_sSocket < 0) return false;

	if (!SocketListen(m_sSocket))
	{
		SocketClose(m_sSocket);
		return false;
	}

	if ((m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0)) == NULL)//   创建完成端口的句柄
	{
		SocketClose(m_sSocket);
         return false;
	}

	this->m_acceptThread = true;
	m_haThread = CreateThread(NULL, 0, AcceptThread, (LPVOID)this, 0, &m_athreadID);    //   创建连接线程，用来接收客户端的连接

	this->m_workThread = true;
	m_hwThread = CreateThread(NULL, 0, WorkThread, (LPVOID)this, 0, &m_wthreadID); //   创建工作线程，用来处理完成端口消息的
	return true;

}

void CTcpSvrSocket::SvrStop(void)
{

	if (m_sSocket >= 0)
	{
		DWORD dwByteTrans = 0;
		 //PostQueuedCompletionStatus(m_hIocp, dwByteTrans, 0, 0);
		SocketClose(m_sSocket);
		m_sSocket = -1;
	}	


	map<SOCKET, void*>::iterator ClientIter;
	for (ClientIter = m_MapClient.begin(); ClientIter != m_MapClient.end();++ClientIter)
	{
		//DEBUGMSG("to delete pFrontDevInfo\n");   
		OVERLAPPEDPLUS *olp = (OVERLAPPEDPLUS*)(ClientIter->second);
		closesocket(olp->s);
		GlobalFree(olp);
		olp = NULL;
	}
	m_MapClient.clear();

	if (m_haThread != NULL)
	{
		m_acceptThread = false;
		WaitForSingleObject((HANDLE)m_haThread, INFINITE);
		CloseHandle((HANDLE)m_haThread);
		m_haThread = NULL;
	}
	if (m_hwThread != NULL)
	{
		m_workThread = false;
		WaitForSingleObject((HANDLE)m_hwThread, INFINITE);
		CloseHandle((HANDLE)m_hwThread);
		m_hwThread = NULL;
	}


}

DWORD __stdcall CTcpSvrSocket::AcceptThread(LPVOID Param)
{
	DWORD Flags;
	DWORD RecvBytes = 0;
	struct sockaddr_in server;   
	int len = sizeof(server);   
	char szAddr[16];
	CTcpSvrSocket * pthis = (CTcpSvrSocket *)Param;
	LPPER_HANDLE_DATA	PerHandleData = NULL;
	LPPER_IO_DATA		PerIoData = NULL;

	while(pthis->m_acceptThread)
	{
		SOCKET client;
		if ((client= WSAAccept(pthis->m_sSocket, NULL, NULL, NULL, 0)) == INVALID_SOCKET)
		{
			//   错误处理
			DEBUGMSG("WSAAccept() failed with error %d\n", GetLastError());
			continue;
		}
		//DEBUGMSG("%d socket accept :%d\n", (long)pthis, client);
		//OVERLAPPEDPLUS *olp;
		if ((PerHandleData = (LPPER_HANDLE_DATA) GlobalAlloc (GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
		{
			DEBUGMSG("1GlobalAlloc() failed with error %d\n", GetLastError());
			return 0;
		}      
		PerHandleData->Socket = client;
		if (CreateIoCompletionPort((HANDLE)client, pthis->m_hIocp, (DWORD)PerHandleData, 0) == NULL)
			continue;

		if ((PerIoData = (LPPER_IO_DATA) GlobalAlloc (GPTR,sizeof(PER_IO_DATA))) == NULL)
		{
			DEBUGMSG("2GlobalAlloc() failed with error %d\n", GetLastError());
			return 0;
		}

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		//pthis->PerIoData->BytesSEND = 0;
		//pthis->PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->buffer;
		PerIoData->OperationType = 0;
		DEBUGMSG("accept,socket:%d, PerHandleData%x, PerIoData:%x\n", client,(DWORD) PerHandleData,(DWORD) PerIoData);
		DEBUGMSG("buf:%d\n", (long)PerIoData->DataBuf.buf);
		pthis->m_MapClient[client] = PerIoData;
		//printf("to call OnAccept\n");
		getpeername(PerHandleData->Socket,(sockaddr*)&server,&len);
		sprintf(szAddr, "%s", inet_ntoa(server.sin_addr));
		pthis->OnAccept(client, szAddr);    //   调用 OnAccept()通知应用程序有新客户端连接


		Flags = 0;
		//printf("to WSARecv\n");
		if (WSARecv(client, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
			&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				int err = WSAGetLastError();

				DEBUGMSG("WSARecv() failed with error %d %d\n", WSAGetLastError(), err);
				if (PerHandleData != NULL)
				{
					PerHandleData->Socket = -1;
					GlobalFree(PerHandleData);
				}
				if (PerIoData != NULL)
				{
					PerIoData->DataBuf.buf = NULL;
					GlobalFree(PerIoData);
				}

				PerHandleData = NULL;
				PerIoData = NULL;
				//return 0;
				continue;
			}
		}

		

	}

	if (PerHandleData != NULL)
				{
					PerHandleData->Socket = -1;
					GlobalFree(PerHandleData);
				}
				if (PerIoData != NULL)
				{
					PerIoData->DataBuf.buf = NULL;
					GlobalFree(PerIoData);
				}

				PerHandleData = NULL;
				PerIoData = NULL;
	return 0;
}

DWORD __stdcall CTcpSvrSocket::WorkThread(LPVOID Param)
{
	CTcpSvrSocket * pthis = (CTcpSvrSocket *)Param;
	//char buffer[1496];
	char szAddr[16] = {0};
	void * re = NULL;
	char *p = NULL;
	int nHead = 0;
	OVERLAPPED * pOverlap = NULL;
	DWORD berByte;
	struct sockaddr_in server = {0};   
	int len = sizeof(server);   

	DWORD BytesTransferred = 0;
	LPOVERLAPPED  lpOverlapped;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;        
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	

	while(pthis->m_workThread)
	{
		int ret;
		//printf("GetQueuedCompletionStatus\n");
		if (GetQueuedCompletionStatus(pthis->m_hIocp, &BytesTransferred,
			(LPDWORD)&PerHandleData, (LPOVERLAPPED *) &lpOverlapped, 100) == 0)
		{
			//continue;
			//printf("failed with error %d\n", WSAGetLastError());
			//DEBUGMSG("GetQueuedCompletionStatus() == 0 error %d size:%x\n", GetLastError(), BytesTransferred);
			if (GetLastError() != WAIT_TIMEOUT)
			{
				DEBUGMSG("GetQueuedCompletionStatus() == 0 error %d size:%x\n", GetLastError(), BytesTransferred);
				DEBUGMSG("error != TIMEOUT %d Closing socket %d\n", (long)pthis, PerHandleData->Socket);
				//pthis->OnClose(PerHandleData->Socket, szAddr);
				if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
				{
					DEBUGMSG("closesocket() failed with error %d\n", WSAGetLastError());
					//if (WSAGetLastError() == 10038) continue;
					//return 0;
				}
				
				PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(lpOverlapped, PER_IO_DATA, Overlapped);
				DEBUGMSG("1free socket:%d, PerHandleData%x, PerIoData:%x\n", PerHandleData->Socket,(DWORD) PerHandleData,(DWORD) PerIoData);
				if (PerHandleData != NULL)
				{
					PerHandleData->Socket = -1;
					GlobalFree(PerHandleData);
				}
				if (PerIoData != NULL)
				{
					PerIoData->DataBuf.buf = NULL;
					GlobalFree(PerIoData);
				}
				PerHandleData = NULL;
				PerIoData = NULL;
				continue;
			}
			continue;
		}

		PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(lpOverlapped, PER_IO_DATA, Overlapped);
		getpeername(PerHandleData->Socket,(sockaddr*)&server,&len);
		sprintf(szAddr, "%s", inet_ntoa(server.sin_addr));

		//if (pthis->m_MapClient.find(pthis->PerHandleData->Socket) == pthis->m_MapClient.end()) continue;
		if (BytesTransferred == 0 && PerIoData != NULL && PerIoData->OperationType == OP_READ)
		{
			//printf("%d Closing socket %d\n", (long)pthis, PerHandleData->Socket);
			pthis->OnClose(PerHandleData->Socket, szAddr);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				DEBUGMSG("closesocket() failed with error %d\n", WSAGetLastError());
				//if (WSAGetLastError() == 10038) continue;
				//return 0;
			}
			//PerHandleData->Socket = -1;
			//PerIoData->DataBuf.buf = NULL;
			DEBUGMSG("2free socket:%d, PerHandleData%x, PerIoData:%x\n", PerHandleData->Socket,(DWORD) PerHandleData,(DWORD) PerIoData);
			if (PerHandleData != NULL)
			{
				PerHandleData->Socket = -1;
				GlobalFree(PerHandleData);
			}
			if (PerIoData != NULL)
			{
				PerIoData->DataBuf.buf = NULL;
				GlobalFree(PerIoData);
			}

			PerHandleData = NULL;
			PerIoData = NULL;
			continue;
		}     

		if (PerHandleData != NULL)// && PerIoData->OperationType == OP_READ)
		{

			//调用 OnRead() 通知应用程序，服务器收到来自客户端的网络数据
			//printf("BytesTransferred = %d\n", BytesTransferred);
			

			//printf("to set buff%d %d\n", (long)PerIoData, (long)PerIoData->buffer);
			//pthis->PerIoData->DataBuf.buf[BytesTransferred] = 0;
			//DEBUGMSG("PerIoData:%x", PerIoData);
			if (PerIoData->DataBuf.buf == NULL) continue;
			try
			{
				PerIoData->DataBuf.buf[BytesTransferred] = 0;
			}
			catch(...)
			{
				continue;
			}
			pthis->OnRead(PerHandleData->Socket, szAddr, PerIoData->DataBuf.buf, (int)BytesTransferred);
		


			Flags = 0;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->buffer;
			PerIoData->OperationType = 0;

			if (WSARecvFrom(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				(struct sockaddr*)&server, &len, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecvFrom() failed with error %d\n", WSAGetLastError());
					if (PerHandleData != NULL)
					{
						PerHandleData->Socket = -1;
						GlobalFree(PerHandleData);
					}
					if (PerIoData != NULL)
					{
						PerIoData->DataBuf.buf = NULL;
						GlobalFree(PerIoData);
					}

					PerHandleData = NULL;
					PerIoData = NULL;
					continue;
				}
			}
		}

	}
	if (PerHandleData != NULL)
			{
				PerHandleData->Socket = -1;
				GlobalFree(PerHandleData);
			}
			if (PerIoData != NULL)
			{
				PerIoData->DataBuf.buf = NULL;
				GlobalFree(PerIoData);
			}

			PerHandleData = NULL;
			PerIoData = NULL;
	return 0;
}
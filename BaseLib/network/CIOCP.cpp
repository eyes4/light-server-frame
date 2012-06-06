//////////////////////////////////////////////////////////////////////////////////////////
//   Iocp 实现文件
 
#include "StdAfx.h"
#include "iocp.h"
 
static bool bInit = false;
 
DWORD __stdcall CIOCP::WorkThread(LPVOID Param)
{
     CIOCP * pthis = (CIOCP *)Param;
 
     void * re;
     OVERLAPPED * pOverlap;
     DWORD berByte;
     while(pthis->m_workThread)
     {
         int ret;
         ret = GetQueuedCompletionStatus(pthis->m_hIocp, &berByte, (LPDWORD)&re, (LPOVERLAPPED *)&pOverlap, INFINITE);
 
         if (ret == ERROR_SUCCESS)
         {
 
         }
 
         if (berByte == 0)
         {
              //   客户端断开连接
              pthis->OnClose(re);
              OVERLAPPEDPLUS *olp = (OVERLAPPEDPLUS *)pOverlap;
              closesocket(olp->s);
              delete olp;        //   释放 与socket绑定的结构体变量
              continue;
         }
 
         if (re == NULL) return 0;
 
         OVERLAPPEDPLUS *olp = (OVERLAPPEDPLUS *)pOverlap;
 
         switch(olp->OpCode)
         {
         case OP_READ:
              pthis->OnRead(re, olp->wbuf.buf, berByte);     //   调用 OnRead() 通知应用程序，服务器收到来自客户端的网络数据
              WSARecv(olp->s, &olp->wbuf, 1, &olp->dwBytes, &olp->dwFlags, &olp->ol, NULL); //   继续调用一个接收的 I/O 异步请求
              break;
         default:
              break;
         }
     }
     return 0;
}
 
DWORD __stdcall CIOCP::AcceptThread(LPVOID Param)
{
     CIOCP * pthis = (CIOCP *)Param;
     while(pthis->m_acceptThread)
     {
         SOCKET client;
         if ((client= accept(pthis->m_sSocket, NULL, NULL)) == INVALID_SOCKET)
         {
              //   错误处理
         }
         pthis->OnAccept(client);    //   调用 OnAccept()通知应用程序有新客户端连接
         
     }
     return 1;
}
 
CIOCP::CIOCP(void)
{
}
 
CIOCP::~CIOCP(void)
{
}
 
bool CIOCP::Init(void)
{
     if (bInit)
         return true;
 
     WSADATA wsd;
     if (WSAStartup(MAKEWORD(2,2), &wsd) != 0) 
         return false; 
 
     bInit = true;
     return true;
}
 
bool CIOCP::Listen(int port)
{
     if (!bInit)
         if (!Init())
              return false;
 
     m_sSocket = socket(AF_INET, SOCK_STREAM, 0); 
 
     if (m_sSocket == INVALID_SOCKET) 
         return false; 
 
     //SOCKADDR_IN addr; 
     sockaddr_in addr;
     addr.sin_family = AF_INET; 
     addr.sin_port = htons(port); 
     addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
     //addr.sin_addr.S_un.S_addr = inet_addr(ip); 
 
     if (bind(m_sSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) 
         return false; 
 
     if (listen(m_sSocket, 10) == SOCKET_ERROR) 
         return false; 
 
     if ((m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0)) == NULL)     //   创建完成端口的句柄
         return false;
 
     this->m_acceptThread = true;
     g_haThread = CreateThread(NULL, 0, AcceptThread, (LPVOID)this, 0, &m_athreadID);    //   创建连接线程，用来接收客户端的连接
 
     this->m_workThread = true;
     g_hwThread = CreateThread(NULL, 0, WorkThread, (LPVOID)this, 0, &m_wthreadID); //   创建工作线程，用来处理完成端口消息的
     return true;
}
 
bool CIOCP::SetIoCompletionPort(SOCKET socket, void *p, char *buf, int len)
{
     if (CreateIoCompletionPort((HANDLE)socket, m_hIocp, (ULONG_PTR)p, 0) == NULL)
         return false;
 
     OVERLAPPEDPLUS *olp = new OVERLAPPEDPLUS;
     memset(olp, 0, sizeof(OVERLAPPEDPLUS));
     olp->s = socket;
     if (buf)
     {
         //   这里可以使用用户自定义的缓冲区地址，如果用户不想设置，也可以采用默认分配的缓冲区
         olp->wbuf.buf = buf;
         olp->wbuf.len = len;
     }
     else
     {
         olp->wbuf.buf = olp->buf;
         olp->wbuf.len = 4096;
     }
     olp->OpCode = OP_READ;
     int ret = WSARecv(olp->s, &olp->wbuf, 1, &olp->dwBytes, &olp->dwFlags, &olp->ol, NULL);
     if (ret == SOCKET_ERROR)
         if (WSAGetLastError() != ERROR_IO_PENDING)
              return false;
     return true;
}
 
void CIOCP::OnAccept(SOCKET socket)
{
     this->SetIoCompletionPort(socket, NULL);
}
 
//===================================================================================
bool CIOCPClient::Connect(char *ip, int port)
{
         //   连接服务器
     if (!bInit)
         if (!Init())
              return false;
 
     //   初始化连接socket
     m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if (m_socket == SOCKET_ERROR)
     {
//       printf("cocket Create fail");
         return false;
     }
 
     // 填写服务器地址信息
     // 端口为1982
     // IP地址为INADDR_ANY，注意使用htonl将IP地址转换为网络格式ServerAddr.sin_family = AF_INET;
     sockaddr_in ClientAddr;
     ClientAddr.sin_family = AF_INET;
     ClientAddr.sin_port = htons(port);    
     ClientAddr.sin_addr.s_addr = inet_addr(ip);
 
     // 绑定监听端口
     bind(m_socket, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
 
     if (connect(m_socket, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)
     {
         return false;
     }
     
     if ((m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0)) == NULL)     //   创建完成端口的句柄
         return false;
 
     this->m_workThread = true;
     g_hwThread = CreateThread(NULL, 0, WorkThread, (LPVOID)this, 0, &m_wthreadID); //   创建工作线程，用来处理完成端口消息的
 
     this->SetIoCompletionPort(m_socket, &m_socket);    //   设置完成端口监听的socket
     return true;
}
 
void CIOCPClient::Send(char *buf, int len)
{
     send(m_socket, buf, len, 0);
}
 
 
///////////////////////////////////////////////////////////////////////////////////
// IOCPclient 应用代码
 
#include "stdafx.h"
#include "IOCP.h"
#include "TClientSocket.h"
class Iocp :public CIOCPClient 
{
      void OnRead(void * p, char *buf, int len)
      {
          printf(buf);
          Sleep(1000);
          this->Send(buf, len);
      }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
     Iocp <a href="http://www.server-development.cn/go.php/tags/iocp/" target="_blank" title="http://www.server-development.cn/go.php/tags/iocp/" class="mykeyword">iocp</a>;
     iocp.Init();
     iocp.Connect("127.0.0.1", 4311);
     iocp.Send("test\0", 5);
     
     gets(new char[1000]);
     return 0;
}
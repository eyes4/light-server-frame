/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CTcpSvrSocket.h $
 *
 *  @brief TCP并发服务器Socket类头文件
 *
 *  $Id: CTcpSvrSocket.h 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#ifndef __CTCPSVRSOCKET_H__
#define __CTCPSVRSOCKET_H__
#include <string>
#include <map>
#include "common.h"
#include "common_thread.h"
#include "common_socket.h"

using namespace std;
#ifdef WIN32

const int OP_READ = 0;
const int OP_WRITE = 1;
const int OP_ACCEPT = 2;
/*
     OVERLAPPEDPLUS 结构体设计思路
     OVERLAPPED 是一个固定的用于处理网络消息事件返回值的结构体变量
     在完成端口和重叠I/O模型里用于返回消息事件的结果
     因为在处理网络消息的时候，发送的是一个返回值的结构体指针，只要结构体
     的前面部分满足系统的要求，在系统操作成功的时候也就会把这个结构体指针
     发回给用户，我们只要在系统定义的结构体后面扩展一些自己的东西，就可以
     很轻松的确定该消息是谁发过来的。
     不过好像完成端口在设计的时候也满足了这样的需求，所以在这里我只是放入
     一些与系统连接有关的数据，用户需要存放的数据这里就不在存放
     这里存储与系统相关的数据有：
     socket 
     OpCode 本次消息的操作类型（在完成端口的操作里面，是以消息通知系统，
         读数据/写数据，都是要发这样的消息结构体过去的，所以如果系统要同时
         进行读写操作的话，就需要有一个变量来区分操作了）
 
     WSABUF   wbuf;                  //   读写缓冲区结构体变量
     DWORD    dwBytes, dwFlags; //   一些在读写时用到的标志性变量
     char buf[4096];                  //   自己的缓冲区
     上面的4个变量存放的是一些与消息相关的数据，都是一些操作上用到的，
     这些东西都是固定的，具体作用需要参考一下完成端口相关函数的参数接口
*/
#define DATA_BUFSIZE 8192
struct OVERLAPPEDPLUS
{
     OVERLAPPED		ol;
     SOCKET			s;
     int			OpCode;
     WSABUF			wbuf;
     DWORD			dwBytes, dwFlags;
     char			buf[4096];
};

typedef struct                        //这个玩意就是灌数据，取数据的一个自定义数据结构
                                     //和那个wm_data差不了多少，不过就是老要塞一个OverLapped结构，
{
   OVERLAPPED Overlapped;
   WSABUF DataBuf;
   CHAR Buffer[DATA_BUFSIZE];                    
   DWORD BytesSEND;                                 //发送字节数
   DWORD BytesRECV;                                
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

// 但I/O操作数据
typedef struct tagPER_IO_DATA
{
    OVERLAPPED Overlapped;
    WSABUF DataBuf;
    char buffer[8196];
    int BufferLen;
    int OperationType;   // 可以作为读写的标志，为简单，我忽略了
}PER_IO_DATA, *LPPER_IO_DATA;


typedef struct
{
   SOCKET Socket;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;
#endif

class CTcpSvrSocket
{
public:
	CTcpSvrSocket(void);
	~CTcpSvrSocket(void);
	bool SvrListen(const char *szBindIP, const unsigned short usListenPort);
	void SvrStop(void);
	virtual void OnRead(SOCKET socket, const char *szClientIP,  char *buf, int len)=0;
	virtual void OnAccept(SOCKET socket, const char *szClientIP) =0;
	virtual void OnClose(SOCKET socket, const char *szClientIP) =0;
private:
	bool m_bInited;
	SOCKET m_sSocket;
	map<SOCKET, void *> m_MapClient;
	//map<SOCKET, string> m_RecvStrMap;
	DWORD m_wthreadID;
	
	bool m_workThread;
	
#ifdef WIN32
	HANDLE m_hwThread;     //   工作线程句柄
	HANDLE m_haThread;     //   连接线程句柄
	DWORD m_athreadID;
	HANDLE m_hIocp;             //   完成端口的句柄
	LPPER_HANDLE_DATA PerHandleData;
   LPPER_IO_OPERATION_DATA PerIoData;
  bool m_acceptThread;
	static DWORD __stdcall WorkThread(void* Param);
  static DWORD __stdcall AcceptThread(LPVOID Param);
#endif
#ifdef _LINUX

	int m_epfd;
public:
	ThreadHandle m_hwThread;     //   工作线程句柄
	static THREAD_FUN _WorkThread(void* Param);
	void WorkThread(void);
#endif

};

#endif


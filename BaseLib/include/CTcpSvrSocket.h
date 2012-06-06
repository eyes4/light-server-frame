/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CTcpSvrSocket.h $
 *
 *  @brief TCP����������Socket��ͷ�ļ�
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
     OVERLAPPEDPLUS �ṹ�����˼·
     OVERLAPPED ��һ���̶������ڴ���������Ϣ�¼�����ֵ�Ľṹ�����
     ����ɶ˿ں��ص�I/Oģ�������ڷ�����Ϣ�¼��Ľ��
     ��Ϊ�ڴ���������Ϣ��ʱ�򣬷��͵���һ������ֵ�Ľṹ��ָ�룬ֻҪ�ṹ��
     ��ǰ�沿������ϵͳ��Ҫ����ϵͳ�����ɹ���ʱ��Ҳ�ͻ������ṹ��ָ��
     ���ظ��û�������ֻҪ��ϵͳ����Ľṹ�������չһЩ�Լ��Ķ������Ϳ���
     �����ɵ�ȷ������Ϣ��˭�������ġ�
     ����������ɶ˿�����Ƶ�ʱ��Ҳ����������������������������ֻ�Ƿ���
     һЩ��ϵͳ�����йص����ݣ��û���Ҫ��ŵ���������Ͳ��ڴ��
     ����洢��ϵͳ��ص������У�
     socket 
     OpCode ������Ϣ�Ĳ������ͣ�����ɶ˿ڵĲ������棬������Ϣ֪ͨϵͳ��
         ������/д���ݣ�����Ҫ����������Ϣ�ṹ���ȥ�ģ��������ϵͳҪͬʱ
         ���ж�д�����Ļ�������Ҫ��һ�����������ֲ����ˣ�
 
     WSABUF   wbuf;                  //   ��д�������ṹ�����
     DWORD    dwBytes, dwFlags; //   һЩ�ڶ�дʱ�õ��ı�־�Ա���
     char buf[4096];                  //   �Լ��Ļ�����
     �����4��������ŵ���һЩ����Ϣ��ص����ݣ�����һЩ�������õ��ģ�
     ��Щ�������ǹ̶��ģ�����������Ҫ�ο�һ����ɶ˿���غ����Ĳ����ӿ�
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

typedef struct                        //���������ǹ����ݣ�ȡ���ݵ�һ���Զ������ݽṹ
                                     //���Ǹ�wm_data��˶��٣�����������Ҫ��һ��OverLapped�ṹ��
{
   OVERLAPPED Overlapped;
   WSABUF DataBuf;
   CHAR Buffer[DATA_BUFSIZE];                    
   DWORD BytesSEND;                                 //�����ֽ���
   DWORD BytesRECV;                                
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

// ��I/O��������
typedef struct tagPER_IO_DATA
{
    OVERLAPPED Overlapped;
    WSABUF DataBuf;
    char buffer[8196];
    int BufferLen;
    int OperationType;   // ������Ϊ��д�ı�־��Ϊ�򵥣��Һ�����
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
	HANDLE m_hwThread;     //   �����߳̾��
	HANDLE m_haThread;     //   �����߳̾��
	DWORD m_athreadID;
	HANDLE m_hIocp;             //   ��ɶ˿ڵľ��
	LPPER_HANDLE_DATA PerHandleData;
   LPPER_IO_OPERATION_DATA PerIoData;
  bool m_acceptThread;
	static DWORD __stdcall WorkThread(void* Param);
  static DWORD __stdcall AcceptThread(LPVOID Param);
#endif
#ifdef _LINUX

	int m_epfd;
public:
	ThreadHandle m_hwThread;     //   �����߳̾��
	static THREAD_FUN _WorkThread(void* Param);
	void WorkThread(void);
#endif

};

#endif


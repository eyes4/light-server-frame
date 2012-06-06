/////////////////////////////////////////////////////////////////////////////////////
//   Iocp ͷ�ļ�
 
#pragma once
 
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )
 
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
struct OVERLAPPEDPLUS
{
     OVERLAPPED    ol;
     SOCKET        s;
     int OpCode;
     WSABUF   wbuf;
     DWORD    dwBytes, dwFlags;
     char buf[4096];
};
 
class CIOCP
{
protected:
     HANDLE g_hwThread;     //   �����߳̾��
     DWORD m_wthreadID;
     HANDLE g_haThread;     //   �����߳̾��
     DWORD m_athreadID;
public:
     bool m_workThread;
     bool m_acceptThread;
     HANDLE m_hIocp;             //   ��ɶ˿ڵľ��
     SOCKET m_sSocket;
     
public:
     CIOCP(void);
     ~CIOCP(void);
     virtual void OnRead(void * p, char *buf, int len){};
     virtual void OnAccept(SOCKET socket);
     virtual void OnClose(void * p){};
     bool SetIoCompletionPort(SOCKET socket, void *p, char *buf = NULL, int len = 0);
         //   ��һ��socket��һ���Զ���Ľṹ���������ɶ˿ڣ��൱�ڰ�socket��һ���ṹ��������а󶨣���
         //   ��������������3�������¼���ʱ�򣬸ýṹ��������ٴ��ظ�����
         //   �����Ϳ������ֵ�ǰ�����¼����Ǹ�socket������
     bool Init(void);
     bool Listen(int port);
     static DWORD __stdcall WorkThread(LPVOID Param);
     static DWORD __stdcall AcceptThread(LPVOID Param);
};
 
class CIOCPClient: public CIOCP
{
protected:
     SOCKET m_socket;
public:
     bool Connect(char *ip, int port);
     void Send(char *buf, int len);
};
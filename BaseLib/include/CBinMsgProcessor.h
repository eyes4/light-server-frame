/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CBinMsgProcessor.h $
 *
 *  @brief �������˶�������Ϣ�����ࡣ���ڴ�����������������Ķ�������ʽ�Ľ�����Ϣ
 *
 *  $Id: CBinMsgProcessor.h 17 2008-11-18 11:38:38Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __CBINMSGPROCESSOR_H__
#define __CBINMSGPROCESSOR_H__

#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "../include/common.h"
#include "../include/common_socket.h"

using namespace std;
/**
* @brief �������յ��Ķ�������Ϣ�ṹ��
*/
typedef struct _TBinMsg
{
	SOCKET socket;		/**<��ͻ��˽������ӵ��׽���*/
	char clientip[16];		/**<�ͻ���IP��ַ*/
	int nMsgLen;		/**<*/
	char *pMsg;		/**<��Ϣ, ע�⣬�ڴ���ʱ���䣬�������Ҫfree*/
}TBinMsg;

class CBinMsgProcessor
{
public:
	/*
	* @brief ��������Ϣ�����๹�캯��
	* @param nTimeOut δ�յ���Ϣ�������Ͽ��ĳ�ʱʱ��
	*/
	CBinMsgProcessor(int nTimeOut, char *szTail);

	/*
	* @brief �ַ���Ϣ��������������
	*/
	~CBinMsgProcessor();

	/*
	* @brief �ͻ��������¼�
	* @param socket �ͻ�������socket
	* @param szClientIP �ͻ���IP��ַ
	*/
	void OnAccept(SOCKET socket, const char *szClientIP);

	/*
	* @breif �յ��ͻ�����Ϣ�¼�
	* @param socket �ͻ�������socket
	* @param szClientIP �ͻ���IP��ַ
	* @param buf ��Ϣ������ָ��
	* @param len ��Ϣ����
	*/
	void OnRead(SOCKET socket, const char *szClientIP, char *buf, int len);

	/*
	* @brief �ͻ��˶Ͽ������¼�
	* @param socket �ͻ�������socket
	* @param szClientIP �ͻ���IP��ַ
	*/
	void OnClose(SOCKET socket, const char *szClientIP);

	/*
	* @brief ��ǰ��Ϣ�����������������Ϣ����
	* @return ��Ϣ����
	*/
	int MsgCount(void);

	/*
	* @brief ����Ϣ�����������ȡ��Ϣ
	* @return ���ص���Ϣ��void *���ͣ��ɵ�����ת���ɾ�������ͣ���TStrMsg *
	*/
	void *GetMsg(void);

	/*
	* @brief �����ϵ���Ϣ����Ϣ��������е�����ͨ����GetMsg�����
	*/
	void PopMsg(void);

	/*
	* @brief ��鳬ʱ����Ϣ�Ŀͻ��ˣ��˺�����CServer��ʱ���á�
	*/
	void CheckTimeout(void);
private:
	int m_nTailSize;
	char m_szTail[32];
	queue<TBinMsg *> m_MsgQueue;			/**<��Ϣ�������*/
	//map<SOCKET, string> m_RecvStrMap;		/**<���ڴ�Ž��յ��Ĳ���������Ϣ����socket��Ӧ*/
	map<SOCKET, int> m_TimeOutTick;			/**<��socket��Ӧ�ĳ�ʱ��ʱ��*/
	int m_nSlientTimeOut;							/**<Ĭ�ϵĳ�ʱʱ��*/
};

#endif


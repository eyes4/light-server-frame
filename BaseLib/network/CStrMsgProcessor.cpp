
#include "../include/CStrMsgProcessor.h"

CStrMsgProcessor::CStrMsgProcessor(int nTimeOut, char *szTail)
{
	m_nTimeOut = nTimeOut;
	strcpy(m_szTail, szTail);
	m_nTailSize = strlen(szTail);
	InitMutex(&m_Mutex);
}

CStrMsgProcessor::~CStrMsgProcessor()
{
	DestroyMutex(&m_Mutex);
}

void CStrMsgProcessor::OnAccept(SOCKET socket, const char *szClientIP)
{
	EnterMutex(&m_Mutex);
	//DEBUGMSG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Accept socket:%d\n", socket);
	m_RecvStrMap[socket] = "";
	m_TimeOutTick[socket] = 10000;
	LeaveMutex(&m_Mutex);
	TStrMsg *pMsg = new TStrMsg();
	pMsg->socket = socket;
	pMsg->msg = string("#1$");//特殊消息，表示Accept
	sprintf(pMsg->clientip, "%s", szClientIP);
	m_MsgQueue.push(pMsg);
}

void CStrMsgProcessor::OnRead(SOCKET socket, const char *szClientIP, char *buf, int len)
{
	if (m_RecvStrMap.find(socket) == m_RecvStrMap.end()) return;
	int nHead = 0;
	m_TimeOutTick[socket] = m_nTimeOut / 10;
	char *p = strstr(buf, m_szTail);//"$");
	while (p != NULL)
	{
		m_RecvStrMap[socket].append(buf, nHead, p - &buf[nHead]);
		nHead = (int)(p - buf + m_nTailSize);//1);
		TStrMsg *pMsg = new TStrMsg();
		pMsg->socket = socket;
		pMsg->msg = m_RecvStrMap[socket];
		sprintf(pMsg->clientip, "%s", szClientIP);
		m_MsgQueue.push(pMsg);
		p = strstr(&buf[nHead], m_szTail);//"$");
		m_RecvStrMap[socket] = "";
		//nHead = 0;
		
	}

	if (nHead < len)
	{
		//cout<<nHead<<" "<<len<<" "<<m_RecvStrMap[socket]<<endl;
		m_RecvStrMap[socket].append(buf, nHead, len - nHead);
	}	
}

void CStrMsgProcessor::OnClose(SOCKET socket, const char *szClientIP)
{
	//cout<<"----------------------------------onclose"<<endl;
	EnterMutex(&m_Mutex);
	TStrMsg *pMsg = new TStrMsg();
	pMsg->socket = socket;
	pMsg->msg = "#0$";//特殊消息，表示网络断线
	sprintf(pMsg->clientip, "%s", szClientIP);
	m_MsgQueue.push(pMsg);
	m_RecvStrMap.erase(socket);
	if (m_TimeOutTick.find(socket) != m_TimeOutTick.end())
		m_TimeOutTick.erase(socket);
	LeaveMutex(&m_Mutex);
}

int CStrMsgProcessor::MsgCount(void)
{
	return (int)m_MsgQueue.size();
}

void *CStrMsgProcessor::GetMsg(void)
{
	return (void *)m_MsgQueue.front();
}

void CStrMsgProcessor::PopMsg(void)
{
	TStrMsg *pMsg = m_MsgQueue.front();
	delete pMsg;
	m_MsgQueue.pop();
}

void CStrMsgProcessor::CheckTimeout(void)
{
	
	EnterMutex(&m_Mutex);
	map<SOCKET, int>::iterator ClientIter;
	for (ClientIter = m_TimeOutTick.begin(); ClientIter != m_TimeOutTick.end(); )
    {
		ClientIter->second--;
		if (ClientIter->second <= 0)
		{
			map<SOCKET, int>::iterator itErase= ClientIter;
			++ClientIter;
			SOCKET s = itErase->first;

			TStrMsg *pMsg = new TStrMsg();
			pMsg->socket = s;
			pMsg->msg = "#0$";//特殊消息，表示网络断线
			sprintf(pMsg->clientip, "%s", "");
			m_MsgQueue.push(pMsg);
			m_TimeOutTick.erase(s);
			m_RecvStrMap.erase(s);
			DEBUGMSG("*******************************close socket:%d\n", s);
			SocketClose(s);
		}
		else
		{
			++ClientIter;
		}
    }
	LeaveMutex(&m_Mutex);
}

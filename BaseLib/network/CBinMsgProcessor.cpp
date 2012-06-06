
#include "../include/CBinMsgProcessor.h"

CBinMsgProcessor::CBinMsgProcessor(int nSlienceTimeOut, char *szTail)
{
	m_nSlienceTimeOut = nSlienceTimeOut;
	strcpy(m_szTail, szTail);
	m_nTailSize = strlen(szTail);
}

CBinMsgProcessor::~CBinMsgProcessor()
{

}

void CBinMsgProcessor::OnAccept(SOCKET socket, const char *szClientIP)
{
	//m_RecvStrMap[socket] = "";
	DEBUGMSG("on accept:%s\n", szClientIP);
	m_TimeOutTick[socket] = 10;
	m_TimeOutTick[socket] = 10000;
}

void CBinMsgProcessor::OnRead(SOCKET socket, const char *szClientIP, char *buf, int len)
{
//	int nHead = 0;
//	m_TimeOutTick[socket] = m_nTimeOut / 100;
//	char *p = strstr(buf, "$");
//	while (p != NULL)
//	{
//		m_RecvStrMap[socket].append(buf, nHead, p - &buf[nHead]);
//		nHead = (int)(p - buf + 1);
//		TStrMsg *pMsg = new TStrMsg();
//		pMsg->socket = socket;
//		pMsg->msg = m_RecvStrMap[socket];
//		sprintf(pMsg->clientip, "%s", szClientIP);
//		m_MsgQueue.push(pMsg);
//		p = strstr(&buf[nHead], "$");
//		m_RecvStrMap[socket] = "";
//		//nHead = 0;
//		
//	}
//
//	if (nHead < len)
//	{
//		//of<<nHead<<" "<<len<<" "<<m_RecvStrMap[socket]<<endl;
//		m_RecvStrMap[socket].append(buf, nHead, len - nHead);
//	}	
	//DEBUGMSG("on read:%s %d\n", szClientIP, len);
	TBinMsg *pMsg = (TBinMsg*)malloc(sizeof(TBinMsg));
	pMsg->socket = socket;
	sprintf(pMsg->clientip, "%s", szClientIP);
	pMsg->pMsg = (char *)malloc(len);
	pMsg->nMsgLen = len;
	memcpy(pMsg->pMsg, buf, len);
	//DEBUGMSG("read %s\n", &buf[21]);
	m_MsgQueue.push(pMsg);
}

void CBinMsgProcessor::OnClose(SOCKET socket, const char *szClientIP)
{
	//m_RecvStrMap.erase(socket);
	while (MsgCount() > 0)
	{ 
		PopMsg();
	}
	if (m_TimeOutTick.find(socket) != m_TimeOutTick.end())
		m_TimeOutTick.erase(socket);
}

int CBinMsgProcessor::MsgCount(void)
{
	//DEBUGMSG("MsgCount :%d\n", (int)m_MsgQueue.size());
	return (int)m_MsgQueue.size();
}

void *CBinMsgProcessor::GetMsg(void)
{
	return (void *)m_MsgQueue.front();
}

void CBinMsgProcessor::PopMsg(void)
{
	TBinMsg *pMsg = m_MsgQueue.front();
	//DEBUGMSG("to delete pMsg->pMsg\n");
	free(pMsg->pMsg);
	//DEBUGMSG("to delete pMSG\n");
	free(pMsg);
	//DEBUGMSG("to Pop\n");
	m_MsgQueue.pop();
}

void CBinMsgProcessor::CheckTimeout(void)
{
	map<SOCKET, int>::iterator ClientIter;
	for (ClientIter = m_TimeOutTick.begin(); ClientIter != m_TimeOutTick.end(); )
    {
		ClientIter->second--;
		if (ClientIter->second <= 0)
		{
			map<SOCKET, int>::iterator itErase= ClientIter;
			++ClientIter;
			SOCKET s = itErase->first;
			m_TimeOutTick.erase(itErase);
			SocketClose(s);
		}
		else
		{
			++ClientIter;
		}
    }
}


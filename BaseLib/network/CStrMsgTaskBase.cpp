/****************************************************
 *  @file    $URL $
 *
 *  @brief 任务基类
 *
 *  $Id: CStrMsgTaskBase.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/CStrMsgTaskBase.h"

CStrMsgTaskBase::CStrMsgTaskBase(void)
{
}

CStrMsgTaskBase::~CStrMsgTaskBase(void)
{
}

void CStrMsgTaskBase::SetInfo(void *pInfo)
{
	
	TStrMsg *pMsg = (TStrMsg *)pInfo;
	sprintf(m_Msg.clientip, "%s", pMsg->clientip);
	m_Msg.socket = pMsg->socket;
	m_Msg.msg = pMsg->msg;
}

void CStrMsgTaskBase::Run(int id)
{
	if(m_Msg.msg == "#0$")
	{
		DEBUGMSG("close socket:%d\n", m_Msg.socket);
		DoCloseSocket(m_Msg.socket, m_Msg.clientip);
		SocketClose(m_Msg.socket);
		m_Msg.socket = -1;
		return;
	}
	else if (m_Msg.msg == "#1$")
	{
		DoAcceptSocket(m_Msg.socket, m_Msg.clientip);
		return;
	}

	ProcessMessage(m_Msg.socket, m_Msg.clientip, m_Msg.msg);
}

void CStrMsgTaskBase::DoCloseSocket(int nSocket, string strClientIP)
{
}

void CStrMsgTaskBase::DoAcceptSocket(int nSocket, string strClientIP)
{
}
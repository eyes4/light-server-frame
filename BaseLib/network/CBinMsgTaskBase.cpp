/****************************************************
 *  @file    $URL $
 *
 *  @brief 任务基类
 *
 *  $Id: CBinMsgTaskBase.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/CBinMsgTaskBase.h"

CBinMsgTaskBase::CBinMsgTaskBase(void)
{
}

CBinMsgTaskBase::~CBinMsgTaskBase(void)
{
}

void CBinMsgTaskBase::SetInfo(void *pInfo)
{
	
	/*TBinMsg *pMsg = (TBinMsg *)pInfo;
	sprintf(m_Msg.clientip, "%s", pMsg->clientip);
	m_Msg.socket = pMsg->socket;
	m_Msg.msg = pMsg->msg;*/
}

void CBinMsgTaskBase::Run(int id)
{
	/*if(m_Msg.msg == "#0$")
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
	}*/

	//ProcessMessage(m_Msg.socket, m_Msg.clientip, m_Msg.msg);
}

void CBinMsgTaskBase::DoCloseSocket(int nSocket, string strClientIP)
{
}

void CBinMsgTaskBase::DoAcceptSocket(int nSocket, string strClientIP)
{
}
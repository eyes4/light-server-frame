/****************************************************
 *  @file    $URL$
 *
 *  @brief 任务类基类
 *
 *  $Id: CBinMsgTaskBase.h 17 2008-11-18 11:38:38Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef _CBinMsgTaskBase_H_
#define _CBinMsgTaskBase_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <cstring>
#include <errno.h>

#include "common.h"
#include "common_socket.h"
#include "CBinMsgProcessor.h"

using namespace std;
class CBinMsgTaskBase
{
public:
	CBinMsgTaskBase();
	~CBinMsgTaskBase();
	void SetInfo(void *pInfo);
	void Run(int id);

	virtual int ProcessMessage(int nSocket, string strClientIP, string strMsg) = 0;
	virtual void DoCloseSocket(int nSocket, string strClientIP);
	virtual void DoAcceptSocket(int nSocket, string strClientIP);
protected:
	TBinMsg m_Msg;
	
	
};

#endif

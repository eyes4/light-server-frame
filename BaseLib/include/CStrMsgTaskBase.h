/****************************************************
 *  @file    $URL$
 *
 *  @brief 任务类基类
 *
 *  $Id: CStrMsgTaskBase.h 17 2008-11-18 11:38:38Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef _CStrMsgTaskBase_H_
#define _CStrMsgTaskBase_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <cstring>
#include <errno.h>

#include "common.h"
#include "common_socket.h"
#include "CStrMsgProcessor.h"

using namespace std;
class CStrMsgTaskBase
{
public:
	CStrMsgTaskBase();
	~CStrMsgTaskBase();
	void SetInfo(void *pInfo);
	void Run(int id);

	virtual int ProcessMessage(int nSocket, string strClientIP, string strMsg) = 0;
	virtual void DoCloseSocket(int nSocket, string strClientIP);
	virtual void DoAcceptSocket(int nSocket, string strClientIP);
protected:
	TStrMsg m_Msg;
	
	
};

#endif

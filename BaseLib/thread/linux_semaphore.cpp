/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/linux_semaphore.cpp $
 *
 *  linux semaphore c file
 *
 *  $Id: linux_semaphore.cpp 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include <errno.h>
#include "linux_semaphore.h"

int SemCreate(int nInitVal, SemHandle *pHandle)
{
	//static CGSemHandle hSem;
	int nRes = sem_init(pHandle, 0, nInitVal);
	DEBUGMSG("init sem:%d  res:%d   err:%d\n", pHandle, nRes, errno);
	if (nRes == 0)
		return 0;
	else
		return -1;
	//return (nRes == 0)?hSem:(CGSemHandle)0;
}

void SemDestroy(SemHandle *pSem)
{
	sem_destroy(pSem);
}

void SemPost(SemHandle *pSem)
{
	DEBUGMSG("post sem:%d\n", pSem);
	sem_post(pSem);
}

void SemWait(SemHandle *pSem)
{
	int nErr;
	do 
	{
		//DEBUGMSG("wait sem:%d\n", pSem);
        	nErr = sem_wait(pSem);
	} while (nErr == -1 && errno == EINTR);
}


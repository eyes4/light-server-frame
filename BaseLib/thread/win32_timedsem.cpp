/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/win32_timedsem.cpp $
 *
 *  @brief win32—” ±–≈∫≈¡ø
 *
 *  $Id: win32_timedsem.cpp 11 2008-08-21 14:05:08Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#include "../include/common_timedsem.h"

void TimedSemCreate(TimedSemHandle *pHandle, int nInitVal)
{
	*pHandle = CreateSemaphore(NULL, nInitVal, 65535, NULL);
}

void TimedSemDestroy(TimedSemHandle hSem)
{
	CloseHandle(hSem);
}

void TimedSemPost(TimedSemHandle hSem)
{
	ReleaseSemaphore(hSem, 1, NULL);
}

bool TimedSemWait(TimedSemHandle hSem, int nTimeOut)
{
	WaitForSingleObject(hSem, nTimeOut);
	return true;
}

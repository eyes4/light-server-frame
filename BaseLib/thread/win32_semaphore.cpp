/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/win32_semaphore.cpp $
 *
 *  @brief win32����
 *
 *  $Id: win32_semaphore.cpp 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/common_semaphore.h"

int SemCreate(INT32 nInitVal, SemHandle *pHandle)
{
	if ((*pHandle = CreateSemaphore(NULL, nInitVal, 65535, NULL))!= NULL)
		return 0;
	else
		return -1;
}

void SemDestroy(SemHandle *pSem)
{
	CloseHandle(*pSem);
}

void SemPost(SemHandle *pSem)
{
	ReleaseSemaphore(*pSem, 1, NULL);
}

void SemWait(SemHandle *pSem)
{
	WaitForSingleObject(*pSem, INFINITE);
}

/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/win32_thread.cpp $
 *
 *  @brief win32 Ïß³Ì
 *
 *  $Id: win32_thread.cpp 11 2008-08-21 14:05:08Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#include "../include/common_thread.h"

ThreadHandle ThreadCreate(THREADFUNC_TYPE (* pThreadFun)( void * ),  void * pParam)
{
    return (ThreadHandle)_beginthreadex(NULL, 0, (THREADFUNC_TYPE) pThreadFun, pParam, NULL, NULL);
}

void ThreadExit(void)
{
    _endthreadex(0);
}

void CloseThreadHandle(ThreadHandle hThread)
{
    CloseHandle((HANDLE) hThread);
}

void WaitForThread(ThreadHandle hThread)
{
    WaitForSingleObject((HANDLE)hThread, INFINITE);
}

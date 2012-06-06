/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/linux_thread.cpp $
 *
 *  linux thread c file
 *
 *  $Id: linux_thread.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "linux_thread.h"

ThreadHandle ThreadCreate(THREAD_FUN (* pThreadFun)( void * ),  void * pParam)
{
    pthread_t threadID;
    pthread_create(&threadID, NULL, (THREAD_FUN)pThreadFun, pParam);
    return threadID;
}

void ThreadExit(void)
{
  pthread_exit(0);
}

void CloseThreadHandle(ThreadHandle hThread)
{
	//
    //CloseHandle((HANDLE) hThread);
}

void WaitForThread(ThreadHandle hThread)
{
    void * retval;
    pthread_join(hThread, &retval);
}


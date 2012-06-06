/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/thread/linux_timedsem.cpp $
 *
 *  linux timedsem c file
 *
 *  $Id: linux_timedsem.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#include "linux_timedsem.h"

int TimedSemCreate(TimedSemHandle *pHandle, int nInitVal)
{
	int res;
	pHandle->nCount = nInitVal;
	res = pthread_mutex_init(&pHandle->mtx, 0);
	if (res != 0) return res;
  res = pthread_cond_init(&pHandle->cond, 0);
  if (res != 0)
	{
		pthread_mutex_destroy(&pHandle->mtx);
		return res;
	}
	return res;
}

void TimedSemDestroy(TimedSemHandle hSem)
{
	pthread_cond_destroy(&hSem.cond);
    pthread_mutex_destroy(&hSem.mtx);
}

void TimedSemPost(TimedSemHandle hSem)
{
	pthread_mutex_lock(&hSem.mtx);
    hSem.nCount++; 
    pthread_cond_signal(&hSem.cond);
    pthread_mutex_unlock(&hSem.mtx);
}

bool TimedSemWait(TimedSemHandle hSem, int nTimeOut)
{
	struct timespec abs_ts; 
    struct timeval cur_tv;
	int rc;
	pthread_mutex_lock(&hSem.mtx);
    while (hSem.nCount <= 0)
    { 
        if (nTimeOut >= 0)
        {
            gettimeofday(&cur_tv, NULL);
            abs_ts.tv_sec = cur_tv.tv_sec + nTimeOut / 1000; 
            abs_ts.tv_nsec = cur_tv.tv_usec * 1000
                + (nTimeOut % 1000) * 1000000;
            rc = pthread_cond_timedwait(&hSem.cond, &hSem.mtx, &abs_ts);
            if (rc == ETIMEDOUT) { 
                pthread_mutex_unlock(&hSem.mtx);
                return false;
            }
        }
        else
            pthread_cond_wait(&hSem.cond, &hSem.mtx);
    } 
    hSem.nCount--;
    pthread_mutex_unlock(&hSem.mtx);
    return true;
}

